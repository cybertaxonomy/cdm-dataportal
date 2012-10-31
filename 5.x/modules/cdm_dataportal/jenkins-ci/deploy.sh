#!/bin/bash

#
# 
#
# 

echo $WORKSPACE

exit 0 

#####


if [ -z "$1" ]; then
	echo "version parameter missing\nUsage: deploy.sh <version-number>"
for 

VERSION=$1

# create release tag and branch for the module 
svn copy http://dev.e-taxonomy.eu/svn/trunk/drupal/modules/cdm_dataportal http://dev.e-taxonomy.eu/svn/tags/drupal/module-cdm_dataportal/$VERSION
svn copy http://dev.e-taxonomy.eu/svn/tags/drupal/module-cdm_dataportal/$VERSION http://dev.e-taxonomy.eu/svn/branches/drupal/module-cdm_dataportal-RELEASE-$VERSION

#create release tag and branch for the themes 
svn copy http://dev.e-taxonomy.eu/svn/trunk/drupal/themes http://dev.e-taxonomy.eu/svn/tags/drupal/themes/$VERSION
svn copy http://dev.e-taxonomy.eu/svn/tags/drupal/themes/$VERSION http://dev.e-taxonomy.eu/svn/branches/drupal/themes-RELEASE-$VERSION

#create the target folder
if [ !-d target ]; then 
	mkdir target 
fi
cd target

#create the module-cdm_dataportal archive 
svn export http://dev.e-taxonomy.eu/svn/tags/drupal/module-cdm_dataportal/$VERSION ./cdm_dataportal
tar czf cdm_dataportal-$VERSION.tar.gz ./cdm_dataportal 

# create the drupal5-cdm_dataportal archive
ARCHIVE_URL=(`lynx -dump http://wp5.e-taxonomy.eu/download/dataportal/stable/ | grep "download/dataportal/stable/drupal5-cdm_dataportal" | head -n 1 | sed -e "s/.*\(http.*\)/\1/g"`) 
curl --output drupal5-cdm_dataportal.tar.gz $ARCHIVE_URL

tar xzf drupal5-cdm_dataportal.tar.gz
cd drupal5-cdm_dataportal/sites/all  
./update-to.sh $VERSION
cd ../../../
tar czf drupal5-cdm_dataportal-$VERSION.tar.gz drupal5-cdm_dataportal
rm -r drupal5-cdm_dataportal.tar.gz drupal5-cdm_dataportal/

# create the new folder on the server and upload everything
ssh root@160.45.63.172 "mkdir /var/www/download/dataportal/$VERSION"
ssh root@160.45.63.172 "rm -r /var/www/download/dataportal/stable"
ssh root@160.45.63.172 "ln -s /var/www/download/dataportal/$VERSION /var/www/download/dataportal/stable"
scp drupal5-cdm_dataportal root@wp5.e-taxonomy.eu:/var/www/download/dataportal/$VERSION/
scp drupal5-cdm_dataportal-$VERSION.tar.gz root@wp5.e-taxonomy.eu:/var/www/download/dataportal/$VERSION/ 
ssh root@160.45.63.172 "chown -R www-data:www-data /var/www/download/dataportal/$VERSION"
ssh root@160.45.63.172 "chown -R www-data:www-data /var/www/download/dataportal/stable"

#
echo "cdm_dataportal deployment done!"










