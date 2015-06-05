-------------------------------------------------------------------------------
                            Drupal_CDM Code Sniffer
-------------------------------------------------------------------------------

Use this sniff for Drupal CDM Portals.

This sniff is based on Drupal Code Sniffer, but has the check for camel case
variables disabled, since DRUPAL CDM Portals use variable names that should be
the same as in the CDM Server Java implementation.

Drupal Code Sniffer (drupalcs) is a coding standard validation tool for Drupal
and contributed modules/themes.

Installation
------------

Requirements:
  - PEAR
  - PHPCS
  - Drupal sniff

- Install PEAR  ( http://pear.php.net/manual/en/installation.php )
- Install PHPCS ( http://pear.php.net/package/PHP_CodeSniffer )
- Install drupalcs sniff (http://drupal.org/node/1552878)
- Sym-link the drupalcs directory into the standards folder for PHP_CodeSniffer.
  The code for that looks like this:
$> sudo ln -sv /path/to/drupalcs/Drupal $(pear config-get php_dir)/PHP/CodeSniffer/Standards 
- Install this Drupal_CDM sniff
- Sym-link the Drupal_CDM directory into the standards folder for PHP_CodeSniffer.
  The code for that looks like this:
$> sudo ln -sv /path/to/drupalcs/Drupal_CDM $(pear config-get php_dir)/PHP/CodeSniffer/Standards 

Usage (running in a shell)
--------------------------

$> phpcs --standard=Drupal_CDM --extensions=php,module,inc,install,test,profile,theme /path/to/cdm_dataportal_drupal_module

Working with Editors
--------------------
Drupal Code Sniffer can be used with various editors.

Editors:

eclipse: http://drupal.org/node/1420004
Komodo: http://drupal.org/node/1419996
Netbeans: http://drupal.org/node/1420008
Sublime Text: http://drupal.org/node/1419996
vim: http://drupal.org/node/1419996

