<?php

class ModuleTests extends PHPUnit_Framework_TestCase {

  function test_load_polytomousKey() {

    $polytomousKeysPager = cdm_ws_get(CDM_WS_POLYTOMOUSKEY, $polytomousKeyUuid);

    print("\n\n<h4>Benchmarking web services: deep initialization vs. bit-by-bit initialization</h4>\n"
		. $polytomousKeysPager->count . " PolytomousKeys to load.\n");

    print("<table border=\"1\">\n<tr><th>key uuid</td><td>bit-by-bit initialization</td><td>deep initialization</td></tr>\n");
    flush();

    $time_load_bbb_total = 0;
    $time_load_deep_total = 0;
    $i = 0;
    foreach ($polytomousKeysPager->records as $polytomousKey) {

      if (! is_uuid($polytomousKey->uuid)) {
        continue;
      }
      //			if($i++ == 4){

      //				break;

      //			}


      print("<tr><td>$polytomousKey->uuid</td>");
      // ---- bit-by-bit ---- //

      $time_load_start = microtime(true);
      $polytomousKey = cdm_ws_get(CDM_WS_POLYTOMOUSKEY, $polytomousKey->uuid);
      _load_polytomousKeySubGraph($polytomousKey->root);
      $time_load = microtime(true) - $time_load_start;
      //			if($i++ == 1){

      //				var_dump($polytomousKey);

      //			}

      $time_load_bbb_total += $time_load;
      print("<td>" . sprintf('%3.3f', $time_load) . "s</td>");

      // ---- deep ---- //

      $time_load_start = microtime(true);
      $polytomousKey = cdm_ws_get("portal/" . CDM_WS_POLYTOMOUSKEY, array($polytomousKey->uuid, "loadWithNodes"));
      $time_load = microtime(true) - $time_load_start;
      //			if($i++ == 1){

//        var_dump($polytomousKey);

//      }

      $time_load_deep_total += $time_load;
      print("<td>" . sprintf('%3.3f', $time_load) . "s</td></tr>\n");

    }
    print("<tr><td>total time</td><td>" . sprintf('%3.3f', $time_load_bbb_total) . "s</td><td>" . sprintf('%3.3f', $time_load_deep_total) . "s</td></tr>\n");
    flush();

  }

}
