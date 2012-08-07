<?php
// $Id$

/**
 * Copyright (C) 2007 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1
 * See http://www.mozilla.org/MPL/MPL-1.1.html for the full license terms.
 */
class RenderHints {
  private static $renderStack = array ();
  private static $footnoteListKey = false;
  
  // private constructor
  private function __construct() {
  }
  public static function getFootnoteListKey() {
    return self::$footnoteListKey;
  }
  public static function setFootnoteListKey($key) {
    self::$footnoteListKey = $key;
  }
  public static function pushToRenderStack($pathelement) {
    array_push(self::$renderStack, $pathelement);
  }
  public static function popFromRenderStack() {
    return array_pop(self::$renderStack);
  }
  public static function sizeof() {
    return sizeof(self::$renderStack);
  }
  
  /**
   *
   * @return
   *
   *
   */
  public static function getRenderPath() {
    return join('.', array_reverse(self::$renderStack));
  }
  public static function getHtmlElementID($cdmBase) {
    return 'id="' . RenderHints::getRenderPath() . '(' . $cdmBase->class . ':' . $cdmBase->uuid . ')"';
  }
  
  // stop users from cloning
  public function __clone() {
    trigger_error('Cloning instances of the singleton class RenderHints is prohibited', E_USER_ERROR);
  }
}