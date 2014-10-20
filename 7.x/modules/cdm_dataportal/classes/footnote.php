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
  public $key, $object, $enclosing_tag;

  /**
   * Private constructor.
   */
  public function __construct($footnoteKey, $object, $enclosing_tag = NULL) {
    $this->key = $footnoteKey;
    $this->object = $object;
    $this->enclosing_tag = $enclosing_tag;
  }

  /**
   * @todo please document this function.
   */
  public function doRender() {
    $variables = array(
      'footnoteKey' => $this->key,
      'footnoteText' => $this->object
    );
    if(is_string($this->enclosing_tag)){
      $variables['enclosing_tag'] = $this->enclosing_tag;
    }
    return theme('cdm_footnote',
      $variables
    );
  }
}
