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

class RenderHints
{
  
    
    private static $renderStack = array();
    
    private static $footnoteListKey = false;
    
    // private constructor
    private function __construct() {
      
    }

    public static function getfootnoteListKey(){
      return self::$footnoteListKey;
    }
    
    public static function setfootnoteListKey($key){
      return self::$footnoteListKey = $footnoteListKey;
    }
    
    public static function pushToRenderStack($pathelement){
      return array_push(self::$renderStack, $pathelement);
    }
    
   
    public static function popFromRenderStack(){
      return array_pop(self::$renderStack);
    }
    
    /**
     * 
     * @return
     */
    public static function getRenderPath(){
      return join('.', array_reverse(self::$renderStack));
    }

    // stop users from cloning
    public function __clone() {
      
        trigger_error('Cloning instances of the singleton class RenderHints is prohibited', E_USER_ERROR);
    }

}