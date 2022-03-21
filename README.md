# CDM Data Portal

Drupal 7 modules and themes as web frontend to publish data hosted in a [cdm server](https://github.com/cybertaxonomy/cdm-server).

This project has a monolithic structure, that is it contains multiple projects in one git repository.

Project dependencies required for bundling the final installation package are managed via composer. 
Dependencies of sub-projects like *modules/cdm_dataportal*, *themes/zen_dataportal*, etc still have manually copied dependencies. 

Information on setting up the development environment are found in [](DEVELOPER.md)

## Project structure

* `./drush`: commands, configuration and site aliases for Drush
* `./debug`: scripts that help debugging problems with drupal multisite setups
* `./modules`: drupal modules, the actual **cdm_dataportal** module is found here. 
* `./scripts/composer`: scripts, installed by composer and manually added ones
* `./scripts/jenkins-ci`: scripts for testing, release, deployment in jenkins.
* `./site`: documentation source code, will be build by maven (see `pom.xml`)
* `./src`: java project with selenium tests
* `./themes`: drupal themes

**Generated folders which must not be comitted to the repository:**

* `./target`: target folder of maven, used for the side documentation (source in `./site`) and selenium tests (source in `./src`)
* `./screenshots`: screenshots taken during the selenium test execution
* `./vendor`: composer cache
* `./web`: final installation bundle of drupal 7 with modules, themes, profiles, etc. that is created by `composer update`

**Creation of the composer project**

The parent composer project (`./composer.json`) has been created using the official drupal project template:

~~~
composer create-project drupal-composer/drupal-project:7.x-dev -n dist
~~~

### On using composer with drupal projects

https://www.drupal.org/docs/develop/using-composer/using-composer-to-install-drupal-and-manage-dependencies

https://www.drupal.org/docs/develop/using-composer/using-composer-with-drupal

https://www.drupal.org/docs/develop/using-composer/managing-dependencies-for-a-custom-project

### Installation

**NOTE:** For detailed instructions which also cover the setup of apache and mysql please refer 
to: https://cybertaxonomy.org/dataportal/installation

#### Requirements

* mysql (v5.x) or MariaDB (v10.0 to v10.3) server, create a new user "drupaluser", the password should be the 
same as defined in setting.php and plugin='mysql_native_password'
* http server: apache or nginx; in this guide we will only cover the configuration of apache 2 (v 2.4)
* php 7, or php 5.6+ if php 7.x is not yet available for your system. php 8 will not work!
* java8 (https://dev.e-taxonomy.eu/redmine/projects/edit/wiki/Install_OracleJdk_on_Debian)

##### php

Install php 7.4 or 7.2 with the following extensions:

~~~
export PHP_VERSION=7.4 ; apt-get install php$PHP_VERSION php$PHP_VERSION-mysql php$PHP_VERSION-gd php$PHP_VERSION-json php$PHP_VERSION-curl php$PHP_VERSION-xml php$PHP_VERSION-mbstring zip unzip php$PHP_VERSION-zip  libapache2-mod-php$PHP_VERSION
~~~

For running the CDM Data Portal it is required to assign sufficient memory to php. 
Please open your `/etc/php/7.4/apache2/php.ini` and set the `memory_limit` parameter (memory_limit) to at least `128M`. 
The php.ini responsible for the php processes executed in apache is found in current Debian Linux and derivatives at
`/etc/php/7.4/apache2/php.ini` or `/etc/php/7.2/apache2/php.ini` .

~~~
;;;;;;;;;;;;;;;;;;;
; Resource Limits ;
;;;;;;;;;;;;;;;;;;;
memory_limit = 128M      ; Maximum amount of memory a script may consume (128MB)
~~~

##### Git

Git is needed for downloading (cloning) the  CDM Dataportal Drupal 7 installation package and to keep it up-to-date. 

~~~
sudo apt-get install git
~~~

#### Download & extract

Download the latest release from https://cybertaxonomy.org/download/dataportal/stable/ to the location where you want 
to install the cdm-dataportal Drupal 7 project e.g.

~~~
cd /var/www
wget https://cybertaxonomy.org/download/dataportal/stable/drupal-7-cdm-dataportal-5.23.0.tar.gz
~~~

This archive contains a shallow clone of the whole project together with a ready to use drupal 7 installation with the 
cdm-dataportal module, zen_dataportal theme and other requirements. The drupal-7 installation is in the sub folder `./web`

extract and adapt the ownership of the some folders:

~~~
tar -xzf drupal-7-cdm-dataportal-5.23.0.tar.gz
./drupal-7-cdm-dataportal/scripts/admin/fix-permissions.sh  --web-user www-data
~~~

##### Apache2 configuration

Install required modules

~~~
sudo a2enmod headers
sudo a2enmod ssl
sudo a2enmod rewrite
~~~


You may now want to copy the apache 2 site configuration files from `scripts/apache2.4/` to `/etc/apache2/sites-available/` 
and to activate one of them, preferably the ssl site configuration:

~~~
cd /var/www
sudo cp drupal-7-cdm-dataportal/scripts/apache2.4/dataportal.test* /etc/apache2/sites-available/
sudo a2ensite dataportal.test-ssl.conf
sudo a2ensite dataportal.test.conf
sudo systemctl restart apache2
~~~

You may now want to add the hostname to the `/etc/hosts` file:

~~~
echo "127.0.0.1 dataportal.test" | sudo tee -a  /etc/hosts
~~~

**NOTE**: The virtual host `dataportal.test` is only suitable for development purposes.
For production systems you will need to rename the virtual host so that it matches a public host name.

 Site installation

Now you are prepared to install a dataportal drupal site. 

If you want to use a local copy of an existing dataportal please follow the instructions in [On migrating Data Portal sites between servers](https://dev.e-taxonomy.eu/redmine/projects/edit/wiki/CdmDataportalSiteMigration)
to copy the dataportal files to your local develop environment.

If you want to start from scratch use the following instructions.

A template for the below script can be found in `scripts/user/`
Make an executable copy from `new-site.sh.template` as `new-site.sh` 

~~~
cp new-site.sh.template new-site.sh; chmod u+x new-site.sh
~~~

Adapt the below shown variables in the script to match your desired setup:

~~~
################################################################
## Configure below variables

SITE_NAME='test-site'

# HOST_NAME and PROTOCOL determine the base URL of the new site
# The default values will form the base URL like http://dataportal.test
# See also MULTI_SITE below
HOST_NAME='dataportal.test'
PROTOCOL='https' # values 'http' ot https'
# For MULTI_SITE=0 the site will be installed at the base BASE_URL
# In multisite setups (MULTI_SITE=1), however, the site URL results 
# in http://dataportal.test/test-site
# !! Mutisite support ist still experimental !!
MULTI_SITE=0 # values: 1 = true, 0 or other = false

ADMIN_USR='admin'
ADMIN_PWD='change--me'
ADMIN_EMAIL='admin@dataportal.test'

MYSQL_USR='root'
MYSQL_PWD='change--me'
DB_PREFIX='drupal7_dataportal_'


################################################################
~~~

Execute the script **from within the folder** `scripts/user/`, otherwise the `$DRUPAL_ROOT` variable will not match the `./web` folder !

Once the script has fished it will print out the final URL of the new site together with other useful information.

### Update - method 1

**Strategy**: Downloading of the installation package for a new release of the cdm-dataportal and replacing the old 
installation by the content of the installation package.

* Pro: Few simple steps.
* Con: The steps described here are not suitable for multi-site installation or when additional modules are installed.

~~~
cd /var/www
mv drupal-7-cdm-dataportal drupal-7-cdm-dataportal.last
~~~

remove old installation packages

~~~
rm drupal-7-cdm-dataportal*.tar.gz*
~~~

now follow the steps in the chapter **Download & extract** above.

finally copy the default site to the new installation 

~~~
cp -r drupal-7-cdm-dataportal.last/web/sites/default/ drupal-7-cdm-dataportal/web/sites/
~~~

apply any pending database updates

~~~
drush updatedb
drush cc all
~~~

Once you have confirmed that the updated installation is working correctly:

~~~
rm -r drupal-7-cdm-dataportal.last
~~~

### Update - method 2

**Strategy**: In-place updating of the installation by making use of git and drush or composer. 

*THIS METHOD IS RECOMMENDED FOR MOST SITUATIONS**

* Pro: Update of drupal and custom modules independent of what is provided by the installation package.
* Con: Initial preparation is more complex, 

#### Preparation

**Install composer v 1.10.x**

!!! *Composer v2.x would fail to preserve existing site installations* !!!

In case the version provided by apt is too old (<1.10) or too recent (>=2.0.0), you may want to install composer 
manually in the project directory `/var/www/drupal-7-cdm-dataportal`. 
In the following we assume you have installed composer this way and  will use `./composer` 
instead of `composer`.

~~~
cd /var/www/drupal-7-cdm-dataportal
php -r "copy('https://getcomposer.org/installer', 'composer-setup.php');"
~~~

Below command may fail when the `composer-setup.php` has changed after these lines have been written.
So you may receive the message "*Installer corrupt*". In this case, please update the corresponding 
command with the new one from [http://getcomposer.org/download/](http://getcomposer.org/download/). 

~~~
php -r "if (hash_file('sha384', 'composer-setup.php') === '906a84df04cea2aa72f40b5f787e49f22d4c2f19492ac310e8cba5b96ac8b64115ac402c8cd292b8a03482574915d1a8') { echo 'Installer verified'; } else { echo 'Installer corrupt'; unlink('composer-setup.php'); } echo PHP_EOL;"
php composer-setup.php
~~~

Now install the latest composer **1.x.x** version:
And clean up.

~~~
php composer-setup.php --filename=composer --version 1.10.25
php -r "unlink('composer-setup.php');"
~~~

**Un-shallow the installation package**

The installation package contains a shallow clone of the git repository only. In order to easily upgrade the 
dataportal-module it is highly recommended to un-shallow the git repository clone

~~~
git fetch --unshallow
git config remote.origin.fetch "+refs/heads/*:refs/remotes/origin/*"
git fetch origin
git checkout origin/master
~~~

**install dependencies**

This will also install **drush**

~~~
 ./composer install --no-dev
~~~

#### Updating to a specific cdm-dataportal release

NOTE: The modules and themes in the `./web/sites/all/` are sym-linked to the source code in `./modules` and `./themes`. 
Hence, updating the dataportal only involves checking out the according release from the git remote:

To see the list of available release tags:

~~~
git tag --list | egrep '[0-9]+\.[0-9]+\.[0-9]+' | sort -V
~~~

Checkout the release tag. For example to checkout the release `5.23.0`:

~~~
git checkout 5.23.0
scripts/admin/fix-permissions --web-user www-data
~~~

Git will respond with a warning that "*You are in 'detached HEAD' state.*", this is OK and no need to be concerned.

Finally apply any pending database updates and clear the cache. **NOTE**: Below we assume that the command is being 
executed in the installation package root e.g. `/var/www/drupal-7-cdm-dataportal` and that the site is available under 
https://dataportal.test. Please adapt to your specific settings if needed.

~~~
./vendor/drush/drush/drush -r /var/www/drupal-7-cdm-dataportal/web/ -l https://dataportal.test updatedb
./vendor/drush/drush/drush -r /var/www/drupal-7-cdm-dataportal/web/ -l https://dataportal.test cc all
~~~

#### Updating drupal and modules

**IMPORTANT**: *If the update-dependencies.sh fails at some point it is crucial to restore the backup that has been 
crated as first step in the update process. See below for details on restoring updates.**

The drupal-7-cdm-dataportal installer provides a script for convenient and secure updating of single or multisite setups.
It will 

1. create backups archives in `$HOME/drupal-cdm-dataportal-backups`
1. set the site(s) to maintenance mode   
1. Update drupal core and contributed modules via `composer`
1. Run any pending database updates and clear the caches

A brief help for this script is available from `scripts/admin/update-dependencies.sh --help` 

~~~
USAGE: update-dependencies.sh [--deactivate-install] [--multi-site] [--mailto <ADDRESS>]
  --deactivate-install :  The install.php will be hidden by appending '.off' to the filename
  -h, --help:  Print this help text
  --mailto <ADDRESS>:  send a email to the ADDRESS with a log of the update process
  --multi-site:  Do a multi-site update. Requires dataportals-drush. 
        See https://dev.e-taxonomy.eu/svn/trunk/server-scripts/dataportal-admin/
  --site-url:  The site url to be used with drush. This option disables the --multi-site option
~~~

Update a single site installation with default site URL

~~~
scripts/admin/update-dependencies.sh --deactivate-install 
~~~

Update a single site installation with custom site URL

~~~
scripts/admin/update-dependencies.sh --deactivate-install --site-url http://edit.test/d7/cichorieae/ 
~~~

Update a multi-site installation with custom site URL

~~~
scripts/admin/update-dependencies.sh --deactivate-install --multi-site
~~~

#### Recover from broken updates / Restore backups

**NOTE**: *The backup archives are tar files which unfortunately have colon characters in their name and thus can not 
be extracted by tar (tar interprets parts of the file name as host name and tries to restore the file on a remote 
machine). Therefore, the archive needs to be renamed or symlinked before extraction!*

Backup archive are found in `~/drupal-cdm-dataportal-backups`

~~~
cd /var/www 
ln -s ~/drupal-cdm-dataportal-backups/drupal-cdm-dataportal-backup-${timestamp}.tar.gz ./drupal-cdm-dataportal-backup.tar.gz
mv drupal-7-cdm-dataportal drupal-7-cdm-dataportal-old; mkdir drupal-7-cdm-dataportal; cd drupal-7-cdm-dataportal; tar -xzf ../drupal-cdm-dataportal-backup.tar.gz ./
~~~
