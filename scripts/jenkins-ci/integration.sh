#!/bin/bash
###
# Continous intergration build with jenkins
#   call this script from within jenkins with:
#   >    bash -ex $WORKSPACE/jenkins-ci/integration.sh $WORKSPACE $JOB_NAME $dbUser $dbPassword
#
# references:
#   http://thinkshout.com/blog/2010/09/sean/beginners-guide-using-hudson-continuous-integration-drupal
#   http://drush.ws/help/3

echo "do not run this script - only kept for reference."
exit 0

#------------------

WORKSPACE=$1
JOB_NAME=$2
dbUser=$3
dbPassword=$4

dbName="jenkins_$JOB_NAME"

drupalBaseURL=http://160.45.63.201/dataportal/
drupalRoot=/var/www/drupal/
drupalSiteName="jenkins"
drupalInstallationProfile="CDM_DataPortal_Testing"

cdmServerURL=http://160.45.63.201:8080/cichorieae/
cdmClassificationUUID=534e190f-3339-49ba-95d9-fa27d5493e3e

# copy installation profiles
echo ">>> copying installation profiles to ${drupalRoot}profiles/"
svn export ${WORKSPACE}/profile/ /tmp/drupal_profiles
cp -R  /tmp/drupal_profiles/* ${drupalRoot}profiles/
rm -R /tmp/drupal_profiles

# copy module
#echo ">>> copying module ${drupalRoot}profiles/"
#rm -R ${drupalRoot}sites/all/modules/cdm_dataportal
#svn export ${WORKSPACE} ${drupalRoot}/sites/all/modules/

# drop all tables in database
echo ">>> clearing database ..."
MYSQLCMD="mysql --user=$dbUser --password=$dbPassword -D $dbName"
$MYSQLCMD -BNe "show tables" | awk '{print "set foreign_key_checks=0; drop table `" $1 "`;"}' | $MYSQLCMD
unset MYSQLCMD

# install drupal site
echo ">>> installing drupal site ..."
cd $drupalRoot
DRUSH="drush --uri=${drupalBaseURL}jenkins/"
## drush si only works with drupal 7 so the folowing does not yet work
#yes | drush si --profile=${drupalInstallationProfile} --clean-url=0 --sites-subdir=${drupalSiteName} --db-url=mysql://${dbUser}:${dbPassword}@localhost/${dbName}
# and we will use a preset sub site directory and ur own install script:
wget -O /tmp/jenkins-drupal-install ${drupalBaseURL}jenkins/install.php?profile=$drupalInstallationProfile
rm /tmp/jenkins-drupal-install

$DRUSH vset --yes cdm_webservice_url $cdmServerURL
$DRUSH vset --yes cdm_taxonomictree_uuid $cdmClassificationUUID