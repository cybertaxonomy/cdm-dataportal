<?php

class Footnote
{
  
    public $key, $object, $theme, $themeArguments;
    
    
    // private constructor
    public function __construct($footnoteKey, $object, $theme = null, array $themeArguments = array()) {
        $this->key = $footnoteKey;
        $this->object = $object;
        $this->theme = $theme;
        if(!is_array($themeArguments)){
          $themeArguments = array();
        }
        $this->themeArguments = $themeArguments;
    }

    public function doRender(){
      
      if($this->theme){
      $args = $this->themeArguments;
      array_unshift($this->object);
      array_unshift($this->theme);

        $out = call_user_func_array('theme', $args);
      } else {
        $out = $this->object;
      }
      return theme('cdm_footnode', $this->key, $out);
    }

}