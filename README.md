# CDM Data Portal

Drupal 7 modules and themes as web frontend to publish data hosted in a [cdm server](https://github.com/cybertaxonomy/cdm-server).

This project has a monolithic structure, that is it contains multiple projects in one git repository.

Project dependencies required for bundling the final installation package are managed via composer. 
Dependencies of sub-projects like *modules/cdm_dataportal*, *themes/zen_dataportal*, etc still have manually copied dependencies. 

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
to: https://cybertaxonomy.eu/dataportal/installation

#### Download & extract

Download the latest release from https://cybertaxonomy.eu/download/dataportal/stable/ to the location where you want 
to install the cdm-dataportal Drupal 7 project e.g.

~~~
cd /var/www
wget https://cybertaxonomy.eu/download/dataportal/stable/drupal-7-cdm-dataportal-5.23.0.tar.gz
~~~

This archive contains a shallow clone of the whole project together with a ready to use drupal 7 installation with the 
cdm-dataportal module, zen_dataportal theme and other requirements. The drupal-7 installation is in the sub folder `./web`

extract and adapt the ownership of the some folders:

~~~
tar -xzf drupal-7-cdm-dataportal-5.23.0.tar.gz
./drupal-7-cdm-dataportal/scripts/admin/fix-permissions.sh  --web-user www-data
~~~

##### Apache2 configuration

You may now want to copy the apache 2 site configuration files from `scripts/apache2.4/` to `/etc/apache2/sites-available/` 
and to activate one of them, preferably the ssl site configuration:

~~~
sudo cp drupal-7-cdm-dataportal/scripts/apache2/dataportal.test* /etc/apache2/sites-available/
sudo a2ensite dataportal.test-ssl.conf
sudo systemctl restart apache2
~~~

##### Site installation

Now you are prepared to install a dataportal drupal site. 

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

In case the version provided by apt is too old (<1.10), you can install composer manually in the project 
directory `/var/www/drupal-7-cdm-dataportal`. In the following we assume you have installed composer this way and 
will use `./composer` instead of `composer`.

~~~
cd /var/www/drupal-7-cdm-dataportal
php -r "copy('https://getcomposer.org/installer', 'composer-setup.php');"
php -r "if (hash_file('sha384', 'composer-setup.php') === '756890a4488ce9024fc62c56153228907f1545c228516cbf63f885e036d37e9a59d27d63f46af1d4d07ee0f76181c7d3') { echo 'Installer verified'; } else { echo 'Installer corrupt'; unlink('composer-setup.php'); } echo PHP_EOL;"
php composer-setup.php --filename=composer --version 1.10.22
php -r "unlink('composer-setup.php');"
~~~

The above commands fails with "Installer corrupt"? Please update the corresponding command with the new one from [http//getcomposer.org/download/](http//getcomposer.org/download/). It is **important not use use a version > 1.*!**

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

The drupal-7-cdm-dataportal installer provides a script for convenient and secure updating of single or multisite setups.
It will 

1. create backups
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
