<?php

/**
 * This debug script can be used to debug multisite installations.
 * It will print out diagnostic information which can help debugging
 * site setup and configuration problems.
 * 
 * This script should be run with the drush scr (drush php-script) command
 * which allows the inclusion of a PHP script file to be loaded after the 
 * Drush bootstrap of the Drupal site.
 * 
 * INSTALLATION:
 * 	copy or link this file into the drupal root folder
 * 
 * USAGE:
 * 	In the drupal root folder execute the following drush command
 *  
 *   	drush -l ${site-ulr} scr ~/drupal_site_debug.php
 */
$NL="\n";

global $base_url, $base_path, $base_root;

require_once('includes/database/database.inc');

print("=============== GENERAL ===============" . $NL);
print("DRUPAL_ROOT: " . DRUPAL_ROOT . $NL);
print("base_url: " . $base_url . $NL);
print("base_path: " . $base_path . $NL);
print("base_root: " . $base_root . $NL);

$conf = false;

/* =============================================
   this section  is +/- a copy of conf_path()
   additional lines are marked with '// DEBUG customization'
*/

$confdir = 'sites';
$sites = array();
if (file_exists(DRUPAL_ROOT . '/' . $confdir . '/sites.php')) {
  // This will overwrite $sites with the desired mappings.
  include(DRUPAL_ROOT . '/' . $confdir . '/sites.php');
}

print (">>> sites.php included". $NL);  // DEBUG customization
print_r($sites);                        // DEBUG customization
print (">>> finding site folder". $NL); // DEBUG customization

$uri = explode('/', $_SERVER['SCRIPT_NAME'] ? $_SERVER['SCRIPT_NAME'] : $_SERVER['SCRIPT_FILENAME']);
$server = explode('.', implode('.', array_reverse(explode(':', rtrim($_SERVER['HTTP_HOST'], '.')))));
for ($i = count($uri) - 1; $i > 0; $i--) {
  if($conf) break;                      // DEBUG customization
  for ($j = count($server); $j > 0; $j--) {
    $dir = implode('.', array_slice($server, -$j)) . implode('.', array_slice($uri, 0, $i));
    if (isset($sites[$dir]) && file_exists(DRUPAL_ROOT . '/' . $confdir . '/' . $sites[$dir])) {
      $dir = $sites[$dir];
      print $NL . ("\ttrying dir $confdir/$dir") . $NL; // DEBUG customization
    }

    $site_folder = DRUPAL_ROOT . '/' . $confdir . '/' . $dir;  // DEBUG customization
    print "\t" . $site_folder . (file_exists($site_folder) ? ': OK' : ' : is missing!') . $NL; // DEBUG customization
    print "\t" . $site_folder . '/settings.php' . (file_exists($site_folder . '/settings.php') ? ': OK' : ' : is missing!') . $NL; // DEBUG customization

    if (file_exists(DRUPAL_ROOT . '/' . $confdir . '/' . $dir . '/settings.php') || (!$require_settings && file_exists(DRUPAL_ROOT . '/' . $confdir . '/' . $dir))) {
      $conf = "$confdir/$dir";
      print $NL . "\t site folder found: " . $conf; // DEBUG customization
      break;                                        // DEBUG customization
    }
  }
}
/* ============================================= */

print (">>> resulting site folder"  . $NL); // DEBUG customization
print("conf_path: " . conf_path(false) . $NL);

// TODO this is not working!
print "Database connection:" .$NL;
print_r(Database::getConnectionInfo());


print("=============== Request ===============" . $NL); 
print("q=" . $_REQUEST['q'] . $NL);