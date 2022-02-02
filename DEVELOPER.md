# Development of the EDIT Data Portal

*Mainly information on setting up the development environment for working with phpStorm.*

## Installing the drupal cdm_dataportal package

**NOTE**: Before starting with the installation, it is highly recommended that you read this chapter to the end. Since
the installation method for development environment differs in some details.

Follow the instructions in the [README.md](README.md) and make sure you are using the
Update - method 2](README.md##update---method-2), but respect the below recommended **2** deviations:

**1.**

For development you may want to choose different folder for installing the cdm-dataportal Drupal 7 project, 
than suggested in the [README.md](README.md). You could, for example, replace the commands given in the
[Download & extract](README.md#download--extract) chapter by:

~~~
mkdir -p ~/workspace
cd ~/workspace
wget https://cybertaxonomy.eu/download/dataportal/stable/drupal-7-cdm-dataportal-5.23.0.tar.gz
~~~

... and continue in the README.md at the corresponding position.
Now, you will of course need to replace all `cd /var/www` commands by `cd ~/workspace` !!!

**2.**

In the chapter [Apache2 configuration](README.md#apache2-configuration) you will need to change the 
`/etc/apache2/sites-available/` to adapt it to the different installation folder, and you also want to enable the
`AliasMatch` directive to be able to work with multiple sites in parallel:

Assuming your user-name is `andreas` and you have installed the cdm-dataportal Drupal 7 project in `/home/andreas/workspace/`
you will need to change three lines int the apache config file to. (Last line in no longer commented!).

~~~
DocumentRoot /home/andreas/workspace/drupal-7-cdm-dataportal/web
<Directory "/home/andreas/workspace/drupal-7-cdm-dataportal/web/">
AliasMatch ^/([^/]+)(.*)		/home/andreas/workspace/drupal-7-cdm-dataportal/web/$2
~~~~

Once you have completed the installation, you might have created a new dataportal site to start with, or you have cloned
a site according to the instructions in developer wiki page 
[On migrating Data Portal sites between servers](https://dev.e-taxonomy.eu/redmine/projects/edit/wiki/CdmDataportalSiteMigration)

In case you have **problems** to get your dataportal site running please read the hints in the 
[**troubleshooting**](https://dev.e-taxonomy.eu/redmine/projects/edit/wiki/CdmDataportalSiteMigration##Troubleshooting) 
section of above linked page.

## Install Xdebug

install Xdebug:

~~~
pecl install xdebug
~~~

at the end of the install-log, pecl shows something like:

~~~
Build process completed successfully
Installing '/usr/lib/php/20151012/xdebug.so'
install ok: channel://pecl.php.net/xdebug-2.6.0
configuration option "php_ini" is not set to php.ini location
You should add "zend_extension=/usr/lib/php/20151012/xdebug.so" to php.ini
~~~

add xdebug to the `php.ini`, e.g.: `/etc/php/7.4/apache2/php.ini` Use the `xdebug.so` installation location printed out 
in by pecl.

~~~
zend_extension=/usr/lib/php/20151012/xdebug.so
xdebug.idekey=PHPSTORM
xdebug.remote_enable=1
xdebug.remote_port=9008
~~~

Restart apache  to apply the settings 

~~~
systemctl restart apache2
~~~

## PhpStorm

Start phpStorm and import the whole cdm-dataportal Drupal 7 project which is for example installed in 
`/home/andreas/workspace/drupal-7-cdm-dataportal/`

![](images/phpstorm-new-project-from-existing-files.png)

**NOTE**: Since the project contains many libraries, phpStorm will take a couple of minutes (>10 minutes) to index all code. 
Please wait until the indexing has finished.

Once phpStorm has finished the whole import task, two messages will pop up. Once about phpStorm having set the php version for 
the project to a different version than specified in the project config, and another on the Drupal support.

![](images/phpstorm-drupal-support-popup.png)

Click on "Enable Drupal Support" and check all options in the following dialog:

![](images/phpstorm-enable-drupal-support-dialog.png)



