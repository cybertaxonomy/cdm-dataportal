<?php

/**
 * Returns the localized representations of the modifiers hold by the
 * supplied cdm instance concatenated into one string.
 *
 * @param object $iModifieable
 *   cdm instance of an class implementing the interface IModifieable:
 *   DescriptionElementBase, StateDate, State
 *
 * @return String
 *   localized representations of the modifiers hold by the
 *   supplied cdm instance concatenated into one string
 */
function cdm_modifers_representations($iModifieable, $glue = ', ') {
  $modifiers_strings = array();
  if (isset($iModifieable->modifiers)) {
    foreach ($iModifieable->modifiers as $modifier) {
      $modifiers_strings[] = cdm_term_representation($modifier);
    }
  }
  return implode(', ', $modifiers_strings);
}

/**
 * Filters the given set of description elements and prefers computed elements over others.
 *
 * Computed description elements
 * are identified by the MarkerType.COMPUTED()
 *
 * If the given set contains at least one computed element only
 * the computed elements are returned.
 *
 * @param array $description_elements
 *   An array of CDM DescriptionElementBase instances
 *
 * @return array
 *   only the computed description elements otherwise all others.
 *
 * @deprecated this is replaced by the cdmlib DistributionUtil class!!!
 */
function cdm_description_elements_prefer_computed($description_elements){

  $computed_elements = array();
  $other_elements = array();

  if (!empty($description_elements)) {
    foreach ($description_elements as $element) {
      if (cdm_entity_has_marker($element, UUID_MARKERTYPE_COMPUTED)) {
        $computed_elements[] = $element;
      }
      else {
        $other_elements[] = $element;
      }
    }
  }

  if (count($computed_elements) > 0) {
    return $computed_elements;
  }
  else {
    return $other_elements;
  }
}

/**
 * Creates a query parameter array based on the setting stored in the drupal variable CDM_DISTRIBUTION_FILTER.
 *
 * @return array
 *   An array with distribution filter query parameters
 */
function cdm_distribution_filter_query() {
  $cdm_distribution_filter = get_array_variable_merged(CDM_DISTRIBUTION_FILTER, CDM_DISTRIBUTION_FILTER_DEFAULT);
  $query = array();

  if ($cdm_distribution_filter['filter_rules']['statusOrderPreference']) {
    $query['statusOrderPreference'] = 1;
  }
  if ($cdm_distribution_filter['filter_rules']['subAreaPreference']) {
    $query['subAreaPreference'] = 1;
  }
  if (is_array($cdm_distribution_filter['hideMarkedAreas']) && count($cdm_distribution_filter['hideMarkedAreas']) > 0) {
    $query['hideMarkedAreas'] = '';
    foreach ($cdm_distribution_filter['hideMarkedAreas'] as $marker_type => $enabled) {
      if ($enabled) {
        $query['hideMarkedAreas'] .= ($query['hideMarkedAreas'] ? ',' : '') . $marker_type;
      }
    }
  }

  return $query;
}

/**
 * Merge the fields 'annotations', 'markers', 'sources', 'media' from the source CDM DescriptionElement into  the target.
 *
 * @param object $target
 *   The source CDM DescriptionElement
 * @param object $source
 *   The target CDM DescriptionElement
 */
function cdm_merge_description_elements(&$target, &$source) {
  static $fields_to_merge = array('annotations', 'markers', 'sources', 'media');

  foreach ($fields_to_merge as $field) {
    if (is_array($source->$field)) {
      if (!is_array($target->$field)) {
        $target->$field = $source->$field;
      }
      else {
        $target->$field = array_merge($target->$field, $source->$field);
      }
    }
  }
}

/**
 * Adds an entry to the end of the table of content items list
 *
 * The  table of content items are crated internally by calling
 * toc_list_item() the resulting item is added to the statically cached
 * list of toc elements
 *
 * @param string $label
 *   The label of toc entry
 * @param $class_attribute_suffix
 *   The suffix to be appended to the class attribute prefix: "feature-toc-item-"
 * @param string $fragment
 *   Optional parameter to define a url fragment different from the $label,
 *   if the $fragment is not defined the $label will be used
 */
function cdm_toc_list_add_item($label, $class_attribute_suffix, $fragment = NULL, $as_first_element = FALSE) {
  $toc_list_items = &cdm_toc_list();

  if (!$fragment) {
    $fragment = $label;
  }
  $fragment = generalizeString($fragment);

  $class_attributes = 'feature-toc-item-' . $class_attribute_suffix;

  $new_item = toc_list_item(
    theme(
      'cdm_feature_name',
      array('feature_name' => $label)),
      array('class' => $class_attributes),
      $fragment
    );

  if ($as_first_element) {
    array_unshift($toc_list_items, $new_item);
  }
  else {
    $toc_list_items[] = $new_item;
  }

}

/**
 * Returns the statically cached table of content items as render array.
 *
 * @see cdm_toc_list_add_item()
 *
 * @return array
 *   a render array of table of content items suitable for theme_item_list()
 */
function &cdm_toc_list(){
  $toc_list_items = &drupal_static('toc_list_items', array());

  return $toc_list_items;
}

/**
 * Prepares an empty Drupal block for displaying description elements of a specific CDM Feature.
 *
 * The block can also be used for pseudo Features like a bibliography. Pseudo features are
 * derived from other data on the fly and so not exist as such in the cdm data. In case
 * of pseudo features the $feature is left empty
 *
 * @param $feature_name
 *   A label describing the feature, usually the localized feature representation.
 * @param object $feature
 *   The CDM Feature for which the block is created. (optional)
 * @return object
 *   A Drupal block object
 */
function feature_block($feature_name, $feature = NULL) {
  $block = new stdclass(); // Empty object.
  $block->module = 'cdm_dataportal';
  $block->delta = generalizeString($feature_name);
  $block->region = NULL;
  $block->subject = '<a name="' . $block->delta . '"></a><span class="' . html_class_attribute_ref($feature) . '">'
    . theme('cdm_feature_name', array('feature_name' => $feature_name))
    . '</span>';
  $block->module = "cdm_dataportal-feature";
  $block->content = '';
  return $block;
}


/**
 * Returns a list of a specific type of IdentificationKeys.
 *
 * The list can be restricted by a taxon.
 *
 * @param string $type
 *   The simple name of the cdm class implementing the interface
 *   IdentificationKey, valid values are:
 *   PolytomousKey, MediaKey, MultiAccessKey.
 * @param string $taxonUuid
 *   If given this parameter restrict the listed keys to those which have
 *   the taxon identified be this uuid in scope.
 *
 * @return array
 *   List with identification keys.
 */
function _list_IdentificationKeys($type, $taxonUuid = NULL, $pageSize = NULL, $pageNumber = NULL) {
  if (!$type) {
    drupal_set_message(t('Type parameter is missing'), 'error');
    return;
  }
  $cdm_ws_pasepath = NULL;
  switch ($type) {
    case "PolytomousKey":
      $cdm_ws_pasepath = CDM_WS_POLYTOMOUSKEY;
      break;

    case "MediaKey":
      $cdm_ws_pasepath = CDM_WS_MEDIAKEY;
      break;

    case "MultiAccessKey":
      $cdm_ws_pasepath = CDM_WS_MULTIACCESSKEY;
      break;

  }

  if (!$cdm_ws_pasepath) {
    drupal_set_message(t('Type parameter is not valid: ') . $type, 'error');
  }

  $queryParameters = '';
  if (is_numeric($pageSize)) {
    $queryParameters = "pageSize=" . $pageSize;
  }
  else {
    $queryParameters = "pageSize=0";
  }

  if (is_numeric($pageNumber)) {
    $queryParameters = "pageNumber=" . $pageNumber;
  }
  else {
    $queryParameters = "pageNumber=0";
  }
  $queryParameters = NULL;
  if ($taxonUuid) {
    $queryParameters = "findByTaxonomicScope=$taxonUuid";
  }
  $pager = cdm_ws_get($cdm_ws_pasepath, NULL, $queryParameters);

  if (!$pager || $pager->count == 0) {
    return array();
  }
  return $pager->records;
}


/**
 * Creates a list item for a table of content, suitable as data element for a themed list
 *
 * @see theme_list()
 *
 * @param $label
 * @param $http_request_params
 * @param $attributes
 * @return array
 */
function toc_list_item($label, $attributes = array(), $fragment = null) {

  // we better cache here since drupal_get_query_parameters has no internal static cache variable
  $http_request_params = drupal_static('http_request_params', drupal_get_query_parameters());

  $item =  array(
    'data' => l(
      $label,
      $_GET['q'],
      array(
        'attributes' => array('class' => array('toc')),
        'fragment' => generalizeString($label),
        'query' => $http_request_params,
      )
    ),
  );
  $item['attributes'] = $attributes;
  return $item;
}

/**
 * Creates the footnotes for the given CDM DescriptionElement instance.
 *
 * Footnotes are created for annotations and original sources,
 * optionally the sources are put into a separate bibliography.
 *
 * @param $descriptionElement
 *   A CDM DescriptionElement instance
 * @param $separator
 *   Optional parameter. The separator string to concatenate the footnote ids, default is ','
 * @param $footnote_list_key_suggestion
 *   will be overridden for original sources if the bibliography block is enabled
 *
 * @return String
 *   The foot note keys
 */
function cdm_create_description_element_footnotes($description_element, $separator = ',',
          $footnote_list_key_suggestion = null, $do_link_to_reference = FALSE,
          $do_link_to_name_used_in_source = FALSE
    ){


  // Annotations as footnotes.
  $footNoteKeys = cdm_annotations_as_footnotekeys($description_element, $footnote_list_key_suggestion);

  // Source references as footnotes.
  $bibliography_settings = get_bibliography_settings();
  $original_source_footnote_tag = $bibliography_settings['enabled'] == 1 ? 'div' : null; // null will cause original_source_footnote_list_key to use the default

  foreach ($description_element->sources as $source) {
    if (_is_original_source_type($source)) {
      $fn_key = FootnoteManager::addNewFootnote(
        original_source_footnote_list_key($footnote_list_key_suggestion),
        theme('cdm_OriginalSource', array(
          'source' => $source,
          'doLink' => $do_link_to_reference,
          'do_link_to_name_used_in_source' => $do_link_to_name_used_in_source

        )),
        $original_source_footnote_tag
      );
      // Ensure uniqueness of the footnote keys.
      cdm_add_footnote_to_array($footNoteKeys, $fn_key);
    }
  }
  // Sort and render footnote keys.
  $footnoteKeyListStr = '';
  asort($footNoteKeys);
  foreach ($footNoteKeys as $footNoteKey) {
    $footnoteKeyListStr .= theme('cdm_footnote_key',
      array(
        'footnoteKey' => $footNoteKey,
        'separator' => ($footnoteKeyListStr ? $separator : '')));
  }
  return $footnoteKeyListStr;
}


  /**
   * @param $element
   * @param $feature_block_settings
   * @param $element_text
   *   used to decide if the source references should be enclosed in brackets or not
   * @param $footnote_list_key_suggestion
   * @return array
   *   an associative array with the following elements:
   *   - foot_note_keys: all footnote keys as markup
   *   - source_references: an array of the source references citations
   *   - names used in source: an associative array of the names in source,
   *        the name in source strings are de-duplicated
   *        !!!NOTE!!!!: this field will most probably be removed soon (TODO)
   *
   *
   */
  function handle_annotations_and_sources($element, $feature_block_settings, $element_text, $footnote_list_key_suggestion) {
    $annotations_and_sources = array(
      'foot_note_keys' => NULL,
      'source_references' => array(),
      'names_used_in_source' => array()
    );

    if ($feature_block_settings['sources_as_content'] == 1) {
      foreach ($element->sources as $source) {

        $referenceCitation = theme('cdm_OriginalSource',
          array(
            'source' => $source,
            'doLink' => $feature_block_settings['link_to_reference'] == 1,
            'do_link_to_name_used_in_source' => $feature_block_settings['link_to_name_used_in_source'] == 1,
          )
        );

        if ($referenceCitation) {
          if (empty($element_text)) {
            $annotations_and_sources['source_references'][] = $referenceCitation;
          }
          else {
            $annotations_and_sources['source_references'][] = ' (' . $referenceCitation . ')';
          }
        }

        $name_in_source_render_array = compose_name_in_source(
          $source,
          $feature_block_settings['link_to_name_used_in_source'] == 1
        );

        if(!empty($name_in_source_render_array)){
          $annotations_and_sources['names_used_in_source'][$name_in_source_render_array['#_plaintext']] = drupal_render($name_in_source_render_array);
        }
      } // END of loop over sources

      // annotations footnotes separate.
      $annotations_and_sources['foot_note_keys'] = theme('cdm_annotations_as_footnotekeys',
        array(
          'cdmBase_list' => $element,
          'footnote_list_key' => $footnote_list_key_suggestion,
        )
      );

    } // END of references inline

    // put sources into bibliography if requested ...
    if ($feature_block_settings['sources_as_content'] !== 1 || $feature_block_settings['sources_as_content_to_bibliography'] == 1) {
      $annotations_and_sources['foot_note_keys'] = cdm_create_description_element_footnotes(
        $element, ',',
        $footnote_list_key_suggestion,
        $feature_block_settings['link_to_reference'] == 1,
        $feature_block_settings['link_to_name_used_in_source'] == 1
      );
    }

    return $annotations_and_sources;
  }


  /**
   *
   *
   * @return string
   *  the footnote_list_key
   */
  function original_source_footnote_list_key($key_suggestion = null) {
    if(!$key_suggestion){
      $key_suggestion = RenderHints::getFootnoteListKey();
    }
    $bibliography_settings = get_bibliography_settings();
    $footnote_list_key = $bibliography_settings['enabled'] == 1 ? 'BIBLIOGRAPHY' : 'BIBLIOGRAPHY-' . $key_suggestion;
    return $footnote_list_key;
  }

  /**
   * Provides the according tag name for the description element markup which fits the  $feature_block_settings['as_list'] value
   *
   * @param $feature_block_settings
   *   A feature_block_settings array, for details, please see get_feature_block_settings($feature_uuid = 'DEFAULT')
   */
  function cdm_feature_block_element_tag_name($feature_block_settings){
    switch ($feature_block_settings['as_list']){
      case 'ul':
      case 'ol':
        return 'li';
      case 'div':
        if(isset($feature_block_settings['element_tag'])){
          return $feature_block_settings['element_tag'];
        }
        return 'span';
      case 'dl':
        return 'dd';
      default:
        return 'div'; // should never happen, throw error instead?
    }
  }


/* ==================== COMPOSE FUNCTIONS =============== */

  /**
   * Returns a set of feature blocks for a taxon profile from the $mergedFeatureNodes of a given $taxon.
   *
   * The taxon profile consists of drupal block elements, one for the description elements
   * of a specific feature. The structure is defined by specific FeatureTree.
   * The chosen FeatureTree is merged with the list of description elements prior to using this method.
   *
   * The merged nodes can be obtained by making use of the
   * function cdm_ws_descriptions_by_featuretree().
   *
   * @see cdm_ws_descriptions_by_featuretree()
   *
   * @param $mergedFeatureNodes
   *
   * @param $taxon
   *
   * @return array
   *  A Drupal render array containing feature blocks and the table of content
   *
   * @ingroup compose
   */
  function compose_cdm_feature_nodes($mergedFeatureNodes, $taxon) {

    $block_list = array();

    RenderHints::pushToRenderStack('feature_nodes');

    $gallery_settings = getGallerySettings(CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME);

    // Create a drupal block for each feature
    foreach ($mergedFeatureNodes as $node) {

      if ((isset($node->descriptionElements['#type']) ||
          has_feature_node_description_elements($node)) && $node->feature->uuid != UUID_IMAGE) { // skip empty or supressed features

        $feature_name = cdm_term_representation($node->feature, 'Unnamed Feature');

        $block = feature_block($feature_name, $node->feature);
        $block->content = array();
        $block_content_is_empty = TRUE;
        $block_content_weight = 0;

        /*
         * Content/DISTRIBUTION.
         */

        if ($node->feature->uuid == UUID_DISTRIBUTION) {
          $block = compose_feature_block_distribution($taxon, $node->descriptionElements, $node->feature);
          $block_content_is_empty = FALSE;
        }
        /*
         * Content/COMMON_NAME.
         */
        else if ($node->feature->uuid == UUID_COMMON_NAME) {
          $common_names_render_array = compose_cdm_common_names($node->descriptionElements, $node->feature);
          $block->content[] = $common_names_render_array;
          $block_content_is_empty = FALSE;
        }

        else if ($node->feature->uuid == UUID_USE_RECORD) {
          $block_uses_content_html = theme('cdm_block_Uses', array('taxonUuid' => $taxon->uuid));
          $block->content[] = markup_to_render_array($block_uses_content_html);
          $block_content_is_empty = FALSE;
        }

        /*
         * Content/ALL OTHER FEATURES.
         */
        else {

          $media_list = array();
          $out_child_elements = '';

          if (isset($node->descriptionElements)) {
            $taxon_uuid = NULL;
            if(isset($taxon) ) {
              $taxon_uuid = $taxon->uuid;
            }
            $elements_render_array = compose_cdm_descriptionElements($node->descriptionElements, $node->feature, $taxon_uuid);
            $block_content_is_empty = empty($elements_render_array);
            $block->content[] = $elements_render_array;
          }

          // Content/ALL OTHER FEATURES/Subordinate Features
          // subordinate features are printed inline in one floating text,
          // it is expected hat subordinate features can "contain" TextData,
          // Qualitative- and Qualitative- DescriptioneElements
          if (isset($node->childNodes[0])) {

            // TODO support more than one level of children.
            // can this be solved by recursively calling this function?
            // @see http://dev.e-taxonomy.eu/trac/ticket/2393
            $text = '';
            foreach ($node->childNodes as $child) {

              if (isset($child->descriptionElements) && is_array($child->descriptionElements)) {
                foreach ($child->descriptionElements as $element) {

                  //TODO it woud be better if we could use compose_cdm_descriptionElements() directly instead of the below stuff

                  if (is_array($element->media)) {
                    // Append media of subordinate elements to list of main
                    // feature.
                    $media_list = array_merge($media_list, $element->media);
                  }

                  switch ($element->class) {
                    case 'TextData':
                      // FIXME use compose_cdm_descriptionElementTextData()
                      if(isset($element->multilanguageText_L10n->text)){
                        $out_child_elements = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
                      }
                      $out_child_elements = str_replace($child->feature->titleCache, '<em>' . $child->feature->representation_L10n . '</em>', $out_child_elements);
                      break;
                    case 'CategoricalData':
                      $out_child_elements  = '<em>' . $child->feature->representation_L10n . '</em> '
                        . theme('cdm_descriptionElement_CategoricalData', array('element' => $element));
                      break;
                    case 'QuantitativeData':
                      $out_child_elements = '<em>' . $child->feature->representation_L10n . '</em> '
                        . theme('cdm_descriptionElement_QuantitativeData', array('element' => $element));

                  }

                }
                $text .= " " . $out_child_elements;
                $out_child_elements = '';
              }
            }
            $block_content_is_empty = $block_content_is_empty && empty($text);
            $block->content[] = markup_to_render_array($text);
            $block_content_is_empty = $block_content_is_empty && empty($media_list);
          }

          $block->content[] = compose_feature_media_gallery($node, $media_list, $gallery_settings);
          /*
           * Footnotes for the feature block
           */
          $block->content[] = markup_to_render_array(theme('cdm_footnotes', array('footnoteListKey' => 'BIBLIOGRAPHY-' . $node->feature->uuid)));
          $block->content[] = markup_to_render_array(theme('cdm_footnotes', array('footnoteListKey' => $node->feature->uuid)));
          $block->content[] = markup_to_render_array(theme('cdm_annotation_footnotes', array('footnoteListKey' => $node->feature->uuid)));
        } // END all other features

        if(!$block_content_is_empty){ // skip empty block content
          $block_list[] = $block;
          cdm_toc_list_add_item(cdm_term_representation($node->feature), $node->feature->uuid);
        } // END: skip empty block content
      } // END: skip empty or supressed features
    } // END: creating a block per feature

    drupal_alter('cdm_feature_node_blocks', $block_list, $taxon);

    RenderHints::popFromRenderStack();

    return _block_get_renderable_array($block_list);
  }

  /**
   * @param $node
   * @param $media_list
   * @param $gallery_settings
   * @return array
   */
  function compose_feature_media_gallery($node, $media_list, $gallery_settings) {

    if (isset($node->descriptionElements)) {
      $media_list = array_merge($media_list, cdm_dataportal_media_from_descriptionElements($node->descriptionElements));
    }

    $captionElements = array('title', 'rights');

    if (isset($media_list[0]) && isset($gallery_settings['cdm_dataportal_media_maxextend']) && isset($gallery_settings['cdm_dataportal_media_cols'])) {
      $gallery = theme('cdm_media_gallerie', array(
        'mediaList' => $media_list,
        'galleryName' => CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME . '_' . $node->feature->uuid,
        'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
        'cols' => $gallery_settings['cdm_dataportal_media_cols'],
        'captionElements' => $captionElements,
      ));
      return markup_to_render_array($gallery);
    }

    return markup_to_render_array('');
  }

  /**
   * @param $taxon
   * @param $descriptionElements
   *   an associative array with two elements:
   *   - '#type': must be 'DTO'
   *   - 'DistributionInfoDTO': a CDM DistributionInfoDTO object as returned by the DistributionInfo web service
   * @param $feature
   *
   * @ingroup compose
   */
  function compose_feature_block_distribution($taxon, $descriptionElements, $feature) {
    $text_data_glue = '';
    $text_data_sortOutArray = FALSE;
    $text_data_enclosingTag = 'ul';
    $text_data_out_array = array();

    $distributionElements = NULL;
    $distribution_info_dto = NULL;
    $distribution_sortOutArray = FALSE;

    $feature_block_settings = get_feature_block_settings(UUID_DISTRIBUTION);

    // TODO use feature_block_settings instead of 'distribution_sort'
    if (variable_get('distribution_sort', 'NO_SORT') != 'NO_SORT') {
      $distribution_glue = '';
      $distribution_enclosingTag = 'dl';
    } else {
      $distribution_glue = '';
      $distribution_enclosingTag = 'ul';
    }

    if (!isset($descriptionElements['#type']) || !$descriptionElements['#type'] == 'DTO') {
      // skip the DISTRIBUTION section if there is no DTO type element
      return array(); // FIXME is it ok to return an empty array?
    }

    $block = feature_block(
      cdm_term_representation($feature, 'Unnamed Feature'),
      $feature
    );

    // $$descriptionElements['TextData'] is added to to the feature node in merged_taxon_feature_tree()
    if (isset($descriptionElements['TextData'])) {
      // --- TextData
      foreach ($descriptionElements['TextData'] as $text_data_element) {
        $asListElement = FALSE;
        $text_data_render_array = compose_cdm_descriptionElementTextData($text_data_element, $asListElement, $text_data_element->feature->uuid);
        $repr = drupal_render($text_data_render_array);

        if (!array_search($repr, $text_data_out_array)) { // de-duplication !!
          $text_data_out_array[] = $repr;
          // TODO HINT: sorting in compose_cdm_feature_block_elements will
          // not work since this array contains html attributes with uuids
          // and what is about cases like the bibliography where
          // any content can be prefixed with some foot-note anchors?
          $text_data_sortOutArray = TRUE;
          $text_data_glue = '<br/> ';
          $text_data_enclosingTag = 'p';
        }
      }
    }


    if ($text_data_out_array && variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0)) {
      $block->content[] = compose_cdm_feature_block_elements(
        $text_data_out_array,
        $feature,
        $text_data_glue,
        $text_data_sortOutArray,
        $text_data_enclosingTag
      );
    }

    // --- Distribution map
    $distribution_map_query_parameters = NULL;
    if (isset($descriptionElements['DistributionInfoDTO'])) {
      $distribution_map_query_parameters = $descriptionElements['DistributionInfoDTO']->mapUriParams;
    }
    $map_render_element = compose_distribution_map($taxon, $distribution_map_query_parameters);
    $block->content[] = $map_render_element;

    // --- tree or list
    $dto_out_array = array();
    if (isset($descriptionElements['DistributionInfoDTO'])) {
      $distribution_info_dto = $descriptionElements['DistributionInfoDTO'];

      // --- tree
      if (is_object($distribution_info_dto->tree)) {
        $dto_out_array[] = theme('cdm_description_ordered_distributions', array('distribution_tree' => $distribution_info_dto->tree));
      }

      // --- sorted element list
      if (is_array($distribution_info_dto->elements) && count($distribution_info_dto->elements) > 0) {
        foreach ($distribution_info_dto->elements as $descriptionElement) {
          if (is_object($descriptionElement->area)) {
            $sortKey = $descriptionElement->area->representation_L10n;
            $distributionElements[$sortKey] = $descriptionElement;
          }
        }
        ksort($distributionElements);
        $distribution_element_render_array = compose_cdm_descriptionElement_Distribution($distributionElements);
        $dto_out_array[] = drupal_render($distribution_element_render_array);

      }
      //
      $block->content[] = compose_cdm_feature_block_elements(
        $dto_out_array,
        $feature,
        $distribution_glue,
        $distribution_sortOutArray,
        $distribution_enclosingTag
      );
    }

    // --- TextData at the bottom
    if ($text_data_out_array && !variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0)) {
      $block->content[] = compose_cdm_feature_block_elements(
        $text_data_out_array,
        $feature,
        $text_data_glue,
        $text_data_sortOutArray,
        $text_data_enclosingTag
      );
    }

    $block->content[] = markup_to_render_array(theme('cdm_footnotes', array('footnoteListKey' => 'BIBLIOGRAPHY-' . UUID_DISTRIBUTION)));
    $block->content[] = markup_to_render_array(theme('cdm_footnotes', array('footnoteListKey' => UUID_DISTRIBUTION)));
    $block->content[] = markup_to_render_array(theme('cdm_annotation_footnotes', array('footnoteListKey' => UUID_DISTRIBUTION)));

    return $block;
  }


  /**
   * Composes a drupal render array for CDM TextData description elements.
   *
   * @param $element
   *    The CDM TextData description element which contains the textual  information.
   * @param $asListElement
   *    A boolean which determines whether the citations should
   *     be rendered as a list or not.
   *    TODO remove and replace $asListElement by $feature_block_settings['as_list']
   *  @param $feature_uuid
   *
   * @return array
   *   A drupal render array with the following elements being used:
   *    - #tag: either 'div', 'li', ...
   *    ⁻ #attributes: class attributes
   *    - #value_prefix: (optionally) contains footnote anchors
   *    - #value: contains the textual content
   *    - #value_suffix: (optionally) contains footnote keys
   *
   * @ingroup compose
   */
  function compose_cdm_descriptionElementTextData($element, $asListElement, $feature_uuid) {

    $footnote_list_key_suggestion = $feature_uuid;

    // FIXME $feature_block_settings should be passed as parameter!!!!!
    $feature_block_settings = get_feature_block_settings($feature_uuid);

    // FIXME remove this hack ---------------------------
    $default_theme = variable_get('theme_default', NULL);
    if($default_theme == 'garland_cichorieae' || $default_theme == 'cyprus'
      || $default_theme == 'flore_afrique_centrale' || $default_theme == 'flora_malesiana' || $default_theme == 'flore_gabon' ){
      $asListElement = $feature_block_settings['as_list'] == 'ul';
    }
    // --------------------------------------------------

    $render_array = array(
      '#type' => 'html_tag',
      '#tag' => $asListElement ? 'li' : 'span',
      '#attributes' => array(
        'class' => 'DescriptionElement DescriptionElement-' . $element->class . '
      ' .  html_class_attribute_ref($element)
      ),
      '#value' => '',
      '#value_suffix' => NULL
    );

    $element_text = '';
    if (isset($element->multilanguageText_L10n->text)) {
      // TODO replacement of \n by <br> should be configurable
      $element_text = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
    }

    $annotations_and_sources = handle_annotations_and_sources($element, $feature_block_settings, $element_text, $footnote_list_key_suggestion);

    $names_used_in_source_markup = '';
    if (!empty($annotations_and_sources['names_used_in_source']) && empty($element_text)) {
      // $element_text ==  NULL  usually occurs only in the case of CITATIONS!!!
      $names_used_in_source_markup = join(', ', $annotations_and_sources['names_used_in_source']) . ': ';
    }

    $source_references_markup = '';
    if(!empty($annotations_and_sources['source_references'])){
      $source_references_markup = '<span class="sources">'. join(' ', $annotations_and_sources['source_references']) . '<span>';
    }

    $render_array['#value'] = $names_used_in_source_markup . $element_text . $source_references_markup;
    $render_array['#value_suffix'] = $annotations_and_sources['foot_note_keys'];

    return $render_array;
  }



  /**
   * Returns HTML for the texts in a description $elements.
   *
   * Joins the texts in $elements and encloses with a HTML tag.
   *
   * @param  $elements
   * @param  feature:
   *  The feature to which the elements given in $elements are belonging to.
   * @param  glue:
   *  Defaults to empty string.
   * @param $sort
   *   Boolean Whether to sort the $elements alphabetically, default is FALSE
   * @param  $enclosing_tag
   *
   * @ingroup compose
   */
  function compose_cdm_feature_block_elements($elements, $feature, $glue = '', $sort = FALSE, $enclosing_tag = 'ul') {

    $feature_block_settings = get_feature_block_settings($feature->uuid);
    if($feature_block_settings['as_list']){
      $enclosing_tag = $feature_block_settings['as_list'];
    }

    $out = '<' . $enclosing_tag . ' class="feature-block-elements" id="' . $feature->representation_L10n . '">';

    if ($sort) {
      sort($elements);
    }

    //TODO it would be nice if any element would get "feature-block-element" as additional class attribute
    //     this will become possible once all $elemets are real Drupal render arrays
    $out .= join($glue, $elements) ;

    $out .= '</' . $enclosing_tag . '>';
    return markup_to_render_array($out);
  }


  /* compose nameInSource or originalNameString as markup
   *
   * @param $source
   * @param $do_link_to_name_used_in_source
   * @param $suppress_for_shown_taxon
   *    the nameInSource will be suppressed when it has the same name as the accepted taxon
   *    for which the taxon page is being created, Defaults to TRUE
   *
   * @return array
   *    A Drupal render array with an additional element, the render array is empty
   *    if the source had no name in source information
   *    - #_plaintext: contains the plaintext version of the name (custom element)
   *
   * @ingroup compose
   */
  function compose_name_in_source($source, $do_link_to_name_used_in_source, $suppress_for_shown_taxon = TRUE) {

    $plaintext = NULL;
    $markup = NULL;
    $name_in_source_render_array = array();

    static $taxon_page_accepted_name = '';
    if($suppress_for_shown_taxon && arg(1) == 'taxon' && empty($taxon_page_accepted_name)){

      $current_taxon = cdm_ws_get(CDM_WS_PORTAL_TAXON, arg(2));
      $taxon_page_accepted_name = $current_taxon->name->titleCache;
    }

    if (isset($source->nameUsedInSource->uuid) && isset($source->nameUsedInSource->titleCache)) {
      // it is a DescriptionElementSource !
      $plaintext = $source->nameUsedInSource->titleCache;
      if($suppress_for_shown_taxon && $taxon_page_accepted_name == $plaintext){
        return $name_in_source_render_array; // SKIP this name
      }
      $markup = theme('cdm_taxonName', array(
        'taxonName' => $source->nameUsedInSource
      ));
      if ($do_link_to_name_used_in_source) {
        $markup = l(
          $markup,
          path_to_name($source->nameUsedInSource->uuid),
          array(
            'attributes' => array(),
            'absolute' => TRUE,
            'html' => TRUE,
          ));
      }
    }
    else if (isset($source->originalNameString) && !empty($source->originalNameString)) {
      // the name used in source can not be expressed as valid taxon name,
      // so the editor has chosen to put the freetext name into ReferencedEntityBase.originalNameString
      // field
      // using the originalNameString as key to avoid duplicate entries
      $plaintext = $source->originalNameString;
      if($suppress_for_shown_taxon && $taxon_page_accepted_name == $plaintext){
        return $name_in_source_render_array; // SKIP this name
      }
      $markup = $source->originalNameString;
    }

    if ($plaintext) { // checks if we have any content
      $name_in_source_render_array = markup_to_render_array($markup);
      $name_in_source_render_array['#_plaintext'] = $plaintext;
    }

    return $name_in_source_render_array;
  }

  /**
   * Composes block of common names for the given DescriptionElements $elements which must be of the feature CommonName
   *
   * @parameter $elements
   *  an array of CDM DescriptionElements either of type CommonName or TextData
   * @parameter $feature
   *  the common feature of all $elements, must be CommonName
   *
   * @return
   *   A drupal render array
   *
   * @ingroup compose
   */
  function compose_cdm_common_names($elements, $feature, $weight = FALSE) {

    $common_name_out = '';
    $common_name_feature_elements = array();
    $textData_commonNames = array();

    $footnote_key_suggestion = 'common-names-feature-block';

    $feature_block_settings = get_feature_block_settings(UUID_COMMON_NAME);

    $element_tag_name = cdm_feature_block_element_tag_name($feature_block_settings);

    if (is_array($elements)) {
      foreach ($elements as $element) {

        if ($element->class == 'CommonTaxonName') {

          // common name without a language or area, should not happen but is possible
          $language_area_key = '';
          if (isset($element->language->representation_L10n)) {
            $language_area_key .= '<b>' . $element->language->representation_L10n . '</b>';
          }
          if(isset($element->area->titleCache) && strlen($element->area->titleCache) > 0){
            $language_area_key .= ($language_area_key ? ' '  : '') . '(' . $element->area->titleCache . ')';
          }

          if(isset($common_names[$language_area_key][$element->name])) {
            // same name already exists for language and areae combination, se we merge the description elements
            cdm_merge_description_elements($common_names[$language_area_key][$element->name], $element);
          } else{
            // otherwise add as new entry
            $common_names[$language_area_key][$element->name] = $element;
          }

        }
        elseif ($element->class == 'TextData') {
          $textData_commonNames[] = $element;
        }
      }
    }
    // Handling common names.
    if (isset($common_names) && count($common_names) > 0) {
      // Sorting the array based on the key (language, + area if set).
      // Comment @WA there are common names without a language, so this sorting
      // can give strange results.
      ksort($common_names);

      // loop over set of elements per language area
      foreach ($common_names as $language_area_key => $elements) {
        ksort($elements); // sort names alphabetically
        $per_language_area_out = array();
        // loop over set of individual elements
        foreach ($elements as $element) {
          if ($element->name) {
            $annotations_and_sources = handle_annotations_and_sources($element, $feature_block_settings, $element->name, $footnote_key_suggestion);
            $source_references_markup = '';
            if(!empty($annotations_and_sources['source_references'])){
              $source_references_markup = ' ' . join(', ', $annotations_and_sources['source_references']);
            }
            $per_language_area_out[] = '<' . $element_tag_name. ' class="' . html_class_attribute_ref($element) . '">'
              . $element->name . $source_references_markup . $annotations_and_sources['foot_note_keys'] . '</' . $element_tag_name. '>';
          }
        } // End of loop over set of individual elements
        $common_name_feature_elements[] = ($language_area_key ? $language_area_key . ': ' : '' ) . join(', ', $per_language_area_out);
      } // End of loop over set of elements per language area


      $common_name_feature_elements_render_array = compose_cdm_feature_block_elements(
        $common_name_feature_elements,
        $feature,
        '; ',
        FALSE,
        $feature_block_settings['as_list']
      );
      $common_name_out .= $common_name_feature_elements_render_array['#markup'];

    }

    // Handling commons names as text data.
    $text_data_out = array();

    foreach ($textData_commonNames as $text_data_element) {
      /* footnotes are not handled correctly in compose_cdm_descriptionElementTextData,
         need to set 'common-names-feature-block' as $footnote_key_suggestion */
      RenderHints::setFootnoteListKey($footnote_key_suggestion);
      $text_data_render_array = compose_cdm_descriptionElementTextData($text_data_element, TRUE, $text_data_element->feature->uuid);
      $text_data_out[] = drupal_render($text_data_render_array);
    }

    $common_name_out_text_data = compose_cdm_feature_block_elements(
      $text_data_out,
      $feature
    );

    $footnotes = theme('cdm_footnotes', array('footnoteListKey' => 'BIBLIOGRAPHY-' . $footnote_key_suggestion));
    $footnotes .= theme('cdm_footnotes', array('footnoteListKey' => $footnote_key_suggestion)); // FIXME is this needed at all?
    $footnotes .= theme('cdm_annotation_footnotes', array('footnoteListKey' => $footnote_key_suggestion));

    return  markup_to_render_array(
      '<div class="common_names_as_common_names">' . $common_name_out . '</div>'
      .'<div class="common_names_as_text_data">' . $common_name_out_text_data['#markup'] . '</div>'
      .$footnotes,
      $weight
    );
  }

  /**
   * Return HTML for a list of description elements.
   *
   * Usually these are of a specific feature type.
   *
   * @param $descriptionElements
   *   array of descriptionElements which belong to the same feature.
   *   These descriptions elements of a Description must be ordered by the chosen feature tree by
   *   calling the function _mergeFeatureTreeDescriptions().
   *   @see _mergeFeatureTreeDescriptions()
   *
   * @param  $feature_uuid
   *
   * @return
   *    A drupal render array for the $descriptionElements, may be an empty array if the textual content was empty.
   *    Footnote key or anchors are not considered to be textual content.
   *
   * @ingroup compose
   */
  function compose_cdm_descriptionElements($descriptionElements, $feature) {

    $render_array = array();
    $elements_out_array = array();
    $distribution_tree = null;

    /*
     * $feature_block_has_content will be set true if at least one of the
     * $descriptionElements contains some text which makes up some content
     * for the feature block. Footnote keys are not considered
     * to be content in this sense.
     */
    $feature_block_has_content = false;

    /*
    $userDefined = mixed_variable_get(LAYOUT_SETTING_PREFIX . $feature_uuid, FEATURE_TREE_LAYOUT_DEFAULTS);
    if(variable_get('distribution_sort',
        'NO_SORT') != 'NO_SORT'){
        $glue = '';
        $enclosingTag = 'dl';
        $entryEnclosingTag = NULL;
    } else if($userDefined &&
        $userDefined['enabled']){
      $glue = $userDefined['glue'];
      $enclosingTag =  $userDefined['enclosingTag'];
      $entryEnclosingTag = $userDefined['entryEnclosingTag'];
    } else { // TODO remove once  LAYOUT_SETTING_PREFIX-{uuid} setting are configured to work for all portals(selenium test still missing!!!)
      $glue = ''; $enclosingTag = 'ul';
      $entryEnclosingTag = NULL ;
    }
    */

    RenderHints::pushToRenderStack($feature->uuid);

    if (is_array($descriptionElements)) {
      // --- normal description element arrays
      foreach ($descriptionElements as $descriptionElement) {

        // --- IMAGE_SOURCES --- //
        if ($descriptionElement->feature->uuid == UUID_IMAGE_SOURCES) {
          $image_sources[] = $descriptionElement;
        }
        // --- USE TEXTDATA --- //
        elseif ($descriptionElement->feature->uuid == UUID_USE) {
          // Do nothing to avoid rendering.
        } else {
          /* decide based on the description element class
           *
           * Features handled here:
           * all except DISTRIBUTION, COMMON_NAME, USES, IMAGES,
           *
           *
           * TODO provide api_hook as extension point for this?
           */
          $feature_block_settings = get_feature_block_settings($descriptionElement->feature->uuid);
          $asListElement  = $feature_block_settings['as_list'] == 'ul';
          switch ($descriptionElement->class) {
            case 'TextData':
              $text_data_render_array = compose_cdm_descriptionElementTextData($descriptionElement, $asListElement, $descriptionElement->feature->uuid);
              $feature_block_has_content = $feature_block_has_content || !empty($text_data_render_array['#value']);
              $elements_out_array[] = drupal_render($text_data_render_array);
              break;
            case 'CategoricalData':
              $feature_block_has_content = true;
              $elements_out_array[] = theme('cdm_descriptionElement_CategoricalData', array('element' => $descriptionElement));
              break;
            case 'QuantitativeData':
              $feature_block_has_content = true;
              $elements_out_array[] = theme('cdm_descriptionElement_QuantitativeData', array('element' => $descriptionElement));
              break;
            case 'IndividualsAssociation':
              $feature_block_has_content = true;
              $elements_out_array[] = theme('cdm_descriptionElement_IndividualsAssociation', array('element' => $descriptionElement));
              break;
            case 'TaxonInteraction':
              $feature_block_has_content = true;
              $elements_out_array[] = theme('cdm_descriptionElement_TaxonInteraction', array('element' => $descriptionElement));
              break;
            case 'Uses':
              /* IGNORE Uses classes, these are handled completely in theme_cdm_UseDescription */
              break;
            default:
              $feature_block_has_content = true;
              $elements_out_array[] = '<li>No method for rendering unknown description class: ' . $descriptionElement->class . '</li>';
          }
        }
      } // --- END loop over normal description element arrays

      // If feature = CITATION sort the list of sources.
      // This is ONLY for FLORA MALESIANA and FLORE d'AFRIQUE CENTRALE.
      if ($descriptionElement->feature->uuid == UUID_CITATION) {
        sort($elements_out_array);
      }

      if (isset($image_sources)) {
        $elements_out_array[] = theme('cdm_description_element_image_source', array('image_sources' => $image_sources, 'asListElement' => TRUE));
      }
    } // END normal description element arrays

    if($feature_block_has_content){
      $render_array = compose_cdm_feature_block_elements(
        $elements_out_array,
        $feature
      );
    }

    RenderHints::popFromRenderStack();
    return $render_array;
  }


  /**
   * Composes the render array for a CDM Distribution element
   *
   * @param $descriptionElements
   *   The CDM Distribution element
   * @param $enclosingTag
   *   The html tag to be use for the enclosing element
   *
   * @return array
   *   A Drupal render array
   *
   * @ingroup compose
   */
  function compose_cdm_descriptionElement_Distribution($descriptionElements, $enclosingTag = "span"){

    $out = '';
    RenderHints::pushToRenderStack('descriptionElementDistribution');
    RenderHints::setFootnoteListKey(UUID_DISTRIBUTION);

    $feature_block_settings = get_feature_block_settings(UUID_DISTRIBUTION);

    foreach ($descriptionElements as $descriptionElement) {
      $annotations_and_sources = handle_annotations_and_sources(
        $descriptionElement,
        $feature_block_settings,
        $descriptionElement->area->representation_L10n,
        UUID_DISTRIBUTION
      );



      $out .= '<' . $enclosingTag . ' class="descriptionElement descriptionElement-' . $descriptionElement->uuid . '">'
        . $descriptionElement->area->representation_L10n;
      if(!empty($annotations_and_sources['source_references'])){
        $out .= ' ' . join(' ', $annotations_and_sources['source_references'] );
      }
      $out .= $annotations_and_sources['foot_note_keys']   . ' </' . $enclosingTag . '>';
    }

    RenderHints::popFromRenderStack();
    return markup_to_render_array($out);
  }


