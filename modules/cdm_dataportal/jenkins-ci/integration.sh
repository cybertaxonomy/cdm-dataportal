#!/bin/bash
###
# Continous intergration build with jenkins
#   call this script from within jenkins with:
#   >    bash -ex $WORKSPACE/jenkins-ci/integration.sh $WORKSPACE $JOB_NAME $dbUser $dbPassword
#
# references:
#   http://thinkshout.com/blog/2010/09/sean/beginners-guide-using-hudson-continuous-integration-drupal
#   http://drush.ws/help/3

WORKSPACE=$1
JOB_NAME=$2
drupalRoot=/var/www/drupal/
drupalSiteName="jenkins"
drupalInstallationProfile="CDM_DataPortal"

dbName="jenkins_$JOB_NAME"
dbUser=$3
dbPassword=$4

# copy installation profiles
echo ">>> copying installation profiles to ${drupalRoot}profiles/"
svn export ${WORKSPACE}/profile/ /tmp/drupal_profiles
cp -R  /tmp/drupal_profiles/* ${drupalRoot}profiles/
rm -R /tmp/drupal_profiles

# copy module
echo ">>> copying module ${drupalRoot}profiles/"
rm -R ${drupalRoot}sites/all/modules/cdm_dataportal
svn export ${WORKSPACE} ${drupalRoot}/sites/all/modules/

# drop all tables in database
echo ">>> clearing database ..."
MYSQLCMD="mysql --user=$dbUser --password=$dbPassword -D $dbName"
$MYSQLCMD -BNe "show tables" | awk '{print "set foreign_key_checks=0; drop table `" $1 "`;"}' | $MYSQLCMD
unset MYSQLCMD

# install drupal site
echo ">>> installing drupal site ..."
cd $drupalRoot
DRUSH="drush --uri=http://160.45.63.201/dataportal/jenkins/"
## drush si only works with drupal 7 so the folowing does not yet work
#yes | drush si --profile=${drupalInstallationProfile} --clean-url=0 --sites-subdir=${drupalSiteName} --db-url=mysql://${dbUser}:${dbPassword}@localhost/${dbName}
# and we will use a preset sub site directory and ur own install script:
wget -O /tmp/jenkins-drupal-install http://160.45.63.201/dataportal/jenkins/install.php?profile=CDM_DataPortal_Testing
rm /tmp/jenkins-drupal-install

$DRUSH vset --yes cdm_webservice_url http://160.45.63.201:8080/cichorieae/

