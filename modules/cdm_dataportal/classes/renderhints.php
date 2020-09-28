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
  private static $annotationsAndSourceConfig = [];

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
   * Set the new FootnoteList key.
   * The key which is replaced by the new value is returned.
   *
   * @param $key
   *  The new FootnoteListKey to set
   * @retrun
   *  The last FootnoteListKey or NULL
   */
  public static function setFootnoteListKey($key) {
    $lastFnListKey = self::$footnoteListKey;
    self::$footnoteListKey = $key;
    return $lastFnListKey;
  }

  /**
   * Reset the FootnoteListKey to the default value = 'PAGE_GLOBAL'
   * An existing key which is cleared will be returned returned.
   * @retrun
   *  The last FootnoteListKey or NULL
   */
  public static function clearFootnoteListKey() {
    $lastFnListKey = self::$footnoteListKey;
    self::$footnoteListKey = self::$footnoteListKeyDefault;
    return $lastFnListKey;
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
   * @return array
   */
  public static function getAnnotationsAndSourceConfig() {
    return self::$annotationsAndSourceConfig;
  }

  /**
   * Set the configuration for handling annotations and sources in the
   * current section of the page.
   *
   * This configuration is being used in handle_annotations_and_sources():
   *
   * @param array $annotationsAndSourceConfig
   *    The associative configuration array with the keys and boolean values:
   *      - 'sources_as_content' => TRUE|FALSE,
   *      - 'link_to_name_used_in_source' => TRUE|FALSE,
   *      - 'link_to_reference' => TRUE|FALSE,
   *      - 'add_footnote_keys' => TRUE|FALSE,
   *      - 'bibliography_aware' => TRUE|FALSE
   * @return array
   * the configuration array which has been set before.
   */
  public static function setAnnotationsAndSourceConfig($annotationsAndSourceConfig) {
    $lastConfig = self::$annotationsAndSourceConfig;
    self::$annotationsAndSourceConfig = $annotationsAndSourceConfig;
    return $lastConfig;
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
