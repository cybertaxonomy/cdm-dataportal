#!/bin/bash

DRUPAL_VERSION="7"

GIT_REPO_URL="edit-git:/var/git/cdm-dataportal.git"

DO_CREATE_DRUPAL_INSTALLER=false


##############################################################
# NOTE: the ssh host should be configured in the ~/.ssh/config:
#     Host edit-deploy
#     HostName <ip ot host name>
#     User <deployment-user>
# 
# At the server to be delpoyed to you need to setup and configure the 

# 1. create the <deployment-user> 
#
# 2. assuming you are logged in as the <deployment-user>:
#
#  echo "umask 0126" >> ~/.bashrc : 
#   
# 3. user 'www-data' must be member of the group of the <deployment-user>
#
# 4. The permissions of the folders to delpoyed to must be set to 775 ownership must be adjusted:
#   chmod -R 775 <the folders>
#   chown -R www-data:deploy <the folders> 
# 
#  
SSH_HOST='edit-deploy'
##############################################################

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

#
# pack and deploy the cdm_dataportal module
#

mkdir -p target

echo "creating the module-cdm_dataportal archive: cdm_dataportal-$VERSION.tar.gz"
tar -C $WORKSPACE -czf target/cdm_dataportal-$VERSION.tar.gz modules/cdm_dataportal

if $DO_CREATE_DRUPAL_INSTALLER ; then  

  echo "creating the drupal${DRUPAL_VERSION}-cdm_dataportal archive ..."
  
  # TODO better use drush for the assembly

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
fi

# create the new folder on the server and upload everything
ssh ${SSH_HOST} "rm -rf /var/www/download/dataportal/$VERSION"
ssh ${SSH_HOST} "mkdir /var/www/download/dataportal/$VERSION"
ssh ${SSH_HOST} "rm -r /var/www/download/dataportal/stable"
ssh ${SSH_HOST} "ln -s /var/www/download/dataportal/$VERSION /var/www/download/dataportal/stable"
scp target/cdm_dataportal-${VERSION}.tar.gz ${SSH_HOST}:/var/www/download/dataportal/${VERSION}/
if $DO_CREATE_DRUPAL_INSTALLER ; then
  scp target/drupal${DRUPAL_VERSION}-cdm_dataportal-${VERSION}.tar.gz ${SSH_HOST}:/var/www/download/dataportal/${VERSION}/
fi
  
# DONE
echo "cdm_dataportal deployment done!"










