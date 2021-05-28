#!/bin/bash

if [ -z "$1" ]; then
	echo "USAGE: update-dependencies.sh [--deactivate-install] [--multisite] [--mailto <ADDRESS>]"
	echo "  --deactivate-install :  The install.php will be hidden by appending '.off' to the filename"
	echo "  --multisite:  Do a multisite update. Requires dataportals-drush. See https://dev.e-taxonomy.eu/svn/trunk/server-scripts/dataportal-admin/"
	echo "  --mailto <ADDRESS>:  send a email to the ADDRESS with a log of the update process"
	exit 1
fi

while [[ "$#" -gt 0 ]]; do
    case $1 in
        --mailto) MAILTO="$2"; shift ;;
        --deactivate-install) deactivate_install=1 ;;
        --multisite) multisite=1 ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

if [[ -z "$(grep 'cybertaxonomy.org/drupal-7-dataportal' composer.json)"  ]]; then
    echo "ERROR: This script must be executed in the root of the drupal-7-cdm-dataportal folder"
    exit -1
fi

# configuration
TMP=$(mktemp)

if (( multisite == 1 )); then
    DRUSH=./vendor/drush/drush/drush
    if [[ ! -e $DRUSH ]]; then 
        echo "Need to install dependencies first ..."
        composer install --no-dev
    fi
else 
    DRUSH=$(which dataportals-drush) 
fi 

DRUSH=$DRUSH -r $(pwd)/web/ 

echo "creating full backup ..."

archive_file=../drupal-7-cdm-dataportal-backup-$(date -I).tar.gz
tar -czf $archive_file ./

echo "backup archive created: "$archive_file

echo "backing up settings and config files to temp backup ${TMP} ..."

# backup modified files
cp web/.htaccess ${TMP}/
cp web/robots*.txt ${TMP}/


exit 0

echo "setting dataportals in update mode ..."
# set all portals into maintainance mode
$DRUSH vset -y maintenance_mode 1

# turn clean urls off since .htaccess will be overwritten during the updatecode
$DRUSH vset clean_url -y 0

# turn off cdm_debug_mode in all sites
$DRUSH vset -y cdm_debug_mode 0

yes y | $DRUSH updatedb

# run all database updates
echo "-------------------------------------------------------------------"
echo "Updating dependencies ..."

composer update --no-dev | tee ${TMP}/composer.log

echo "-------------------------------------------------------------------"
echo "restoring settings and config files from temp backup ..."

# restore original settings and files and disable maintainance mode
rm -f .htaccess.dist
cp web/.htaccess web/.htaccess.dist
cp ${TMP}.htaccess web/
cp ${TMP}robots*.txt web/

if (( deactivate_install == 1 )); then
    # hide the install.php 
    rm install.php.off
    mv install.php install.php.off 
fi 

echo "-------------------------------------------------------------------"
echo "Applying pending database updates ..."
yes y | $DRUSH updatedb

echo "-------------------------------------------------------------------"
echo "dataportal back to production mode ..."

$DRUSH vset clean_url -y 1
$DRUSH vset -y maintenance_mode 0

echo "-------------------------------------------------------------------"
if [[ -n "$MAILTO" ]]; then
    echo "sending email to $MAILTO ..."
    cat ${TMP}/composer.log | mail -s "(`hostname`): cdm-dataportal dependencies update" $MAILTO
    echo "-------------------------------------------------------------------"
fi
echo "DONE"

