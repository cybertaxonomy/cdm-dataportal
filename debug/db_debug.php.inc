<?php
/**
 * Created by PhpStorm.
 * User: andreas
 * Date: 11.01.18
 * Time: 12:39
 *
 * Execute .e.g as
 *
 *   php db_debug.php.inc sites/cyprus/settings.php
 *
 */

$setting_file = $argv[1];

print ("Running db_debug.php with settings from " . $setting_file . "\n");
require($setting_file);
print("db configuration:\n");
print_r($databases);
print ("trying to connect to default db ...\n");
$db_test_settings = $databases["default"]["default"];
$mysqli = mysqli_connect($db_test_settings["host"], $db_test_settings["username"], $db_test_settings["password"], $db_test_settings["database"]);
if ($mysqli->connect_errno) {
  echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
}
echo "Connection opened: " . $mysqli->host_info . "\n";

$res = $mysqli->query("SELECT * FROM " .$db_test_settings["prefix"]. "system");

echo "system table ...\n";
for ($row_no = $res->num_rows - 1; $row_no >= 0; $row_no--) {
  $res->data_seek($row_no);
  $row = $res->fetch_assoc();
  echo " filename = " . $row['filename'] . "\n";
}


