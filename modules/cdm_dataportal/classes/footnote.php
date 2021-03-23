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
   * Creates markup for the footnote.
   *
   * @param $footnote_list_key
   *
   * @return string
   *  The markup for the footnote
   */
  public function doRender($footnote_list_key = null) {

    $enclosing_tag = null;
    if(is_string($this->enclosing_tag)){
      $enclosing_tag = $this->enclosing_tag;
    }

    return render_footnote($this->key, $this->object, $enclosing_tag, $footnote_list_key);
  }

  /**
   * @return string
   *  The footnote text or markup
   */
  public function getContent(){
    return $this->object;
  }
}
