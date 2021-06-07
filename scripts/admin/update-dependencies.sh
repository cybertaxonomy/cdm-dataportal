#!/bin/bash


# -- options
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -h|--help) print_help=1;;
        --mailto) MAILTO="$2"; shift ;;
        --deactivate-install) deactivate_install=1 ;;
        --multi-site) multisite=1 ;;
        --site-url) site_url=$2; shift ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

if [[ -n "$site_url" ]]; then
    unset multisite
fi 

if [[ -z "${site_url}${multisite}" ]]; then
    print_help="1"
fi

# -- help
if [[ "$print_help" == "1" ]]; then
    
cat << "EOF"
Upadates all composer dependencies including the drupal core code as well as modules.
Prior starting the upgrade process a backup of the drupal-cdm-dataportal installation 
is created in $HOME/drupal-cdm-dataportal-backups/.
Some files in ./web/ which may be modified for specific setups are preserved during the 
update process:
 * web/.haccess*
 * web/robots*.txt
 * all symbolic links except for web/polyfills as this is managed through composer.  

USAGE: update-dependencies.sh [--deactivate-install] [--multi-site] [--mailto <ADDRESS>]
  --deactivate-install :  The install.php will be hidden by appending '.off' to the filename
  -h, --help:  Print this help text    
  --mailto <ADDRESS>:  send a email to the ADDRESS with a log of the update process
  --multi-site:  Do a multi-site update. Requires dataportals-drush. See https://dev.e-taxonomy.eu/svn/trunk/server-scripts/dataportal-admin/
  --site-url:  The site url to be used with drush. This option disables the --multi-site option
EOF
	exit 0
fi

# -- tests
if [[ -z "$(grep 'cybertaxonomy.org/drupal-7-dataportal' composer.json)"  ]]; then
    echo "ERROR: This script must be executed in the root of the drupal-7-cdm-dataportal folder"
    exit -1
fi

# -- requirements

COMPOSER=$(which composer)
if [[ -z "$COMPOSER" ]]; then
    # try local composer
    if [[ -x ./composer ]]; then
        COMPOSER=./composer
    else
        echo "composer not found. Please see https://github.com/cybertaxonomy/cdm-dataportal#preparation"
        exit -1
    fi
fi

# -- backups before any modification
TMP=$(mktemp -d)

echo "creating full backup ..."
backups_folder=$HOME/drupal-cdm-dataportal-backups
mkdir -p $backups_folder
archive_file=$backups_folder/drupal-cdm-dataportal-backup-$(date +%F_%T).tar.gz
tar -czf $archive_file ./
echo "backup archive created at "$(readlink -f $archive_file)

echo "back up of settings and config files to ${TMP} ..."
# backup modified files
cp -a web/.htaccess* ${TMP}/
# .htaccess.dist is provided by the drupal/drupal package und must not be in the backup
rm -f ${TMP}/.htaccess.dist
cp -a web/robots*.txt ${TMP}/
# preserve all symlinks
find web/ -maxdepth 1 -type l -exec cp -a {} ${TMP}/ \;

# -- setup 

if [[ "$multisite" == "1" ]]; then
    DRUSH=$(which dataportals-drush) 
else 
    DRUSH=./vendor/drush/drush/drush
    if [[ ! -e $DRUSH ]]; then 
        echo "Need to install dependencies first ..."
        composer install --no-dev --ansi
    fi
fi 

DRUSH=$DRUSH" -r $(pwd)/web/" 
if [[ -n "$site_url" ]]; then
    DRUSH=$DRUSH" -l $site_url"
fi

echo "setting dataportals in update mode ..."
# set all portals into maintenance mode
$DRUSH vset -y maintenance_mode 1

# turn clean urls off since .htaccess will be overwritten during the update
$DRUSH vset clean_url -y 0

# turn off cdm_debug_mode in all sites
$DRUSH vset -y cdm_debug_mode 0

yes y | $DRUSH updatedb

# run all database updates
echo "-------------------------------------------------------------------"
echo "Updating dependencies ..."

composer update --no-dev --ansi | tee ${TMP}/composer.log

echo "-------------------------------------------------------------------"
echo "restoring settings and config files from temp backup ..."

# restore original settings and files and disable maintenance mode
rm -f .htaccess.dist
cp web/.htaccess web/.htaccess.dist
cp -a ${TMP}/.htaccess* web/
cp -a ${TMP}/robots*.txt web/
find ${TMP} -maxdepth 1 -type l -exec cp -a {} web/ \;

if (( deactivate_install == 1 )); then
    # hide the install.php 
    rm -f web/install.php.off
    mv web/install.php web/install.php.off 
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

