<?php

/**
 * test to test if phpUnit is ok
 * @author a.kohlbecker
 *
 */
class ShortnameTest extends PHPUnit_Framework_TestCase {

  /*
   --------'BACILLARIOPHYCEAE (FOSS.)'---------
--------'BACILLUS OBSCURUS (FOSS.)'---------
>>> B. OBSCURUS (FOSS.)
--------'Lapsana communis L.'---------
>>> L. communis L.
--------'Lapsana L.'---------
--------'Lapsana communis subsp. adenophora (Boiss.) Rech.'---------
>>> L. communis subsp. adenophora (Boiss.) Rech.
--------'Asterolampra marylandica'---------
>>> A. marylandica
--------'Ophrys ×"kreutziana"'---------
>>> O. ×"kreutziana"
--------'Ophrys bornmuelleri subsp. bornmuelleri × subsp. grandiflora'---------
>>> O. bornmuelleri subsp. bornmuelleri × subsp. grandiflora
--------'Aegilops ×insulae-cypri'---------
>>> A. ×insulae-cypri

   */

    function test_cdm_dataportal_shortname_of() {
      $testStrings = array(
      	"BACILLARIOPHYCEAE (FOSS.)" => "BACILLARIOPHYCEAE (FOSS.)",
      	"BACILLUS OBSCURUS (FOSS.)" => "B. OBSCURUS (FOSS.)",
      	"Lapsana communis L." => "L. communis L.",
      	"Lapsana L." => "Lapsana L.",
      	"Lapsana communis subsp. adenophora (Boiss.) Rech." => "L. communis subsp. adenophora (Boiss.) Rech.",
      	"Asterolampra marylandica" => "A. marylandica",
          "Ophrys ×\"kreutziana\"" => "O. ×\"kreutziana\"",
          "Ophrys bornmuelleri subsp. bornmuelleri × subsp. grandiflora" => "O. bornmuelleri subsp. bornmuelleri × subsp. grandiflora",
          "Aegilops ×insulae-cypri" => "A. ×insulae-cypri"
      );

      foreach ($testStrings as $nameString => $shortname){
        $tagtxt->type = "name";
        $tagtxt->text = $nameString;
        $name->taggedTitle = array($tagtxt);
        $result = cdm_dataportal_shortname_of($name);

        $this->assertEquals($shortname, $result);
      }
    }
}