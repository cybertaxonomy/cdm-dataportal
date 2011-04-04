<?php
//
// to be run from within {DRUPAL_ROOT}/sites/*/modules/cdm_dataportal/test/phpunit
//
// see http://pkp.sfu.ca/wiki/index.php/Configure_Eclipse_for_PHPUnit#Configure_XDebug_Debugger_to_work_with_PHPUnit
//
print("> bootstrapping Drupal for phpUnit ...\n");

//$phpUnitTestBaseDir = "test".DIRECTORY_SEPARATOR."phpUnit";
//
//while(!str_endsWith( getcwd(), $phpUnitTestBaseDir) || strlen(getcwd()) < strln($phpUnitTestBaseDir)){
//
//}

print(">".getcwd()."\n");

// TestUtils.php must be included at the very first step
require_once ('TestUtils.php');

chdir ("../../../../../../"); // cd to {DRUPAL_ROOT}
require_once  ('includes/bootstrap.inc');

$_SERVER['HTTP_HOST']='127.0.0.1';
$_SERVER['REQUEST_METHOD'] = 'get';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['REQUEST_URI'] = "http://" . $_SERVER['HTTP_HOST'] . "/" . "cichorieae/";

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

print("> bootstrapping done!\n");
