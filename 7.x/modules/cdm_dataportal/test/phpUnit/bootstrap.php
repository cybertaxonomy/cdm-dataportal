<?php
//
// see README contained in this folder

//

// TestUtils.php must be included at the very first step

module_load_include('php', 'bootstrap', 'estUtils');

if (empty($_ENV['DRUPAL_ROOT'])) {
  print('environment variable "DRUPAL_ROOT" mnust point to the root of the Drupal installation.');
  exit(-1);
}
chdir($_ENV['DRUPAL_ROOT']); // cd to {DRUPAL_ROOT}



module_load_include('inc', 'bootstrap', 'bootstrap');

$_SERVER['HTTP_HOST'] = '127.0.0.1';
$_SERVER['REQUEST_METHOD'] = 'get';
//ip_address('127.0.0.1') ;

$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['PHP_SELF'] = $_SERVER['SITE_BASE_PATH'] . "/index.php";
//$_SERVER['REQUEST_URI'] = "http://" . $_SERVER['HTTP_HOST'] . "/" . "flora-malesiana/";


drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

flush();

