<?php
/**
 * @file
 * footnote functions.
 *
 * @copyright
 *   (C) 2007-2020 EDIT
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

/**
 * Creates the footnotes for the given CDM instance.
 *
 * Footnotes are created for annotations and original sources whereas the resulting footnote keys depend on the
 * parameters $footnote_list_key_suggestion and $is_bibliography_aware, see parameter $footnote_list_key_suggestion
 * for more details.
 *
 * possible keys for
 *     - annotation footnotes:
 *       - $footnote_list_key_suggestion
 *       - RenderHints::getFootnoteListKey().'-annotations'
 *     - original source footnotes
 *       - "BIBLIOGRAPHY" (when !$is_bibliography_aware && bibliography_settings['enabled'] == 1 )
 *       - "BIBLIOGRAPHY-$footnote_list_key_suggestion" (when !$is_bibliography_aware && bibliography_settings['enabled'] == 0 )
 *       - $footnote_list_key_suggestion (when $is_bibliography_aware)
 *
 * @param $description_element
 *   A CDM DescriptionElement instance
 * @param string $separator
 *   Optional parameter. The separator string to concatenate the footnote ids, default is ','
 * @param $footnote_list_key_suggestion string
 *    Optional parameter. If this parameter is left empty (null, 0, "") the footnote key will be determined by the nested
 *    method calls by calling RenderHints::getFootnoteListKey(). NOTE: the footnote key for annotations will be set to
 *    RenderHints::getFootnoteListKey().'-annotations'.
 * @param bool $do_link_to_reference
 *    Create a link to the reference pages for sources when TRUE.
 * @param bool $do_link_to_name_used_in_source
 *    Create a link to the name pages for name in source when TRUE.
 * @param bool $is_bibliography_aware
 *    Put source references into the bibliography when this param is TRUE.
 *
 * @return String
 *   The foot note keys
 *
 * @throws \Exception re-throw exception from theme()
 * @see cdm_entities_annotations_footnotekeys()
 *    For original sources the $footnote_list_key_suggestion will be overwritten by bibliography_footnote_list_key() when
 *    $is_bibliography_aware is set TRUE.
 * @$original_source_footnote_tag
 *    null will cause bibliography_footnote_list_key to use the default
 */
function cdm_create_footnotes(
  $description_element,
  $separator = ',',
  $footnote_list_key_suggestion = null,
  $do_link_to_reference = FALSE,
  $do_link_to_name_used_in_source = FALSE,
  $is_bibliography_aware = FALSE
){

  $sources = cdm_entity_sources_sorted($description_element);

  // Annotations as footnotes.
  $footnote_keys = cdm_entity_annotations_as_footnotekeys($description_element, $footnote_list_key_suggestion);

  // Source references as footnotes.
  if($is_bibliography_aware){
    $bibliography_settings = get_bibliography_settings();
    $sources_footnote_list_key = bibliography_footnote_list_key($footnote_list_key_suggestion);
    $original_source_footnote_tag = $bibliography_settings['enabled'] == 1 ? 'div' : null; // null will cause bibliography_footnote_list_key to use the default
  } else {
    $sources_footnote_list_key = $footnote_list_key_suggestion;
    if(!$sources_footnote_list_key) {
      RenderHints::getFootnoteListKey();
    }
    $original_source_footnote_tag = NULL;
  }

  foreach ($sources as $source) {
    if (_is_original_source_type($source)) {
      $fn_key = FootnoteManager::addNewFootnote(
        $sources_footnote_list_key,
        render_original_source(
          $source,
          $do_link_to_reference,
          $do_link_to_name_used_in_source
        ),
        $original_source_footnote_tag
      );
      // Ensure uniqueness of the footnote keys.
      cdm_add_footnote_to_array($footnote_keys, $fn_key);
    }
  }
  // Sort and render footnote keys.
  asort($footnote_keys);
  return footnote_keys_to_markup($footnote_keys, $separator);
}

/**
 * Fetches the list of visible annotations for each of the cdm entities and returns the footnote keys for them.
 *
 * The footnotes are passed to the FootnoteManager in order to store the annotations and to create the footnote keys.
 *
 * @see cdm_fetch_visible_annotations()
 *
 * @param array $cdm_entities
 *   An array of CdmBase instances.
 * @param $footnote_list_key_suggestion string
 *    optional parameter. If this parameter is left empty (null, 0, "") the footnote key will be determined be set to
 *    RenderHints::getFootnoteListKey().'-annotations' otherwise the supplied key will be used.
 *
 * @return array of footnote keys
 */
function cdm_entities_annotations_footnotekeys(array $cdm_entities, $footnote_list_key_suggestion = NULL) {

  $foot_note_keys = [];
  foreach ($cdm_entities as $cdm_entity) {
    $foot_note_keys = array_merge($foot_note_keys, cdm_entity_annotations_as_footnotekeys($cdm_entity,$footnote_list_key_suggestion));
  }

  return $foot_note_keys;
}

/**
 * Fetches the list of visible annotations for the cdm entity or for the comparable
 * object and returns the footnote keys.
 *
 * The footnotes are passed to the FootnoteManager in order to store the
 * annotations and to create the footnote keys.

 * @param stdClass $cdm_entity
 *   A single CdmBase instance ore comparable object.
 * @param $footnote_list_key_suggestion string
 *    optional parameter. If this parameter is left empty (null, 0, "") the footnote key will be determined be set to
 *    RenderHints::getFootnoteListKey().'-annotations' otherwise the supplied key will be used.
 * @return array of footnote keys
 *
 * @see cdm_fetch_visible_annotations()
 */
function cdm_entity_annotations_as_footnotekeys(stdClass $cdm_entity, $footnote_list_key_suggestion = NULL) {

  $foot_note_keys = [];

  // Getting the key for the footnotemanager.
  if (isset($footnote_list_key_suggestion)) {
    $footnote_list_key = $footnote_list_key_suggestion;
  } else {
    $footnote_list_key = RenderHints::getFootnoteListKey() . '-annotations';
  }

  // Adding the footnotes keys.
  $annotations = cdm_fetch_visible_annotations($cdm_entity);
  if (is_array($annotations)) {
    foreach ($annotations as $annotation) {
      $foot_note_keys[] = FootnoteManager::addNewFootnote($footnote_list_key, $annotation->text);
    }
  }

  return $foot_note_keys;
}

/**
 * Creates markup for an array of foot note keys
 *
 * @param array $footnote_keys
 * @param string $separator
 *
 * @return string
 */
function footnote_keys_to_markup(array $footnote_keys, $separator) {

  $footnotes_markup = '';
  foreach ($footnote_keys as $foot_note_key) {
    try {
      $footnotes_markup .= cdm_footnote_key($foot_note_key, ($footnotes_markup ? $separator : ''));
    } catch (Exception $e) {
      drupal_set_message("Exception: " . $e->getMessage(), 'error');
    }
  }
  return $footnotes_markup;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cdm_add_footnote_to_array(&$footnote_list, $footnote) {
  if (!cdm_exist_footnote($footnote_list, $footnote)) {
    $footnote_list[] = $footnote;
  }
}


/**
 * Create markup for the footnotes mapped to the $footnoteListKey.
 *
 * @param null $footnote_list_key
 *  The footnote list key, see RenderHints::getFootnoteListKey()
 * @param $element_tag
 *  The tag for the footnote element
 *
 * @return string
 * @throws \Exception
 */
function cdm_annotation_footnotes($footnote_list_key = null, $element_tag = 'span') {
  if (variable_get('cdm_dataportal_annotations_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES)) {
    return '';
  }
  return cdm_footnotes($footnote_list_key . '-annotations', $element_tag);
}

/**
 * Creates markup for a foot note key
 *
 * @param null $footnoteKey
 * @param string $separator
 * @param bool $separator_off
 *
 * @return string
 *   The footnote key markup
 */
function cdm_footnote_key($footnoteKey = null, $separator = '', $separator_off = false) {

  if (!is_object($footnoteKey) or !isset($footnoteKey->footnoteListKey)) {
    return '';
  }
  if (variable_get('cdm_dataportal_all_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES)) {
    return '';
  }

  if ($separator_off) {
    $separator = '';
  }
  $out = '<span class="footnote-key footnote-key-' . $footnoteKey->keyStr . ' member-of-footnotes-' . $footnoteKey->footnoteListKey . '">'
    . $separator . '<a href="#footnote-' . $footnoteKey->keyStr . '">' . $footnoteKey->keyStr . '</a>' . '</span>';
  return $out;
}

/**
 * @param null $footnoteKey
 * @param null $footnoteText
 * @param string $enclosing_tag
 *   default is 'span'
 *
 * @return string
 */
function footnote_markup($footnoteKey = null, $footnoteText = null, $enclosing_tag = 'span') {
  _add_js_footnotes();
  if($enclosing_tag == null){
    $enclosing_tag = 'span';
  }
  return '<' . $enclosing_tag . ' class="footnote footnote-' . $footnoteKey . '">'
    . '<a name="footnote-' . $footnoteKey . '"></a>'
    . '<span class="footnote-anchor">' . $footnoteKey . '.</span>&nbsp;' . $footnoteText
    . '</' . $enclosing_tag . '>';
}



/**
 * Create markup for the footnotes mapped to the $footnoteListKey.
 *
 * @param null $footnote_list_key
 *  The footnote list key, see RenderHints::getFootnoteListKey()
 * @param $element_tag
 *  The tag for the footnote element
 *
 * @return string
 */
function cdm_footnotes($footnote_list_key = null, $element_tag = 'span') {

  if (variable_get('cdm_dataportal_all_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES)) {
    return '';
  }

  $out = '<' . $element_tag . ' class="footnotes footnotes-' . $footnote_list_key . ' ">'
    . FootnoteManager::renderFootnoteList($footnote_list_key)
    . '</' . $element_tag . '>';

  FootnoteManager::removeFootnoteList($footnote_list_key);
  return $out;
}

/**
 * Renders the footnotes for annotations and sources, etc.
 *
 * @param string $footnote_list_key
 *    RenderHints::getFootnoteListKey() will be used if this parameter is undefined.
 * @param string $enclosingTag
 *    Default tag is 'span'
 *
 * @return string
 *    The markup string
 *
 */
function render_cdm_footnotes($footnote_list_key = NULL, $enclosingTag = 'span'){

  if (variable_get('cdm_dataportal_all_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES)) {
    return '';
  }
  if(!$footnote_list_key){
    $footnote_list_key = RenderHints::getFootnoteListKey();
  }

  $out = '<' . $enclosingTag . ' class="footnotes footnotes-' . $footnote_list_key . ' ">'
    . FootnoteManager::renderFootnoteList($footnote_list_key . '-annotations') . ' ' . FootnoteManager::renderFootnoteList($footnote_list_key)
    . '</' . $enclosingTag . '>';
  FootnoteManager::removeFootnoteList($footnote_list_key . '-annotations');
  FootnoteManager::removeFootnoteList($footnote_list_key);
  return $out;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cdm_exist_footnote($footnote_list, $footnote) {
  $result = FALSE;
  if (is_array($footnote_list)) {
    foreach ($footnote_list as $element) {
      if ($element == $footnote) {
        $result = TRUE;
      }
    }
  }
  return $result;
}
