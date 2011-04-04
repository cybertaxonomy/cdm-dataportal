<?php

define("TEST_BASE_FOLDER", getcwd());
define("TEST_RESOURCES_FOLDER", TEST_BASE_FOLDER .DIRECTORY_SEPARATOR . 'resources'. DIRECTORY_SEPARATOR);

class TestUtils {
	static function load_from_json_resource ($fileInTestResources){

		if(DIRECTORY_SEPARATOR == '/'){
		  $fileInTestResources = str_replace("\\", DIRECTORY_SEPARATOR, $fileInTestResources);
		} else {
			$fileInTestResources = str_replace("/", DIRECTORY_SEPARATOR, $fileInTestResources);
		}

		$absoluteFilePath = TEST_RESOURCES_FOLDER .  $fileInTestResources;
	   if(!is_file($absoluteFilePath)){
	   	print ("ERROR: File '$absoluteFilePath' does not exist.");
	   }
	   if(!is_readable($absoluteFilePath)){
	   	print ("ERROR: File '$absoluteFilePath' is not readable");
	   }

		$datastr = file_get_contents($absoluteFilePath);
		if(!is_string($datastr)){
			print ("ERROR: File '$absoluteFilePath' is empty: $datastr");
		}

		$obj = json_decode($datastr);

		if(!$obj){
      print ("ERROR: File '$absoluteFilePath' contains invalid json");
    }
	  return $obj;
  }
}