<?php
/**
 * @file
 * Class to provide a footnote.
 *
 * @copyright
 *   (C) 2007-2012 EDIT
 *   European Distributed Institute of Taxonomy
 *   http://www.e-taxonomy.eu
 *
 *   The contents of this module are subject to the Mozilla
 *   Public License Version 1.1.
 * @see http://www.mozilla.org/MPL/MPL-1.1.html
 */

/**
 * Provides a footnote.
 */
class Footnote {
  public $key, $object, $theme, $themeArguments;

  /**
   * Private constructor.
   */
  public function __construct($footnoteKey, $object, $theme = NULL, array $themeArguments = array()) {
    $this->key = $footnoteKey;
    $this->object = $object;
    $this->theme = $theme;
    if (!is_array($themeArguments)) {
      $themeArguments = array();
    }
    $this->themeArguments = $themeArguments;
  }

  /**
   * @todo please document this function.
   */
  public function doRender() {
    if ($this->theme) {
      $args = $this->themeArguments;
      array_unshift($this->object);
      array_unshift($this->theme);
      $out = call_user_func_array('theme', $args);
    }
    else {
      $out = $this->object;
    }
    return theme('cdm_footnote', array('footnoteKey' => $this->key, 'footnoteText' => $out));
  }
}
