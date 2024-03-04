<?php

const BIBLIOGRAPHY_FOOTNOTE_KEY = PSEUDO_FEATURE_BIBLIOGRAPHY;


/**
 * Returns the localized representations of the modifiers hold by the
 * supplied cdm instance concatenated into one string.
 *
 * @param object $is_modifieable
 *   cdm instance of an class implementing the interface IModifieable:
 *   DescriptionElementBase, StateDate, State
 *
 * @return String
 *   localized representations of the modifiers hold by the
 *   supplied cdm instance concatenated into one string
 */
function cdm_modifiers_representations($is_modifieable, $glue = ', ') {
  $modifiers_strings = array();
  if (isset($is_modifieable->modifiers)) {
    foreach ($is_modifieable->modifiers as $modifier) {
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
    $temp_hidden = '';
    foreach ($cdm_distribution_filter['hiddenAreaMarkerType'] as $marker_type => $enabled) {
      if ($enabled) {
        $query['fallbackAreaMarkerType'] .= ($query['fallbackAreaMarkerType'] ? ',' : '') . $marker_type;
      }
    }
    if ($temp_hidden){
        $query['fallbackAreaMarkerType'] = $temp_hidden;
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
function cdm_merge_description_elements($target, $source) {
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
 *   The suffix to be appended to the class attribute prefix:
 *   "feature-toc-item-"
 * @param string $fragment
 *   Optional parameter to define a url fragment different from the $label,
 *   if the $fragment is not defined the $label will be used
 * @param boolean $as_first_element
 *   Place the new item as fist one. This parameter is ignored when $weight is
 *   being set
 * @param integer $weight
 *   Defines the weight for the ordering the item in the toc list.
 *   The item will be placed at the end if weigth is NULL.
 *
 * @throws \Exception re-throws exception from theme()
 */
function cdm_toc_list_add_item($label, $class_attribute_suffix, $fragment = NULL, $as_first_element = FALSE, $weight = null) {

  $toc_list_items = &cdm_toc_list();

  if (!$fragment) {
    $fragment = $label;
  }
  $fragment = generalizeString($fragment);

  $class_attributes = 'feature-toc-item-' . $class_attribute_suffix;
  $label = t($label);
   /* if (function_exists('i18n_string') ) {
        $label =  i18n_string($label);
    }*/
  $new_item = toc_list_item(
    theme(
      'cdm_feature_name',
      array('feature_name' => $label)),
      array('class' => $class_attributes),
      $fragment
    );

  if ($weight === null && $as_first_element) {
    reset($toc_list_items);
    $weight = key($toc_list_items); // returns null for empty arrays
    $weight = $weight !== null ? $weight - FEATURE_BLOCK_WEIGHT_INCREMENT : 0;
  }
  else if($weight === null) {
      end($toc_list_items);
      $weight = key($toc_list_items); // returns null for empty arrays
      $weight = $weight !== null ? $weight + FEATURE_BLOCK_WEIGHT_INCREMENT : 0;
  }
  $toc_list_items[$weight] = $new_item;
  ksort($toc_list_items); // sort so that the last element has always the highest weight

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
 * Prepares an empty Drupal block for displaying description elements of a
 * specific CDM Feature.
 *
 * The block can also be used for pseudo Features like a bibliography. Pseudo
 * features are derived from other data on the fly and so not exist as such in
 * the cdm data. For pseudo features the $feature can be created using
 * make_pseudo_feature().
 *
 * @param $feature_name
 *   A label describing the feature, usually the localized feature
 *   representation.
 * @param object $feature
 *   The CDM Feature for which the block is created.
 *
 * @return object
 *   A Drupal block object
 *
 * @throws \Exception re-throws exception from theme()
 */
function feature_block($feature_name, $feature) {
  $block = new stdclass();
  //$block->module = 'cdm_dataportal';
  $block->region = NULL;
  $class_attribute = html_class_attribute_ref($feature);
  $block_delta = isset_not_empty($feature->class) && $feature->class !== 'PseudoFeature' ? $feature_name : $feature->label;
  $block->delta = generalizeString($block_delta);
  $block->subject = '<a name="' . $block->delta . '"></a><span class="' . $class_attribute . '">'
    . theme('cdm_feature_name', array('feature_name' => $feature_name))
    . '</span>';
  $block->module = "cdm_dataportal-feature";
  $block->content = '';
  return $block;
}
/**
 * Prepares an empty Drupal block for displaying description elements of a
 * specific CDM DTO Feature.
 *
 * The block can also be used for pseudo Features like a bibliography. Pseudo
 * features are derived from other data on the fly and so not exist as such in
 * the cdm data. For pseudo features the $feature can be created using
 * make_pseudo_feature().
 *
 * @param $feature_name
 *   A label describing the feature, usually the localized feature
 *   representation.
 * @param object $feature
 *   The CDM Feature for which the block is created.
 *
 * @return object
 *   A Drupal block object
 *
 * @throws \Exception re-throws exception from theme()
 */
function new_feature_block($feature_name, $feature_node) {
    $block = new stdclass();
    //$block->module = 'cdm_dataportal';
    $block->region = NULL;
    $class_attribute = html_class_attribute_ref($feature_node);
    $block_delta = isset($feature_node->class) && $feature_node->class !== 'PseudoFeature' ? $feature_name : $feature_node->label;
    $block->delta = generalizeString($block_delta);
    $block->subject = '<a name="' . $block->delta . '"></a><span class="' . $class_attribute . '">'
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
  $cdm_ws_base_path = NULL;
  switch ($type) {
    case "PolytomousKey":
      $cdm_ws_base_path = CDM_WS_POLYTOMOUSKEY;
      break;

    case "MediaKey":
      $cdm_ws_base_path = CDM_WS_MEDIAKEY;
      break;

    case "MultiAccessKey":
      $cdm_ws_base_path = CDM_WS_MULTIACCESSKEY;
      break;

  }

  if (!$cdm_ws_base_path) {
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
  $pager = cdm_ws_get($cdm_ws_base_path, NULL, $queryParameters);

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
 * Compare callback to be used in usort() to sort render arrays produced by
 * compose_description_element().
 *
 * @param $a
 * @param $b
 *
 * @return int Returns < 0 if $a['#value'].$a['#value-suffix'] from is less than
 * $b['#value'].$b['#value-suffix'], > 0 if it is greater than $b['#value'].$b['#value-suffix'],
 * and 0 if they are equal.
 */
function compare_description_element_render_arrays($a, $b){
  if ($a['#value'].$a['#value-suffix'] == $b['#value'].$b['#value-suffix']) {
    return 0;
  }
  return ($a['#value'].$a['#value-suffix'] < $b['#value'].$b['#value-suffix']) ? -1 : 1;

}


/**
 * @param $element
 * @param $feature_block_settings
 * @param $element_markup
 * @param $footnote_list_key_suggestion
 * @param bool $prepend_feature_label
 *
 * @return array|null
 */
function compose_description_element($element, $feature_block_settings, $element_markup, $footnote_list_key_suggestion, $prepend_feature_label = FALSE)
{
    if (is_cdm_entity($element)){
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
    }else{
        if (isset_not_empty($element->typedLabel[0])){
            $render_array = array(
                '#type' => 'html_tag',
                '#tag' => cdm_feature_block_element_tag_name($feature_block_settings),

                '#attributes' => array(
                    'class' => array(
                        'DescriptionElement',
                        'DescriptionElement-' . $element->typedLabel[0]->cdmClass,
                        html_class_attribute_ref($element)
                    )
                ),

                '#value' => '',
                '#value_suffix' => NULL

            );
        }else{
            $render_array = array(
                '#type' => 'html_tag',
                '#tag' => cdm_feature_block_element_tag_name($feature_block_settings),

                '#attributes' => array(
                    'class' => array(
                        'DescriptionElement',
                        'DescriptionElement-' . $element->clazz,
                        html_class_attribute_ref($element)
                    )
                ),

                '#value' => '',
                '#value_suffix' => NULL

            );
        }

    }


  RenderHints::setAnnotationsAndSourceConfig(handle_annotations_and_sources_config($feature_block_settings));
  if (is_cdm_entity($element))  {
      $annotations_and_sources = handle_annotations_and_sources(
          $element, $element_markup, $footnote_list_key_suggestion
      );
  }else{
      $annotations_and_sources = handle_new_annotations_and_sources(
          $element, $element_markup, $footnote_list_key_suggestion
      );
  }


  $timescope_markup = '';
  if(isset_not_empty($element->timeperiod)){
    $timescope_markup = ' ' . timePeriodToString($element->timeperiod, true);
  }
  $modifier_markup = '';
    if(isset_not_empty($element->modifyingText_l10n)){
        $modifier_markup = ' ' . $element->modifyingText_l10n;
    }

  // handle the special case were the TextData is used as container for original source with name
  // used in source information without any text stored in it.
  $names_used_in_source_markup = '';
  if ($annotations_and_sources->hasNameUsedInSource() && empty($element_markup)) {
    $names_used_in_source_markup = join(', ', $annotations_and_sources->getNameUsedInSource()) . ': ';
    // remove all <span class="nameUsedInSource">...</span> from all source_references
    // these are expected to be at the end of the strings
    $pattern = '/ <span class="nameUsedInSource">.*$/';
    foreach( $annotations_and_sources->getSourceReferences() as &$source_reference){
      $source_reference = preg_replace($pattern , '', $source_reference);
    }
  }

  $source_references_markup = '';
  if ($annotations_and_sources->hasSourceReferences()) {
    $source_references_markup = '<span class="sources">' . join(' ', $annotations_and_sources->getSourceReferences()) . '<span>';
  }

  $feature_label = '';
  if ($prepend_feature_label) {
    $feature_label = '<span class="nested-feature-tree-feature-label">' . $element->feature->representation_L10n . ':</span> ';
  }
  $content_markup = $names_used_in_source_markup . $element_markup . $modifier_markup . $timescope_markup . $source_references_markup;

  if(!$content_markup && $feature_block_settings['sources_as_content'] !== 1){
    // no textual content? So skip this element completely, even if there could be an footnote key
    // see #4379
    return null;
  }

    $render_array['#value'] = $feature_label . $content_markup;
    $render_array['#value_suffix'] = $annotations_and_sources->footNoteKeysMarkup();
  return $render_array;
}

/**
 * Provides the according tag name for the description element markup which
 * fits the  $feature_block_settings['as_list'] value
 *
 * @param $feature_block_settings
 *   A feature_block_settings array, for details, please see
 *   get_feature_block_settings($feature_uuid = 'DEFAULT')
 *
 * @return mixed|string
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
  function make_feature_block_list($mergedFeatureNodes, $taxon) {

    $block_list = array();

    $gallery_settings = getGallerySettings(CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME);

    $use_description_features = array(UUID_USE);
    $contains_distribution = false;
    foreach($mergedFeatureNodes as $feature_node){
        if ($feature_node->term->uuid == UUID_DISTRIBUTION){
            $contains_distribution = true;
            break;
        }
    }

    RenderHints::pushToRenderStack('feature_block');
    // Create a drupal block for each feature
    $block_weight = - FEATURE_BLOCK_WEIGHT_INCREMENT;
    foreach ($mergedFeatureNodes as $feature_node) {

      $block_weight = $block_weight + FEATURE_BLOCK_WEIGHT_INCREMENT;

      if ((isset($feature_node->descriptionElements['#type']) ||
          has_feature_node_description_elements($feature_node)) && $feature_node->term->uuid != UUID_IMAGE) { // skip empty or suppressed features

        RenderHints::pushToRenderStack($feature_node->term->uuid);
          
        $feature_name = cdm_term_representation($feature_node->term, 'Unnamed Feature');
        $feature_block_settings = get_feature_block_settings($feature_node->term->uuid);
        

        $block = feature_block($feature_name, $feature_node->term);
        $block->content = array();
        $block_content_is_empty = TRUE;

        if(array_search($feature_node->term->uuid, $use_description_features) !== false) {
          // do not show features which belong to the UseDescriptions, these are
          // handled by compose_feature_block_items_use_records() where the according descriptions are
          // fetched again separately.
          // UseDescriptions are a special feature introduced for palmweb
          continue;
        }

        /*
         * Content/DISTRIBUTION.
         */
        if ($feature_node->term->supportsDistribution) {
          $block = compose_feature_block_distribution($taxon, $feature_node->descriptionElements, $feature_node->term, $contains_distribution);
          $block_content_is_empty = FALSE;
        }

        /*
         * Content/COMMON_NAME.
         */
        else if ($feature_node->term->uuid == UUID_COMMON_NAME) {
          $common_names_render_array = compose_feature_block_items_feature_common_name($feature_node->descriptionElements, $feature_node->term);
          $block->content[] = $common_names_render_array;
          $block_content_is_empty = FALSE;
        }

        /*
         * Content/Use Description (Use + UseRecord)
         */
        else if ($feature_node->term->uuid == UUID_USE_RECORD) {
          $block->content[] = cdm_block_use_description_content($taxon->uuid, $feature_node->term);
          $block_content_is_empty = FALSE;
        }

        /*
         * Content/ALL OTHER FEATURES.
         */
        else {

          $media_list = array();
          $elements_render_array = array();
          $child_elements_render_array = null;

          if (isset($feature_node->descriptionElements[0])) {
            $elements_render_array = compose_feature_block_items_generic($feature_node->descriptionElements);
          }

          // Content/ALL OTHER FEATURES/Subordinate Features
          // subordinate features are printed inline in one floating text,
          // it is expected hat subordinate features can "contain" TextData,
          // Qualitative- and Qualitative- DescriptionElements
          if (isset($feature_node->childNodes[0])) {
            $child_elements_render_array = compose_feature_block_items_nested($feature_node, $media_list, $feature_block_settings);
            $elements_render_array = array_merge($elements_render_array, $child_elements_render_array);
          }
          $block_content_is_empty = $block_content_is_empty && empty($media_list) && empty($elements_render_array);
          if(!$block_content_is_empty){
            $block->content[] = compose_feature_block_wrap_elements($elements_render_array, $feature_node->term, $feature_block_settings['glue']);
            $block->content[] = compose_feature_media_gallery($feature_node, $media_list, $gallery_settings);
            /*
             * Footnotes for the feature block
             */
            $block->content[] = markup_to_render_array(render_footnotes(PSEUDO_FEATURE_BIBLIOGRAPHY . '-' . $feature_node->term->uuid));
            $block->content[] = markup_to_render_array(render_footnotes($feature_node->term->uuid));
          }
        } // END all other features

        // allows modifying the block contents via a the hook_cdm_feature_node_block_content_alter
        drupal_alter('cdm_feature_node_block_content', $block->content, $feature_node->term, $feature_node->descriptionElements);

        if(!$block_content_is_empty){ // skip empty block content
          $block_list[$block_weight] = $block;
          cdm_toc_list_add_item(cdm_term_representation($feature_node->term), $feature_node->term->uuid, null, FALSE, $block_weight);
        } // END: skip empty block content
      } // END: skip empty or suppressed features
      RenderHints::popFromRenderStack();
    } // END: creating a block per feature

    RenderHints::popFromRenderStack();

    drupal_alter('cdm_feature_node_blocks', $block_list, $taxon);

    ksort($block_list);

    return  $block_list;
  }


/**
 * Returns a set of feature blocks for a taxon profile from the $mergedFeatureNodes of a given $taxon_dto.
 *
 * The taxon profile consists of drupal block elements, one for the description elements
 * of a specific feature. The structure is defined by specific FeatureTree.
 * The chosen FeatureTree is merged with the list of description elements prior to using this method.
 *
 * The merged nodes come with the taxon_dto
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
function make_feature_block_new_list($mergedFeatureNodes, $taxon) {

    $block_list = array();

    $gallery_settings = getGallerySettings(CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME);

    $use_description_features = array(UUID_USE);


    RenderHints::pushToRenderStack('feature_block');
    // Create a drupal block for each feature
    $block_weight = - FEATURE_BLOCK_WEIGHT_INCREMENT;
    foreach ($mergedFeatureNodes as $feature_node) {

        $block_weight = $block_weight + FEATURE_BLOCK_WEIGHT_INCREMENT;

        if ((isset($feature_node->facts->items[0]) )){
                //|| has_feature_node_description_elements($feature_node)) && $feature_node->term->uuid != UUID_IMAGE) { // skip empty or suppressed features

            RenderHints::pushToRenderStack($feature_node->uuid);

            //$feature_name = cdm_term_representation($feature_node->term, 'Unnamed Feature');
            $feature_name = $feature_node-> label;

            //TODO!!
            $feature_block_settings = get_feature_block_settings($feature_node->uuid);


            $block = new_feature_block($feature_name, $feature_node);
            $block->content = array();
            $block_content_is_empty = TRUE;

            if(array_search($feature_node->uuid, $use_description_features) !== false) {
                // do not show features which belong to the UseDescriptions, these are
                // handled by compose_feature_block_items_use_records() where the according descriptions are
                // fetched again separately.
                // UseDescriptions are a special feature introduced for palmweb
                continue;
            }

            /*
             * Content/DISTRIBUTION.
             */

            if (isset($feature_node->descriptionElements["DistributionInfoDTO"]) ) {
                $block = compose_feature_block_distribution($taxon, $feature_node->descriptionElements, $feature_node);
                $block_content_is_empty = FALSE;
            }
            else if ($feature_node->label == 'IUCN Status') {
                $feature_node->descriptionElements['#type'] = "DTO";
                $feature_node->descriptionElements['DistributionInfoDTO'] = $feature_node->facts-> items[0];
                $block = compose_feature_block_distribution($taxon, $feature_node->descriptionElements, $feature_node, true);
                $block_content_is_empty = FALSE;
            }
            /*
             * Content/COMMON_NAME.
             */
            else if ($feature_node->uuid == UUID_COMMON_NAME) {
                $common_names_render_array = compose_new_feature_block_items_feature_common_name($feature_node->facts);
                $block->content[] = $common_names_render_array;
                $block_content_is_empty = FALSE;
            }

            /*
             * Content/Use Description (Use + UseRecord)
             */
            else if ($feature_node->label == 'UseRecord') {
                $block->content[] = cdm_block_use_description_content($taxon->uuid, $feature_node->term);
                $block_content_is_empty = FALSE;
            }

            /*
             * Content/ALL OTHER FEATURES.
             */
            else {

                $media_list = array();
                $elements_render_array = array();
                $child_elements_render_array = null;

                if (isset($feature_node->facts->items[0])) {
                    $elements_render_array = compose_new_feature_block_items_generic($feature_node->facts, $feature_node-> uuid);
                }

                // Content/ALL OTHER FEATURES/Subordinate Features
                // subordinate features are printed inline in one floating text,
                // it is expected hat subordinate features can "contain" TextData,
                // Qualitative- and Qualitative- DescriptionElements
                if (isset($feature_node->childNodes[0])) {
                    $child_elements_render_array = compose_feature_block_items_nested($feature_node, $media_list, $feature_block_settings);
                    $elements_render_array = array_merge($elements_render_array, $child_elements_render_array);
                }
                $block_content_is_empty = $block_content_is_empty && empty($media_list) && empty($elements_render_array);
                if(!$block_content_is_empty){
                    $block->content[] = compose_new_feature_block_wrap_elements($elements_render_array, $feature_node->uuid, $feature_node->label, $feature_block_settings['glue']);
                    $block->content[] = compose_feature_media_gallery($feature_node, $media_list, $gallery_settings);
                    /*
                     * Footnotes for the feature block
                     */
                    $block->content[] = markup_to_render_array(render_footnotes(PSEUDO_FEATURE_BIBLIOGRAPHY . '-' . $feature_node->uuid));
                    $block->content[] = markup_to_render_array(render_footnotes($feature_node->uuid));
                }
            } // END all other features

            // allows modifying the block contents via a the hook_cdm_feature_node_block_content_alter
            drupal_alter('cdm_feature_node_block_content', $block->content, $feature_node->label, $feature_node->descriptionElements);

            if(!$block_content_is_empty){ // skip empty block content
                $block_list[$block_weight] = $block;
                cdm_toc_list_add_item(cdm_term_representation($feature_node->label), $feature_node->uuid, null, FALSE, $block_weight);
            } // END: skip empty block content
        } // END: skip empty or suppressed features
        RenderHints::popFromRenderStack();
    } // END: creating a block per feature

    RenderHints::popFromRenderStack();

    drupal_alter('cdm_feature_node_blocks', $block_list, $taxon);

    ksort($block_list);

    return  $block_list;
}

/**
 * Creates a render array of description elements  held by child nodes of the given feature node.
 *
 * This function is called recursively!
 *
 * @param $feature_node
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
function compose_feature_block_items_nested($feature_node, &$media_list, $feature_block_settings, $main_feature = NULL)
{

  if(!$main_feature){
    $main_feature = $feature_node->term;
  }
  /*
   * TODO should be configurable, options; YES, NO, AUTOMATIC
   * (automatic will only place the label if the first word of the description element text is not the same)
   */
  $prepend_feature_label = false;

  $render_arrays = array();
  foreach ($feature_node->childNodes as $child_node) {
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
   * @param $feature_node
   *  The merged feature three node which potentially contains media in its description elements.
   * @param $media_list
   *    Additional media to be merged witht the media contained in the nodes description elements
   * @param $gallery_settings
   * @return array
   *
   * @ingroup compose
   */
  function compose_feature_media_gallery($feature_node, $media_list, $gallery_settings) {

    if (isset($feature_node->descriptionElements)) {
      $media_list = array_merge($media_list, cdm_dataportal_media_from_descriptionElements($feature_node->descriptionElements));
    }

    $captionElements = array('title', 'rights');

    if (isset($media_list[0]) && isset($gallery_settings['cdm_dataportal_media_maxextend']) && isset($gallery_settings['cdm_dataportal_media_cols'])) {
      $gallery = compose_cdm_media_gallery(array(
        'mediaList' => $media_list,
        'galleryName' => CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME . '_' . $feature_node->term->uuid,
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
  function compose_feature_block_distribution($taxon, $descriptionElements, $feature, $contains_distribution = false) {
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
    $block->content = array();

    // $$descriptionElements['TextData'] is added to to the feature node in merged_taxon_feature_tree()
    if (isset($descriptionElements['TextData'])) {
      // --- TextData
      foreach ($descriptionElements['TextData'] as $text_data_element) {
          if (isset($text_data_element->typedLabel )){
              $render_array = compose_new_description_element_text_data($text_data_element, $feature-> uuid, $feature_block_settings);
              $repr = drupal_render($render_array);
              //$repr = cdm_tagged_text_to_markup($text_data_element-> typedLabel);
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
          }else{
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
    }


    if ($text_data_out_array && variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0)) {
      $block->content[] = compose_feature_block_wrap_elements(
        $text_data_out_array, $feature, $text_data_glue, $text_data_sortOutArray
      );
    }

    // --- Distribution map
      if ($feature->uuid == UUID_DISTRIBUTION) {
          $distribution_map_query_parameters = NULL;


          $map_visibility = variable_get(DISTRIBUTION_MAP_VISIBILITY, DISTRIBUTION_MAP_VISIBILITY_DEFAULT);
          if (variable_get(DISTRIBUTION_MAP_VISIBILITY, DISTRIBUTION_MAP_VISIBILITY_DEFAULT) == 'always' ||
              $map_visibility == 'automatic' && isset($descriptionElements['DistributionInfoDTO']->mapUriParams)
          ) {
              $distribution_map_query_parameters = $descriptionElements['DistributionInfoDTO']->mapUriParams;
          }
          $map_render_element = compose_distribution_map($distribution_map_query_parameters);
          $block->content[] = $map_render_element;
      }
    $dto_out_array = array();

    // --- Condensed Distribution
    if(variable_get(DISTRIBUTION_CONDENSED) && isset($descriptionElements['DistributionInfoDTO']->condensedDistribution)){
      $condensed_distribution_markup = '<p class="condensed_distribution">';

	  if(isset($descriptionElements['DistributionInfoDTO']->condensedDistribution->htmlString)){
		$condensed_distribution_markup .= $descriptionElements['DistributionInfoDTO']->condensedDistribution->htmlString;
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
      if (is_object($distribution_info_dto->tree) && ($feature->uuid == UUID_DISTRIBUTION || !$contains_distribution)) {
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

    $block->content[] = markup_to_render_array(render_footnotes('BIBLIOGRAPHY-' . UUID_DISTRIBUTION));
    $block->content[] = markup_to_render_array(render_footnotes(UUID_DISTRIBUTION));

    return $block;
  }

/**
 * Creates the markup for the media associated a DescriptionElement instance.
 *
 * @param $descriptionElement
 *    the DescriptionElement instance
 * @param $mimeTypePreference array
 *    An array of mime type strings. the order of the mimetypes is the order of preference.
 *    E.g.: array('application/pdf','image/jpeg')
 *
 * @return string
 *    The markup
 *
 * @ingroup compose
 */
function cdm_description_element_media($descriptionElement, $mimeTypePreference) {

  $out = '';

  _add_js_thickbox();

  $feature = $descriptionElement->feature;
  $medias = $descriptionElement->media;

  foreach ($medias as $media) {
    $pref_representations = cdm_preferred_media_representations($media, $mimeTypePreference, 300, 400);
    $media_representation = array_shift($pref_representations);
    if ($media_representation) {

      $content_type_directory = media_content_type_dir($media_representation);

      switch($content_type_directory){
        case 'image':
          $out .= compose_cdm_media_mime_image($media_representation, $feature);
          break;
        case 'application':
        case 'text':
        default:
          $out .= compose_cdm_media_mime_application($media_representation, $feature);
      }
    }
    else {
      // Media has empty or corrupt representation
      if(user_is_logged_in()){
        drupal_set_message('The media entity (' . l($media->uuid, path_to_media($media->uuid)) .') has empty or corrupt representation parts. Maybe the URI is empty.' , 'warning');
      }
    }
  }
  return $out;
}

/**
 * Creates the markup for the media associated a DescriptionElement instance.
 *
 * @param $descriptionElement
 *    the DescriptionElement instance
 * @param $mimeTypePreference array
 *    An array of mime type strings. the order of the mimetypes is the order of preference.
 *    E.g.: array('application/pdf','image/jpeg')
 *
 * @return string
 *    The markup
 *
 * @ingroup compose
 */
function cdm_new_description_element_media($descriptionElement, $mimeTypePreference) {

    $out = '';

    _add_js_thickbox();

    $feature = $descriptionElement->feature;
    $medias = $descriptionElement->media;

    foreach ($medias as $media) {
        $pref_representations = cdm_preferred_media_representations($media, $mimeTypePreference, 300, 400);
        $media_representation = array_shift($pref_representations);
        if ($media_representation) {

            $content_type_directory = media_content_type_dir($media_representation);

            switch($content_type_directory){
                case 'image':
                    $out .= compose_cdm_media_mime_image($media_representation, $feature);
                    break;
                case 'application':
                case 'text':
                default:
                    $out .= compose_cdm_media_mime_application($media_representation, $feature);
            }
        }
        else {
            // Media has empty or corrupt representation
            if(user_is_logged_in()){
                drupal_set_message('The media entity (' . l($media->uuid, path_to_media($media->uuid)) .') has empty or corrupt representation parts. Maybe the URI is empty.' , 'warning');
            }
        }
    }
    return $out;
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
  function compose_new_description_element_text_data($element, $feature_uuid, $feature_block_settings, $prepend_feature_label = FALSE) {

    $footnote_list_key_suggestion = $feature_uuid;

    $element_markup = '';
    if ($element->typedLabel[0]->label != null) {
      // TODO replacement of \n by <br> should be configurable
      $element_markup = str_replace("\n", "<br/>", $element->typedLabel[0]->label);
    }

    $render_array = compose_description_element($element, $feature_block_settings, $element_markup, $footnote_list_key_suggestion, $prepend_feature_label);

    return $render_array;
  }
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

  $render_array = compose_cdm_specimen_or_observation($element->associatedSpecimenOrObservation);

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
 * @return array
 *   a drupal render array for given CategoricalData element
 *
 * @ingroup compose
 */
function compose_description_element_categorical_data($element, $feature_block_settings, $prepend_feature_label = FALSE) {

  $state_data_markup = render_state_data($element);

  $render_array = compose_description_element($element, $feature_block_settings, $state_data_markup, $element->feature->uuid, $prepend_feature_label);

  return $render_array;
}


/**
 * Renders a single instance of the type TemporalData.
 *
 * @param $element
 *  The CDM TemporalData entity
 *
 * @param $feature_block_settings
 *
 * @param bool $prepend_feature_label
 *   Used in nested feature trees to put the feature as label in front of the description element text representation.
 *
 * @return array
 *   a drupal render array for given TemporalData element
 *
 * @ingroup compose
 */
function compose_description_element_temporal_data($element, $feature_block_settings, $prepend_feature_label = FALSE) {

  $markup = '<span class="period-label">' . $element->period->label .'</span>';
  $render_array = compose_description_element($element, $feature_block_settings, $markup, $element->feature->uuid, $prepend_feature_label);
  return $render_array;
}



/**
 * @param $element
 *
 * @return string
 * the markup
 */
function render_state_data($element) {

  $state_data_items = [];

  $out = '';

  if (isset($element->stateData)) {
    foreach ($element->stateData as $state_data) {

      $state = NULL;

      if (isset($state_data->state)) {
        $state = cdm_term_representation($state_data->state);

          $sample_count = 0;
          if (isset($state_data->count)) {
            $sample_count = $state_data->count;
            $state .= ' [' . $state_data->count . ']';
          }
    
          if (isset($state_data->modifyingText_l10n)) {
            $state .= ' ' . $state_data->modifyingText_l10n;
          }
    
          $modifiers_strings = cdm_modifiers_representations($state_data);
          $state_data_markup = ($modifiers_strings ?  $modifiers_strings . ' ' : '') . $state;
          // we could use strip_tags() to reduce the markup to text for the key but this is expensive
          $sort_key = str_pad($sample_count, 6, '0', STR_PAD_LEFT) . '_' . $state_data_markup;
          $state_data_items[$sort_key] = $state_data_markup;
      }

    }
    krsort($state_data_items);
    $out = '<span class="' . html_class_attribute_ref($element) . '">' . join(', ', $state_data_items) .  '</span>';
  }

  return $out;
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
 * @return array
 *   drupal render array for the given QuantitativeData element
 *
 * @ingroup compose
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

  $out = render_quantitative_statistics($element);

  $render_array = compose_description_element($element, $feature_block_settings, $out, $element->feature->uuid, $prepend_feature_label);

  return $render_array;
}

/**
 * Composes the HTML for quantitative statistics
 * @param $element
 *
 * @return string
 */
function render_quantitative_statistics($element) {

  $out = '';
  $type_representation = NULL;
  $min_max = statistical_values_array();
  $sample_size_markup = null;

  if (isset($element->statisticalValues)) {
    $out = '<span class=\"' . html_class_attribute_ref($element) . '\">';
    $other_values_markup = [];
    foreach ($element->statisticalValues as $statistical_val) {

      // compile the full value string which also may contain modifiers
      if (isset($statistical_val->value)) {
        $statistical_val->_value = $statistical_val->value;
      }
      $val_modifiers_strings = cdm_modifiers_representations($statistical_val);
      if ($val_modifiers_strings) {
        $statistical_val->_value = ' ' . $val_modifiers_strings . ' ' . $statistical_val->_value;
      }

      // either put into min max array or into $other_values
      // for generic output to be appended to 'min-max' string
      if (array_key_exists(statistical_measure_term2min_max_key($statistical_val->type), $min_max)) {
        $min_max[statistical_measure_term2min_max_key($statistical_val->type)] = $statistical_val;
      }
      else {
        drupal_set_message("Unsupported statistical value type: " . $statistical_val->type->uuid, "error");
      }
    } // end of loop over statisticalValues

    // create markup
    $unit = null;
    if (isset($element->unit)) {
      $unit = ' <span class="unit" title="'
        . cdm_term_representation($element->unit) . '">'
        . cdm_term_representation_abbreviated($element->unit)
        . '</span>';
    }
    $min_max_markup = statistical_values($min_max, $unit);
    $out .= $min_max_markup . '</span>';
  }

  if($sample_size_markup){
    $out .= ' ' . $sample_size_markup;

  }

  // modifiers of the description element itself
  $modifier_string = cdm_modifiers_representations($element);
  $out .= ($modifier_string ? ' ' . $modifier_string : '');
  if (isset($element->modifyingText_l10n)) {
    $out = $element->modifyingText_l10n . ' ' . $out;
  }
  return $out;
}

function statistical_measure_term2min_max_key($term){
  static $uuid2key = [
    UUID_STATISTICALMEASURE_MIN => 'Min',
    UUID_STATISTICALMEASURE_MAX => 'Max',
    UUID_STATISTICALMEASURE_AVERAGE => 'Average',
    UUID_STATISTICALMEASURE_SAMPLESIZE => 'SampleSize',
    UUID_STATISTICALMEASURE_VARIANCE => 'Variance',
    UUID_STATISTICALMEASURE_TYPICALLOWERBOUNDARY => 'TypicalLowerBoundary',
    UUID_STATISTICALMEASURE_TYPICALUPPERBOUNDARY => 'TypicalUpperBoundary',
    UUID_STATISTICALMEASURE_STANDARDDEVIATION => 'StandardDeviation',
    UUID_STATISTICALMEASURE_EXACTVALUE => 'ExactValue',
    UUID_STATISTICALMEASURE_STATISTICALMEASUREUNKNOWNDATA => 'StatisticalMeasureUnknownData'
  ];
  return $uuid2key[$term->uuid];
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
    $label = "";
    if (isset_not_empty($feature->representation_L10n)){
        $label =  $feature->representation_L10n;
    }elseif (isset_not_empty($feature->label)){
        $label = $feature-> label;
    }
    $render_array['elements']['#prefix'] = '<' . $enclosing_tag . ' class="feature-block-elements" id="' . $label . '">';
    $render_array['elements']['#suffix'] = '</' . $enclosing_tag . '>';

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
function compose_new_feature_block_wrap_elements(array $description_element_render_arrays, $feature_uuid, $feature_label, $glue = '', $sort = FALSE)
{
    $feature_block_settings = get_feature_block_settings($feature_uuid);


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
                $element_render_array['#value'] = $glue . " ". $element_render_array['#value'];
            } elseif (isset($element_render_array['#markup'])) {
                $element_render_array['#markup'] = $glue . " ". $element_render_array['#markup'];
            }
        }
        $is_first = false;
    }

    $render_array['elements']['children'] = $description_element_render_arrays;

    $render_array['elements']['#prefix'] = '<' . $enclosing_tag . ' class="feature-block-elements" id="' . $feature_label . '">';
    $render_array['elements']['#suffix'] = '</' . $enclosing_tag . '>';

    return $render_array;
}


  /* compose nameInSource or originalInfo as markup
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
        if (cache_get('current_taxon')){
            $current_taxon = cache_get('current_taxon');
            $taxon_page_accepted_name = $current_taxon-> data-> nameLabel;
        }else{
            $current_taxon = cdm_ws_get(CDM_WS_PORTAL_TAXON, $taxon_uuid);
            $taxon_page_accepted_name = $current_taxon->name->titleCache;
        }

    }else if ($suppress_for_shown_taxon && cache_get('current_taxon')){
        $current_taxon = cache_get('current_taxon');
        if ($current_taxon){
            $taxon_page_accepted_name = $current_taxon-> data-> nameLabel;
        }
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
    }elseif (isset($source->nameInSource) && count($source->nameInSource) >0  ) {
            // it is a DescriptionElementSource !
            $plaintext = cdm_tagged_text_to_string($source->nameInSource);
            if($suppress_for_shown_taxon && $taxon_page_accepted_name == $plaintext){
                return $name_in_source_render_array; // SKIP this name
            }
            $markup = cdm_tagged_text_to_markup($source->nameInSource);
            if ($do_link_to_name_used_in_source) {
                $markup = l(
                    $markup,
                    path_to_name($source->nameInSourceUuid),
                    array(
                        'attributes' => array(),
                        'absolute' => TRUE,
                        'html' => TRUE,
                    ));
            }
        }else if (isset_not_empty($source->originalInfo) ) {
      // the name used in source can not be expressed as valid taxon name,
      // so the editor has chosen to put the freetext name into ReferencedEntityBase.originalInfo
      // field
      // using the originalInfo as key to avoid duplicate entries
      $plaintext = $source->originalInfo;
      if($suppress_for_shown_taxon && $taxon_page_accepted_name == $plaintext){
        return $name_in_source_render_array; // SKIP this name
      }
      $markup = $source->originalInfo;
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
   * @return array
   *    A drupal render array for the $descriptionElements, may be an empty array if the textual content was empty.
   *    Footnote key or anchors are not considered to be textual content.
   *
   * @ingroup compose
   */
  function compose_feature_block_items_generic($description_elements) {

    $elements_out_array = [];
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
        $feature_block_settings = get_feature_block_settings($description_element-> feature->uuid);
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
          case 'TemporalData':
            $elements_out_array[] = compose_description_element_temporal_data($description_element, $feature_block_settings);
            break;
          case 'Uses':
            /* IGNORE Uses classes, these are handled completely in compose_feature_block_items_use_records()  */
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
 * Return HTML for a list of factual data dtos.
 *
 * Usually these are of a specific feature type.
 *
 * @param $description_elements
 *   array of items (dtos) which belong to the same feature.
 *   These items of a Description must be ordered by the chosen feature tree by
 *   calling the function _mergeFeatureTreeDescriptions().
 *   @see _mergeFeatureTreeDescriptions()
 *
 * @return array
 *    A drupal render array for the $descriptionElements, may be an empty array if the textual content was empty.
 *    Footnote key or anchors are not considered to be textual content.
 *
 * @ingroup compose
 */
function compose_new_feature_block_items_generic($description_elements, $feature_uuid) {

    $elements_out_array = [];
    $distribution_tree = null;

    /*
     * $feature_block_has_content will be set true if at least one of the
     * $descriptionElements contains some text which makes up some content
     * for the feature block. Footnote keys are not considered
     * to be content in this sense.
     */
    $feature_block_has_content = false;

    if (is_array($description_elements -> items)) {
        foreach ($description_elements-> items as $description_element) {
            /* decide based on the description element class
             *
             * Features handled here:
             * all except DISTRIBUTION, COMMON_NAME, USES, IMAGES,
             *
             * TODO provide api_hook as extension point for this?
             */
            if ($description_element->clazz == 'IndividualsAssociationDto'){
                $feature_block_settings = get_feature_block_settings('DEFAULT');
            }else{
                $feature_block_settings = get_feature_block_settings($description_element->typedLabel[0]->uuid);
            }

            if (isset_not_empty($description_element->typedLabel[0]) && isset_not_empty($description_element->typedLabel[0]-> cdmClass)){
                $switch_var =$description_element->typedLabel[0]->cdmClass;
            }else{
                $switch_var = $description_element->clazz;
            }

            switch ($switch_var) {
                case 'TextData':
                    $elements_out_array[] = compose_new_description_element_text_data($description_element, $description_element->uuid, $feature_block_settings);
                    break;
                case 'CategoricalData':
                    $elements_out_array[] = compose_description_element_categorical_data($description_element, $feature_block_settings);
                    break;
                case 'QuantitativeData':
                    $elements_out_array[] = compose_new_description_element_text_data($description_element, $feature_uuid, $feature_block_settings);
                    //$elements_out_array[] = compose_description_element_quantitative_data($description_element, $feature_block_settings);
                    break;
                case 'IndividualsAssociation':
                    $elements_out_array[] = compose_description_element_individuals_association($description_element, $feature_block_settings);
                    break;
                case 'IndividualsAssociationDto':
                    $elements_out_array[] = compose_description_element_individuals_association($description_element, $feature_block_settings);
                    break;
                case 'TaxonInteraction':
                    $elements_out_array[] = compose_description_element_taxon_interaction($description_element, $feature_block_settings);
                    break;
                case 'CommonTaxonName':
                    $elements_out_array[] = compose_description_element_common_taxon_name($description_element, $feature_block_settings);
                    break;
                case 'TemporalData':
                    $elements_out_array[] = compose_new_description_element_text_data($description_element, $feature_uuid, $feature_block_settings);
                    break;
                case 'Uses':
                    /* IGNORE Uses classes, these are handled completely in compose_feature_block_items_use_records()  */
                    break;
                default:
                    $feature_block_has_content = true;
                    $elements_out_array[] = markup_to_render_array('<li>No method for rendering unknown description class: ' . $description_element->class . '</li>');
            }
            if (isset_not_empty($elements_out_array)){
                $elements_out_array_last_item = $elements_out_array[count($elements_out_array) - 1];
            }

            // considering not empty as long as the last item added is a render array
            $feature_block_has_content = $feature_block_has_content || !empty($elements_out_array_last_item['#type']);
        }

        // If feature = CITATION sort the list of sources.
        // This is ONLY for FLORA MALESIANA and FLORE d'AFRIQUE CENTRALE.
        if (isset($description_element->feature->uuid) && $description_element->feature->uuid == UUID_CITATION) {
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
          $language_area_key .= $element->language->representation_L10n;
        }
        if(isset($element->area->representation_L10n) && strlen($element->area->representation_L10n) > 0){
          $language_area_key .= ($language_area_key ? ' '  : '') . '(' . $element->area->representation_L10n . ')';
        }
        if($language_area_key){
          $language_area_key = '<span class="language-area-label">' . $language_area_key . '<span class="separator">: </span></span>';
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

      $common_name_feature_elements[] = $language_area_key . join(', ', $per_language_area_out);
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

  $footnotes = render_footnotes('BIBLIOGRAPHY-' . $footnote_key_suggestion);
  $footnotes .= render_footnotes($footnote_key_suggestion);

  return  markup_to_render_array(  // FIXME markup_to_render_array should no longer be needed
    '<div class="common-taxon-name">' . $common_name_out . '</div>'
    .'<div class="text-data">' . drupal_render($common_name_out_text_data) . '</div>'
    .$footnotes,
    $weight
  );
}


/**
 * Composes block of common names for the given items $elements which must be dtos for the feature CommonName
 * TODO: This needs adaption to the dto implementation
 * @parameter $elements
 *  an array of CDM DescriptionElement DTOs either of type CommonName or TextData
 * @parameter $feature //TODO for the dto implementation we do not need this?
 *  the common feature of all $elements, must be CommonName
 *
 * @return
 *   A drupal render array
 *
 * @ingroup compose
 */
function compose_new_feature_block_items_feature_common_name($elements, $weight = FALSE) {

    $common_name_out = '';
    $common_name_feature_elements = array();
    $textData_commonNames = array();

    $footnote_key_suggestion = 'common-names-feature-block';

    $feature_block_settings = get_feature_block_settings(UUID_COMMON_NAME);

    if (is_array($elements->items)) {
        foreach ($elements->items as $element) {

            if ($element->clazz == 'CommonNameDto') {

                // common name without a language or area, should not happen but is possible
                $language_area_key = '';
                if (isset($element->language)) {
                    $language_area_key .= $element->language;
                }
                if(isset($element->area) && strlen($element->area) > 0){
                    $language_area_key .= ($language_area_key ? ' '  : '') . '(' . $element->area . ')';
                }
                if($language_area_key){
                    $language_area_key = '<span class="language-area-label">' . $language_area_key . '<span class="separator">: </span></span>';
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

            $common_name_feature_elements[] = $language_area_key . join(', ', $per_language_area_out);
        } // End of loop over set of elements per language area
/*
 *     $common_name_feature_elements_render_array = compose_feature_block_wrap_elements(
      $common_name_feature_elements, $feature, '; ', FALSE
    );
 */

        $common_name_feature_elements_render_array = compose_new_feature_block_wrap_elements(
            $common_name_feature_elements, UUID_COMMON_NAME, "Common Name", $feature_block_settings['glue']
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

    $common_name_out_text_data = compose_new_feature_block_wrap_elements(
        $text_data_out, UUID_COMMON_NAME, "Common Name", $feature_block_settings['glue']
    );

    $footnotes = render_footnotes('BIBLIOGRAPHY-' . $footnote_key_suggestion);
    $footnotes .= render_footnotes($footnote_key_suggestion);

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
 * @param $footnote_key
 *
 * @return array
 *   Drupal render array
 *
 * @ingroup compose
 */
function compose_description_element_common_taxon_name($element, $feature_block_settings, $footnote_key = NULL)
{

  if(!$footnote_key) {
    $footnote_key = $element->feature->uuid;
  }

  $name = '';
  if(isset($element->name)){
    $name = $element->name;
  }
  if (isset($element->transliteration) && isset($element->name)){
      $name = $name ." [". $element->transliteration ."]";
  }


  return compose_description_element($element, $feature_block_settings, $name, $footnote_key);
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

  $markup_array = array();
  RenderHints::pushToRenderStack('descriptionElementDistribution');
  RenderHints::setFootnoteListKey(UUID_DISTRIBUTION);

  $feature_block_settings = get_feature_block_settings(UUID_DISTRIBUTION);
  $enclosingTag = cdm_feature_block_element_tag_name($feature_block_settings);

  foreach ($description_elements as $description_element) {
    RenderHints::setAnnotationsAndSourceConfig(handle_annotations_and_sources_config($feature_block_settings));
    $annotations_and_sources = handle_annotations_and_sources(
      $description_element, $description_element->area->representation_L10n, UUID_DISTRIBUTION
    );

    $status = distribution_status_label_and_markup([$description_element->status]);

    $out = '';
    $out .= '<' . $enclosingTag . ' class="descriptionElement descriptionElement-' . $description_element->uuid
      . ' " title="' . $status['label']. '">'
      . $description_element->area->representation_L10n
      . $status['markup'];
    if($annotations_and_sources->hasSourceReferences()){
      $out .= ' ' . join(' ', $annotations_and_sources->getSourceReferences());
    }
    $out .= $annotations_and_sources->footNoteKeysMarkup() . '</' . $enclosingTag . '>';
    $markup_array[] = $out;
  }

  RenderHints::popFromRenderStack();
  return markup_to_render_array(join('<span class="separator">' . $feature_block_settings['glue'] . '</span>', $markup_array));
}

  /**
   * @param array $distribution_status
   * @return array an array with following keys
   *   - 'label': the plain text status label
   *   - 'markup': markup for the status
   */
  function distribution_status_label_and_markup(array $distribution_status, $status_glue = '&#8210; ') {

    $status_markup = '';
    $status_label = '';

    foreach($distribution_status as $status) {
      if(isset($status->representation_L10n)){
          $status_label .= ($status_label ? $status_glue : '') . $status->representation_L10n ;
      }else{
          $status_label .= ($status_label ? $status_glue : '') . $status->label ;
      }
      if (isset($status->idInVocabulary)){
          $status_markup .=  '<span class="distributionStatus"> '
              . ($status_markup ? $status_glue : '')
              . '<span class="distributionStatus-' . $status->idInVocabulary . '">'
              .  $status->representation_L10n
              . '</span></span>';

      }else{
          $status_markup .=  '<span class="distributionStatus"> '
              . ($status_markup ? $status_glue : '')
              . '<span class="distributionStatus-' . $status->label . '">'
              .  $status->label
              . '</span></span>';
      }

    };
    return ['label' => $status_label, 'markup' => $status_markup];
  }


  /**
   * Provides the merged feature tree for a taxon profile page.
   *
   * The merging of the profile feature tree is actually done in
   * _mergeFeatureTreeDescriptions(). See this method  for details
   * on the structure of the merged tree.
   *
   * This method provides a hook which can be used to modify the
   * merged feature tree after it has been created, see
   * hook_merged_taxon_feature_tree_alter()
   *
   * @param $taxon
   *    A CDM Taxon instance
   *
   * @return object
   *    The merged feature tree
   *
   */
  function merged_taxon_feature_tree($taxon) {
      $feature_tree = get_profile_feature_tree();

      // 1. fetch descriptions_by_featuretree but exclude the distribution feature
      //find all distribution features in feature tree
      $distribution_feature_uuids = array();
      foreach ($feature_tree->root ->childNodes as $childNode ){
          if ($childNode->term->supportsDistribution){
              $distribution_feature_uuids[]= $childNode->term->uuid;
          }
      }

     $merged_tree = cdm_ws_descriptions_by_featuretree($feature_tree, $taxon->uuid, array(UUID_DISTRIBUTION));


      // 2. find the distribution feature node
    foreach ($distribution_feature_uuids as $distribution_feature_uuid){
          $distribution_node =& cdm_feature_tree_find_node($merged_tree->root->childNodes, $distribution_feature_uuid);

          if ($distribution_node) {
              // 3. get the distributionInfoDTO
              $query_parameters = cdm_distribution_filter_query();
              $query_parameters['part'] = array('mapUriParams');
              if (variable_get(DISTRIBUTION_CONDENSED)) {
                  $query_parameters['part'][] = 'condensedDistribution';
              }
              if (variable_get(DISTRIBUTION_ORDER_MODE, DISTRIBUTION_ORDER_MODE_DEFAULT) == 'TREE') {
                  $query_parameters['part'][] = 'tree';
              } else {
                  $query_parameters['part'][] = 'elements';
              }


              $query_parameters['omitLevels'] = array();
              foreach (variable_get(DISTRIBUTION_TREE_OMIT_LEVELS, array()) as $uuid) {
                  if (is_uuid($uuid)) {
                      $query_parameters['omitLevels'][] = $uuid;
                  }
              }
              $customStatusColorsJson = variable_get(DISTRIBUTION_STATUS_COLORS, NULL);
              if ($customStatusColorsJson) {
                  $query_parameters['statusColors'] = $customStatusColorsJson;
              }
              $query_parameters['features'] = $distribution_feature_uuid;
              if (variable_get(CDM_AREATREE_UUID)){
                  $query_parameters['areaTree'] = variable_get(CDM_AREATREE_UUID);
              }
              if (variable_get(CDM_STATUSTREE_UUID)){
                  $query_parameters['statusTree'] = variable_get(CDM_STATUSTREE_UUID);
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
      }

    // allows modifying the merged tree via a the hook_cdm_feature_node_block_content_alter
    drupal_alter('merged_taxon_feature_tree', $taxon, $merged_tree);

    return $merged_tree;
  }

/**
 * Provides the merged feature tree for a taxon profile page.
 *
 * The merging of the profile feature tree is actually done in
 * _mergeFeatureTreeDescriptions(). See this method  for details
 * on the structure of the merged tree.
 *
 * This method provides a hook which can be used to modify the
 * merged feature tree after it has been created, see
 * hook_merged_taxon_feature_tree_alter()
 *
 * @param $taxon_dto
 *    A CDM Taxon DTO instance
 *
 * @return object
 *    The merged feature tree
 *
 */
function merged_taxon_new_feature_tree($taxon) {

    // 1. fetch descriptions_by_featuretree but exclude the distribution feature
    //$merged_tree = cdm_ws_descriptions_by_featuretree(get_profile_feature_tree(), $taxon->uuid, array(UUID_DISTRIBUTION));
    $merged_tree =  $taxon-> taxonFacts->items;
    if ($merged_tree == null){
        return array();
    }

    // 2. find the distribution feature node
    $distribution_nodes =& cdm_feature_tree_new_find_distributionNode($merged_tree);

    foreach ($distribution_nodes as $distribution_node) {
        if (isset($distribution_node->facts->items[0])) {

            $distribution_info_dto = $distribution_node->facts->items[0];

            $distribution_node->descriptionElements = array('#type' => 'DTO');
            if (isset($distribution_node->facts->items[1])) {
                $distribution_node->descriptionElements['TextData'] = array_slice($distribution_node->facts->items, 1);
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

  /**
   * @param $distribution_tree
   *  A tree cdm TreeNode items. A TreeNode item has a NamedArea as nodeId
   *  and Distribution items as data array. Per data array some Distributions may
   *  be with status information, others only with sources, others with both.
   *  Each node may also have subordinate node items in the children field.
   *  TreeNode:
   *   - array data
   *   - array children
   *   - int numberOfChildren
   *   - stdClass nodeId
   *
   * @param $feature_block_settings
   *
   * @return array
   * @throws \Exception
   */
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
 * this function should produce markup as the
 * compose_description_elements_distribution() function.
 *
 * @param array $tree_nodes
 *  An array of cdm TreeNode items. A TreeNode item has a NamedArea as nodeId
 *  and Distribution items as data array. Per data array some Distributions may
 *  be with status information, others only with sources, others with both.
 *  TreeNode:
 *   - array data
 *   - array children
 *   - int numberOfChildren
 *   - stdClass nodeId
 * @param array $feature_block_settings
 * @param $markup
 * @param $hierarchy_style
 * @param int $level_index
 *
 * @throws \Exception
 *
 * @see compose_description_elements_distribution()
 * @see compose_distribution_hierarchy()
 *
 */
  function _compose_distribution_hierarchy(array $tree_nodes, array $feature_block_settings, &$markup, $hierarchy_style, $level_index = -1){

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
      if (isset($node->nodeId->representation_L10n)){
          $label = $node->nodeId->representation_L10n;
      }else{
          $label = $node->nodeId->label;
      }

      $distributions = $node->data;
      $distribution_uuids = array();
      $distribution_aggregate = NULL;
      $status = ['label' => '', 'markup' => ''];

      foreach($distributions as $distribution){
        $distribution_uuids[] = $distribution->uuid;
        // if there is more than one distribution we aggregate the sources and
        // annotations into a synthetic distribution so that the footnote keys
        // can be rendered consistently
        if(!$distribution_aggregate) {
          $distribution_aggregate = $distribution;
          if(isset($distribution->status)){
            $distribution_aggregate->status = [$distribution->status];
          } else {
            $distribution_aggregate->status = [];
          }
          if ((is_array($distribution_aggregate->sources) &&!isset($distribution_aggregate->sources ) )|| (!isset($distribution_aggregate->sources)) && $distribution_aggregate->clazz == "DistributionDto"){
              $distribution_aggregate->sources = array();
          }else if (!is_array($distribution_aggregate->sources) && !isset($distribution_aggregate->sources-> items[0])){
            $distribution_aggregate->sources->items[0] = array();
          }
          if(!isset($distribution_aggregate->annotations)){
            $distribution_aggregate->annotations = array();
          }
        } else {
          if(isset($distribution->status)){
            $distribution_aggregate->status[] = $distribution->status;
          }
          if (is_array($distribution->sources) ){
            $distribution_aggregate->sources= array_merge($distribution_aggregate->sources,
                    $distribution->sources);
          }else{
            if(isset($distribution->sources) && $distribution->sources->count >0) {
                if (!isset($distribution_aggregate->sources) || $distribution_aggregate->sources->count == 0){
                    $distribution_aggregate -> sources = $distribution->sources;
                }else{
                    //$distribution_aggregate->sources->items= array_merge($distribution_aggregate->sources->items,
                      //      $distribution->sources->items);
                    if (is_array($distribution_aggregate->sources->items) && is_array($distribution->sources->items)){
                        $array = [...$distribution_aggregate->sources->items, ... $distribution->sources->items];
                        $distribution_aggregate->sources->items= array_unique($array, SORT_REGULAR);
                    }
                    $distribution_aggregate->sources->count = count($distribution_aggregate->sources->items);
                }
            }
          }
          if(isset($distribution->annotations)) {
              if (is_array($distribution->annotations)){
                  $annotations = $distribution->annotations;
              }else{
                  $annotations = $distribution->annotations->items;
              }
           $distribution_aggregate->annotations = array_merge($distribution_aggregate->annotations,
              $annotations);
          }
        }
      }

      $annotations_and_sources =  null;
      if($distribution_aggregate) {
        RenderHints::setAnnotationsAndSourceConfig(handle_annotations_and_sources_config($feature_block_settings));
        $annotations_and_sources = handle_new_annotations_and_sources(
          $distribution_aggregate, $label, UUID_DISTRIBUTION
        );
        $status = distribution_status_label_and_markup($distribution_aggregate->status, $level_style['status_glue']);
      }

      $per_node_markup[$node_index] .= '<' . $enclosingTag . ' class="descriptionElement'
        . join(' descriptionElement-', $distribution_uuids)
        . ' level_index_' . $level_index
        . ' " title="' . $status['label'] . '">'
        . '<span class="area_label">' . $label
        . $level_style['label_suffix'] . '</span>'
        . $status['markup']
      ;

      if(isset($annotations_and_sources)){
        if($annotations_and_sources->hasSourceReferences()){
          $per_node_markup[$node_index] .= ' ' . join(', ' , $annotations_and_sources->getSourceReferences());
        }
        if($annotations_and_sources->hasFootnoteKeys()) {
          $per_node_markup[$node_index] .= $annotations_and_sources->footNoteKeysMarkup();
        }
      }

      if(isset($node->children[0])){
        $per_node_markup[$node_index] .= $level_style['item_suffix'];
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


/**
 * Provides the content for a block of Uses Descriptions for a given taxon.
 *
 * Fetches the list of TaxonDescriptions tagged with the MARKERTYPE_USE
 * and passes them to the theme function theme_cdm_UseDescription().
 *
 * @param string $taxon_uuid
 *   The uuid of the Taxon
 *
 * @return array
 *   A drupal render array
 */
function cdm_block_use_description_content($taxon_uuid, $feature) {

  $use_description_content = array();

  if (is_uuid($taxon_uuid )) {
    $markerTypes = array();
    $markerTypes['markerTypes'] = UUID_MARKERTYPE_USE;
    $useDescriptions = cdm_ws_fetch_all(CDM_WS_PORTAL_TAXON . '/' . $taxon_uuid . '/descriptions', $markerTypes);
    if (!empty($useDescriptions)) {
      $use_description_content = compose_feature_block_items_use_records($useDescriptions, $taxon_uuid, $feature);
    }
  }

  return $use_description_content;
}

/**
 * Creates a trunk of a feature object which can be used to build pseudo feature blocks like the Bibliography.
 *
 * @param $representation_L10n
 * @param String $pseudo_feature_key
 *    Will be set as uuid but should be one of 'BIBLIOGRAPHY', ... more to come. See also get_feature_block_settings()
 *
 * @return object
 *  The feature object
 */
function make_pseudo_feature($representation_L10n, $pseudo_feature_key = null){
  $feature = new stdClass;
  $feature->representation_L10n = $representation_L10n;
  $feature->uuid = NULL; // $pseudo_feature_key;
  $feature->label = $pseudo_feature_key;
  $feature->class = 'PseudoFeature';

  return $feature;

}

/**
 * @param $root_nodes, for obtaining the  root nodes from a description you can
 * use the function get_root_nodes_for_dataset($description);
 *
 * @return string
 */
function render_description_string($root_nodes, &$item_cnt = 0) {

  $out = '';

  $description_strings= [];
  if (!empty($root_nodes)) {
    foreach ($root_nodes as $root_node) {
      if(isset($root_node->descriptionElements)) {
        foreach ($root_node->descriptionElements as $element) {
          $feature_label = $element->feature->representation_L10n;
          if($item_cnt == 0){
            $feature_label = ucfirst($feature_label);
          }
          switch ($element->class) {
            case 'CategoricalData':
              $state_data = render_state_data($element);
              if (!empty($state_data)) {
                if(is_suppress_state_present_display($element, $root_node)){
                  $description_strings[] = '<span class="'. html_class_attribute_ref($element) .'"><span class="label">' . $feature_label . '</span>: '  . '</span>';
                } else {
                  $description_strings[] = '<span class="'. html_class_attribute_ref($element) .'"><span class="label">' . $feature_label . '</span>: ' . $state_data . '</span>;' ;
                }
              }
              break;
            case 'QuantitativeData':
              $description_strings[] = '<span class="'. html_class_attribute_ref($element) .'"><span class="label">' . $feature_label . '</span>: ' . render_quantitative_statistics($element) . '</span>;';
              break;
          }
        }
        $item_cnt++;
      }

      // recurse child nodes
      $child_markup = render_description_string($root_node->childNodes, $item_cnt);
      if($child_markup){
        $description_strings[] = $child_markup;
      }
    }
    if(count($description_strings) > 0){
      // remove last semicolon
      $description_strings[count($description_strings) - 1] = preg_replace('/;$/', '', $description_strings[count($description_strings) - 1]);
    }
    $out  = join(' ', $description_strings);
  }
  return $out;
}

/**
 * Compose a description as a table of Feature<->State
 *
 * @param $description_uuid
 *
 * @return array
 *    The drupal render array for the page
 *
 * @ingroup compose
 */
function  compose_description_table($description_uuid, $descriptive_dataset_uuid = NULL) {

  RenderHints::pushToRenderStack('description_table');

  $render_array = [];

  $description = cdm_ws_get(CDM_WS_PORTAL_DESCRIPTION, [$description_uuid]);
  $dataSet = NULL;
  // dataset passed as parameter
  if ($descriptive_dataset_uuid != NULL) {
    foreach ($description->descriptiveDataSets as $set) {
      if ($set->uuid == $descriptive_dataset_uuid) {
        $dataSet = $set;
        break;
      }
    }
  }

  if(!empty($description->descriptiveDataSets)) {
    // only one dataset present
    if (!isset($dataSet) && sizeof($description->descriptiveDataSets) == 1) {
      foreach ($description->descriptiveDataSets as $dataSet) {
        break;
      }
    }

    // generate description title
    RenderHints::pushToRenderStack('title');
    if (isset($dataSet)) {

      $described_entity_title = NULL;
      if(isset($description->describedSpecimenOrObservation)){
        $described_entity_title = $description->describedSpecimenOrObservation->titleCache;
      } else if($description->taxon) {
          $described_entity_title = render_taxon_or_name($description->taxon);
      }
      $title = 'Descriptive Data ' . $dataSet->titleCache .
        ($described_entity_title ? ' for ' . $described_entity_title : '');
    }
    $render_array['title'] = markup_to_render_array($title, null, '<h3 class="title">', '</h3>');
    RenderHints::popFromRenderStack();
    // END of --- generate description title

    if (isset($description->types)) {
      foreach ($description->types as $type) {
        if ($type == 'CLONE_FOR_SOURCE') {
          $render_array['source'] = markup_to_render_array("Aggregation source from " . $description->created, null, '<div class="date-created">', '</div>');
          break;
        }
      }
    }
  }
  // multiple datasets present see #8714 "Show multiple datasets per description as list of links"
  else {
    $items = [];
    foreach ($description->descriptiveDataSets as $dataSet) {
      $path = path_to_description($description->uuid, $dataSet->uuid);
      $attributes['class'][] = html_class_attribute_ref($description);
      $items[] = [
        'data' => $dataSet->titleCache . icon_link($path),
      ];
    }
    $render_array['description_elements'] = [
      '#title' => 'Available data sets for description',
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#items' => $items,
    ];
  }

  $described_entities = [];
  if (isset($description->describedSpecimenOrObservation)) {
    $decr_entitiy = '<span class="label">Specimen:</span> ' . render_cdm_specimen_link($description->describedSpecimenOrObservation);
    $described_entities['specimen'] = markup_to_render_array($decr_entitiy, null, '<div>', '</div>');
  }
  if (isset($description->taxon)) {
    $decr_entitiy = '<span class="label">Taxon:</span> ' . render_taxon_or_name($description->taxon, url(path_to_taxon($description->taxon->uuid)));
    $described_entities['taxon'] = markup_to_render_array($decr_entitiy, null, '<div>', '</div>');
  }

  if(count($described_entities)){
    $render_array['described_entities'] = $described_entities;
    $render_array['described_entities']['#prefix'] = '<div class="described-entities">';
    $render_array['described_entities']['#suffix'] = '</div>';
  }


  $root_nodes = get_root_nodes_for_dataset($description);


  $rows = [];
  $rows = description_element_table_rows($root_nodes, $rows);

  // --- create headers
  $header = [0 => [], 1 => []];

  foreach($rows as $row) {
    if(array_search('Character', $row['class']) && array_search('Character', $header[0]) === false){
      $header[0][] = 'Character';
    } elseif (array_search('Feature', $row['class']) && array_search('Feature', $header[0]) === false){
      $header[0][] = 'Feature';
    }
    if(array_search('has_state', $row['class']) && array_search('States', $header[1]) === false){
      $header[1][] = 'States';
    } elseif (array_search('has_values', $row['class']) && array_search('Values', $header[1]) === false){
      $header[1][] = 'Values';
    }
  }
  asort($header[0]);
  asort($header[1]);
  $header[0] = join('/', $header[0]);
  $header[1] = join('/', $header[1]);

  // ---

  if (!empty($rows)) {
    $render_array['table'] = markup_to_render_array(theme('table', [
      'header' => $header,
      'rows' => $rows,
      'caption' => statistical_values_explanation(),
      'title' => "Table"
    ]));
  }

  // --- sources
  if (isset($description->sources) and !empty($description->sources)) {
    $items = [];
    foreach ($description->sources as $source) {
      if ($source->type == 'Aggregation' and isset($source->cdmSource)){
        $cdm_source_entity = $source->cdmSource;
        switch($cdm_source_entity->class){
          case 'Taxon':
            $source_link_markup = render_taxon_or_name($cdm_source_entity) . icon_link(path_to_taxon($cdm_source_entity->uuid, false));
            break;
          case 'TaxonDescription':
          case 'NameDescription':
          case 'SpecimenDescription':
            $source_link_markup = render_cdm_entity_link($cdm_source_entity);
            break;
          default:
            $source_link_markup = '<span class="error">Unhandled CdmSource</span>';
        }
        $items[$cdm_source_entity->titleCache] = [
          'data' => $source_link_markup
        ];
      }
    }
    ksort($items);
    $render_array['sources'] = [
      '#title' => 'Sources',
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#items' => $items,
      '#attributes' => ['class' => 'sources']
    ];
    $render_array['#prefix'] = '<div class="description-table">';
    $render_array['#suffix'] = '</div>';
  }

  RenderHints::popFromRenderStack();

  return $render_array;
}

/**
 * For a given description returns the root nodes according to the
 *corresponding term tree. The term tree is determined as follow:
 * 1. If description is part of a descriptive data set the term tree of that
 *    data set is used (FIXME handle multiple term trees)
 * 2. Otherwise the portal taxon profile tree is used
 * @param $description
 *
 * @return array
 */
function get_root_nodes_for_dataset($description) {
  if (!empty($description->descriptiveDataSets)) {
    foreach ($description->descriptiveDataSets as $dataSet) {
      break;// FIXME handle multiple term trees
    }
    $tree = cdm_ws_get(CDM_WS_TERMTREE, $dataSet->descriptiveSystem->uuid);
    $root_nodes = _mergeFeatureTreeDescriptions($tree->root->childNodes, $description->elements);
  }
  else {
    $root_nodes = _mergeFeatureTreeDescriptions(get_profile_feature_tree()->root->childNodes, $description->elements);
  }
  return $root_nodes;
}

/**
 * Recursively creates an array of row items to be used in theme_table.
 *
 * The array items will have am element 'class' with information on the
 * nature of the DescriptionElement ('has_values' | 'has_state') and on the
 * type of the FeatureNode ('Feature' | 'Character')
 *
 * @param array $root_nodes
 * @param array $row_items
 * @param int $level
 *     the depth in the hierarchy
 *
 * @return array
 *  An array of row items to be used in theme_table
 *
 *
 */
function description_element_table_rows($root_nodes, $row_items, $level = 0) {

  $indent_string = '&nbsp;&nbsp;&nbsp;';
  foreach ($root_nodes as $root_node) {
    if(isset($root_node->descriptionElements)) {
      foreach ($root_node->descriptionElements as $element) {
        $level_indent = str_pad('', $level * strlen($indent_string), $indent_string);
        switch ($element->class) {
          case 'QuantitativeData':
            $row_items[] = [
              'data' => [
                [
                  'data' => markup_to_render_array($level_indent . $element->feature->representation_L10n),
                  'class' => ['level_' . $level]
                ],
                render_quantitative_statistics($element)
              ],
              'class' => ['level_' . $level, 'has_values', $element->feature->class]
            ];
            break;
          case 'CategoricalData':
            default:
            if (!empty($element->stateData)) {
              $supress_state_display = is_suppress_state_present_display($element, $root_node);
              if(!$supress_state_display){
                $state_cell = render_state_data($element);
              } else {
                $state_cell = "<span> </span>";
              }
              $row_items[] = [
                'data' => [
                  [
                    'data' => markup_to_render_array($level_indent . $element->feature->representation_L10n),
                    'class' => ['level_' . $level]
                  ],
                  $state_cell,
                ],
                'class' => ['level_' . $level, 'has_state', $element->feature->class]
              ];
            }
            break;
        }
      }
    }
    // recurse child nodes
    $row_items = description_element_table_rows($root_node->childNodes, $row_items, $level + 1);
  }
  return $row_items;
}

/**
 * @param $element
 * @param $root_node
 *
 * @return bool
 */
function is_suppress_state_present_display($element, $root_node) {
  return count($element->stateData) == 1 & $element->stateData[0]->state->representation_L10n == 'present' && is_array($root_node->childNodes);
}



