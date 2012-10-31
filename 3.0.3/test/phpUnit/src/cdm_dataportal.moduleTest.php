<?php

class ModuleTests extends PHPUnit_Framework_TestCase {

	function test_load_polytomousKey() {

		$polytomousKeysPager = cdm_ws_get(CDM_WS_POLYTOMOUSKEY, $polytomousKeyUuid);

		print("\n\nBenchmarking web services: deep initialization vs. bit-by-bit initialization\n"
		. $polytomousKeysPager->count . " PolytomousKeys to load.\n");

		print("1) bit-by-bit initialization ...\n");
		flush();
		$time_load_total = 0;
		$i = 0;
		foreach($polytomousKeysPager->records as $polytomousKey){
			if(! is_uuid($polytomousKey->uuid)){
				continue;
			}
			$time_load_start = microtime(true);
			$polytomousKey = cdm_ws_get(CDM_WS_POLYTOMOUSKEY, $polytomousKey->uuid);
			_load_polytomousKeySubGraph($polytomousKey->root);
			$time_load = microtime(true) - $time_load_start;
			print($polytomousKey->uuid . " :\t" . sprintf('%3.3f', $time_load). "s\n");
			if($i++ == 1){
				var_dump($polytomousKey);
			}
			$time_load_total += $time_load;
		}
		flush();
		print("total time = " . sprintf('%3.3f', $time_load_total) . "s\n");


		print("\n2) deep initialization ...\n");
		flush();
		$time_load_total = 0;
		$i = 0;
		foreach($polytomousKeysPager->records as $polytomousKey){
		  if(! is_uuid($polytomousKey->uuid)){
        continue;
      }
      $time_load_start = microtime(true);
			$polytomousKey = cdm_ws_get("portal/" . CDM_WS_POLYTOMOUSKEY, array($polytomousKey->uuid, "loadWithNodes"));
			$time_load = microtime(true) - $time_load_start;
			print($polytomousKey->uuid . " :\t" . sprintf('%3.3f', $time_load). "s\n");
			if($i++ == 1){
        var_dump($polytomousKey);
      }
			$time_load_total += $time_load;
		}
		print("total time = " . sprintf('%3.3f', $time_load_total) . "s\n");
		flush();

	}

}