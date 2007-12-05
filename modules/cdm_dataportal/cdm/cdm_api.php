<?php
// $Id$

/**
 * @file
 * Functions which are required or useful when accessing and processing CDM Data Store Webservices
 * 
 * Naming conventions:
 * ----------------------
 * 
 *  - all webservice access methods are prefixed with cdm_ws
 */

/**
 * Enter description here...
 *
 * @param unknown_type $tag
 * @return unknown
 */
function cdm_taggedtext2html(array $taggedText, $tag = 'span'){
   foreach($taggedText as $class=>$value){
     $out .= '<'.$tag.' class="'.$class.'">'.$value.'</ '.$tag.'>';
   }
   return $out;
}

/**
 * The whatis service returns the type 
 * i.e. DTO class name and simplename & cdm class name and simplename of the instance referenced by the $uuid parameter. 
 * 
 *
 * @param unknown_type $uuid
 * @return false if the cdm store has no matching instance. An associative array with the following key, value pairs:
 *   - 'cdmName': name of the cdm class as returned by Class.getName()
 *   - ' 
 */
function cdm_ws_whatis($uuid){
  
}

/**
 * load a name from the CDM Webservice
 *
 * @param String $uuid
 * @return a NameTO instance or false 
 */
function cdm_ws_get_nameTO($uuid){
  
  $url = variable_get('cdm_webservice_url', '')."name/?uuid=$uuid";
  $name = simplexml_load_file(url);
  if(!$name){
    watchdog('CDM', "cdm_get_nameTO() - failed to load $url", WATCHDOG_ERROR);
  }
  return $name;
}

/**
 * load a list of names from the CDM Webservice
 *
 * @param unknown_type $page
 * @param unknown_type $hide_unaccepted
 */
function cdm_ws_list_names($page = 1, $hide_unaccepted){
   
}