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
 * The $footnoteListKey for the current page part that should be stored in the
 * the RenderHints class by calling @see RenderHints::setFootnoteListKey($footnoteListKey)
 */
class FootnoteManager {
  private static $fnstore = [];
  private static $blockedFootnoteLitsKeys = [];
  /*
   * An associative array holding information on
   * special footnote sets which are to be handle separately
   *
   * The key of the map is the $footnoteListKey which is also the
   * key of the  $fnstore map
   *
   * The values are associative arrays with the following optional elements:
   *  - enclosing_tag: the enclosing tag to be used for rendering of the footnote,
   *    @see theme_cdm_footnote()
   *  - key_format: 'latin', 'ROMAN', 'roman', 'ALPHA', 'alpha'
   * and one required element:
   *  - key_index: the set specific counter, to replace the
   *    default $footnote_key_index
   */
  private static $fn_sets = array();
  private static $default_set_definition = array(
    'key_index' => 1,
    'enclosing_tag' => null,
    'key_format' => 'numeric'
  );

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
   * @param string $footnoteListKey
   * @param string $separator
   *
   * @return string
   *   The rendered footnotelist or an empty string if the $footnoteListKey is
   *   blocked
   */
  public static function renderFootnoteList($footnoteListKey, $separator = ', ') {

    if(self::isBlockedFootnoteListKey($footnoteListKey)){
      // the footnote key is blocked, don't render it!
      return '';
    }
    $out = '';
    if (array_key_exists($footnoteListKey, self::$fnstore)) {
      foreach (self::$fnstore[$footnoteListKey] as $fn) {
        $out .= $fn->doRender($footnoteListKey) . $separator;
      }
      $out = substr($out, 0, strlen($out) - strlen($separator));
    }
    return $out;
  }

  /**
   * Provides the content of foot notes
   *
   * @param string $footnoteListKey
   *
   * @return array
   *   The content of foot notes or an empty array if the $footnoteListKey is
   *   blocked
   */
  public static function footnoteListContents($footnoteListKey) {

    if(self::isBlockedFootnoteListKey($footnoteListKey)){
      // the footnote key is blocked, don't render it!
      return [];
    }
    $content_list = [];
    if (array_key_exists($footnoteListKey, self::$fnstore)) {
      foreach (self::$fnstore[$footnoteListKey] as $fn) {
        $content_list[] = $fn->getContent();
      }
    }
    return $content_list;
  }

  /**
   * Add a new footnote.
   *
   * @param $footnoteListKey
   * @param $object
   *
   * @return FootnoteKey
   */
  public static function addNewFootnote($footnoteListKey, $object = NULL) {
    if (!$object || !$footnoteListKey || self::isBlockedFootnoteListKey($footnoteListKey)) {
      return null;
    }
    if (!array_key_exists($footnoteListKey, self::$fnstore)) {
      self::$fnstore[$footnoteListKey] = array();
    }

    $key_label = NULL;
    if (!($key_label = self::footnoteExists($footnoteListKey, $object))) {

      $set_def = &self::matchFootnoteSetDefinition($footnoteListKey);

      $fn_index = $set_def['key_index']++;

      // see http://php.net/manual/de/function.base-convert.php
      switch($set_def['key_format']) {
        case 'ROMAN':
          $key_label = roman_numerals($fn_index);
          break;
        case 'roman':
          $key_label = strtolower(roman_numerals($fn_index));
          break;
        case 'ALPHA':
          $key_label = num2alpha($fn_index - 1);
          break;
        case 'alpha':
          $key_label = strtolower(num2alpha($fn_index - 1));
          break;
        case 'latin':
        default:
          $key_label = $fn_index;
      }

      $fn = new Footnote($key_label, $object, $set_def['enclosing_tag']);
      self::$fnstore[$footnoteListKey][$key_label] = $fn;
    }

    return new FootnoteKey($key_label, $footnoteListKey);
  }

  /**
   * Check if a footnote exists.
   *
   * @param $footnoteListKey
   * @param $object
   *
   * @return bool|int|string
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
   * Register special footnote set for the given $footnoteListKey which is to be handle separately
   * @param $footnoteListKey
   * @param $enclosing_tag: the enclosing tag to be used for rendering of the footnote, @see theme_cdm_footnote()
   * @param $key_format: 'latin', 'ROMAN', 'roman', 'ALPHA', 'alpha'
   */
  public static function registerFootnoteSet($footnoteListKey, $enclosing_tag = null, $key_format = null){

    $set_def = array('key_index' => 1);
    if($enclosing_tag){
      $set_def['enclosing_tag'] = $enclosing_tag;
    } else {
      $set_def['enclosing_tag'] = self::$default_set_definition['enclosing_tag'];
    }
    if($key_format){
      $set_def['key_format'] = $key_format;
    } else {
      $set_def['key_format'] = self::$default_set_definition['key_format'];
    }
    self::$fn_sets[$footnoteListKey] = $set_def;
  }

  /**
   * Returns the footnote set definition which has been
   * registered with a key which matches the given $footnoteListKey.
   * A match is the first key which is equal to or which starts with $footnoteListKey
   * If no match is found the default is returned.
   *
   * @see registerFootnoteSet()
   *
   * @param $footnoteListKey
   * @return array
   */
  public static function &matchFootnoteSetDefinition($footnoteListKey){

    foreach(self::$fn_sets as $fn_set_key => &$fn_set_def){
      if($footnoteListKey == $fn_set_key || str_beginsWith($footnoteListKey, $fn_set_key)){
        return $fn_set_def;
      }
    }
    return self::$default_set_definition;
  }

  public static function blockFootnotesFor($footnoteListKey) {
    if(array_search($footnoteListKey, self::$blockedFootnoteLitsKeys) === false){
      self::$blockedFootnoteLitsKeys[] = $footnoteListKey;
    }
  }
  public static function unblockFootnotesFor($footnoteListKey) {
    $index = array_search($footnoteListKey, self::$blockedFootnoteLitsKeys);
    if($index !== false){
      unset(self::$blockedFootnoteLitsKeys[$index]);
    }
  }

  /**
   * @param $footnoteListKey
   *
   * @return bool
   */
  public static function isBlockedFootnoteListKey($footnoteListKey) {
    return array_search($footnoteListKey, self::$blockedFootnoteLitsKeys) !== false;
  }

  /**
   * Stop users from cloning.
   */
  public function __clone() {
    trigger_error('Cloning instances of the singleton class FootNoteManager is prohibited', E_USER_ERROR);
  }
}
