# CDM Data Portal

Drupal 7 modules and themes as web frontend to publish data hosted in a [cdm server](https://github.com/cybertaxonomy/cdm-server).

This project has a monolithic strucure, that is it contains multiple projects in one git repository.

Project dependencies required for bundeling the final insallation package are managed via composer. Dependencies of the sub projects like *modules/cdm_dataportal*, *themes/zen_dataportal*, etc still have manually copied dependencies. 

## Project structure

* `./drush`: drush, as installed by composer
* `./debug`: scripts that help debugging problems with drupal multisite setups
* `./modules`: drupal modules, the actual **cdm_dataportal** module is found here. 
* `./scripts`: scripts, installed by composer and manually added ones
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




