<?php

if(empty($_ENV['LD_LIBRARY_PATH'])){
  print('environment variable "LD_LIBRARY_PATH" should contain the path to the php executables.');
  exit(-1);
}

$php_bin= $_ENV['LD_LIBRARY_PATH'];

// assemble the required command arguments for to be delegated to phpunit
// we have to skip the first arguemnt since this is containing this very
// script!
$args = $GLOBALS['argv'];
array_shift($args);
$arg_str = implode(' ', $args);

// execute phpunit, we are using here the backtick operator:
// see http://php.net/manual/language.operators.execution.php
$execute = `$php_bin/phpunit $arg_str`;
echo $execute;
