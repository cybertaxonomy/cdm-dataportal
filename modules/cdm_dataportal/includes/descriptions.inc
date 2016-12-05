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

  $feature_block_settings = get_feature_block_settings(UUID_DISTRIBUTION);
  if($feature_block_settings['sort_elements'] == SORT_HIERARCHICAL){
    $query['distributionOrder'] = 'AREA_ORDER';
  }

  $query['recipe'] = variable_get(DISTRIBUTION_CONDENSED_RECIPE, DISTRIBUTION_CONDENSED_RECIPE_DEFAULT);



  if ($cdm_distribution_filter['filter_rules']['statusOrderPreference']) {
    $query['statusOrderPreference'] = 1;
  }
  if ($cdm_distribution_filter['filter_rules']['subAreaPreference']) {
    $query['subAreaPreference'] = 1;
  }
  if (is_array($cdm_distribution_filter['hiddenAreaMarkerType']) && count($cdm_distribution_filter['hiddenAreaMarkerType']) > 0) {
    $query['hiddenAreaMarkerType'] = '';
    foreach ($cdm_distribution_filter['hiddenAreaMarkerType'] as $marker_type => $enabled) {
      if ($enabled) {
        $query['hiddenAreaMarkerType'] .= ($query['hiddenAreaMarkerType'] ? ',' : '') . $marker_type;
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

  usort($description_element->sources, 'compare_original_sources');
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
 * Compare callback to be used in usort() to sort render arrays produced by compose_description_element().
 *
 * @param $a
 * @param $b
 */
function compare_description_element_render_arrays($a, $b){
  if ($a['#value'].$a['#value-suffix'] == $b['#value'].$b['#value-suffix']) {
    return 0;
  }
  return ($a['#value'].$a['#value-suffix'] < $b['#value'].$b['#value-suffix']) ? -1 : 1;

}


/**
 * @param $render_array
 * @param $element
 * @param $feature_block_settings
 * @param $element_markup
 * @param $footnote_list_key_suggestion
 */
function compose_description_element($element, $feature_block_settings, $element_markup, $footnote_list_key_suggestion, $prepend_feature_label = FALSE)
{

  $render_array = array(
    '#type' => 'html_tag',
    '#tag' => cdm_feature_block_element_tag_name($feature_block_settings),

    '#attributes' => array(
      'class' => array(
        'DescriptionElement',
        'DescriptionElement-' . $element->class,
        html_class_attribute_ref($element)
      )
    ),

    '#value' => '',
    '#value_suffix' => NULL

  );

  $annotations_and_sources = handle_annotations_and_sources($element, $feature_block_settings, $element_markup, $footnote_list_key_suggestion);

  // handle the special case were the TextData is used as container for original source with name
  // used in source information without any text stored in it.
  $names_used_in_source_markup = '';
  if (!empty($annotations_and_sources['names_used_in_source']) && empty($element_markup)) {
    $names_used_in_source_markup = join(', ', $annotations_and_sources['names_used_in_source']) . ': ';
    // remove all <span class="nameUsedInSource">...</span> from all source_references
    // these are expected to be at the end of the strings
    $pattern = '/ <span class="nameUsedInSource">.*$/';
    foreach( $annotations_and_sources['source_references'] as &$source_reference){
      $source_reference = preg_replace($pattern , '', $source_reference);
    }
  }


  $source_references_markup = '';
  if (!empty($annotations_and_sources['source_references'])) {
    $source_references_markup = '<span class="sources">' . join(' ', $annotations_and_sources['source_references']) . '<span>';
  }

  $feature_label = '';
  if ($prepend_feature_label) {
    $feature_label = '<span class="nested-feature-tree-feature-label">' . $element->feature->representation_L10n . ':</span> ';
  }
  $content_markup = $names_used_in_source_markup . $element_markup . $source_references_markup;

  if(!$content_markup && $feature_block_settings['sources_as_content'] !== 1 || $feature_block_settings['sources_as_content_to_bibliography'] == 1){
    // no textual content, so skip this element completely, even if there could be an footnote key
    // see #4379
    return null;
  }

    $render_array['#value'] = $feature_label . $content_markup;
    $render_array['#value_suffix'] = $annotations_and_sources['foot_note_keys'];
  return $render_array;
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

    usort($element->sources, 'compare_original_sources');

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
  function compose_feature_blocks($mergedFeatureNodes, $taxon) {

    $block_list = array();

    $gallery_settings = getGallerySettings(CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME);


    RenderHints::pushToRenderStack('feature_block');
    // Create a drupal block for each feature
    foreach ($mergedFeatureNodes as $node) {

      if ((isset($node->descriptionElements['#type']) ||
          has_feature_node_description_elements($node)) && $node->feature->uuid != UUID_IMAGE) { // skip empty or suppressed features

        RenderHints::pushToRenderStack($node->feature->uuid);
          
        $feature_name = cdm_term_representation($node->feature, 'Unnamed Feature');
        $feature_block_settings = get_feature_block_settings($node->feature->uuid);
        

        $block = feature_block($feature_name, $node->feature);
        $block->content = array();
        $block_content_is_empty = TRUE;

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
          $common_names_render_array = compose_feature_block_items_feature_common_name($node->descriptionElements, $node->feature);
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
          $elements_render_array = array();
          $child_elements_render_array = null;

          if (isset($node->descriptionElements[0])) {
            $elements_render_array = compose_feature_block_items_generic($node->descriptionElements, $node->feature);
          }

          // Content/ALL OTHER FEATURES/Subordinate Features
          // subordinate features are printed inline in one floating text,
          // it is expected hat subordinate features can "contain" TextData,
          // Qualitative- and Qualitative- DescriptionElements
          if (isset($node->childNodes[0])) {
            $child_elements_render_array = compose_feature_block_items_nested($node, $media_list, $feature_block_settings);
            $elements_render_array = array_merge($elements_render_array, $child_elements_render_array);
          }
          $block_content_is_empty = $block_content_is_empty && empty($media_list) && empty($elements_render_array);
          if(!$block_content_is_empty){
            $block->content[] = compose_feature_block_wrap_elements($elements_render_array, $node->feature);
            $block->content[] = compose_feature_media_gallery($node, $media_list, $gallery_settings);
            /*
             * Footnotes for the feature block
             */
            $block->content[] = markup_to_render_array(theme('cdm_footnotes', array('footnoteListKey' => 'BIBLIOGRAPHY-' . $node->feature->uuid)));
            $block->content[] = markup_to_render_array(theme('cdm_footnotes', array('footnoteListKey' => $node->feature->uuid)));
            $block->content[] = markup_to_render_array(theme('cdm_annotation_footnotes', array('footnoteListKey' => $node->feature->uuid)));
          }
        } // END all other features

        // allows modifying the block contents via a the hook_cdm_feature_node_block_content_alter
        drupal_alter('cdm_feature_node_block_content', $block->content, $node->feature, $node->descriptionElements);

        if(!$block_content_is_empty){ // skip empty block content
          $block_list[] = $block;
          cdm_toc_list_add_item(cdm_term_representation($node->feature), $node->feature->uuid);
        } // END: skip empty block content
      } // END: skip empty or suppressed features
      RenderHints::popFromRenderStack();
    } // END: creating a block per feature

    RenderHints::popFromRenderStack();

    drupal_alter('cdm_feature_node_blocks', $block_list, $taxon);

    return _block_get_renderable_array($block_list);
  }

/**
 * Creates a render array of description elements  held by child nodes of the given feature node.
 *
 * This function is called recursively!
 *
 * @param $node
 *   The feature node.
 * @param array $media_list
 *   List of CDM Media entities. All media of subordinate elements should be passed to the main feature.ä
 * @param $feature_block_settings
 *   The feature block settings.
 * @param $main_feature
 *  Only used internally in recursive calls.
 *
 * @return array
 *  A Drupal render array
 *
 * @ingroup compose
 */
function compose_feature_block_items_nested($node, &$media_list, $feature_block_settings, $main_feature = NULL)
{

  if(!$main_feature){
    $main_feature = $node->feature;
  }
  /*
   * TODO should be configurable, options; YES, NO, AUTOMATIC
   * (automatic will only place the label if the first word of the description element text is not the same)
   */
  $prepend_feature_label = false;

  $render_arrays = array();
  foreach ($node->childNodes as $child_node) {
    if (isset($child_node->descriptionElements[0])) {
      foreach ($child_node->descriptionElements as $element) {

        if (isset($element->media[0])) {
          // Append media of subordinate elements to list of main
          // feature.
          $media_list = array_merge($media_list, $element->media);
        }

        $child_node_element = null;
        switch ($element->class) {
          case 'TextData':
            $child_node_element = compose_description_element_text_data($element, $element->feature->uuid, $feature_block_settings, $prepend_feature_label);
            break;
          case 'CategoricalData':
            $child_node_element = compose_description_element_categorical_data($element, $feature_block_settings, $prepend_feature_label);
            break;
          case 'QuantitativeData':
            $child_node_element = compose_description_element_quantitative_data($element, $feature_block_settings, $prepend_feature_label);

        }
        if (is_array($child_node_element)) {
          $render_arrays[] = $child_node_element;
        }
      }
    }

    if(isset($child_node->childNodes[0])){
      $render_arrays = array_merge($render_arrays, compose_feature_block_items_nested($child_node, $media_list, $feature_block_settings, $main_feature ));
    }
  }

  return $render_arrays;
}

  /**
   *
   * @param $node
   *  The merged feature three node which potentially contains media in its description elements.
   * @param $media_list
   *    Additional media to be merged witht the media contained in the nodes description elements
   * @param $gallery_settings
   * @return array
   *
   * @ingroup compose
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
   * Composes the distribution feature block for a taxon
   *
   * @param $taxon
   * @param $descriptionElements
   *   an associative array with two elements:
   *   - '#type': must be 'DTO'
   *   - 'DistributionInfoDTO': a CDM DistributionInfoDTO object as returned by the DistributionInfo web service
   * @param $feature
   *
   * @return array
   *  A drupal render array
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

    // TODO use feature_block_settings instead of DISTRIBUTION_ORDER_MODE
    if (variable_get(DISTRIBUTION_ORDER_MODE, DISTRIBUTION_ORDER_MODE_DEFAULT) != 'TREE') {
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
        $text_data_render_array = compose_description_element_text_data($text_data_element, $text_data_element->feature->uuid, $feature_block_settings);
        $repr = drupal_render($text_data_render_array);

        if (!array_search($repr, $text_data_out_array)) { // de-duplication !!
          $text_data_out_array[] = $repr;
          // TODO HINT: sorting in compose_feature_block_wrap_elements will
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
      $block->content[] = compose_feature_block_wrap_elements(
        $text_data_out_array, $feature, $text_data_glue, $text_data_sortOutArray
      );
    }

    // --- Distribution map
    $distribution_map_query_parameters = NULL;

    $map_visibility = variable_get(DISTRIBUTION_MAP_VISIBILITY, DISTRIBUTION_MAP_VISIBILITY_DEFAULT);
    if(variable_get(DISTRIBUTION_MAP_VISIBILITY, DISTRIBUTION_MAP_VISIBILITY_DEFAULT) == 'always' ||
        $map_visibility == 'automatic' && isset($descriptionElements['DistributionInfoDTO']->mapUriParams)
      )
    {
      $distribution_map_query_parameters = $descriptionElements['DistributionInfoDTO']->mapUriParams;
    }
    $map_render_element = compose_distribution_map($distribution_map_query_parameters);
    $block->content[] = $map_render_element;

    $dto_out_array = array();

    // --- Condensed Distribution
    if(variable_get(DISTRIBUTION_CONDENSED) && isset($descriptionElements['DistributionInfoDTO']->condensedDistribution)){
      $condensed_distribution_markup = '<p class="condensed_distribution">';

      $isFirst = true;
      if(isset($descriptionElements['DistributionInfoDTO']->condensedDistribution->indigenous[0])){
        foreach($descriptionElements['DistributionInfoDTO']->condensedDistribution->indigenous as $cdItem){
          if(!$isFirst){
            $condensed_distribution_markup .= ' ';
          }
          $condensed_distribution_markup .= '<span class="status_' . $cdItem->status->idInVocabulary . '">'
          . $cdItem->areaStatusLabel . '</span>';
          $isFirst = false;
        }
      }

      if(isset($descriptionElements['DistributionInfoDTO']->condensedDistribution->foreign[0])) {
        if(!$isFirst){
          $condensed_distribution_markup .= ' ';
        }
        $isFirst = TRUE;
        $condensed_distribution_markup .= '[';
        foreach ($descriptionElements['DistributionInfoDTO']->condensedDistribution->foreign as $cdItem) {
          if (!$isFirst) {
            $condensed_distribution_markup .= ' ';
          }
          $condensed_distribution_markup .= '<span class="status_' . $cdItem->status->titleCache . '">'
            . $cdItem->areaStatusLabel . '</span>';
          $isFirst = false;
        }
        $condensed_distribution_markup .= ']';
      }

      $condensed_distribution_markup .= '&nbsp;' . l(
          font_awesome_icon_markup(
            'fa-info-circle',
            array(
              'alt'=>'help',
              'class' => array('superscript')
            )
          ),
          variable_get(DISTRIBUTION_CONDENSED_INFO_PATH, DISTRIBUTION_CONDENSED_INFO_PATH_DEFAULT) ,
          array('html' => TRUE));
      $condensed_distribution_markup .= '</p>';
      $dto_out_array[] = $condensed_distribution_markup;
    }

    // --- tree or list
    if (isset($descriptionElements['DistributionInfoDTO'])) {
      $distribution_info_dto = $descriptionElements['DistributionInfoDTO'];

      // --- tree
      if (is_object($distribution_info_dto->tree)) {
        $distribution_tree_render_array = compose_distribution_hierarchy($distribution_info_dto->tree, $feature_block_settings);
        $dto_out_array[] = $distribution_tree_render_array;
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
        $distribution_element_render_array = compose_description_elements_distribution($distributionElements);
        $dto_out_array[] = $distribution_element_render_array;

      }
      //
      $block->content[] = compose_feature_block_wrap_elements(
        $dto_out_array, $feature, $distribution_glue, $distribution_sortOutArray
      );
    }

    // --- TextData at the bottom
    if ($text_data_out_array && !variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0)) {
      $block->content[] = compose_feature_block_wrap_elements(
        $text_data_out_array, $feature, $text_data_glue, $text_data_sortOutArray
      );
    }

    $block->content[] = markup_to_render_array(theme('cdm_footnotes', array('footnoteListKey' => 'BIBLIOGRAPHY-' . UUID_DISTRIBUTION)));
    $block->content[] = markup_to_render_array(theme('cdm_footnotes', array('footnoteListKey' => UUID_DISTRIBUTION)));
    $block->content[] = markup_to_render_array(theme('cdm_annotation_footnotes', array('footnoteListKey' => UUID_DISTRIBUTION)));

    return $block;
  }


  /**
   * Composes a drupal render array for single CDM TextData description element.
   *
   * @param $element
   *    The CDM TextData description element.
   *  @param $feature_uuid
   * @param bool $prepend_feature_label
   *   Used in nested feature trees to put the feature as label in front of the description element text representation.
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
  function compose_description_element_text_data($element, $feature_uuid, $feature_block_settings, $prepend_feature_label = FALSE) {

    $footnote_list_key_suggestion = $feature_uuid;

    $element_markup = '';
    if (isset($element->multilanguageText_L10n->text)) {
      // TODO replacement of \n by <br> should be configurable
      $element_markup = str_replace("\n", "<br/>", $element->multilanguageText_L10n->text);
    }

    $render_array = compose_description_element($element, $feature_block_settings, $element_markup, $footnote_list_key_suggestion, $prepend_feature_label);

    return $render_array;
  }


/**
 * Theme function to render CDM DescriptionElements of the type TaxonInteraction.
 *
 * @param $element
 *  The CDM TaxonInteraction entity
 *
 * @return
 *  A drupal render array
 *
 * @ingroup compose
 */
function compose_description_element_taxon_interaction($element, $feature_block_settings) {

  $out = '';


  if (isset($element->description_L10n)) {
    $out .=  ' ' . $element->description_L10n;
  }

  if(isset($element->taxon2)){
    $out = render_taxon_or_name($element->taxon2, url(path_to_taxon($element->taxon2->uuid)));
  }

  $render_array = compose_description_element($element, $feature_block_settings, $out, $element->feature->uuid);

  return $render_array;
}


/**
 * Renders a single instance of the type IndividualsAssociations.
 *
 * @param $element
 *   The CDM IndividualsAssociations entity.
 * @param $feature_block_settings
 *
 * @return array
 *   Drupal render array
 *
 * @ingroup compose
 */
function compose_description_element_individuals_association($element, $feature_block_settings) {

  $out = '';

  $render_array = compose_cdm_specimenOrObservation($element->associatedSpecimenOrObservation);

  if (isset($element->description_L10n)) {
    $out .=  ' ' . $element->description_L10n;
  }

  $out .= drupal_render($render_array);

  $render_array = compose_description_element($element, $feature_block_settings, $out, $element->feature->uuid);

  return $render_array;
}

/**
 * Renders a single instance of the type CategoricalData.
 *
 * @param $element
 *  The CDM CategoricalData entity
 *
 * @param $feature_block_settings
 *
 * @param bool $prepend_feature_label
 *   Used in nested feature trees to put the feature as label in front of the description element text representation.
 *
 * @return string
 *   a html representation of the given CategoricalData element
 *
 * @ingroup compose
 */
function compose_description_element_categorical_data($element, $feature_block_settings, $prepend_feature_label = FALSE) {

  $enclosing_tag = cdm_feature_block_element_tag_name($feature_block_settings);

  $state_data_strings = array();
  if (isset($element->stateData)) {
    foreach ($element->stateData as $state_data) {

      $state  = NULL;

      if (isset($state_data->state)) {
        $state = cdm_term_representation($state_data->state);
      }

      if (isset($state_data->modifyingText_L10n)) {
        $state = ' ' . $state_data->modifyingText_L10n;
      }

      $modifiers_strings = cdm_modifers_representations($state_data);

      $state_data_strings[] = $state . ($modifiers_strings ? ' ' . $modifiers_strings : '');

    }
  }

  $out = '<span class="' . html_class_attribute_ref($element) . '">' . implode(', ', $state_data_strings) . '</span>';

  $render_array = compose_description_element($element, $feature_block_settings, $out, $element->feature->uuid, $prepend_feature_label);

  return $render_array;
}


/**
 * Theme function to render CDM DescriptionElements of the type QuantitativeData.
 *
 * The function renders the statisticalValues contained in the QuantitativeData
 * entity according to the following scheme:
 *
 * (ExtremeMin)-Min-Average-Max-(ExtremeMax)
 *
 * All modifiers of these values are appended.
 *
 * If the QuantitativeData is containing more statisticalValues with further
 * statisticalValue types, these additional measures will be appended to the
 * above string separated by whitespace.
 *
 * Special cases;
 * 1. Min==Max: this will be interpreted as Average
 *
 * @param $element
 *  The CDM QuantitativeData entity
 *
 * @param $feature_block_settings
 *
 * @param bool $prepend_feature_label
 *   Used in nested feature trees to put the feature as label in front of the description element text representation.
 *
 *
 * @return string
 *   a html representation of the given QuantitativeData element
 *
 * @ingroup themeable
 */
function compose_description_element_quantitative_data($element, $feature_block_settings, $prepend_feature_label = FALSE) {
  /*
   * - statisticalValues
   *   - value
   *   - modifiers
   *   - type
   * - unit->representation_L10n
   * - modifyingText
   * - modifiers
   * - sources
   */

  $out = '';
  $type_representation = NULL;
  $min_max = min_max_array();

  $other_values = array();

  if (isset($element->statisticalValues)) {
    $other_values_markup = array();
    foreach ($element->statisticalValues as $statistical_val) {

      // compile the full value string which also may contain modifiers
      if (isset($statistical_val->value)) {
        $statistical_val->_value = $statistical_val->value;
      }
      $val_modifiers_strings = cdm_modifers_representations($statistical_val);
      if ($val_modifiers_strings) {
        $statistical_val->_value = ' ' . $val_modifiers_strings . ' ' . $statistical_val->_value;
      }

      // either put into min max array or into $other_values
      // for generic output to be appended to 'min-max' string
      if (array_key_exists($statistical_val->type->titleCache, $min_max)) {
        $min_max[$statistical_val->type->titleCache] = $statistical_val;
      }
      else {
        $other_values[] = $statistical_val;
      }
    } // end of loop over statisticalValues

    // create markup

    $min_max_markup = min_max_markup($min_max);


    foreach ($other_values as $statistical_val) {
      $statistical_val_type_representation = NULL;
      if (isset($statistical_val->type)) {
        $statistical_val_type_representation = cdm_term_representation($statistical_val->type);
        // $statistical_val->type->termType;
        // $statistical_val->type->userFriendlyTypeName;
      }
      $value_markup = '<span class="' . html_class_attribute_ref($statistical_val) . ' ' . $statistical_val->type->termType . ' ">'
        . $statistical_val->_value . '</span>';
      $value_markup = $value_markup .
        ($statistical_val_type_representation ? ' <span class="type">' . $statistical_val_type_representation . '</span>' : '');
      $other_values_markup[] = $value_markup;
    }


    $out .= $min_max_markup . ' ' . implode($other_values_markup, ', ');
  }

  if (isset($element->unit)) {
    $out .= ' <span class="unit" title="'
      . cdm_term_representation($element->unit) . '">'
      . cdm_term_representation_abbreviated($element->unit)
      . '</span>';
  }

  // modifiers of the description element itself
  $modifier_string = cdm_modifers_representations($element);
  $out .= ($modifier_string ? ' ' . $modifier_string : '');
  if (isset($element->modifyingText_L10n)) {
    $out = $element->modifyingText_L10n . ' ' . $out;
  }

  $render_array = compose_description_element($element, $feature_block_settings, $out, $element->feature->uuid, $prepend_feature_label);

  return $render_array;
}


/**
 * Wraps the render array for the given feature into an enclosing html tag.
 *
 * Optionally the elements can be sorted and glued together by a separator string.
 *
 * @param array $description_element_render_arrays
 *   An list of render arrays. Which are usually are produced by the compose_description_element()
 *   function. The render arrays should be of #type=html_tag, so that they can understand the #attribute property.
 * @param  $feature :
 *  The feature to which the elements given in $elements are belonging to.
 * @param string $glue :
 *  Defaults to empty string.
 * @param bool $sort
 *   Boolean Whether to sort the $elements alphabetically, default is FALSE
 *
 * @return array
 *    A Drupal render array
 *
 * @ingroup compose
 */
  function compose_feature_block_wrap_elements(array $description_element_render_arrays, $feature, $glue = '', $sort = FALSE)
  {

    $feature_block_settings = get_feature_block_settings($feature->uuid);
    $enclosing_tag = $feature_block_settings['as_list'];

    if ($sort) { // TODO remove parameter and replace by $feature_block_settings['sort_elements']
      usort($description_element_render_arrays, 'compare_description_element_render_arrays');
    }

    $is_first = true;
    foreach($description_element_render_arrays as &$element_render_array){
      if(!is_array($element_render_array)){
        $element_render_array = markup_to_render_array($element_render_array);
      }
      $element_render_array['#attributes']['class'][] = "feature-block-element";

      // add the glue!
      if(!$is_first) {
        if (isset($element_render_array['#value'])) {
          $element_render_array['#value'] = $glue . $element_render_array['#value'];
        } elseif (isset($element_render_array['#markup'])) {
          $element_render_array['#markup'] = $glue . $element_render_array['#markup'];
        }
      }
      $is_first = false;
    }

    $render_array['elements']['children'] = $description_element_render_arrays;

    $render_array['elements']['#prefix'] = '<' . $enclosing_tag . ' class="feature-block-elements" id="' . $feature->representation_L10n . '">';
    $render_array['elements']['#suffix'] = '</' . $enclosing_tag . '>';

    return $render_array;
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
    $taxon_uuid = get_current_taxon_uuid();
    if($suppress_for_shown_taxon && $taxon_uuid && empty($taxon_page_accepted_name)){

      $current_taxon = cdm_ws_get(CDM_WS_PORTAL_TAXON, $taxon_uuid);
      $taxon_page_accepted_name = $current_taxon->name->titleCache;
    }

    if (isset($source->nameUsedInSource->uuid) && isset($source->nameUsedInSource->titleCache)) {
      // it is a DescriptionElementSource !
      $plaintext = $source->nameUsedInSource->titleCache;
      if($suppress_for_shown_taxon && $taxon_page_accepted_name == $plaintext){
        return $name_in_source_render_array; // SKIP this name
      }
      $markup = render_taxon_or_name($source->nameUsedInSource);
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
   * Return HTML for a list of description elements.
   *
   * Usually these are of a specific feature type.
   *
   * @param $description_elements
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
  function compose_feature_block_items_generic($description_elements, $feature) {

    $elements_out_array = array();
    $distribution_tree = null;

    /*
     * $feature_block_has_content will be set true if at least one of the
     * $descriptionElements contains some text which makes up some content
     * for the feature block. Footnote keys are not considered
     * to be content in this sense.
     */
    $feature_block_has_content = false;

    if (is_array($description_elements)) {
      foreach ($description_elements as $description_element) {
          /* decide based on the description element class
           *
           * Features handled here:
           * all except DISTRIBUTION, COMMON_NAME, USES, IMAGES,
           *
           * TODO provide api_hook as extension point for this?
           */
        $feature_block_settings = get_feature_block_settings($description_element->feature->uuid);
        switch ($description_element->class) {
          case 'TextData':
            $elements_out_array[] = compose_description_element_text_data($description_element, $description_element->feature->uuid, $feature_block_settings);
            break;
          case 'CategoricalData':
            $elements_out_array[] = compose_description_element_categorical_data($description_element, $feature_block_settings);
            break;
          case 'QuantitativeData':
            $elements_out_array[] = compose_description_element_quantitative_data($description_element, $feature_block_settings);
            break;
          case 'IndividualsAssociation':
            $elements_out_array[] = compose_description_element_individuals_association($description_element, $feature_block_settings);
            break;
          case 'TaxonInteraction':
            $elements_out_array[] = compose_description_element_taxon_interaction($description_element, $feature_block_settings);
            break;
          case 'CommonTaxonName':
            $elements_out_array[] = compose_description_element_common_taxon_name($description_element, $feature_block_settings);
            break;
          case 'Uses':
            /* IGNORE Uses classes, these are handled completely in theme_cdm_UseDescription */
            break;
          default:
            $feature_block_has_content = true;
            $elements_out_array[] = markup_to_render_array('<li>No method for rendering unknown description class: ' . $description_element->class . '</li>');
        }
        $elements_out_array_last_item = $elements_out_array[count($elements_out_array) - 1];
        // considering not empty as long as the last item added is a render array
        $feature_block_has_content = $feature_block_has_content || !empty($elements_out_array_last_item['#type']);
      }

      // If feature = CITATION sort the list of sources.
      // This is ONLY for FLORA MALESIANA and FLORE d'AFRIQUE CENTRALE.
      if (isset($description_element) && $description_element->feature->uuid == UUID_CITATION) {
        sort($elements_out_array);
      }
    }

    // sanitize: remove empty and NULL items from the render array
    $tmp_out_array = $elements_out_array;
    $elements_out_array = array();
    foreach($tmp_out_array as $item){
      if(is_array($item) && count($item) > 0){
        $elements_out_array[] = $item;
      }
    }

    return $elements_out_array;
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
function compose_feature_block_items_feature_common_name($elements, $feature, $weight = FALSE) {

  $common_name_out = '';
  $common_name_feature_elements = array();
  $textData_commonNames = array();

  $footnote_key_suggestion = 'common-names-feature-block';

  $feature_block_settings = get_feature_block_settings(UUID_COMMON_NAME);

  if (is_array($elements)) {
    foreach ($elements as $element) {

      if ($element->class == 'CommonTaxonName') {

        // common name without a language or area, should not happen but is possible
        $language_area_key = '';
        if (isset($element->language->representation_L10n)) {
          $language_area_key .= '<span class="language-label">' . $element->language->representation_L10n . '</span>';
        }
        if(isset($element->area->titleCache) && strlen($element->area->titleCache) > 0){
          $language_area_key .= ($language_area_key ? ' '  : '') . '(' . $element->area->titleCache . ')';
        }

        if(isset($common_names[$language_area_key][$element->name])) {
          // same name already exists for language and area combination, se we merge the description elements
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

      foreach ($elements as $element) {
        $common_name_render_array = compose_description_element_common_taxon_name($element, $feature_block_settings, $footnote_key_suggestion);
        $common_name_markup = drupal_render($common_name_render_array);
        // IMPORTANT!
        // during the above drupal_render the theme_html_tag function is executed, which adds a "\n" character to the end of the markup
        // this is an error and the trailing whitespace needs to be removed
        if(str_endsWith($common_name_markup, "\n")){
          $common_name_markup = substr($common_name_markup, 0, strlen($common_name_markup) - 1);
        }
        $per_language_area_out[] = $common_name_markup;
      }

      $common_name_feature_elements[] = ($language_area_key ? $language_area_key . ': ' : '' ) . join(', ', $per_language_area_out);
    } // End of loop over set of elements per language area


    $common_name_feature_elements_render_array = compose_feature_block_wrap_elements(
      $common_name_feature_elements, $feature, '; ', FALSE
    );
    $common_name_out .= drupal_render($common_name_feature_elements_render_array); // FIXME should this be a render array instead?

  }

  // Handling commons names as text data.
  $text_data_out = array();

  foreach ($textData_commonNames as $text_data_element) {
    /* footnotes are not handled correctly in compose_description_element_text_data,
       need to set 'common-names-feature-block' as $footnote_key_suggestion */
    RenderHints::setFootnoteListKey($footnote_key_suggestion);
    $text_data_render_array = compose_description_element_text_data($text_data_element, $text_data_element->feature->uuid, $feature_block_settings);
    $text_data_out[] = drupal_render($text_data_render_array);
  }

  $common_name_out_text_data = compose_feature_block_wrap_elements(
    $text_data_out, $feature
  );

  $footnotes = theme('cdm_footnotes', array('footnoteListKey' => 'BIBLIOGRAPHY-' . $footnote_key_suggestion));
  $footnotes .= theme('cdm_footnotes', array('footnoteListKey' => $footnote_key_suggestion)); // FIXME is this needed at all?
  $footnotes .= theme('cdm_annotation_footnotes', array('footnoteListKey' => $footnote_key_suggestion));

  return  markup_to_render_array(  // FIXME markup_to_render_array should no longer be needed
    '<div class="common-taxon-name">' . $common_name_out . '</div>'
    .'<div class="text-data">' . drupal_render($common_name_out_text_data) . '</div>'
    .$footnotes,
    $weight
  );
}

/**
 * Renders a single instance of the type CommonTaxonName.
 *
 * @param $element
 *   The CDM CommonTaxonName entity.
 * @param $feature_block_settings
 *
 * @param $footnote_key_suggestion
 *
 * @param $element_tag_name
 *
 * @return array
 *   Drupal render array
 *
 * @ingroup compose
 */
function compose_description_element_common_taxon_name($element, $feature_block_settings, $footnote_key_suggestion = NULL)
{

  if(!$footnote_key_suggestion) {
    $footnote_key_suggestion = $element->feature->uuid;
  }

  $name = '';
  if(isset($element->name)){
    $name = $element->name;
  }


  return compose_description_element($element, $feature_block_settings, $name, $footnote_key_suggestion);
}

/**
 * Composes the render array for a CDM Distribution description element
 *
 * @param array $description_elements
 *   Array of CDM Distribution instances
 * @param $enclosingTag
 *   The html tag to be use for the enclosing element
 *
 * @return array
 *   A Drupal render array
 *
 * @ingroup compose
 */
function compose_description_elements_distribution($description_elements){

  $out = '';
  RenderHints::pushToRenderStack('descriptionElementDistribution');
  RenderHints::setFootnoteListKey(UUID_DISTRIBUTION);

  $feature_block_settings = get_feature_block_settings(UUID_DISTRIBUTION);
  $enclosingTag = cdm_feature_block_element_tag_name($feature_block_settings);

  foreach ($description_elements as $description_element) {
    $annotations_and_sources = handle_annotations_and_sources(
      $description_element,
      $feature_block_settings,
      $description_element->area->representation_L10n,
      UUID_DISTRIBUTION
    );


    list($status_label, $status_markup) = distribution_status_label_and_markup($description_element);

    $out .= '<' . $enclosingTag . ' class="descriptionElement descriptionElement-' . $description_element->uuid
      . ' " title="' . $status_label. '">'
      . $description_element->area->representation_L10n
      . $status_markup;
    if(!empty($annotations_and_sources['source_references'])){
      $out .= ' ' . join(' ', $annotations_and_sources['source_references'] );
    }
    $out .= $annotations_and_sources['foot_note_keys']   . ' </' . $enclosingTag . '>';
  }

  RenderHints::popFromRenderStack();
  return markup_to_render_array($out);
}

  /**
   * @param $descriptionElement
   * @return array
   */
  function distribution_status_label_and_markup($descriptionElement, $status_glue = '&#8210; ') {
    $status_markup = '';
    $status_label = '';

    if (isset($descriptionElement->status)) {
      $status_label = $descriptionElement->status->representation_L10n;
      $status_markup =  '<span class="distributionStatus"> '
        . $status_glue
        . '<span class="distributionStatus-' . $descriptionElement->status->idInVocabulary . '">'
        . $status_label
        . '</span></span>';

    };
    return array($status_label, $status_markup);
  }


  /**
   * Provides the merged feature tree for a taxon profile page.
   *
   * The merging of the profile feature tree is actully done in
   * _mergeFeatureTreeDescriptions(). See this method  for details
   * on the structure of the merged tree.
   *
   * This method provides t hook which can be used to modify the
   * merged feature tree after it has been created, see
   * hook_merged_taxon_feature_tree_alter()
   *
   * @param $taxon
   *   A CDM Taxon instance
   *
   * @return object
   *  The merged feature tree
   *
   */
  function merged_taxon_feature_tree($taxon) {

    // 1. fetch descriptions_by_featuretree but exclude the distribution feature
    $merged_tree = cdm_ws_descriptions_by_featuretree(get_profile_feature_tree(), $taxon->uuid, array(UUID_DISTRIBUTION));


    // 2. find the distribution feature node
    $distribution_node =& cdm_feature_tree_find_node($merged_tree->root->childNodes, UUID_DISTRIBUTION);

    if ($distribution_node) {
      // 3. get the distributionInfoDTO
      $query_parameters = cdm_distribution_filter_query();
      $query_parameters['part'] = array('mapUriParams');
      if(variable_get(DISTRIBUTION_CONDENSED)){
        $query_parameters['part'][] = 'condensedDistribution';
      }
      if (variable_get(DISTRIBUTION_ORDER_MODE, DISTRIBUTION_ORDER_MODE_DEFAULT) == 'TREE') {
        $query_parameters['part'][] = 'tree';
      }
      else {
        $query_parameters['part'][] = 'elements';
      }
      $query_parameters['omitLevels'] = array();
      foreach(variable_get(DISTRIBUTION_TREE_OMIT_LEVELS, array()) as $uuid){
        if(is_uuid($uuid)){
          $query_parameters['omitLevels'][] = $uuid;
        }
      }
      $customStatusColorsJson = variable_get(DISTRIBUTION_STATUS_COLORS, NULL);
      if ($customStatusColorsJson) {
        $query_parameters['statusColors'] = $customStatusColorsJson;
      }

      $distribution_info_dto = cdm_ws_get(CDM_WS_PORTAL_DESCRIPTION_DISTRIBUTION_INFO_FOR, $taxon->uuid, queryString($query_parameters));
      // 4. get distribution TextData is there are any
      $distribution_text_data = cdm_ws_fetch_all(CDM_WS_DESCRIPTIONELEMENT_BY_TAXON,
        array(
          'taxon' => $taxon->uuid,
          'type' => 'TextData',
          'features' => UUID_DISTRIBUTION
        )
      );

      // 5. put all distribution data into the distribution feature node
      if ($distribution_text_data //if text data exists
        || ($distribution_info_dto && isset($distribution_info_dto->tree) && $distribution_info_dto->tree->rootElement->numberOfChildren > 0) // OR if tree element has distribution elements
        || ($distribution_info_dto && !empty($distribution_info_dto->elements))
      ) { // OR if DTO has distribution elements
        $distribution_node->descriptionElements = array('#type' => 'DTO');
        if ($distribution_text_data) {
          $distribution_node->descriptionElements['TextData'] = $distribution_text_data;
        }
        if ($distribution_info_dto) {
          $distribution_node->descriptionElements['DistributionInfoDTO'] = $distribution_info_dto;
        }
      }
    }

    // allows modifying the merged tree via a the hook_cdm_feature_node_block_content_alter
    drupal_alter('merged_taxon_feature_tree', $taxon, $merged_tree);

    return $merged_tree;
  }


  function compose_distribution_hierarchy($distribution_tree, $feature_block_settings){

    static $hierarchy_style;
    // TODO expose $hierarchy_style to administration or provide a hook
    if( !isset($hierarchy_style)){
      $hierarchy_style = get_array_variable_merged(DISTRIBUTION_HIERARCHY_STYLE, DISTRIBUTION_HIERARCHY_STYLE_DEFAULT);
    }

    $render_array = array();

    RenderHints::pushToRenderStack('descriptionElementDistribution');
    RenderHints::setFootnoteListKey(UUID_DISTRIBUTION);

    // Returning NULL if there are no description elements.
    if ($distribution_tree == null) {
      return $render_array;
    }
    // for now we are not using a render array internally to avoid performance problems
    $markup = '';
    if (isset($distribution_tree->rootElement->children)) {
      $tree_nodes = $distribution_tree->rootElement->children;
      _compose_distribution_hierarchy($tree_nodes, $feature_block_settings, $markup, $hierarchy_style);
    }

    $render_array['distribution_hierarchy'] = markup_to_render_array(
      $markup,
      0,
      '<div id="distribution_hierarchy" class="distribution_hierarchy">',
      '</div>'
    );

    RenderHints::popFromRenderStack();

    return $render_array;
  }

  /**
   * this function should produce markup as the compose_description_elements_distribution()
   * function.
   *
   * @see compose_description_elements_distribution()
   *
   * @param $distribution_tree
   * @param $feature_block_settings
   * @param $tree_nodes
   * @param $markup
   * @param $hierarchy_style
   */
  function _compose_distribution_hierarchy($tree_nodes, $feature_block_settings, &$markup, $hierarchy_style, $level_index = -1){

    $level_index++;
    static $enclosingTag = "span";

    $level_style = array_shift($hierarchy_style);
    if(count($hierarchy_style) == 0){
      // lowest defined level style will be reused for all following levels
      $hierarchy_style[] = $level_style;
    }

    $node_index = -1;
    $per_node_markup = array();
    foreach ($tree_nodes as $node){

      $per_node_markup[++$node_index] = '';

      $label = $node->nodeId->representation_L10n;

      $distributions = $node->data;
      $distribution_uuids = array();
      $distribution_aggregate = NULL;
        foreach($distributions as $distribution){

          $distribution_uuids[] = $distribution->uuid;

          // if there is more than one distribution we aggregate the sources and
          // annotations into a synthetic distribution so that the footnote keys
          // can be rendered consistently
          if(!$distribution_aggregate) {
            $distribution_aggregate = $distribution;
            if(!isset($distribution_aggregate->sources[0])){
              $distribution_aggregate->sources = array();
            }
            if(!isset($distribution_aggregate->annotations[0])){
              $distribution_aggregate->annotations = array();
            }
          } else {
            if(isset($distribution->sources[0])) {
              $distribution_aggregate->sources = array_merge($distribution_aggregate->sources,
                $distribution->sources);
            }
            if(isset($distribution->annotations[0])) {
              $distribution_aggregate->annotations = array_merge($distribution_aggregate->annotations,
                $distribution->annotations);
            }
          }
        }

      $status_label= '';
      $status_markup = '';
      $annotations_and_sources =  null;
      if($distribution_aggregate) {
        $annotations_and_sources = handle_annotations_and_sources(
          $distribution_aggregate,
          $feature_block_settings,
          $label,
          UUID_DISTRIBUTION
        );

        list($status_label, $status_markup) = distribution_status_label_and_markup($distribution, $level_style['status_glue']);
      }

      $per_node_markup[$node_index] .= '<' . $enclosingTag . ' class="descriptionElement'
        . join(' descriptionElement-', $distribution_uuids)
        . ' level_index_' . $level_index
        . ' " title="' . $status_label . '">'
        . '<span class="area_label">' . $label
        . $level_style['label_suffix'] . '</span>'
        . $status_markup
      ;

      if(isset($annotations_and_sources)){
        if(!empty($annotations_and_sources['source_references'])){
          $per_node_markup[$node_index] .= ' ' . join(', ' , $annotations_and_sources['source_references']);
        }
        if($annotations_and_sources['foot_note_keys']) {
          $per_node_markup[$node_index] .= $annotations_and_sources['foot_note_keys'];
        }
      }

      if(isset($node->children[0])){
        _compose_distribution_hierarchy(
          $node->children,
          $feature_block_settings,
          $per_node_markup[$node_index],
          $hierarchy_style,
          $level_index
        );
      }

      $per_node_markup[$node_index] .= '</' . $enclosingTag . '>';
    }
    $markup .= $level_style['item_group_prefix']  . join( $level_style['item_glue'], $per_node_markup) . $level_style['item_group_postfix'];
  }


