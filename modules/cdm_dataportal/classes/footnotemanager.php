<?php
// $Id$

/**
* Copyright (C) 2007 EDIT
* European Distributed Institute of Taxonomy 
* http://www.e-taxonomy.eu
* 
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See http://www.mozilla.org/MPL/MPL-1.1.html for the full license terms.
*/

class FootnoteManager
{
  
    
    private static $fnstore = array();
    
    private static $nextFootnoteKey = 1;
    
    // private constructor
    private function __construct() {
      
    }

 
    /**
     * @param $footnoteListKey a string as key to the list of footnotes
     * @return an array of footnotes objects
     *  
     */
    public static function getFootnoteList($footnoteListKey){
      return array_key_exists($footnoteListKey, self::$fnstore) ? self::$fnstore[$footnoteListKey] : NULL;
    }
    
    /**
     * 
     * @param $footnoteListKey
     * @param $separator
     * @return unknown_type
     */
    public static function renderFootnoteList($footnoteListKey, $separator = ', '){
      $out = '';
      if(array_key_exists($footnoteListKey, self::$fnstore)){
        foreach(self::$fnstore[$footnoteListKey] as $fn){
          $out .= $fn->doRender() . $separator;
        }
        $out = substr($out, 0, strlen($out)-strlen($separator));
      }
      return $out;
    }
    
    /**
     * 
     * @param $footnoteListKey
     * @param $object
     * @param $theme
     * @param $themeArguments
     * @return unknown_type
     */
    public static function addNewFootnote($footnoteListKey, $object, $theme = NULL, $themeArguments = array()){

      if(!array_key_exists($footnoteListKey, self::$fnstore)){
          self::$fnstore[$footnoteListKey] = array();
      }
      
      $fnKey = NULL;
      if( !($fnKey = self::footnoteExists($footnoteListKey, $object)) ){
        $fnKey = self::$nextFootnoteKey++;
        $fn = new Footnote($fnKey, $object, $theme, $themeArguments);
        self::$fnstore[$footnoteListKey][$fnKey] = $fn;
        
      }
      
      return $fnKey;
    }
    
    /**
     * 
     * @param $footnoteListKey
     * @param $object
     * @return unknown_type
     */
    private static function footnoteExists($footnoteListKey, $object){
      foreach(self::$fnstore[$footnoteListKey] as $key=>$fn){
        /**
         * When using the comparison operator (==), object variables are compared in a simple manner, namely: 
         * Two object instances are equal if they have the same attributes and values, and are instances of the same class. 
         */
        if($object == $fn->object){
          return $key;
        }
      }
      return FALSE;
    }
    
    // stop users from cloning
    public function __clone() {
      
        trigger_error('Cloning instances of the singleton class FootNoteManager is prohibited', E_USER_ERROR);
    }

}