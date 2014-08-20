<?php
/**
 * @file
 * Class to manage footnotes.
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
 * Manages footnotes in multiple list. Each of these footnote lists is
 * identified by a footnoteListKey.
 *
 * The $footnoteListKey for the curtent page part that should be stored in the
 * the RenderHints class by calling @see RenderHints::setFootnoteListKey($footnoteListKey)
 */
class FootnoteManager {
  private static $fnstore = array();
  private static $nextFootnoteKey = 1;

  /**
   * Private constructor.
   */
  private function __construct() {}

  /**
   * Get a list of footnotes.
   *
   * @param string $footnoteListKey
   *   A string as key to the list of footnotes.
   *
   * @return array
   *   An array of footnotes objects.
   */
  public static function getFootnoteList($footnoteListKey) {
    return array_key_exists($footnoteListKey, self::$fnstore) ? self::$fnstore[$footnoteListKey] : NULL;
  }

  /**
   * Remove a list of footnotes.
   *
   * @param string $footnoteListKey
   *   A string as key to the list of footnotes.
   *
   * @return void
   */
  public static function removeFootnoteList($footnoteListKey) {
    if (array_key_exists($footnoteListKey, self::$fnstore)) {
      unset(self::$fnstore[$footnoteListKey]);
    }
  }

  /**
   * Render a footnote list
   *
   * @param array $footnoteListKey
   * @param string $separator
   *
   * @return string
   *   The rendered footnotelist.
   */
  public static function renderFootnoteList($footnoteListKey, $separator = ', ') {
    $out = '';
    if (array_key_exists($footnoteListKey, self::$fnstore)) {
      foreach (self::$fnstore[$footnoteListKey] as $fn) {
        $out .= $fn->doRender() . $separator;
      }
      $out = substr($out, 0, strlen($out) - strlen($separator));
    }
    return $out;
  }

  /**
   * Add a new footnote.
   *
   * @param $footnoteListKey
   * @param $object
   * @param $theme
   * @param $themeArguments
   *
   * @return FootnoteKey
   */
  public static function addNewFootnote($footnoteListKey, $object = NULL, $theme = NULL, $themeArguments = array()) {
    if (!$object) {
      return FALSE;
    }
    if (!array_key_exists($footnoteListKey, self::$fnstore)) {
      self::$fnstore[$footnoteListKey] = array();
    }

    $fnKey = NULL;
    if (!($fnKey = self::footnoteExists($footnoteListKey, $object))) {
      $fnKey = self::$nextFootnoteKey++;
      $fn = new Footnote($fnKey, $object, $theme, $themeArguments);
      self::$fnstore[$footnoteListKey][$fnKey] = $fn;
    }

    return new FootnoteKey($fnKey, $footnoteListKey);
  }

  /**
   * Check if a footnote exists.
   *
   * @param $footnoteListKey
   * @param $object
   *
   * @return unknown_type
   */
  private static function footnoteExists($footnoteListKey, $object) {
    foreach (self::$fnstore[$footnoteListKey] as $key => $fn) {
      /*
      When using the comparison operator (==), object variables are compared
      in a simple manner, namely:
      Two object instances are equal if they have the same attributes and
      values, and are instances of the same class.
      */
      if ($object == $fn->object) {
        return $key;
      }
    }
    return FALSE;
  }

  /**
   * Stop users from cloning.
   */
  public function __clone() {
    trigger_error('Cloning instances of the singleton class FootNoteManager is prohibited', E_USER_ERROR);
  }
}
