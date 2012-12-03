<?php
/**
 * @file
 * This PHP class converts XML-based data into JSON formatted data.
 *
 * This program makes use of several open source PHP utility classes and
 * functions.
 *
 * First Created on: Oct/04/2006
 * Last Modified on: Oct/07/2006 (updated to Drupal coding standard Oct 19/2012)
 *
 * @licence
 *   This code is made available free of charge with the rights to use, copy,
 *   modify, merge, publish and distribute. This Software shall be used for
 *   Good, not Evil.
 */

module_load_include('php', 'cdm_api', 'json/JSON');

// Internal program-specific Debug option.
define("DEBUG", FALSE);

// Maximum Recursion Depth that we can allow.
define("MAX_RECURSION_DEPTH_ALLOWED", 25);

// An empty string.
define("EMPTY_STR", "");

// SimpleXMLElement object property name for attributes.
define("SIMPLE_XML_ELEMENT_OBJECT_PROPERTY_FOR_ATTRIBUTES", "@attributes");

// SimpleXMLElement object name.
define("SIMPLE_XML_ELEMENT_PHP_CLASS", "SimpleXMLElement");

class xml2json {

  /**
   * Transforms the XML based String data into JSON format.
   *
   * If the input XML string is in table format, the resulting JSON output
   * will also be in table format. Conversely, if the input XML string is
   * in tree format, the resulting JSON output will also be in tree format.
   *
   * @param string $xmlStringContents
   *   XML data string.
   *
   * @return mixed
   *   JSON.
   */
  public static function transformXmlStringToJson($xmlStringContents) {
    /*
     Get the SimpleXMLElement representation of the function input
     parameter that contains XML string. Convert the XML string
     contents to SimpleXMLElement type. SimpleXMLElement type is
     nothing but an object that can be processed with normal property
     selectors and (associative) array iterators.
     simplexml_load_string returns a SimpleXMLElement object which
     contains an instance variable which itself is an associative array of
     several SimpleXMLElement objects.
     */
    $simpleXmlElementObject = simplexml_load_string($xmlStringContents);

    if ($simpleXmlElementObject == NULL) {
      return(EMPTY_STR);
    }

    $simpleXmlRootElementName = $simpleXmlElementObject->getName();

    if (DEBUG) {
      // Uncomment to see the inner details of
      // the SimpleXMLElement object.
      // var_dump($simpleXmlRootElementName);
      // var_dump($simpleXmlElementObject);
    }

    $jsonOutput = EMPTY_STR;
    // Let us convert the XML structure into PHP array structure.
    $array1 = xml2json::convertSimpleXmlElementObjectIntoArray($simpleXmlElementObject);

    if (($array1 != NULL) && (sizeof($array1) > 0)) {
      // Create a new instance of Services_JSON.
      $json = new Services_JSON();
      $jsonOutput = $json->encode($array1);

      if (DEBUG) {
        // var_dump($array1);
        // var_dump($jsonOutput);
      }
    }
    /* End of if (($array1 != NULL) && (sizeof($array1) > 0)). */

    return($jsonOutput);
  }
  /* End of function transformXmlStringToJson. */

  /**
   * Converts a SimpleXmlElementObject into an associative array.
   *
   * This function accepts a SimpleXmlElementObject as a single argument.
   * If the input XML is in table format (i.e. non-nested), the resulting
   * associative array will also be in a table format. Conversely, if the input
   * XML is in tree (i.e. nested) format, this function will return an
   * associative array (tree/nested) representation of that XML.
   *
   * There are so many ways to turn an XML document into a PHP array. Out of all
   * those options, the recursive logic here uses a method that is very nicely
   * documented by the PHP open source community in the SimpleXMLElement section
   * of the PHP manual available at www.php.net. Credit goes to all those kind
   * PHP (People Helping People!!!) souls.
   *
   * Caution and Remarks: IT IS A RECURSIVE FUNCTION.
   *
   * @param mixed $simpleXmlElementObject
   *   Simple XML Element Object.
   * @param int $recursionDepth
   *   Needs to be used only when this function is called recursively. It can
   *   be omitted when this function is called from another function.
   *
   * @return array
   *   If everything is successful, it returns an associate array containing
   *   the data collected from the XML format. Otherwise, it returns NULL.
   */
  public static function convertSimpleXmlElementObjectIntoArray($simpleXmlElementObject, &$recursionDepth = 0) {
    // Keep an eye on how deeply we are involved in recursion.
    if ($recursionDepth > MAX_RECURSION_DEPTH_ALLOWED) {
      // Fatal error. Exit now.
      return(NULL);
    }

    if ($recursionDepth == 0) {
      if (get_class($simpleXmlElementObject) != SIMPLE_XML_ELEMENT_PHP_CLASS) {
        // If the external caller doesn't call this function initially
        // with a SimpleXMLElement object, return now.
        return(NULL);
      }
      else {
        // Store the original SimpleXmlElementObject sent by the caller.
        // We will need it at the very end when we return from here for good.
        $callerProvidedSimpleXmlElementObject = $simpleXmlElementObject;
      }
    }
    /* End of if ($recursionDepth == 0) { */

    if (get_class($simpleXmlElementObject) == SIMPLE_XML_ELEMENT_PHP_CLASS) {

      // Get a copy of the simpleXmlElementObject.
      $copyOfsimpleXmlElementObject = $simpleXmlElementObject;

      // Get the object variables in the SimpleXmlElement object for us
      // to iterate.
      $simpleXmlElementObject = get_object_vars($simpleXmlElementObject);
    }

    // It needs to be an array of object variables.
    if (is_array($simpleXmlElementObject)) {

      // Initialize the result array.
      $resultArray = array();

      // Is the input array size 0? Then, we reached the rare CDATA text if any.
      if (count($simpleXmlElementObject) <= 0) {

        // Let us return the lonely CDATA. It could even be whitespaces.
        return (trim(strval($copyOfsimpleXmlElementObject)));
      }

      // Let us walk through the child elements now.
      foreach ($simpleXmlElementObject as $key => $value) {
        // When this block of code is commented, XML attributes will be
        // added to the result array.
        // Uncomment the following block of code if XML attributes are
        // NOT required to be returned as part of the result array.
        /*
         if((is_string($key)) && ($key == SIMPLE_XML_ELEMENT_OBJECT_PROPERTY_FOR_ATTRIBUTES)) {
         continue;
         }
         */
        // Let us recursively process the current element we just visited.
        // Increase the recursion depth by one.
        $recursionDepth++;
        $resultArray[$key] = xml2json::convertSimpleXmlElementObjectIntoArray($value, $recursionDepth);

        // Decrease the recursion depth by one.
        $recursionDepth--;
      }
      /* End of foreach($simpleXmlElementObject as $key=>$value) { */

      if ($recursionDepth == 0) {
        // That is it. We are heading to the exit now.
        // Set the XML root element name as the root [top-level] key of
        // the associative array that we are going to return to the caller of
        // this recursive function.
        $tempArray = $resultArray;
        $resultArray = array();
        $resultArray[$callerProvidedSimpleXmlElementObject->getName()] = $tempArray;
      }

      return ($resultArray);
    }
    else {
      // We are now looking at either the XML attribute text or
      // the text between the XML tags.
      return (trim(strval($simpleXmlElementObject)));
    }
    /* End of else. */

  }
  /* End of function convertSimpleXmlElementObjectIntoArray. */

}
/* End of class xml2json. */
