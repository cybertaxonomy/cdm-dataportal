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
 * Adds an array of render options to specific tagged text elements.
 *
 * The attributes are added as $tagged_text_item['attributes']. Existing 'attributes' are
 * preserved by merging new ones  to the existing array.
 *
 * @param array $taggedtxt
 * @param array $attributes_map array
 *   An array of arrays with following elements in each element array:
 *    - 'filter-type': the tagged text type to which the attributes are applicable
 *    - 'filter-uuid': (optional) and optional filter to only match a specific uuid.
 *    - 'attributes': array of attributes as accepted by drupal_attributes()
 *    - 'prefix': like the drupal_render() '#prefix' option
 *    - 'suffix': like the drupal_render() '#prefix' option
 */
function cdm_tagged_text_add_options(array &$taggedtxt, array $attributes_map){

  foreach ($attributes_map as $attribute_data){
    foreach ($taggedtxt as &$tt){
      if($tt->type == $attribute_data['filter-type']){
        if(isset($attribute_data['filter-uuid'])){
          if($tt->uuid !== $attribute_data['filter-uuid']){
            // ignore
            continue;
          }
        }
        // $tt matched type and filter-uuid if set, apply options
        foreach (array('attributes', 'prefix', 'suffix') as $key ){
          if(isset($attribute_data[$key])){
            $tt->$key = $attribute_data[$key];
          }
        }
      }
    }
  }
}

/**
 * Walks the passed TaggedText array to find all elements which have a
 * TaggedText->entityReference for CDM an entity. For each of these the
 * taggedTexts is loaded from the webservice and the original entry in
 * the TaggedText array will be replaced by the newly loaded array.
 *
 * Existing 'attributes' (@see cdm_tagged_text_add_options()) fields will be preserved by
 * copying them to each of the replacement tagged text items. 'prefix' will be added to
 * the first of the replaced items and 'suffix' to the last one.
 *
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
      $referenced_tt = null;
      if(cdm_type_has_tagged_text($tt->entityReference->type)) {
        $base_uri = cdm_ws_base_uri($tt->entityReference->type);
        if($base_uri){
          if(str_endsWith($base_uri, '$0')){
            // sanitize placeholders at the end
            $base_uri = substr($base_uri, 0, strlen($base_uri) - 2);
          }
            $tagged_text_method = "/taggedText";
            if($base_uri == CDM_WS_NAME){
              $tagged_text_method = "/taggedName";
            }
            if(!str_endsWith($base_uri, '/')){
              $base_uri .= '/';
            }
            $referenced_tt = cdm_ws_get($base_uri . $tt->entityReference->uuid . $tagged_text_method);
        }
        if($referenced_tt){
          if(isset($tt->attributes)){
            foreach($referenced_tt as $reftt){
              $reftt->attributes = $tt->attributes;
            }
          }
          if(isset($tt->prefix)){
            $referenced_tt[0]->prefix = $tt->prefix;
          }
          if(isset($tt->suffix)){
            $referenced_tt[count($referenced_tt)-1]->suffix = $tt->suffix;
          }
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
 * see tagged_text_extract_reference_and_detail()
 *
 * In addition to the tagged text element fields as defined in the cdm this method also recognizes:
 *    - 'attributes': array of attributes as accepted by drupal_attributes()
 *    - 'prefix': like the drupal_render() '#prefix' option
 *    - 'suffix': like the drupal_render() '#prefix' option
 * See also cdm_tagged_text_add_options()
 *
 * The algorithm of this functions is basically the same as for
 * eu.etaxonomy.cdm.strategy.cache.TaggedTextFormatter.createString(List<TaggedText> tags, HTMLTagRules htmlRules)
 *
 * @param array $taggedtxt
 *   Array with text items to convert.
 * @param array $skiptags
 *   Array of tag names to skip
 * @param $tag
 * @param $options
 *     - 'html' (default FALSE): Whether $text is HTML or just plain-text. NOTE! in this case the text is not
 *              translated by t()
 *
 * @return string
 *   The markup.
 */
function cdm_tagged_text_to_markup(array $taggedtxt, $skiptags = array(), $tag = 'span', $options = array()) {

  $out = '';
  $was_separator = false;
  $last_type = null;
  $last_suffix = '';
  $last_text = '';
  $i = 0;
  foreach ($taggedtxt as $tt) {
    if (!in_array($tt->type, $skiptags) && $tt->text && !(is_tagged_text_sepatator_type($tt->type) && count($taggedtxt) > $i+1 && in_array($taggedtxt[$i+1]->type, $skiptags)) ) {

      $is_first = $i == 0;
      $is_separator = is_tagged_text_sepatator_type($tt->type);
      if(str_beginsWith($tt->type, 'PLACEHOLDER_')){
          $out .= '</' . $tag . '>' . $last_suffix;
          $out .=  '{'. $tt->type . '}';
      } else {

        // attributes
        $attributes = array();
        if(isset($tt->attributes)){
          $attributes = $tt->attributes;
        }
        if(!isset($attributes['class'])){
          $attributes['class'] = array();
        }
        $attributes['class'][] = $tt->type;
        if(isset($tt->entityReference)){
          $attributes['class'][] = html_class_attribute_ref($tt->entityReference);
        }

        // prefix and suffix
        $prefix = '';
        $suffix = '';
        if(isset($tt->prefix)){
          $prefix = $tt->prefix;
        }
        if(isset($tt->suffix)){
          $suffix = $tt->suffix;
        }

        if(($last_suffix || $last_type && $last_type != $tt->type) && $tag) {
          $out .= '</' . $tag . '>' . $last_suffix;
        }
        if(($prefix || !$last_type || $last_type != $tt->type) && $tag){
          $out .= $prefix . '<' . $tag . drupal_attributes($attributes) . '>';
        }
        if(!$is_separator && !$was_separator && !$is_first){
          $out .= " ";
        }else if ($is_separator){
          $out = rtrim($out);
        }
        if(isset($options['html']) &&  $options['html']){
          $out .= $tt->text;
        } else {
            if (is_array($tt->text ))
            {
                $temp = $tt->text[0];
                $tt->text = "[\"".$temp."\"]";
            }
            $out .= t('@text', array('@text' => $tt->text));
        }
        $was_separator = $is_separator;
        $last_type = $tt->type;
        $last_suffix = $suffix;
        $last_text = $tt->text;
      }
    }else if ($was_separator){
       //$out = str_replace( $last_text, '', $out);
    }
    $i++;
  }
  if($tag) {
    $out .= '</' . $tag . '>';
  }
  return $out;
}

function cdm_tagged_text_with_ref_footnote_to_markup(array $taggedtxt, $skiptags = array(), $tag = 'span', $annotations_and_sources = null) {

    $out = '';
    $was_separator = false;
    $last_type = null;
    $last_suffix = '';
    $last_text = '';
    $i = 0;
    foreach ($taggedtxt as $tt) {
        if (!in_array($tt->type, $skiptags) && $tt->text && !(is_tagged_text_sepatator_type($tt->type) && count($taggedtxt) > $i+1 && in_array($taggedtxt[$i+1]->type, $skiptags)) ) {

            $is_first = $i == 0;
            $is_separator = is_tagged_text_sepatator_type($tt->type);
            if(str_beginsWith($tt->type, 'PLACEHOLDER_')){
                $out .= '</' . $tag . '>' . $last_suffix;
                $out .=  '{'. $tt->type . '}';
            } else {

                // attributes
                $attributes = array();
                if(isset($tt->attributes)){
                    $attributes = $tt->attributes;
                }
                if(!isset($attributes['class'])){
                    $attributes['class'] = array();
                }
                $attributes['class'][] = $tt->type;
                if(isset($tt->entityReference)){
                    $attributes['class'][] = html_class_attribute_ref($tt->entityReference);
                }

                // prefix and suffix
                $prefix = '';
                $suffix = '';
                if(isset($tt->prefix)){
                    $prefix = $tt->prefix;
                }
                if(isset($tt->suffix)){
                    $suffix = $tt->suffix;
                }

                if(($last_suffix || $last_type && $last_type != $tt->type) && $tag) {
                    $out .= '</' . $tag . '>' . $last_suffix;
                }
                if(($prefix || !$last_type || $last_type != $tt->type) && $tag){
                    $out .= $prefix . '<' . $tag . drupal_attributes($attributes) . '>';
                }
                if(!$is_separator && !$was_separator && !$is_first){
                    $out .= " ";
                }else if ($is_separator){
                    $out = rtrim($out);
                }
                if ($tt->type == 'typeDesignation' && isset_not_empty($tt->link)){
                    $out .= create_html_link($tt);
                }
                elseif(isset($options['html']) &&  $options['html']){
                    $out .= $tt->text;
                } else {
                    if (is_array($tt->text ))
                    {
                        $temp = $tt->text[0];
                        $tt->text = "[\"".$temp."\"]";
                    }
                    $out .= t('@text', array('@text' => $tt->text));

                }
                $footnote_markup = '';
                $annotations_and_sources = new AnnotationsAndSources();
                if ($tt->type == 'reference' && isset_not_empty($tt->entityReference)){
                    $ref_link = '';
                    if(isset_not_empty($tt->link) || isset_not_empty($tt->doi) ){
                        $ref_link .= create_html_link($tt, false, true, true);
                    }
                    $footnote_markup .= '<span class="reference">' . $tt->entityReference->label . $ref_link .'</span>';
                }
                if ($tt->type == 'secReference' && isset_not_empty($tt->entityReference)){
                    $ref_link = '';
                    if(isset_not_empty($tt->link) || isset_not_empty($tt->doi) ){
                        $ref_link .= create_html_link($tt);
                    }
                    $footnote_markup .= '<span class="reference">' . $tt->entityReference->label . $ref_link .'</span>';
                }

                if ($footnote_markup) {
                    $test = RenderHints::getFootnoteListKey();
                    $foot_note_key_type_des = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $footnote_markup);
                    if(isset($foot_note_key_type_des)){
                        $annotations_and_sources->addFootNoteKey($foot_note_key_type_des);
                        $out .= $annotations_and_sources->footNoteKeysMarkup();
                    }
                }
                $was_separator = $is_separator;
                $last_type = $tt->type;
                $last_suffix = $suffix;
                $last_text = $tt->text;
            }
        }else if ($was_separator){
            //$out = str_replace( $last_text, '', $out);
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
 * eu.etaxonomy.cdm.strategy.cache.TaggedTextFormatter.createString(List<TaggedText> tags)
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
  static $separator_names = array('separator', 'postSeparator', 'secNameInSourceSeparator', 'nameInSourceSeparator');
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
 * Extracts from a tagged text array all tagged text for references which is concatenated with citation
 * detail separator and citation detail into one single element.
 *
 * @param $tagged_text
 *    The tagged text to operate on
 * @param string $ref_tag_type
 *    The tag type for a reference is "reference", this is uses as default.
 * @param bool $replace_with_placeholder
 *    Indicates the method to add a empty placeholder tagged text alement as relpacement for the extrated tagged text
 *    elements.
 * @return array
 */
function tagged_text_extract_reference(&$tagged_text, $ref_tag_type = "reference", $replace_with_placeholder = false) {

  $extracted_tt = array();
  if (is_array($tagged_text)) {
    $extract_pos = null;
    $tt_legth = count($tagged_text);
    for ($i = 0; $i < $tt_legth; $i++) {
      if ($tagged_text[$i]->type == $ref_tag_type){
        if ($i > 0 && $tagged_text[$i - 1]->type == 'separator') {
          // the reference may be preceeded by a separator in case it is not a in-reference
          $extracted_tt[] = $tagged_text[$i - 1];
          unset($tagged_text[$i - 1]);
        } else {
          // need to add a separator since the reference tagged text will become the first element
          // ant thus will not be preceded by a separator
          $extracted_tt[] = tagged_text_new('separator', ' ');
        }
        $extracted_tt[] = $tagged_text[$i];
        if ($replace_with_placeholder) {
          // text must not be null, see cdm_tagged_text_to_markup()
          $tagged_text[$i] = tagged_text_new("PLACEHOLDER_" . $ref_tag_type, "PLACEHOLDER_" . $ref_tag_type);
        } else {
          unset($tagged_text[$i]);
        }
      }
    }
  }
  if(isset_not_empty($tagged_text)){
      $tagged_text = array_values($tagged_text); // re-index array to make it continuous again
  }

  return $extracted_tt;
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
 * @param string $ref_detail_tag_type
 * @return array
 */
function tagged_text_extract_reference_and_detail(&$tagged_text, $ref_tag_type = "secReference", $replace_with_placeholder = false) {

  $extracted_tt = array();
  if (is_array($tagged_text)) {
    $extract_pos = null;
    $tt_length = count($tagged_text);
    for ($i = 0; $i < $tt_length - 1; $i++) {
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
  if (isset_not_empty($tagged_text)){
      $tagged_text = array_values($tagged_text);// re-index array to make it continuous again
  }

  return $extracted_tt;
}

/**
 * Extracts the tagged text for sec references name in source with separator from a tagged text array.
 * @param $tagged_text
 *    The tagged text to operate on
 * @param bool $replace_with_placeholder
 *    Indicates the method to add a empty placeholder tagged text alement as relpacement for the extrated tagged text
 *    elements.
 *
 * @return array
 */
function tagged_text_extract_secReference_name_in_source(&$tagged_text) {

  $extracted_tt = array();
  if (is_array($tagged_text)) {

    $tt_length = count($tagged_text);
    $index = -1;
    for ($i = 0; $i < $tt_length; $i++) {
      if ($i < $tt_length -1 && isset($tagged_text[$i]) && $tagged_text[$i+1]->type == 'name' && $tagged_text[$i]->type == "secNameInSourceSeparator") {
        $extracted_tt[++$index] = $tagged_text[$i];
        $extracted_tt[++$index] = $tagged_text[$i + 1];

        unset($tagged_text[$i]);
        unset($tagged_text[$i + 1]);
        continue;
        //search for all elements with type secNameInSource and the last separator
      }
      if (isset($tagged_text[$i]) && $tagged_text[$i]->type == 'secNameInSourceSeparator') {
        $extracted_tt[++$index] = $tagged_text[$i];

        unset($tagged_text[$i]);
        break;
        //search for all elements with type secNameInSource and the last separator
      }
      if(isset($tagged_text[$i])  && $index >0){
          $extracted_tt[++$index] = $tagged_text[$i];
          unset($tagged_text[$i]);
      }


    }
  }
  if (isset_not_empty($tagged_text)){
      $tagged_text = array_values($tagged_text); // re-index array to make it continuous again
  }

  return $extracted_tt;
}

function tagged_text_extract_nomstatus(&$tagged_text) {

  $extracted_tt = [];
  if (is_array($tagged_text)) {
    $itemcnt = count($tagged_text); // preserve the count since the array may shrink while processing
    for ($i = 0; $i < $itemcnt; $i++) {
      if(isset($tagged_text[$i])){ // prevent from accessing removed indices
        if ($tagged_text[$i]->type == "nomStatus"){
          $extracted_status_items = [];
          $extracted_status_items[] = $tagged_text[$i];
          if(isset($tagged_text[$i + 1]) && $tagged_text[$i + 1]->type == "postSeparator"){
            $extracted_status_items[] = $tagged_text[$i + 1];
            unset($tagged_text[$i + 1]);
          }
          if ($tagged_text[$i - 1]->type == "separator"){
            array_unshift($extracted_status_items, $tagged_text[$i - 1]);
            unset($tagged_text[$i - 1]);
          }
          unset($tagged_text[$i]);
          $extracted_tt = array_merge($extracted_tt, $extracted_status_items);
        }
      }
    }
  }
  if(isset_not_empty($tagged_text)){
      $tagged_text = array_values($tagged_text);// re-index array to make it continuous again
  }
  return $extracted_tt;
}

/**
 * Tries to find a tagged text elements that matches the $type and $regex_pattern
 * and returns all preceding elements as new array.
 *
 * @param $tagged_text
 *  The tagged text to crop
 * @param $type
 *  The tagged text type to match
 * @param $regex_pattern
 *  A PREG regex pattern
 *
 * @return array
 *  A new tagged text array
 */
function tagged_text_crop_at(&$tagged_text, $type, $regex_pattern) {
  $cropped_tagged_text = array();
  if (is_array($tagged_text)) {
    for ($i = 0; $i < count($tagged_text); $i++) {
      if ($tagged_text[$i]->type == $type && preg_match($regex_pattern, $tagged_text[$i]->text)) {
        break;
      }
      $cropped_tagged_text[] = $tagged_text[$i];
    }
  }
  return $cropped_tagged_text;
}

function tagged_text_extract(&$tagged_text, $type, $replace_with_placeholder = false) {
  $matching_elements = array();
  if (isset($tagged_text) && is_array($tagged_text)) {
    for ($i = 0; $i < count($tagged_text); $i++) {
      if($tagged_text[$i]->type == $type){
        $matching_elements[] = $tagged_text[$i];
        if($replace_with_placeholder){
          // text must not be null, see cdm_tagged_text_to_markup()
          $tagged_text[$i] = tagged_text_new("PLACEHOLDER_" . $type, "PLACEHOLDER_" . $type);
        } else {
          unset($tagged_text[$i]);
        }
      }
    }
  }
  if (isset_not_empty($tagged_text)) {
      $tagged_text = array_values($tagged_text); // re-index array to make it continuous again
  }
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