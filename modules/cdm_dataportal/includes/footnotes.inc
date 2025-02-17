<?php
/**
 * @file
 * functions for annotation and sources handling which are to be rendered as
 * footnotes.
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
 * @param bool $add_annotations
 *    Footnotes for annotations will not be created if this is FALSE
 *
 * @return array
 *   An array of the footnote keys for the supplied cdm entity
 *
 * NOTE: Only used in @see handle_annotations_and_sources()
 */
function cdm_entity_footnotes(
  $cdm_entity,
  $footnote_list_key = NULL,
  $do_link_to_reference = FALSE,
  $do_link_to_name_used_in_source = FALSE,
  $is_bibliography_aware = FALSE,
  $add_annotations = TRUE,
  $add_sources = TRUE
)
{
    if ($add_sources){
        $sources = cdm_entity_sources_sorted($cdm_entity);
    }


    if (!isset($footnote_list_key) || !$footnote_list_key) {
        $footnote_list_key = RenderHints::getFootnoteListKey();
    }

    // Annotations as footnotes.
    if ($add_annotations) {
        $footnote_keys = cdm_entity_annotations_as_footnote_keys($cdm_entity, $footnote_list_key);
    }

    // Source references as footnotes.
    if ($is_bibliography_aware) {
        $bibliography_settings = get_bibliography_settings();
        $sources_footnote_list_key = bibliography_footnote_list_key($footnote_list_key);
        $original_source_footnote_tag = $bibliography_settings['enabled'] == 1 ? 'div' : null; // null will cause bibliography_footnote_list_key to use the default
    } else {
        $sources_footnote_list_key = $footnote_list_key;
        $original_source_footnote_tag = NULL;
    }

    if (isset_not_empty($sources)) {

        foreach ($sources as $source) {
            if (_is_original_source_type($source)) {
                if (is_cdm_entity($source) || isset_not_empty($source->label)) {
                  $fn_key = FootnoteManager::addNewFootnote(
                    $sources_footnote_list_key,
                    render_original_source(
                      $source,
                      $do_link_to_reference,
                      $do_link_to_name_used_in_source
                    ),
                    $original_source_footnote_tag
                  );
                }else{
                  $fn_key = FootnoteManager::addNewFootnote(
                    $sources_footnote_list_key,
                    render_original_source_dto(
                      $source,
                      $do_link_to_reference,
                      $do_link_to_name_used_in_source
                    ),
                    $original_source_footnote_tag
                  );
                }

                // Ensure uniqueness of the footnote keys.
                if (isset_not_empty($footnote_keys) && array_search($fn_key, $footnote_keys) === false) {
                    $footnote_keys[] = $fn_key;
                }
                if (!isset_not_empty($footnote_keys) ){
                    $footnote_keys[] = $fn_key;
                }
            }
        }
    }
  // Sort and render footnote keys.
  asort($footnote_keys);
  return $footnote_keys;
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

    if (isset($cdm_entity->annotations) && !is_cdm_entity($cdm_entity)){
        $annotations_or_dtos = $cdm_entity->annotations;
    }else if (isset($cdm_entity->citation)) {
        //the cdm_entity is already a source
        return $foot_note_keys;
    }else if (is_cdm_entity($cdm_entity) || $cdm_entity->class == 'DerivedUnitFacade' || $cdm_entity->class == 'TypeDesignationDTO'){
        $annotations_or_dtos = cdm_fetch_visible_annotations($cdm_entity);
    }



  if (isset($annotations_or_dtos) && is_array($annotations_or_dtos)) {
    foreach ($annotations_or_dtos as $annotation_or_dto) {
      $foot_note_keys[] = FootnoteManager::addNewFootnote($footnote_list_key, $annotation_or_dto->text);
    }
  }else if (isset($annotations_or_dtos) && isset($annotations_or_dtos->items) && $annotations_or_dtos->count>0){
      foreach ($annotations_or_dtos->items as $annotation_or_dto) {
          $foot_note_keys[] = FootnoteManager::addNewFootnote($footnote_list_key, $annotation_or_dto->text);
      }
  }

  return $foot_note_keys;
}

/**
 * Fetches the list of visible annotations for the cdm entity or for the comparable
 * object and returns a list of the annotation texts
 *
 * @param stdClass $cdm_entity
 *   A single CdmBase instance ore comparable object.
 *
 * @see cdm_fetch_visible_annotations()
 */
function cdm_entity_annotations_as_content(stdClass $cdm_entity) {

  $annotation_texts = [];
  $annotations_or_dtos = cdm_fetch_visible_annotations($cdm_entity);
  if (is_array($annotations_or_dtos)) {
    foreach ($annotations_or_dtos as $annotation_or_dto) {
      if($annotation_or_dto->text){
        $annotation_texts[] = $annotation_or_dto->text;
      }
    }
  }
  return $annotation_texts;
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
  if (isset_not_empty($footnote_keys)){
      usort($footnote_keys, function($a, $b) {
          if ($a == $b) return 0;
          if ($a == null) return -1;
          if ($b == null) return 1;
          if (is_numeric($a->keyStr) && is_numeric($b->keyStr)){
              return $a->keyStr - $b->keyStr;
          }else if (is_numeric($b->keyStr) && !is_numeric($a->keyStr)){
              return -1;
          }else if (is_numeric($a->keyStr) && !is_numeric($b->keyStr)){
              return 1;
          }
          return strlen($a->keyStr) - strlen($b->keyStr) ?: strcmp($a->keyStr, $b->keyStr);});
  }else{
      return $footnotes_markup;
  }

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
 * @return \FootnoteKey
 *  The FootnoteKey
 */
function handle_name_relationship_as_footnote($name_rel, $code_edition_source = true)
{
    $footnote_markup = '';
    $fnkey = null;
    if ($code_edition_source) {

        if (isset($name_rel->ruleConsidered) && $name_rel->ruleConsidered) {
            $footnote_markup = '<span class="rule_considered">' . $name_rel->ruleConsidered . '</span>';
        }
        if (isset($name_rel->ruleConsidered) && $name_rel->ruleConsidered && (isset($name_rel->codeEditionSource->citation) || isset($name_rel->codeEditionSource->label))){
            $footnote_markup .= '; ';
        }
        if (isset($name_rel->codeEditionSource->citation)) {
            $footnote_markup .= '<span class="reference">' . $name_rel->codeEditionSource->citation->titleCache . '</span>';
        }else if (isset($name_rel->codeEditionSource) && isset_not_empty($name_rel->codeEditionSource->label) && isset($name_rel->codeEditionSource->label[0]->label)){
            $footnote_markup .= '<span class="reference">' . $name_rel->codeEditionSource->label[0]->label . '</span>';
        }

        if ($footnote_markup) {
            $fnkey = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $footnote_markup);
        }
    }else{
        if (isset($name_rel->source->citation)) {
            $footnote_markup .= '<span class="reference">' . $name_rel->source->citation->titleCache . '</span>';
        }
        if (isset($name_rel->source->citationMicroReference) && $name_rel->source->citationMicroReference) {
            $footnote_markup .= (isset($name_rel->source->citation) ? ':' : '') . ' <span class="reference_detail">' . $name_rel->source->citationMicroReference . '</span>';
        }
        if ($footnote_markup) {
            $fnkey = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $footnote_markup);
        }
    }
  return $fnkey;
}

/**
 * Creates footnote markup for the name relationship and
 * registers it in the {@link \FootnoteManager}. The resulting foonote
 * key is returned as markup.
 *
 * @param $name_rel
 *   The cdm name relationship as DTO
 * @return \FootnoteKey
 *  The FootnoteKey
 */
function handle_new_name_relationship_as_footnote($name_rel)
{
    $footnote_markup = '';
    $fnkey = null;
    if (isset($name_rel->ruleConsidered) && $name_rel->ruleConsidered) {
        $footnote_markup = '<span class="rule_considered">' . $name_rel->ruleConsidered . '</span>; ';
    }
    if (isset($name_rel->source->label)) {
        $footnote_markup .= '<span class="reference">' . $name_rel->source->label . '</span>';
    }
    //I think we do not need this because the label is created by Formatter
   // if (isset($name_rel->source->citationMicroReference) && $name_rel->source->citationMicroReference) {
    //    $footnote_markup .= (isset($name_rel->source->citation) ? ':' : '') . ' <span class="reference_detail">' . $name_rel->source->citationMicroReference . '</span>';
    //}
    if ($footnote_markup) {
        $fnkey = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $footnote_markup);
    }
    return $fnkey;
}

/**
 * Creates footnote markup for nomenclatural status and
 * registers it in the {@link \FootnoteManager}. The resulting foonote
 * key is returned as markup.
 *
 * @param $nom_status
 * @return \FootnoteKey
 *  The FootnoteKey
 */
function handle_nomenclatural_status_as_footnote($nom_status)
{
  // NomenclaturalStatus is a subclass of ReferencedEntityBase
  // and has the same structure as TaxonNameRelationship

  return handle_name_relationship_as_footnote($nom_status, true);
}

/**
 * Creates footnote markup for nomenclatural reference of the name and
 * registers it in the {@link \FootnoteManager}. The resulting foonote
 * key is returned as markup.
 *
 * @param $name
 * The name whose nomenclatural reference is to be shown as footnote
 * @return \FootnoteKey
 *  The FootnoteKey
 */
function handle_nomenclatural_reference_as_footnote($name)
{
  $footnote_markup = '';
  $footnote_key_markup = '';
  if (isset($name->nomenclaturalSource->citation) && $name->nomenclaturalSource->citation) {
    $footnote_markup .= '<span class="reference">' . $name->nomenclaturalSource->citation->titleCache . '</span>';
  }
  if (isset($name->nomenclaturalSource->citationMicroReference)) {
    $footnote_markup .= ($footnote_key_markup ? ':' : '') . '<span class="reference_detail">' . $name->nomenclaturalSource->citationMicroReference . '</span>';
  }
  $fnkey = null;
  if ($footnote_markup) {
    $fnkey = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), $footnote_markup);
  }
  return $fnkey;
}

/* ============ annotations_and_sources handling =================== */
const ANNOTATIONS_AND_SOURCE_CONFIG_DEFAULT = [
  'sources_as_content' => FALSE,
  'annotations_as_content' => FALSE,
  'link_to_name_used_in_source' => TRUE,
  'link_to_reference' => TRUE,
  'add_footnote_keys' => TRUE,
  'bibliography_aware' => FALSE
];

/**
 * Provides the default configuration for handle_annotations_and_sources() in the
 * synonymy.
 *
 * @return bool[]
 */
function synonymy_annotations_and_source_config() {
  static $annotations_and_sources_config = null;
  if(!$annotations_and_sources_config){
    $bibliography_settings = get_bibliography_settings();
    $annotations_and_sources_config = [
      'sources_as_content' => FALSE,
      'link_to_name_used_in_source' => TRUE,
      'link_to_reference' => TRUE,
      'add_footnote_keys' => TRUE,
      'bibliography_aware' => $bibliography_settings['enabled'] == 1
    ];
  }
  return $annotations_and_sources_config;
}

/**
 * Provides the default configuration for typedesignations which
 * are passed to the handle_annotations_and_sources()
 * function:
 * - 'sources_as_content' => TRUE,
 * - 'link_to_name_used_in_source' => FALSE,
 * - 'link_to_reference' => TRUE,
 * - 'add_footnote_keys' => FALSE,
 * - 'bibliography_aware' => FALSE
 *
 * @return array
 */
function annotations_and_sources_config_typedesignations() {
  static $annotations_and_sources_config = [
    'sources_as_content' => FALSE,
   'link_to_name_used_in_source' => FALSE,
    'link_to_reference' => TRUE,
    'add_footnote_keys' => TRUE,
    'bibliography_aware' => FALSE
  ];
  return $annotations_and_sources_config;
}

function annotations_and_sources_config_media() {
  static $annotations_and_sources_config = [
    'sources_as_content' => FALSE,
    'link_to_name_used_in_source' => FALSE,
    'link_to_reference' => TRUE,
    'add_footnote_keys' => TRUE,
    'bibliography_aware' => FALSE
  ];
  return $annotations_and_sources_config;
}

/**
 * Provides the default configuration for occurrences which
 * are passed to the handle_annotations_and_sources()
 * function:
 * - 'sources_as_content' => TRUE,
 * - 'link_to_name_used_in_source' => TRUE,
 * - 'link_to_reference' => FALSE,
 * - 'add_footnote_keys' => FALSE,
 * - 'bibliography_aware' => FALSE
 *
 * @return array
 */
function annotations_and_sources_config_occurrences() {
  static $annotations_and_sources_config = [
    'sources_as_content' => TRUE,
    'link_to_name_used_in_source' => TRUE,
    'link_to_reference' => FALSE,
    'add_footnote_keys' => FALSE,
    'bibliography_aware' => FALSE
  ];
  return $annotations_and_sources_config;
}

function annotation_and_source_config_taxon_node() {
  static $conf = null;
  if(!$conf){
    $bibliography_settings = get_bibliography_settings();
    $conf = [
      'sources_as_content' => false,
      'link_to_name_used_in_source' => false,
      'link_to_reference' => true,
      'add_footnote_keys' => true,
      'bibliography_aware' => $bibliography_settings['enabled'] == 1
    ];
  }
  return $conf;
}

/**
 * Creates a handle_annotations_and_sources configuration array from feature_block_settings.
 *
 * The handle_annotations_and_sources configuration array is meant to be used for the
 * method handle_annotations_and_sources().
 *
 * @param $feature_block_settings array
 *
 * @return array
 *   The configuration array for handle_annotations_and_sources()
 */
function handle_annotations_and_sources_config($feature_block_settings){

  $config = [
    'sources_as_content' => $feature_block_settings['sources_as_content'] == 1,
    'link_to_name_used_in_source' => $feature_block_settings['link_to_name_used_in_source'] == 1,
    'link_to_reference' => $feature_block_settings['link_to_reference'] == 1,
    'add_footnote_keys' => (
      $feature_block_settings['sources_as_content'] !== 1 ||
      $feature_block_settings['sources_as_content_to_bibliography'] == 1
    ),
    'bibliography_aware' => TRUE // FIXME shouldn't this be retrieved from the settings?
  ];
  return $config;
}

/**
 * @param $entity
 *    The cdm entity for which the annotations and sources are to be handled.
 * @param $inline_text_prefix
 *   Only used to decide if the source references should be enclosed in
 *   brackets or not when displayed inline. This text will not be included into
 *   the response.
 * @param $footnote_list_key_suggestion string
 *   Optional parameter. If this parameter is left empty (null, 0, "") the
 *   footnote key will be determined by the nested method calls by calling
 *   RenderHints::getFootnoteListKey().
 *
 * @return AnnotationsAndSources
 *  an object with the following elements:
 *   - foot_note_keys: all footnote keys as markup
 *   - source_references: an array of the source references citations
 *   - names used in source: an associative array of the names in source,
 *        the name in source strings are de-duplicated
 *        !!!NOTE!!!!: this field will most probably be removed soon (TODO)
 */
function handle_annotations_and_sources($entity, $inline_text_prefix = null, $footnote_list_key_suggestion = null, AnnotationsAndSources $annotationsAndSources = null, $is_create_annotation_footnotes = true) {

  if($annotationsAndSources == null){
    $annotationsAndSources = new AnnotationsAndSources();
  }


  // some entity types only have single sources:
  $sources = cdm_entity_sources_sorted($entity);



  $config = RenderHints::getAnnotationsAndSourceConfig();

  if ($config['sources_as_content']) {
    foreach ($sources as $source) {
      if (_is_original_source_type($source)) {
        if (is_cdm_entity($source) || isset_not_empty($source->label)){
          $reference_citation = render_original_source(
            $source,
            $config['link_to_reference'],
            $config['link_to_name_used_in_source']
          );
        }else{
          $reference_citation = render_original_source_dto(
            $source,
            $config['link_to_reference'],
            $config['link_to_name_used_in_source']
          );
        }


        if ($reference_citation) {
          if (empty($inline_text_prefix)) {
            $annotationsAndSources->addSourceReferencesCitation($reference_citation);
          } else {
            $annotationsAndSources->addSourceReferencesCitation(' (' . $reference_citation . ')');
          }
        }

        // also put the name in source into the array, these are already included in the $reference_citation but are
        // still required to be available separately in some contexts.
        $name_in_source_render_array = compose_name_in_source(
          $source,
          $config['link_to_name_used_in_source']
        );

        if (!empty($name_in_source_render_array)) {
          $annotationsAndSources->putNamesUsedInSource($name_in_source_render_array['#_plaintext'], drupal_render($name_in_source_render_array));
        }
      }
    } // END of loop over sources

    // annotations footnotes separate from sources
    if ($is_create_annotation_footnotes){
      $footnote_keys = cdm_entity_annotations_as_footnote_keys($entity, $footnote_list_key_suggestion);
      $annotationsAndSources->addAllFootNoteKeys($footnote_keys);
    }


  } // END of references inline

  // footnotes for sources and annotations or put into into bibliography if requested ...
  $do_foonotes_as_content = isset($config['annotations_as_content']) && $config['annotations_as_content'];
  if($do_foonotes_as_content){
    $annotationsAndSources->setAnnotations(cdm_entity_annotations_as_content($entity));
  }
  if ($config['add_footnote_keys']) {
    $annotationsAndSources->addAllFootNoteKeys(cdm_entity_footnotes(
      $entity,
      $footnote_list_key_suggestion,
      $config['link_to_reference'],
      $config['link_to_name_used_in_source'],
      (!empty($config['bibliography_aware']) ? $config['bibliography_aware'] : FALSE),
      !$do_foonotes_as_content
    ));
  }
  return $annotationsAndSources;
}

/**
 * TODO: This needs to be adapted to the new DTO implementation
 * @param $entity
 *    The cdm entity dto for which the annotations and sources are to be handled.
 * @param $inline_text_prefix
 *   Only used to decide if the source references should be enclosed in
 *   brackets or not when displayed inline. This text will not be included into
 *   the response.
 * @param $footnote_list_key_suggestion string
 *   Optional parameter. If this parameter is left empty (null, 0, "") the
 *   footnote key will be determined by the nested method calls by calling
 *   RenderHints::getFootnoteListKey().
 *
 * @return AnnotationsAndSources
 *  an object with the following elements:
 *   - foot_note_keys: all footnote keys as markup
 *   - source_references: an array of the source references citations
 *   - names used in source: an associative array of the names in source,
 *        the name in source strings are de-duplicated
 *        !!!NOTE!!!!: this field will most probably be removed soon (TODO)
 */
function handle_new_annotations_and_sources($entity, $inline_text_prefix = null, $footnote_list_key_suggestion = null, AnnotationsAndSources $annotationsAndSources = null) {

    if($annotationsAndSources == null){
        $annotationsAndSources = new AnnotationsAndSources();
    }

    // some entity types only have single sources: TODO maybe we do not need this anymore and do all sorting on serverside

    if (!isset($entity->sources) && !isset($entity->annotations)){
        return $annotationsAndSources;
    }
    $sources = cdm_entity_new_sources_sorted($entity);
    //if ($sources == null){
    //    return $annotationsAndSources;
    //}
    $config = RenderHints::getAnnotationsAndSourceConfig();

    if ($config['sources_as_content']) {
        foreach ($sources as $source) {
            if (_is_original_source_type($source)) {
                $reference_citation = render_original_source(
                    $source,
                    $config['link_to_reference'],
                    $config['link_to_name_used_in_source']
                );

                if ($reference_citation) {
                    if (empty($inline_text_prefix)) {
                        $annotationsAndSources->addSourceReferencesCitation($reference_citation);
                    } else {
                        $annotationsAndSources->addSourceReferencesCitation(' (' . $reference_citation . ')');
                    }
                }

                // also put the name in source into the array, these are already included in the $reference_citation but are
                // still required to be available separately in some contexts.
                $name_in_source_render_array = compose_name_in_source(
                    $source,
                    $config['link_to_name_used_in_source']
                );

                if (!empty($name_in_source_render_array)) {
                    $annotationsAndSources->putNamesUsedInSource($name_in_source_render_array['#_plaintext'], drupal_render($name_in_source_render_array));
                }
            }
        } // END of loop over sources

     //annotations footnotes separate from sources
        $footnote_keys = cdm_entity_annotations_as_footnote_keys($entity, $footnote_list_key_suggestion);
        $annotationsAndSources->addAllFootNoteKeys($footnote_keys);

    } // END of references inline

    // footnotes for sources and annotations or put into bibliography if requested ...
    $do_foonotes_as_content = isset($config['annotations_as_content']) && $config['annotations_as_content'];
    if($do_foonotes_as_content){
        $annotationsAndSources->setAnnotations(cdm_entity_annotations_as_content($entity));
    }
    //TODO
    if ($config['add_footnote_keys']) {

       $annotationsAndSources->addAllFootNoteKeys(cdm_entity_footnotes(
            $entity,
            $footnote_list_key_suggestion,
            $config['link_to_reference'],
            $config['link_to_name_used_in_source'],
            (!empty($config['bibliography_aware']) ? $config['bibliography_aware'] : FALSE),
            !$do_foonotes_as_content
        ));
        //$annotationsAndSources->addAllFootNoteKeys($footnote_keys);
    }
    return $annotationsAndSources;
}
function handle_new_only_sources($entity, $inline_text_prefix = null, $footnote_list_key_suggestion = null, AnnotationsAndSources $annotationsAndSources = null) {

    if($annotationsAndSources == null){
        $annotationsAndSources = new AnnotationsAndSources();
    }

    // some entity types only have single sources: TODO maybe we do not need this anymore and do all sorting on serverside

    if (!isset($entity->sources) && !isset($entity->source)){
        return $annotationsAndSources;
    }
    $sources = cdm_entity_new_sources_sorted($entity);
    //if ($sources == null){
    //    return $annotationsAndSources;
    //}
    $config = RenderHints::getAnnotationsAndSourceConfig();

    if ($config['sources_as_content']) {
        foreach ($sources as $source) {
            if (_is_original_source_type($source)) {
                $reference_citation = render_original_source(
                    $source,
                    $config['link_to_reference'],
                    $config['link_to_name_used_in_source']
                );

                if ($reference_citation) {
                    if (empty($inline_text_prefix)) {
                        $annotationsAndSources->addSourceReferencesCitation($reference_citation);
                    } else {
                        $annotationsAndSources->addSourceReferencesCitation(' (' . $reference_citation . ')');
                    }
                }

                // also put the name in source into the array, these are already included in the $reference_citation but are
                // still required to be available separately in some contexts.
                $name_in_source_render_array = compose_name_in_source(
                    $source,
                    $config['link_to_name_used_in_source']
                );

                if (!empty($name_in_source_render_array)) {
                    $annotationsAndSources->putNamesUsedInSource($name_in_source_render_array['#_plaintext'], drupal_render($name_in_source_render_array));
                }
            }
        } // END of loop over sources



    } // END of references inline



    //TODO
    if ($config['add_footnote_keys']) {
        $annotationsAndSources->addAllFootNoteKeys(cdm_entity_footnotes(
            $entity,
            $footnote_list_key_suggestion,
            $config['link_to_reference'],
            $config['link_to_name_used_in_source'],
            (!empty($config['bibliography_aware']) ? $config['bibliography_aware'] : FALSE),
            false
        ));
    }
    return $annotationsAndSources;
}
function handle_new_only_annotations($entity, $inline_text_prefix = null, $footnote_list_key_suggestion = null, AnnotationsAndSources $annotationsAndSources = null) {

    if($annotationsAndSources == null){
        $annotationsAndSources = new AnnotationsAndSources();
    }

    // some entity types only have single sources: TODO maybe we do not need this anymore and do all sorting on serverside

    if (!isset($entity->annotations)){
        return $annotationsAndSources;
    }

    $config = RenderHints::getAnnotationsAndSourceConfig();

    if ($config['sources_as_content']) {

        // annotations footnotes separate from sources
        $footnote_keys = cdm_entity_annotations_as_footnote_keys($entity, $footnote_list_key_suggestion);
        $annotationsAndSources->addAllFootNoteKeys($footnote_keys);

    } // END of references inline

    // footnotes for sources and annotations or put into bibliography if requested ...
    $do_foonotes_as_content = isset($config['annotations_as_content']) && $config['annotations_as_content'];
    if($do_foonotes_as_content){
        $annotationsAndSources->setAnnotations(cdm_entity_annotations_as_content($entity));
    }
    //TODO
    if ($config['add_footnote_keys']) {
        $annotationsAndSources->addAllFootNoteKeys(cdm_entity_footnotes(
            $entity,
            $footnote_list_key_suggestion,
            $config['link_to_reference'],
            $config['link_to_name_used_in_source'],
            (!empty($config['bibliography_aware']) ? $config['bibliography_aware'] : FALSE),
            !$do_foonotes_as_content,
            false
        ));
    }
    return $annotationsAndSources;
}


/**
 * Get the source or the sources from a cdm entity and return them ordered by see compare_original_sources()
 * (Some entity types only have single sources)
 * @param $entity
 *
 * @return array
 */
function cdm_entity_sources_sorted($entity) {
  if (isset($entity->source) && is_object($entity->source)) {
    $sources = [$entity->source];
  }
  else if (isset($entity->sources)) {
    $sources = $entity->sources;
  }else {
      // try to load the sources
      if(isset_not_empty($entity->uuid) && is_cdm_entity($entity)){
          $sources = cdm_ws_fetch_all(cdm_compose_url('portal/' . cdm_ws_base_uri($entity->class), [ $entity->uuid, 'sources']));
      } else {
          // this may happen with DerivedUnitFaced, also with others?
          $sources = [];
      }
  }


  if (isset($sources-> items)){
      usort($sources-> items, 'compare_original_sources');
      return $sources-> items;
  }else if (isset_not_empty($sources)){
      usort($sources, 'compare_original_sources');
  }

  return $sources;
}

/**
 * Get the source or the sources from a cdm entity and return them ordered by see compare_original_sources()
 * (Some entity types only have single sources) TODO this should be done serverside and the sources should be added to the dto
 * @param $entity
 *
 * @return array
 */
function cdm_entity_new_sources_sorted($entity) {
    if (isset_not_empty($entity->source) && is_object($entity->source)) {
        $sources = [$entity->source];
    }
    else if (isset($entity->sources)) {
        $sources = $entity->sources;
    }
    else {
        // try to load the sources
        if(isset_not_empty($entity->uuid) && isset_not_empty($entity->class)){
            $sources = cdm_ws_fetch_all(cdm_compose_url('portal/' . cdm_ws_base_uri($entity->class), [ $entity->uuid, 'sources']));
            if (!is_array($sources)){
                $sources = [];
            }
        } else {
            // this may happen with DerivedUnitFaced, also with others?
            $sources = [];
        }
    }
    if (!is_array($sources) && isset_not_empty($sources->items)){
        $sources = $sources->items;
    }
    usort($sources, 'compare_original_sources');
    return $sources;
}

/**
 * Compare two different footnotes objects.
 *
 * The comparison is based on the footnote key. The one which is
 * displayed as footnote number.
 *
 * @param mixed $a
 *   Footnote object $a.
 * @param mixed $b
 *   Footnote object $b.
 *
 * @return int
 *
 * @deprecated seems to be unused: TODO remove!
 */
function footnotes_key_compare($a, $b) {
  $res = 0;
  if (empty($a) || empty($b)) {
    return $res;
  }
  if ($a->keyStr < $b->keyStr) {
    $res = -1;
  }
  elseif ($a->keyStr > $b->keyStr) {
    $res = 1;
  }
  return $res;
}


