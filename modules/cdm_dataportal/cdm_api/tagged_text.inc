<?php

/**
 * @file
 * Functions for handling CDM TaggedText arrays
 *
 *
 * @copyright
 *   (C) 2007-2018 EDIT
 *   European Distributed Institute of Taxonomy
 *   http://www.e-taxonomy.eu
 *
 *   The contents of this module are subject to the Mozilla
 *   Public License Version 1.1.
 * @see http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * @author
 *   - Andreas Kohlbecker <a.kohlbecker@BGBM.org>
 */

function tagged_text_new($tag_type, $text = null){
  $tt = new stdClass();
  $tt->type = $tag_type;
  $tt->text = $text;
  return $tt;
}

/**
 * Walks the passed TaggedText array to find all elements which have a
 * TaggedText->entityReference. For each of these the taggedTexts is loaded
 * from the webservice and the original entry in the TaggedText array will be
 * replaced by the newly loaded array.
 *
 * @param array $taggedtxt
 *    The original TaggedText array
 * @param array $skiptags
 *    Optional list of tag names to skip
 * @return array
 *    The new tagged text with all TaggedText->entityReference objects expanded
 */
function cdm_tagged_text_expand_entity_references(array $taggedtxt, $skiptags = array()) {
  $tagged_text_expanded = array();
  foreach ($taggedtxt as $tt) {
    if (isset($tt->entityReference) && !in_array($tt->type, $skiptags)) {
      $base_uri = cdm_ws_base_uri($tt->entityReference->type);
      if($base_uri){
        $tagged_text_method = "/taggedText";
        if($base_uri == CDM_WS_NAME){
          $tagged_text_method = "/taggedName";
        }
        $referenced_tt = cdm_ws_get($base_uri . "/" . $tt->entityReference->uuid . $tagged_text_method);
        if($referenced_tt){
          $tagged_text_expanded = array_merge($tagged_text_expanded, $referenced_tt);
          continue;
        }
      }
    }
    // default case
    $tagged_text_expanded[] = $tt;
  }
  return $tagged_text_expanded;
}

/**
 * Converts an array of TaggedText items into corresponding html tags.
 *
 * Each item is provided with a class attribute which is set to the key of the
 * TaggedText item.
 *
 * Tagged text where the type starts with 'PLACEHOLDER_' will be added to the markup as plain text whereas the
 * taggedText->type wrapped in curly brackets: '{'. $tt->text . '}' is used as text.
 * see tagged_text_extract_secref()
 *
 * The algorithm of this functions is basically the same as for
 * eu.etaxonomy.cdm.strategy.cache.TaggedCacheHelper.createString(List<TaggedText> tags, HTMLTagRules htmlRules)
 *
 * @param array $taggedtxt
 *   Array with text items to convert.
 * @param array $skiptags
 *   Array of tag names to skip
 *
 * @return string
 *   The markup.
 */
function cdm_tagged_text_to_markup(array $taggedtxt, $skiptags = array(), $tag = 'span') {

  $out = '';
  $was_separator = false;
  $last_type = null;
  $i = 0;
  foreach ($taggedtxt as $tt) {
    if (!in_array($tt->type, $skiptags) && $tt->text) {
      if(isset($tt->entityReference)){
        $class_attr = $tt->type . " " . html_class_attribute_ref($tt->entityReference);
      } else {
        $class_attr = $tt->type;
      }
      $is_first = $i == 0;
      $is_separator = is_tagged_text_sepatator_type($tt->type);
      if(str_beginsWith($tt->type, 'PLACEHOLDER_')){
        $out .=  '{'. $tt->type . '}';
      } else {
        if(($last_type && $last_type != $tt->type) && $tag) {
          $out .= '</' . $tag . '>';
        }
        if((!$last_type || $last_type != $tt->type) && $tag){
          $out .= '<' . $tag . ' class="' . $class_attr . '">';
        }
        if(!$is_separator && !$was_separator && !$is_first){
          $out .= " ";
        }
        $out .= t('@text', array('@text' => $tt->text));
        $was_separator = $is_separator;
        $last_type = $tt->type;
      }
    }
    $i++;
  }
  if($tag) {
    $out .= '</' . $tag . '>';
  }
  return $out;
}

/**
 * Converts an array of TaggedText items into corresponding plain text string
 *
 * Each item is provided with a class attribute which is set to the key of the
 * TaggedText item.
 *
 * The algorithm of this functions is basically the same as for
 * eu.etaxonomy.cdm.strategy.cache.TaggedCacheHelper.createString(List<TaggedText> tags)
 *
 * @param array $taggedtxt
 *   Array with text items to convert.
 * @param array $skiptags
 *   Array of tag names to skip
 *
 * @return string
 *   The plain text
 */
function cdm_tagged_text_to_string(array $taggedtxt, $skiptags = array()) {

  return cdm_tagged_text_to_markup($taggedtxt, $skiptags, null);
}

/**
 * See cdmlib: boolean eu.etaxonomy.cdm.strategy.cache.TagEnum.isSeparator();
 *
 * @return bool
 */
function is_tagged_text_sepatator_type($tagged_text_type){
  static $separator_names = array('separator', 'postSeparator');
  $result = array_search($tagged_text_type, $separator_names) !== false;
  return $result;
}


/**
 * Finds the text tagged with $tag_type in an array of taggedText instances.
 *
 *
 * @param array $taggedtxt
 *   Array with text items.
 * @param array $include_tag_types
 *   Array of the tag types for which to find text items in the $taggedtxt array, or NULL
 *   to return all texts.
 *
 * @return array
 *   An array with the texts mapped by $tag_type.
 */
function cdm_tagged_text_values(array $taggedtxt, $include_tag_types = NULL) {
  $tokens = array();
  if (!empty($taggedtxt)) {
    foreach ($taggedtxt as $tagtxt) {
      if ($include_tag_types === NULL || array_search($tagtxt->type, $include_tag_types) !== false) {
        $tokens[] = $tagtxt->text;
      }
    }
  }
  return $tokens;
}

/**
 * Preprocess the taggedTitle arrays.
 *
 * Step 1: Turns 'newly' introduces tag types ("hybridSign")
 * into tag type "name"
 *
 * Step 2: Two taggedTexts which have the same type and which have
 * a separator between them are merged together.
 *
 * @param array $taggedTextList
 *    An array of TaggedText objects
 */
function normalize_tagged_text(&$taggedTextList) {

  if (is_array($taggedTextList)) {

    // First pass: rename.
    for ($i = 0; $i < count($taggedTextList); $i++) {

      if ($taggedTextList[$i]->type == "hybridSign") {
        $taggedTextList[$i]->type = "name";
      }
    }

    // Second pass: resolve separators.
    $taggedNameListNew = array();
    for ($i = 0; $i < count($taggedTextList); $i++) {

      // elements of the same type concatenated by a separator should be merged together
      if (isset($taggedTextList[$i + 2]) && $taggedTextList[$i + 1]->type == "separator" && $taggedTextList[$i]->type == $taggedTextList[$i + 2]->type) {
        $taggedName = clone $taggedTextList[$i];
        $taggedName->text = $taggedName->text . $taggedTextList[$i + 1]->text . $taggedTextList[$i + 2]->text;
        $taggedNameListNew[] = $taggedName;
        ++$i;
        ++$i;
        continue;
      }
      // no special handling
      $taggedNameListNew[] = $taggedTextList[$i];

    }
    $taggedTextList = $taggedNameListNew;
  }
}

/**
 * Extracts the tagged text for sec references with separator and citation detail from a tagged text array.
 * @param $tagged_text
 *    The tagged text to operate on
 * @param string $ref_tag_type
 *    The tagtype for a secreference is "secReference", but "relSecReference" is also used in case of relationships.
 * @param bool $replace_with_placeholder
 *    Indicates the method to add a empty placeholder tagged text alement as relpacement for the extrated tagged text
 *    elements.
 * @return array
 */
function tagged_text_extract_secref(&$tagged_text, $ref_tag_type = "secReference", $replace_with_placeholder = false) {

  $extracted_tt = array();
  if (is_array($tagged_text)) {
    $extract_pos = null;
    for ($i = 0; $i < count($tagged_text) - 1; $i++) {
      if ($tagged_text[$i + 1]->type == $ref_tag_type && $tagged_text[$i]->type == "separator"){
        $extracted_tt[0] = $tagged_text[$i];
        $extracted_tt[1] = $tagged_text[$i + 1];

        if($replace_with_placeholder){
          // text must not be null, see cdm_tagged_text_to_markup()
          $tagged_text[$i] = tagged_text_new("PLACEHOLDER_" . $ref_tag_type, "PLACEHOLDER_" . $ref_tag_type);
        } else {
          unset($tagged_text[$i]);
        }
        unset($tagged_text[$i + 1]);
        // also get the microreference which could be in $tagged_text[$i + 3]
        if(isset($tagged_text[$i + 3])  && $tagged_text[$i + 2]->type == "separator" && $tagged_text[$i + 3]->type == "secMicroReference"){
          $extracted_tt[2] = $tagged_text[$i + 2];
          $extracted_tt[3] = $tagged_text[$i + 3];
          unset($tagged_text[$i + 2]);
          unset($tagged_text[$i + 3]);
        }
        break;
      }
    }
  }
  $tagged_text = array_values($tagged_text); // re-index array to make it continuous again
  return $extracted_tt;
}

function tagged_text_extract_nomstatus(&$tagged_text) {

  $extracted_tt = array();
  if (is_array($tagged_text)) {
    for ($i = 0; $i < count($tagged_text) - 1; $i++) {
      if ($tagged_text[$i]->type == "nomStatus"){
        $extracted_tt[] = $tagged_text[$i];
        if(isset($tagged_text[$i + 1]) && $tagged_text[$i + 1]->type == "postSeparator"){
          $extracted_tt[] = $tagged_text[$i + 1];
          unset($tagged_text[$i + 1]);
        }
        if ($tagged_text[$i - 1]->type == "separator"){
          array_unshift($extracted_tt, $tagged_text[$i - 1]);
          unset($tagged_text[$i - 1]);
        }
        unset($tagged_text[$i]);
        break;
      }
    }
  }
  $tagged_text = array_values($tagged_text); // re-index array to make it continuous again
  return $extracted_tt;
}

function tagged_text_extract(&$tagged_text, $type) {
  $matching_elements = array();
  if (is_array($tagged_text)) {
    for ($i = 0; $i < count($tagged_text) - 1; $i++) {
      if($tagged_text[$i]->type == $type){
        $matching_elements[] = $tagged_text[$i];
        unset($tagged_text[$i]);
      }
    }
  }
  $tagged_text = array_values($tagged_text); // re-index array to make it continuous again
  return $matching_elements;
}

function find_tagged_text_elements(&$tagged_text, $type){
  $matching_elements = array();
  if (is_array($tagged_text)) {
    for ($i = 0; $i < count($tagged_text) - 1; $i++) {
      if($tagged_text[$i]->type == $type){
        $matching_elements[] = $tagged_text[$i];
      }
    }
  }
  return $matching_elements;
}