<?php

/**
 * This script converts the terms of the 
 * "World Geographical Scheme for recording plant distributions" 
 * avilable as text delimited files from  
 * http://www.nhm.ac.uk/hosted_sites/tdwg/geo2.htm into
 * an taxomomy.xml file which can be imported into Drupal5
 * as a vocabulary. 
 * The resulting xml file can be imported into Drupal by 
 * the taxonomy_xml module (http://drupal.org/project/taxonomy_xml).
 * 
 * Since the "World Geographical Scheme" consusts of 4 separate tables
 * each for one level of geographical detail this script takes 4 steps to import 
 * the according text delimited files.
 * 
 * Direct download link to the text delimited files packed ad zip archive:
 * http://www.nhm.ac.uk/hosted_sites/tdwg/geography_ed2.zip
 * 
 * USAGE INSTRUCTIONS
 * =======================================
 * 1. Download geography_ed2.zip from the URL noted above
 * 2. Edit the variable $zip_file
 * 3. Run the script from your browser.
 */



$zip_file = "geography_ed2.zip";

$termfiles = array('tblLevel1.txt', 'tblLevel2.txt', 'tblLevel3.txt', 'tblLevel4.txt');

$vid = 20;

//-------------------------------------------------------------------------

$xml_header = '<?xml version="1.0" standalone="no"?>
<!DOCTYPE taxonomy SYSTEM "taxonomy.dtd">
<vocabulary>
<vid>'.$vid.'</vid>
<name>expertdb_georegions</name>
<description>Geographical region terms compatible with the TDWG GeographicalRegions LSID vocabulary (http://rs.tdwg.org/ontology/voc/GeographicRegion.rdf).
The regions are structured in a hierarchy having four levels of detail.</description>
<help></help>
<relations>0</relations>
<hierarchy>1</hierarchy>
<multiple>1</multiple>
<required>0</required>
<nodes></nodes>
<weight>0</weight>';

$xml_footer = '</vocabulary>';

class Term{
  
  var $tid, $vid, $name, $description = '', $synonyms = '', $weight = 0, $depth = 0, $parent = 0;

  function print_xml(){
   //  is currently ommittted !!!
    print ("<term><tid>$this->tid</tid><vid>$this->vid</vid><name>$this->name</name><description>$this->description</description><synonyms>$this->synonyms</synonyms><weight>$this->weight</weight><depth>$this->depth</depth><parent>$this->parent</parent></term>\n");
  }

}

/**
 * Enter description here...
 *
 * @param unknown_type $line
 * @param unknown_type $code_idx
 * @param unknown_type $name_idx
 * @param unknown_type $parentcode_idx
 * @param unknown_type $synonym_idx
 * @param unknown_type $description_idx
 * @return unknown
 */
function addTerm($line, $code_idx, $name_idx, $parentcode_idx = false, $synonym_idx = false, $description_idx = false){
  global $depth, $vid, $tid, $terms;
  
  $tok = explode('*', $line);
  //print(count($tok).'\t');

  if(!$tok || count($tok) < 2){
     return false; 
  }
  
  $term = new Term();
  $term->tid = $tid++;
  $term->vid = $vid;
  $term->depth = $depth;

  $term->name = mb_convert_encoding($tok[$name_idx], 'UTF-8', 'latin1');
    
  //print ($line."\n");
  if($parentcode_idx){
    $parentterm = $terms['L'.($depth - 1).'_'.$tok[$parentcode_idx]];
    $term->parent = $parentterm->tid;
  }

  if($synonym_idx && count($tok) > $synonym_idx){
    $term->synonyms = mb_convert_encoding ($tok[$synonym_idx], 'UTF-8', 'latin1');;
  } 
  
  if($description_idx && count($tok) > $description_idx){
    $term->description = mb_convert_encoding ($tok[$description_idx], 'UTF-8', 'latin1');
  }
  
  $terms['L'.$depth.'_'.$tok[$code_idx]] = $term;
  
}

// open zip file

$zip = new ZipArchive;

if ($zip->open($zip_file) === false) {
  print('ERROR: invalid variable $zip_file: ['.$zip_file.'] Please read the instructions in the php script.');
  exit(-1);
}

// read all terms as Term instances into a associative array using the code fields as key
$terms = array();

$tid = 20000;

// LEVEL 0:
// 	root term 'Terrestrial'
$depth = 0;
addTerm('_ROOT_*Terrestrial', 0, 1);


// LEVEL 1:
// 	L1 code*L1 continent
$depth += 1;
$text = $zip->getFromName($termfiles[$depth - 1]);
$lines = explode("\n", $text);

for($i = 1; $i < count($lines); $i++){
  if( strlen(trim($lines[$i])) > 0){
    $l = trim($lines[$i]).'*_ROOT_';
    addTerm($l, 0, 1, 2);
  }
}

// LEVEL 2:
// 	L2 code * L2 region * L1 code * L2 ISOcode
$depth += 1;
$text = $zip->getFromName($termfiles[$depth - 1]);
$lines = explode("\n", $text);

for($i = 1; $i < count($lines); $i++){
  addTerm($lines[$i], 0, 1, 2, 3, false);
}

// LEVEL 3:
// 	L3 code * L3 area * L2 code * L3 ISOcode * Ed2status*Notes

$depth += 1;
//print("LEVEL 3:$termfiles[$depth]\n");
$text = $zip->getFromName($termfiles[$depth - 1]);
$lines = explode("\n", $text);

//print($lines[0]);
for($i = 1; $i < count($lines); $i++){
  addTerm($lines[$i], 0, 1, 2, 3, 5);
}

// LEVEL 4:
// 	L4 code*L4 country*L3 code*L4 ISOcode*Ed2status*Notes
$depth += 1;
$text = $zip->getFromName($termfiles[$depth - 1]);
$lines = explode("\n", $text);

for($i = 1; $i < count($lines); $i++){
  addTerm($lines[$i], 0, 1, 2, 3, 5);
}


$zip->close();

header("Content-Type: text/xml; charset=UTF-8");
header("Content-Type: text/xml");
print ($xml_header);
foreach ($terms as $t) {
	$t->print_xml();
}
print ($xml_footer);

?>