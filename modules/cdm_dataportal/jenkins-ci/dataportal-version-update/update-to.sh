#!/bin/bash

if [ -z $1 ]
then
  echo "USAGE: update-to.sh <VERSION NUMBER>"
  echo "       - VERSION NUMBER e.g. '1.3.1'"
  echo "       - 'trunk' can also be used to refer to the trunk version"
  echo "       - 'develop' can also be used to refer to the develop branch"
  echo ""
  echo "  e.g.: update-to.sh 3.0.6 to install branches/drupal/module-cdm_dataportal-RELEASE-3.0.6 and branches/drupal/themes-RELEASE-3.0.6"
fi

VERSION=$1


rm -r modules/cdm_dataportal
case "$VERSION" in
    trunk)  svn co http://dev.e-taxonomy.eu/svn/trunk/drupal/7.x/modules/cdm_dataportal modules/cdm_dataportal
            ;;
  develop)  svn co http://dev.e-taxonomy.eu/svn/branches/drupal/develop/7.x/modules/cdm_dataportal modules/cdm_dataportal
            ;;
        *)  svn co http://dev.e-taxonomy.eu/svn/branches/drupal/module-cdm_dataportal-RELEASE-$VERSION modules/cdm_dataportal
            ;;
esac

rm -r themes
case "$VERSION" in
    trunk)  svn co http://dev.e-taxonomy.eu/svn/trunk/drupal/7.x/themes themes
            ;;
  develop)  svn co http://dev.e-taxonomy.eu/svn/branches/drupal/develop/7.x/themes
            ;;
        *)  svn co http://dev.e-taxonomy.eu/svn/branches/drupal/themes-RELEASE-$VERSION themes
            ;;
esac

echo "========================================================================"
echo "NOTE:"
echo "  Now you need to run the Drupal update.php script of all affected sites."
echo "  The dataportals-updatedb script allows you to update multiple"
echo "  DataPortal in batch mode."
echo ""
echo "  The dataportals-updatedb.sh script and others can be obtained from "
echo "  http://dev.e-taxonomy.eu/svn/trunk/server-scripts/dataportal-admin/"
echo "========================================================================"