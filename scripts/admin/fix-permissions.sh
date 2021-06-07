#!/bin/bash


# -- options
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -h|--help) print_help=1;;
        --web-user) web_user="$2"; shift ;;
        --admin-user) admin_user=$2 ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

if [[ -z "${web_user}${admin_user}" ]]; then
    print_help="1"
fi

# -- help
if [[ "$print_help" == "1" ]]; then
    
cat << "EOF"
Fixes permissions for http servers.b/polyfills as this is managed through composer.
Makes the installation bundle folder executable.
Sets the web-user as goup for ./modules and ./themes and as ownder and group for ./web


USAGE: fix-permissions.sh [--web-user USERNAME] [--admin-user USERNAME]
  --web-user   : The user under which the web server. 
                 E.g. www-data in case of apache.
  -h, --help   : Print this help text    
  --admin-user : The user which is being used to manage the installation. 
                 E.g. jenkins when the installation is managed by Jenkins CI
EOF
	exit 0
fi

# -- tests
if [[ -z "$(grep 'cybertaxonomy.org/drupal-7-dataportal' composer.json)"  ]]; then
    echo "ERROR: This script must be executed in the root of the drupal-7-cdm-dataportal folder"
    exit -1
fi

echo "making root folder executable ..."
chmod ugo+x ./

if [[ "$admin_user" ]]; then
    echo "$admin_user as owner of the installation package ..."
    chown -R $admin_user ./
fi

if [[ "$web_user" ]]; then
    echo "Permissions for the web-user $admin_user ..."
    chown -R $web_user:$web_user ./web
    chown -R :$web_user ./modules ./themes
fi

echo "DONE"

