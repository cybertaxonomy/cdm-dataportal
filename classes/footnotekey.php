<?php
/**
 * @file
 * Class to provide a footnote key.
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
 * Provides a footnote key.
 */
class FootnoteKey {
  public $keyStr, $footnoteListKey;

  /**
   * @todo please document this function.
   */
  public function __construct($keyStr, $footnoteListKey) {
    $this->keyStr = $keyStr;
    $this->footnoteListKey = $footnoteListKey;
  }
}
