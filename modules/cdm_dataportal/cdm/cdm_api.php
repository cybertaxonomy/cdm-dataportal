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
 * Converts an array of TagedText items into a sequence of corresponding html tags whereas 
 * each item will provided with a class attribute which set to the key of the TaggedText item.
 *
 * @param TaggedText $tag
 * @return String of HTML 
 */
function cdm_taggedtext2html(array $taggedText, $tag = 'span'){
   foreach($taggedText as $class=>$value){
     $out .= '<'.$tag.' class="'.$class.'">'.$value.'</ '.$tag.'>';
   }
   return $out;
}

/**
 * @return string
 * @param string $url
 * @desc Return string content from a remote file
 * @author Luiz Miguel Axcar (lmaxcar@yahoo.com.br)
*/
function get_content($url)
{
    $ch = curl_init();

    curl_setopt ($ch, CURLOPT_URL, $url);
    curl_setopt ($ch, CURLOPT_HEADER, 0);

    ob_start();

    curl_exec ($ch);
    curl_close ($ch);
    $string = ob_get_contents();

    ob_end_clean();
   
    return $string;    
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

  if(variable_get('cdm_webservice_isStub', 0)){
    $url = urlencode(urlencode($url)).'.xml';
  }
  $url = variable_get('cdm_webservice_url', '').$url;
  //TODO get_content() requires the php curl extension to be installed, maybe we should chose an other function
  $data = get_content($url);
  
  $obj = simplexml_load_string($data);
  
  if(!$obj){
    $backtrace = debug_backtrace();
    watchdog('CDM', $backtrace[1]['function'].' - failed to load '.$url, WATCHDOG_ERROR);
  }
  $obj->ws_url = $url;
  $obj->data = $data;
  
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
function cdm_ws_name_list($page = 1, $onlyAccepted){
   
}