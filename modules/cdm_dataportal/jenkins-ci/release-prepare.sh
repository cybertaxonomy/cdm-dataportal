#!/bin/bash

###############################################################################
#
# Performes preparation tasks for the release of the cdm-dataportal project:
# 
# 1. compile the SASS code to css without development features
# 2. commit and push the the production ready css
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
if [ -x "$COMPASS" ]; then 
  
  git checkout release/$VERSION
  git pull --rebase

  $COMPASS clean $WORKSPACE/themes/zen_dataportal/
  $COMPASS compile $WORKSPACE/themes/zen_dataportal/
  
  git push origin release/$VERSION
  
else 
  echo "ERROR on sass compilation since the evnvironment variable COMPASS is either missing or not the file "$COMPASS" is not executable."
  exit 1
fi

echo "cdm_dataportal release preparation done!"










