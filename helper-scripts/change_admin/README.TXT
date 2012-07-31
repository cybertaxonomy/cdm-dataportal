This directory contains:
------------------------
build.xml - a phing script to dump Drupal 7 Portal databases to zipped sql dump files
build.properties.tpl - template to setup the build


Requirements:
------------------------
Phing, Drush, Mysql, PHP5.2+

Usage
------------------------
These scripts will change the Drupal admin (uid=1) from the original EDIT admin to admin/admin for development and back.
This is done so there is an admin that can be used during development, and to change it back to the original admin user
when development is finished.

Steps:
------------------------
1. install Phing (with PHP-PEAR), see http://www.phing.info/trac/
2. install Drush and create an aliases.drushrc.php alias file for the portals (see the example file). 
    Put it in the Drush folder (C:\ProgramData\Drush on Windows).
3. copy the build.properties.tpl template to build.properties and adjust to your needs
4. from the command line do: 

 a. "phing to_dev_admin" to change to admin for development
 b. phing build" or "phing to_orig_admin" to change back to original admin user