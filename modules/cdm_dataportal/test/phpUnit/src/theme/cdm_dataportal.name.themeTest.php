<?php

class NameThemeTests extends PHPUnit_Framework_TestCase {

    function test_theme_cdm_taxonName() {
      $taxon1_file = "theme/cdm_dataportal.name.themeTest-taxon1.json";
      $taxon = TestUtils::load_from_json_resource($taxon1_file);
      $this->assertEquals($taxon->uuid, "0ae4f7ab-f482-482c-ba57-cf0e4389a417");
      $out = theme_cdm_taxonName($taxon->name);
      print($out);
    }

}