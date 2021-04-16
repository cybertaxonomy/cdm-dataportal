# CDM Data Portal

Drupal 7 modules and themes as web frontend to publish data hosted in a [cdm server](https://github.com/cybertaxonomy/cdm-server).

This project has a monolithic strucure, that is it contains multiple projects in one git repository.

Project dependencies required for bundeling the final insallation package are managed via composer. Dependencies of the sub projects like *modules/cdm_dataportal*, *themes/zen_dataportal*, etc still have manually copied dependencies. 

## Project structure

* `./drush`: drush, as installed by composer
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

Download the latest release from https://cybertaxonomy.eu/download/dataportal/stable/ to the location where you want to install the cdm-dataportal drupal 7 project e.g.

~~~
cd /var/www
wget https://cybertaxonomy.eu/download/dataportal/stable/drupal-7-cdm-dataportal-5.22.0.tar.gz
~~~

This archive contains a shallow clone of the whole project together with a ready to use drupal 7 installation with the cdm-dataportal module, zen_dataportal theme and other requirements. The drupal-7 installation is in the sub folder `./web`

extract and adapt the owner ship of the `./web` sub folder:

~~~
tar -xzf drupal-7-cdm-dataportal-5.22.0.tar.gz
sudo chown -R :www-data drupal-7-cdm-dataportal/web
~~~

Install a dataportal drupal site 

A template for the below sctipt can be found in `scripts/user/`
Make anm executable copy from `new-site.sh.template` as `new-site.sh` 

~~~
cp new-site.sh.template new-site.sh; chmod 775 new-site.sh
~~~

Adapt the variables to match your desired setup:

~~~
export SITE_NAME='test-site'

export ADMIN_USR='admin'
export ADMIN_PWD='change--me'
export ADMIN_EMAIL='admin@dataportal.test'

export MYSQL_USR='root'
export MYSQL_PWD='change--me'

export DB_PREFIX='drupal7_dataportal_'
export MYSQL_URL="mysql://${MYSQL_USR}:${MYSQL_PWD}@localhost/${DB_PREFIX}${SITE_NAME}"

export BASE_URL='http://dataportal.test/'
~~~~

And execurte the script from within the folder `scripts/user/`, other wise the $DRUPAL_ROOT variable will not match the `./web` folder !














