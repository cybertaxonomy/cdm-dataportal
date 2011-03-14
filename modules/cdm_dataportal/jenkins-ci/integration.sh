#!/bin/sh
###
# Continius intergration build with jenkins
#   call this script from within jenkins with:
#   >    bash -e $WORKSPACE/jenkins-ci/integration.sh $JOB_NAME $dbUser $dbPassword
#
# references:
#   http://thinkshout.com/blog/2010/09/sean/beginners-guide-using-hudson-continuous-integration-drupal
#   http://drush.ws/help/3
###
#  The following variables are available to shell scripts
#  
#  BUILD_NUMBER
#      The current build number, such as "153"
#  BUILD_ID
#      The current build id, such as "2005-08-22_23-59-59" (YYYY-MM-DD_hh-mm-ss)
#  JOB_NAME
#      Name of the project of this build, such as "foo"
#  BUILD_TAG
#      String of "hudson-${JOB_NAME}-${BUILD_NUMBER}". Convenient to put into a resource file, a jar file, etc for easier identification.
#  EXECUTOR_NUMBER
#      The unique number that identifies the current executor (among executors of the same machine) that's carrying out this build. This is the number you see in the "build executor status", except that the number starts from 0, not 1.
#  NODE_NAME
#      Name of the slave if the build is on a slave, or "" if run on master
#  NODE_LABELS
#      Whitespace-separated list of labels that the node is assigned.
#  JAVA_HOME
#      If your job is configured to use a specific JDK, this variable is set to the JAVA_HOME of the specified JDK. When this variable is set, PATH is also updated to have $JAVA_HOME/bin.
#  WORKSPACE
#      The absolute path of the workspace.
#  HUDSON_URL
#      Full URL of Hudson, like http://server:port/hudson/
#  BUILD_URL
#      Full URL of this build, like http://server:port/hudson/job/foo/15/
#  JOB_URL
#      Full URL of this job, like http://server:port/hudson/job/foo/
#  SVN_REVISION
#      For Subversion-based projects, this variable contains the revision number of the module.
#  CVS_BRANCH
#      For CVS-based projects, this variable contains the branch of the module. If CVS is configured to check out the trunk, this environment variable will not be set.
#

set drupalRoot=/var/www/drupal/
set drupalSiteName=$JOB_NAME
set drupalInstallationProfile="CDM_DataPortal"

set dbName="jenkins_"$1
set dbUser= $2
set dbPassword=$3

# copy installation profiles
echo ">>> workspace is "$WORKSPACE
cp $WORKSPACE/profile/ $drupalRoot/profiles/

# drop all tables in database
MYSQL --user=$dbUser --password=$dbPassword -D $dbName
$MYSQL -BNe "show tables" | awk '{print "set foreign_key_checks=0; drop table `" $1 "`;"}' | $MYSQL
unset MYSQL

# install drupal site
cd $drupalRoot
yes | drush si --profile=$drupalInstallationProfile --clean-url=0 --sites-subdir=$drupalSiteName --db-url=mysql://$dbUser:$dbPassword@localhost/$dbName






