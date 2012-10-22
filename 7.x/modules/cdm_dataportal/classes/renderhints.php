<?php
/**
 * @file
 * Singleton class to provide render hints.
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
 * Singleton to provide render hints.
 */
class RenderHints {
  private static $renderStack = array();
  private static $footnoteListKey = FALSE;

  /**
   * Private constructor.
   */
  private function __construct() {}

  /**
   * @todo document this function.
   */
  public static function getFootnoteListKey() {
    return self::$footnoteListKey;
  }

  /**
   * @todo document this function.
   */
  public static function setFootnoteListKey($key) {
    self::$footnoteListKey = $key;
  }

  /**
   * @todo document this function.
   */
  public static function pushToRenderStack($pathelement) {
    array_push(self::$renderStack, $pathelement);
  }

  /**
   * @todo document this function.
   */
  public static function popFromRenderStack() {
    return array_pop(self::$renderStack);
  }

  /**
   * @todo document this function.
   */
  public static function sizeof() {
    return sizeof(self::$renderStack);
  }

  /**
   * @todo document this function.
   */
  public static function getRenderPath() {
    return join('.', array_reverse(self::$renderStack));
  }

  /**
   * @todo document this function.
   */
  public static function getHtmlElementID($cdmBase) {
    return 'id="' . RenderHints::getRenderPath() . '(' . $cdmBase->class . ':' . $cdmBase->uuid . ')"';
  }

  /**
   * Stop users from cloning.
   */
  public function __clone() {
    trigger_error('Cloning instances of the singleton class RenderHints is prohibited', E_USER_ERROR);
  }
}
