This directory contains:
------------------------
build.xml - a phing script to dump Drupal 7 Portal databases to zipped sql dump files
build.properties.tpl - template to setup the build
aliases.drushrc.php - example drush alias file

Requirements:
------------------------
Phing, Drush, gzip

Usage:
------------------------
these scripts are used by ETI to quickly export all Drupal 7 databases to store in SVN or WebDAV.
The scripts ensure that export is always done the same way (databases named the same).
Cache and log tables are emptied before exporting.
The scripts can be used to export individual dbs or all 6 portal databases.
For WebDAV a date-time is added to the filename, format is 2012-01-01_01h01m01s_<databasename>.sql.gz
For SVN the format is <databasename>.sql.gz

Steps:
------------------------
1. install Phing (with PHP-PEAR), see http://www.phing.info/trac/
2. install Drush and create an aliases.drushrc.php alias file for the portals (see the example file). 
    Put it in the Drush folder (C:\ProgramData\Drush on Windows).
3. copy the build.properties.tpl template to build.properties and adjust to your needs
4. from the command line do: 
    a. for dumping all databases to both folders: "phing build" or "phing export_all"
    b. for dumping all databases to your webdav folder: "phing export_all_to_webdav" 
    c. for dumping all databases to your svn folder: "phing export_all_to_svn"    
    d. for dumping individual database to svn folder: "phing export_database_to_svn -Ddb=<databasename>" (e.g. palmae)
        Dumping without specifying a <databasename> dumps the default database specified in build.properties
    e. for dumping individual database to webdav folder: "phing export_database_to_webdav -Ddb=<databasename>"
    f. for dumping individual database to both folders: phing export_database -Ddb=<databasename>"
