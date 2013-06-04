#!/bin/bash

#
#
#
#
DRUPAL_VERSION="7"
SVN_USER="edit-jenkins"

if [ -z "$1" ]; then
	echo "version parameter missing\nUsage: deploy.sh <version-number>"
  exit -1
fi
VERSION=$1

# $WORKSPACE is an environment variable set by jenkins
if [ -z "$WORKSPACE" ]; then
  echo "ERROR: environment variable WORKSPACE should be set by jenkins but is missing."
  exit -1
fi

# check if tag exists
set +e # turn of "exit script on failure", since we expect that svn tags may not always exist
TAG_EXISTS=(`svn info http://dev.e-taxonomy.eu/svn/tags/drupal/module-cdm_dataportal/$VERSION 2> /dev/null | grep URL`)
set -e # turn on again "exit script on failure"

#
# compile the sass files to css in the zen_dataportal theme
# this will create the versions needed for production
#
if [ -x "$COMPASS" ]; then  
  $COMPASS clean $WORKSPACE/themes/zen_dataportal/
  $COMPASS compile $WORKSPACE/themes/zen_dataportal/
  svn --username=$SVN_USER ci -m "sass compiled for production purposes prior release"
else 
  echo "ERROR on sass compilation since the evnvironment variable COMPASS is either missing or not the file "$COMPASS" is not executable"
  exit 1
fi 

if [ -z "$TAG_EXISTS" ]; then
	# it is a new version number ...

	# create release tag and branch for the module
	svn --username=$SVN_USER copy -m "release tag for cdm_dataportal $VERSION" http://dev.e-taxonomy.eu/svn/trunk/drupal/${DRUPAL_VERSION}.x/modules/cdm_dataportal http://dev.e-taxonomy.eu/svn/tags/drupal/module-cdm_dataportal/$VERSION
	svn --username=$SVN_USER copy -m "branch for cdm_dataportal $VERSION" http://dev.e-taxonomy.eu/svn/tags/drupal/module-cdm_dataportal/$VERSION http://dev.e-taxonomy.eu/svn/branches/drupal/module-cdm_dataportal-RELEASE-$VERSION

	#create release tag and branch for the themes
	svn --username=$SVN_USER copy -m "release tag for drupal themes $VERSION" http://dev.e-taxonomy.eu/svn/trunk/drupal/${DRUPAL_VERSION}.x/themes http://dev.e-taxonomy.eu/svn/tags/drupal/themes/$VERSION
	svn --username=$SVN_USER copy -m "branch for drupal themes $VERSION" http://dev.e-taxonomy.eu/svn/tags/drupal/themes/$VERSION http://dev.e-taxonomy.eu/svn/branches/drupal/themes-RELEASE-$VERSION
fi


#
# pack and deploy the cdm_dataportal module
#
cd $WORKSPACE/cdm_dataportal
#create the target folder
if [ ! -d target ]; then
	mkdir target
fi
cd target
rm -rf *

# create the module-cdm_dataportal archive
svn --username=$SVN_USER export http://dev.e-taxonomy.eu/svn/tags/drupal/module-cdm_dataportal/$VERSION ./cdm_dataportal
tar czf cdm_dataportal-$VERSION.tar.gz ./cdm_dataportal

# create the drupal${DRUPAL_VERSION}-cdm_dataportal archive ...

# downlod latest and unpack
ARCHIVE_URL=(`lynx -dump http://wp5.e-taxonomy.eu/download/dataportal/stable/ | grep "download/dataportal/stable/drupal${DRUPAL_VERSION}-cdm_dataportal" | head -n 1 | sed -e "s/.*\(http.*\)/\1/g"`)

if [ -z "$ARCHIVE_URL" ]; then
  echo "http://wp5.e-taxonomy.eu/download/dataportal/stable/ does not contain drupal${DRUPAL_VERSION}-cdm_dataportal.tar.gz file, please check this symling on the server"
  exit -1;
fi

curl --output drupal${DRUPAL_VERSION}-cdm_dataportal.tar.gz $ARCHIVE_URL
tar xzf drupal${DRUPAL_VERSION}-cdm_dataportal.tar.gz

# update the update script update-to.sh
rsync -r --exclude=.svn ../jenkins-ci/dataportal-version-update/ drupal${DRUPAL_VERSION}-cdm_dataportal/sites/all

# update the module and themes
cd drupal${DRUPAL_VERSION}-cdm_dataportal/sites/all
./update-to.sh $VERSION

# copy the profiles
cd ../../
rsync -r --exclude=.svn sites/all/modules/cdm_dataportal/profile/ profiles/

# make tar
cd ../
tar czf drupal${DRUPAL_VERSION}-cdm_dataportal-$VERSION.tar.gz drupal${DRUPAL_VERSION}-cdm_dataportal
rm -rf drupal${DRUPAL_VERSION}-cdm_dataportal.tar.gz

# create the new folder on the server and upload everything
ssh root@160.45.63.172 "rm -rf /var/www/download/dataportal/$VERSION"
ssh root@160.45.63.172 "mkdir /var/www/download/dataportal/$VERSION"
ssh root@160.45.63.172 "rm -r /var/www/download/dataportal/stable"
ssh root@160.45.63.172 "ln -s /var/www/download/dataportal/$VERSION /var/www/download/dataportal/stable"
scp cdm_dataportal-${VERSION}.tar.gz root@wp5.e-taxonomy.eu:/var/www/download/dataportal/${VERSION}/
scp drupal${DRUPAL_VERSION}-cdm_dataportal-${VERSION}.tar.gz root@wp5.e-taxonomy.eu:/var/www/download/dataportal/${VERSION}/
ssh root@160.45.63.172 "chown -R www-data:www-data /var/www/download/dataportal/${VERSION}"
ssh root@160.45.63.172 "chown -R www-data:www-data /var/www/download/dataportal/stable"

# DONE
echo "cdm_dataportal deployment done!"










