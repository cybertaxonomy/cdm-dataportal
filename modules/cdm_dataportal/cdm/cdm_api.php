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
 * Loads the XML response for the given url from the CDM Data Store Webservice.
 * The XML is turned into a object wich is retuned. Incase of an error a 
 * approriate watchdog message is generated and the function returns false.
 * 
 * //TODO are we going to support JSON services ?
 *
 * @param String $url the relative url of the web service call. 
 *        Relative means relative to the web service base url which is stored in cdm_webservice_url
 * @return An object or false
 */
function cdm_ws_load($url){

  $obj = simplexml_load_file(variable_get('cdm_webservice_url', '').$url);
  if(!$obj){
    $backtrace = debug_backtrace();
    watchdog('CDM', $backtrace[1]['function'].' - failed to load '.$url, WATCHDOG_ERROR);
  }
  return $obj;
}

/**
 * The whatis service returns the type 
 * i.e. DTO class name and simplename & cdm class name and simplename of the instance referenced by the $uuid parameter. 
 * 
 *
 * @param unknown_type $uuid
 * @return false if the cdm store contains no matching instance. 
 * An associative array with the following key-value pairs:
 *   - 'cdmName':       name of the cdm class as returned by Class.getName(), e.g. eu.etaxonomy.cdm.model.taxon.Taxon
 *   - 'cdmSimpleName': simple name of the cdm class as returned by Class.getSimpleName(), e.g. Taxon
 *   - 'dtoName':       name of the DTO class as returned by Class.getName(), e.g. eu.etaxonomy.cdm.dto.TaxonTO
 *   - 'dtoSimpleName': simple name of the TDO class as returned by Class.getSimpleName(), e.g. TaxonTO
 */
function cdm_ws_whatis($uuid){
  return cdm_ws_load("whatis/?uuid=$uuid");
}

/**
 * load a name from the CDM Webservice
 *
 * @param String $uuid
 * @return a NameTO instance or false 
 */
function cdm_ws_get_name($uuid){
  return cdm_ws_load("name/?uuid=$uuid");
}

/**
 * load a list of names from the CDM Webservice
 *
 * @param unknown_type $page
 * @param unknown_type $hide_unaccepted
 */
function cdm_ws_name_list($page = 1, $hide_unaccepted){
   
}