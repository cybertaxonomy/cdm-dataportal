This directory contains:
------------------------
build.xml - a phing script to create the CDM and Drupal Portal databases from the .gz dump files
build.properties.tpl - template to setup the build

Usage:
------------------------
these scripts are used by ETI to quickly import all CDM and Drupal databases from dump files.
The scripts ensure that import is always done the same way (databases named the same).

Steps:
------------------------
1. install Phing (with PHP-PEAR), see http://www.phing.info/trac/
2. login to http://wp5.e-taxonomy.eu/download/internal/eti-dataportal/ and download the .gz dumps
3. copy the build.properties.tpl template to build.properties and adjust to your needs
4. from the command line do: "phing build" or "phing install_databases"
