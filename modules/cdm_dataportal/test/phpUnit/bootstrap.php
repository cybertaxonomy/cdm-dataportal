<?php
//
// to be run from within {DRUPAL_ROOT}/sites/*/modules/cdm_dataportal/test/phpunit
//

print("> bootstrapping Druapl for phpUnit ...\n");
chdir ("../../../../../../"); // cd to {DRUPAL_ROOT}
require_once 'includes/bootstrap.inc';

$_SERVER['HTTP_HOST']='127.0.0.1';
$_SERVER['REQUEST_METHOD'] = 'get';
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['REQUEST_URI'] = "http://" . $_SERVER['HTTP_HOST'] . "/" . "cichorieae/";

drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

print("> bootstrapping done!\n");
