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
 * A Singleton class wich holds and manages information on the render path in the
 * page element hierarchy and also stored the current FootnoteListKey.
 *
 * RenderPath:
 * The render path is manages as a stack.
 * Usually you will push a new element to this stack at the begining of a theme
 * method: RenderHints::pushToRenderStack('mypageElement');
 * After the output is generated you again pop the current path element from the stack:
 * RenderHints::popFromRenderStack()
 * The current render path can be retrieved by @see getRenderPath().
 *
 * FootnoteListKey
 *
 */
class RenderHints {
  private static $renderStack = array();
  private static $footnoteListKey = '';
  private static $footnoteListKeyDefault = 'PAGE_GLOBAL';

  /**
   * Private constructor.
   */
  private function __construct() {}

  /**
   * @return string
   *   The FootnoteListKey as set or the default FootnoteListKey ('PAGE_GLOBAL')
   */
  public static function getFootnoteListKey() {
    if(self::$footnoteListKey){
      return self::$footnoteListKey;
    } else {
      return self::$footnoteListKeyDefault;
    }
  }

  /**
   * @return bool
   *   true if the FootnoteListKey is unset or reset (== $footnoteListKeyDefault)
   */
  public static function isUnsetFootnoteListKey() {
    return self::$footnoteListKey === self::$footnoteListKeyDefault;
  }

  /**
   * @todo document this function.
   */
  public static function setFootnoteListKey($key) {
    self::$footnoteListKey = $key;
  }

  /**
   * Reset the FootnoteListKey to the default value = 'PAGE_GLOBAL'
   */
  public static function clearFootnoteListKey() {
    self::$footnoteListKey = self::$footnoteListKeyDefault;
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
