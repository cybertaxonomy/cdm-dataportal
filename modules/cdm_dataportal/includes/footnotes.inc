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
const MEMBER_OF_FOOTNOTES = ' member-of-footnotes-';

/**
 * Creates the footnotes for the given CDM instance.
 *
 * Footnotes are created for annotations and original sources whereas the resulting footnote keys depend on the
 * parameters $footnote_list_key and $is_bibliography_aware, see parameter $footnote_list_key
 * for more details.
 *
 * possible keys for annotation and source footnotes:
 *       - $footnote_list_key
 *       - RenderHints::getFootnoteListKey()
 *     - original source footnotes
 *       - "BIBLIOGRAPHY" (when !$is_bibliography_aware && bibliography_settings['enabled'] == 1 )
 *       - "BIBLIOGRAPHY-$footnote_list_key" (when !$is_bibliography_aware && bibliography_settings['enabled'] == 0 )
 *       - $footnote_list_key (when $is_bibliography_aware)
 *
 * @param $cdm_entity
 *   A CDM entity
 * @param string $separator
 *   Optional parameter. The separator string to concatenate the footnote ids, default is ','
 * @param $footnote_list_key string
 *    Optional parameter. If this parameter is left empty (null, 0, "") the footnote key will be determined by the nested
 *    method calls by calling RenderHints::getFootnoteListKey().
 *    For original sources the $footnote_list_key will be overwritten by bibliography_footnote_list_key() when
 *    $is_bibliography_aware is set TRUE.
 * @param bool $do_link_to_reference
 *    Create a link to the reference pages for sources when TRUE.
 * @param bool $do_link_to_name_used_in_source
 *    Create a link to the name pages for name in source when TRUE.
 * @param bool $is_bibliography_aware
 *    Put source references into the bibliography when this param is TRUE.
 *    For original sources the $footnote_list_key will be overwritten
 *    by bibliography_footnote_list_key() when
 *    $is_bibliography_aware is set TRUE.
 * @return String
 *   The foot note keys as markup
 *
 * NOTE: Only used in @see handle_annotations_and_sources()
 */
function render_entity_footnotes(
  $cdm_entity,
  $separator = ',',
  $footnote_list_key = null,
  $do_link_to_reference = FALSE,
  $do_link_to_name_used_in_source = FALSE,
  $is_bibliography_aware = FALSE
){

  $sources = cdm_entity_sources_sorted($cdm_entity);

  if (!isset($footnote_list_key) || !$footnote_list_key) {
    $footnote_list_key = RenderHints::getFootnoteListKey();
  }

  // Annotations as footnotes.
  $footnote_keys = cdm_entity_annotations_as_footnote_keys($cdm_entity, $footnote_list_key);

  // Source references as footnotes.
  if($is_bibliography_aware){
    $bibliography_settings = get_bibliography_settings();
    $sources_footnote_list_key = bibliography_footnote_list_key($footnote_list_key);
    $original_source_footnote_tag = $bibliography_settings['enabled'] == 1 ? 'div' : null; // null will cause bibliography_footnote_list_key to use the default
  } else {
    $sources_footnote_list_key = $footnote_list_key;
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
      if(array_search($fn_key, $footnote_keys)=== false) {
        $footnote_keys[] = $fn_key;
      }
    }
  }
  // Sort and render footnote keys.
  asort($footnote_keys);
  return render_footnote_keys($footnote_keys, $separator);
}

/**
 * Fetches the list of visible annotations for the cdm entity or for the comparable
 * object and returns the footnote keys.
 *
 * The footnotes are passed to the FootnoteManager in order to store the
 * annotations and to create the footnote keys.

 * @param stdClass $cdm_entity
 *   A single CdmBase instance ore comparable object.
 * @param $footnote_list_key string
 *    optional parameter. If this parameter is left empty (null, 0, "") the
 *    footnote key will be set to RenderHints::getFootnoteListKey()
 *    otherwise the supplied $footnote_list_key will be used.
 * @return array of footnote keys
 *
 * @see cdm_fetch_visible_annotations()
 */
function cdm_entity_annotations_as_footnote_keys(stdClass $cdm_entity, $footnote_list_key = NULL) {

  $foot_note_keys = [];

  if (!isset($footnote_list_key) || !$footnote_list_key) {
    $footnote_list_key = RenderHints::getFootnoteListKey();
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
function render_footnote_keys(array $footnote_keys, $separator) {

  $footnotes_markup = '';
  foreach ($footnote_keys as $foot_note_key) {
    try {
      $footnotes_markup .= render_footnote_key($foot_note_key, ($footnotes_markup ? $separator : ''));
    } catch (Exception $e) {
      drupal_set_message("Exception: " . $e->getMessage(), 'error');
    }
  }
  return $footnotes_markup;
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
function render_footnote_key($footnoteKey = null, $separator = '', $separator_off = false) {

  if (!is_object($footnoteKey) or !isset($footnoteKey->footnoteListKey)) {
    return '';
  }
  if (variable_get('cdm_dataportal_all_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES)) {
    return '';
  }

  if ($separator_off) {
    $separator = '';
  }
  $out = '<span class="footnote-key footnote-key-' . $footnoteKey->keyStr . MEMBER_OF_FOOTNOTES . $footnoteKey->footnoteListKey . '">'
    . $separator . '<a href="#footnote-' . $footnoteKey->keyStr . '">' . $footnoteKey->keyStr . '</a>' . '</span>';
  return $out;
}

/**
 * Create the markup for a footnote. This method is used in {@link Footnote::doRender()}
 * @param null $footnoteKey
 * @param null $footnoteText
 * @param string $enclosing_tag
 *   default is 'span'
 *
 * @return string
 *
 */
function render_footnote($footnoteKey = null, $footnoteText = null, $enclosing_tag = 'span', $footnote_list_key = null) {
  _add_js_footnotes();
  if($enclosing_tag == null){
    $enclosing_tag = 'span';
  }
  $class_attribute_member_of = '';
  if($footnote_list_key){
    $class_attribute_member_of = MEMBER_OF_FOOTNOTES . $footnote_list_key;
  }
  return '<' . $enclosing_tag . ' class="footnote footnote-' . $footnoteKey . $class_attribute_member_of . '">'
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
 *   The markup
 */
function render_footnotes($footnote_list_key = null, $element_tag = 'span') {

  if (variable_get('cdm_dataportal_all_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES)) {
    return '';
  }

  if (!isset($footnote_list_key) || !$footnote_list_key) {
    $footnote_list_key = RenderHints::getFootnoteListKey();
  }

  $out = '<' . $element_tag . ' class="footnotes footnotes-' . $footnote_list_key . ' ">'
    . FootnoteManager::renderFootnoteList($footnote_list_key)
    . '</' . $element_tag . '>';

  FootnoteManager::removeFootnoteList($footnote_list_key);
  return $out;
}

/**
 * This method determines the footnote key for original sources to be shown in the bibliography block
 *
 * The footnote key depends on the value of the 'enabled' value of the bibliography_settings
 *    - enabled == 1 -> "BIBLIOGRAPHY"
 *    - enabled == 0 -> "BIBLIOGRAPHY-$key_suggestion"
 *
 * @see get_bibliography_settings() and @see constant BIBLIOGRAPHY_FOOTNOTE_KEY
 *
 * @param $key_suggestion string
 *    optional parameter. If this parameter is left empty (null, 0, "") the footnote key will be retrieved by
 *    calling RenderHints::getFootnoteListKey().

 *
 * @return string
 *  the footnote_list_key
 */
function bibliography_footnote_list_key($key_suggestion = null) {
  if(!$key_suggestion){
    $key_suggestion = RenderHints::getFootnoteListKey();
  }
  $bibliography_settings = get_bibliography_settings();
  $footnote_list_key = $bibliography_settings['enabled'] == 1 ? BIBLIOGRAPHY_FOOTNOTE_KEY : BIBLIOGRAPHY_FOOTNOTE_KEY . '-' . $key_suggestion;
  return $footnote_list_key;
}


/**
 * Creates footnote markup for the name relationship and
 * registers it in the {@link \FootnoteManager}. The resulting foonote
 * key is returned as markup.
 *
 * @param $name_rel
 *   The cdm name relationship
 * @return string
 *  The markup for the footnote key
 */
function handle_name_relationship_as_footnote($name_rel)
{
  $footnote_markup = '';
  $footnote_key_markup = '';
  if (isset($name_rel->ruleConsidered) && $name_rel->ruleConsidered) {
    $footnote_markup = '<span class="rule_considered">' . $name_rel->ruleConsidered . '</span> ';
  }
  if (isset($name_rel->citation)) {
    $footnote_markup .= '<span class="reference">' . $name_rel->citation->titleCache . '</span>';
  }
  if (isset($name_rel->citationMicroReference) && $name_rel->citationMicroReference) {
    $footnote_markup .= (isset($name_rel->citation) ? ':' : '') . ' <span class="reference_detail">' . $name_rel->citationMicroReference . '</span>';
  }
  if ($footnote_markup) {
    $fnkey = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $footnote_markup);
    $footnote_key_markup = render_footnote_key($fnkey,',',TRUE);
  }
  return $footnote_key_markup;
}

/**
 * Creates footnote markup for nomenclatural status and
 * registers it in the {@link \FootnoteManager}. The resulting foonote
 * key is returned as markup.
 *
 * @param $nom_status
 * @return string
 *  The markup for the footnote key
 */
function handle_nomenclatural_status_as_footnote($nom_status)
{
  // NomenclaturalStatus is a subclass of ReferencedEntityBase
  // and has the same structure as TaxonNameRelationship
  return handle_name_relationship_as_footnote($nom_status);
}

/**
 * Creates footnote markup for nomenclatural reference of the name and
 * registers it in the {@link \FootnoteManager}. The resulting foonote
 * key is returned as markup.
 *
 * @param $name
 * The name whose nomenclatural reference is to be shown as footnote
 * @return string
 *  The markup for the footnote key
 */
function handle_nomenclatural_reference_as_footnote($name)
{
  $footnote_markup = '';
  $footnote_key_markup = '';
  if (isset($name->nomenclaturalReference) && $name->nomenclaturalReference) {
    $footnote_markup .= '<span class="reference">' . $name->nomenclaturalReference->titleCache . '</span>';
  }
  if (isset($name->nomenclaturalMicroReference)) {
    $footnote_markup .= ($footnote_key_markup ? ':' : '') . '<span class="reference_detail">' . $name->nomenclaturalMicroReference . '</span>';
  }
  if ($footnote_markup) {
    $fnkey = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $footnote_markup);
    $footnote_key_markup = render_footnote_key($fnkey, ',',TRUE);
  }
  return $footnote_key_markup;
}


