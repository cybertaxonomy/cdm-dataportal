#!/bin/bash

if [ -z $1 ]
then
  echo "USAGE: update-to.sh <VERSION NUMBER>"
  echo "  e.g.: update-to.sh 3.0.6 to install branches/drupal/module-cdm_dataportal-RELEASE-3.0.6 and branches/drupal/themes-RELEASE-3.0.6"
fi

VERSION=$1

rm -rf modules/cdm_dataportal
svn co http://dev.e-taxonomy.eu/svn/tags/drupal/module-cdm_dataportal/$VERSION modules/cdm_dataportal
rm -rf themes
svn co http://dev.e-taxonomy.eu/svn/tags/drupal/themes/$VERSION themes

echo "========================================================================"
echo "NOTE:"
echo "  Now you need to run the Drupal update.php script of all affected sites."
echo "  The dataportals-updatedb script allows you to update multiple"
echo "  DataPortal in batch mode."
echo ""
echo "  The dataportals-updatedb.sh script and others can be obtained from "
echo "  http://dev.e-taxonomy.eu/svn/trunk/server-scripts/dataportal-admin/"
echo "========================================================================"
