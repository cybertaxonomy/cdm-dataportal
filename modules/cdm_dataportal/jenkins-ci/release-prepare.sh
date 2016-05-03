#!/bin/bash

###############################################################################
#
# Performes preparation tasks for the release of the cdm-dataportal project:
# 
# 1. compile the SASS code to css without development features
# 2. commit and push the the production level css
#
###############################################################################



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
# compile the sass files to css in the zen_dataportal theme
# this will create the versions needed for production
#

git checkout release/$VERSION
git pull --rebase origin release/$VERSION

compass clean $WORKSPACE/themes/zen_dataportal/
compass compile $WORKSPACE/themes/zen_dataportal/

git add -A $WORKSPACE/themes/zen_dataportal/css/
git commit -m "release-preparation: production level css"

git push origin release/$VERSION


echo "cdm_dataportal release preparation done!"










