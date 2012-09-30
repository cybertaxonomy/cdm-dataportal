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


class FootnoteKey {
  public $keyStr, $footnoteListKey;

  public function __construct($keyStr, $footnoteListKey) {
    $this->keyStr = $keyStr;
    $this->footnoteListKey = $footnoteListKey;
  }

}
