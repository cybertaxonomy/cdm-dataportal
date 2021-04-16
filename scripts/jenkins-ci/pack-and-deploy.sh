#!/bin/bash

##############################################################
# Creates and tar.gz archive from the whole project and deploys
# it to the server configured as $SSH_HOST below
#
# Steps:
#
#  1. Create subfolder ./drupal-7-cdm-dataportal/
#  2. Make a shallow clone of the treeish (release tag or snapshot, ...)
#  3. Let composer build the ./web folder, which then will contain
#     a runnable drupal-7 installation with cdm_dataportal module and
#     theme.
#  4. Clean up all folders created by composer except of ./web
#  5. Pack the whole  ./drupal-7-cdm-dataportal as tar.gz archive
##############################################################

GIT_REPO_URL="edit-git:/var/git/cdm-dataportal.git"

# here you can use an host short cut configured in ~/.ssh/config
SSH_HOST='edit-deploy'

##############################################################
# NOTE: the ssh host should be configured in the ~/.ssh/config:
#     Host edit-deploy
#     HostName <ip ot host name>
#     User <deployment-user>
# 
# The server to which the project is deployed to needs to prepared:
#
# 1. create the <deployment-user>
#
# 2. assuming you are logged in as the <deployment-user>:
#
#  echo "umask 0126" >> ~/.bashrc : 
#   
# 3. user 'www-data' must be member of the group of the <deployment-user>
#
# 4. The permissions of the folders to deployed to must be set to 775 ownership must be adjusted:
#   chmod -R 775 <the folders>
#   chown -R www-data:deploy <the folders> 
# 
#
##############################################################

TARGET_DIR=drupal-7-cdm-dataportal

if [ -z "$1" ]; then
	echo "version parameter missing\nUsage: pack-and-deploy.sh <version-number>"
  exit -1
fi
VERSION=$1

# $WORKSPACE is an environment variable set by jenkins
if [ -z "$WORKSPACE" ]; then
  echo "ERROR: environment variable WORKSPACE should be set by jenkins but is missing."
  exit -1
fi

ARCHIVE_FILE=$TARGET_DIR'-'$VERSION'.tar.gz'

echo "cleaning up"
rm -rf ./dist
mkdir ./dist
cd ./dist
git clone --depth 1 --branch $VERSION $GIT_REPO_URL $TARGET_DIR
cd $TARGET_DIR
composer install --no-dev
rm -rf vendor
cd ../
echo "creating the installation archive: $ARCHIVE_FILE"
tar czf $ARCHIVE_FILE $TARGET_DIR
# rm -rf $TARGET_DIR

if [[ -n "$SSH_HOST" && "$VERSION" =~ [0-9]+\.[0-9]+\.[0-9]+  ]] ; then
  echo "deploying release $ARCHIVE_FILE to $SSH_HOST"
  # create the new folder on the server and upload everything
  ssh ${SSH_HOST} "rm -rf /var/www/download/dataportal/$VERSION"
  ssh ${SSH_HOST} "mkdir /var/www/download/dataportal/$VERSION"
  ssh ${SSH_HOST} "rm -r /var/www/download/dataportal/stable"
  ssh ${SSH_HOST} "ln -s /var/www/download/dataportal/$VERSION /var/www/download/dataportal/stable"
  scp $ARCHIVE_FILE ${SSH_HOST}:/var/www/download/dataportal/${VERSION}/
else
  echo "deployment of development version"
  ssh ${SSH_HOST} "mkdir /var/www/download/dataportal/$VERSION"
  scp $ARCHIVE_FILE ${SSH_HOST}:/var/www/download/dataportal/${VERSION}/
fi

echo "pack-and-deploy done"










