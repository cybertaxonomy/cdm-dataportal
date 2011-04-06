<?php
//
// to be run from within {DRUPAL_ROOT}/sites/*/modules/cdm_dataportal/test/phpunit
//
// see http://pkp.sfu.ca/wiki/index.php/Configure_Eclipse_for_PHPUnit#Configure_XDebug_Debugger_to_work_with_PHPUnit
//

// TestUtils.php must be included at the very first step
require_once ('TestUtils.php');

print("> bootstrapping Drupal for phpUnit ...\n");

//print(">".getcwd()."\n");


chdir ("../../../../../../"); // cd to {DRUPAL_ROOT}
require_once  ('includes/bootstrap.inc');

$_SERVER['HTTP_HOST']='127.0.0.1';
$_SERVER['REQUEST_METHOD'] = 'get';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['PHP_SELF'] = $_SERVER['SITE_BASE_PATH']."/index.php";
//$_SERVER['REQUEST_URI'] = "http://" . $_SERVER['HTTP_HOST'] . "/" . "flora-malesiana/";

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

print("> bootstrapping done!\n");

flush();

