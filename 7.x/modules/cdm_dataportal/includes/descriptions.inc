<?php

/**
 * Returns the localized representations of the modifiers hold by the supplied cdm instance concatenated into one string.
 *
 * @param object $iModifieable
 *   cdm instance of an class implementing the interface IModifieable: DescriptionElementBase, StateDate, State
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
 * Filters the given set of description elements and prefers computed elements over
 * others. Computed description elements are identified by the MarkerType.COMPUTED()
 *
 * If the given set contains at least one computed element only the computed elements
 * returned.
 *
 * @param $description_elements
 *   An array of CDM DescriptionElementBase instances
 * @return only the computed description elements otherwise all others.
 * @deprecated this is replaced by the cdmlib DistributionUtil class!!!
 */
function cdm_description_elements_prefer_computed($description_elements){

  $computed_elements = array();
  $other_elements = array();

  if(!empty($description_elements)){
    foreach ($description_elements as $element) {
      if(cdm_entity_has_marker($element, UUID_MARKERTYPE_COMPUTED)){
        $computed_elements[] = $element;
      } else {
        $other_elements[] = $element;
      }
    }
  }

  if(count($computed_elements) > 0) {
    return $computed_elements;
  } else {
    return $other_elements;
  }
}

function cdm_distribution_filter_query(){
  $cdm_distribution_filter = get_array_variable_merged(CDM_DISTRIBUTION_FILTER, CDM_DISTRIBUTION_FILTER_DEFAULT);
  $query = array();

  if($cdm_distribution_filter['filter_rules']['statusOrderPreference']){
    $query['statusOrderPreference'] = 1;
  }
  if($cdm_distribution_filter['filter_rules']['subAreaPreference']){
    $query['subAreaPreference'] = 1;
  }
  if(is_array($cdm_distribution_filter['hideMarkedAreas']) && count($cdm_distribution_filter['hideMarkedAreas']) > 0){
    $query['hideMarkedAreas'] = '';
    foreach ($cdm_distribution_filter['hideMarkedAreas'] as $marker_type => $enabled)
      if($enabled){
        $query['hideMarkedAreas'] .= ($query['hideMarkedAreas'] ? ',' : '') . $marker_type;
      }
  }

  return $query;
}

/**
 * Merge the fields 'annotations', 'markers', 'sources', 'media' from the source CDM DescriptionElement into  the target.
 *
 * @param $target
 *     The source CDM DescriptionElement
 * @param $source
 *     The target CDM DescriptionElement
 */
function cdm_merge_description_elements(&$target, &$source){
  static $fields_to_merge = array('annotations', 'markers', 'sources', 'media');

  foreach ($fields_to_merge as $field){
    if(is_array($source->$field)) {
      if(!is_array($target->$field)){
        $target->$field = $source->$field;
      } else {
        $target->$field = array_merge($target->$field, $source->$field);
      }
    }
  }
}

/**
 * Prepares the items for a table of content list.
 *
 * see also hook_cdm_feature_node_toc_items_alter()
 *
 * @param $feature_nodes
 *   An array of CDM FeatureNode instances
 *
 * @return array
 *   The items array is an array suitable for theme_item_list().
 */
function cdm_feature_node_toc_items($feature_nodes) {
  $items = array();

  // we better cache here since drupal_get_query_parameters has no internal static cache variable
  $http_request_params = drupal_get_query_parameters();

  foreach ($feature_nodes as $node) {

    if (isset($node->descriptionElements['#type']) || hasFeatureNodeDescriptionElements($node)) {

      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
      //TODO HACK to implement images for taxa, should be removed.
      if ($node->feature->uuid != UUID_IMAGE) {
        $items[] = array(
          'data' => l(
                theme('cdm_feature_name', array('feature_name' => $featureRepresentation)),
                $_GET['q'],
                array(
                    'attributes' => array('class' => array('toc')),
                    'fragment' => generalizeString($featureRepresentation),
                    'query' => $http_request_params
                )
            ),
          'class' => array('feature-toc-item-' . $node->feature->uuid),
        );
      }
    }
  }

  drupal_alter('cdm_feature_node_toc_items', $items);

  return $items;
}

/**
 * Creates the footnotes for the given CDM DescriptionElement instance.
 *
 * Footnotes are created for annotatins and original sources.
 *
 * @param $descriptionElement
 *     A CDM DescriptionElement instance
 * @param $separator
 *     Optional parameter. The separator string to concatenate the footnode ids, default is ','
 * @return the string of foot note keys
 */
function cdm_create_description_element_footnotes($description_element, $separator = ','){

  // Annotations as footnotes.
  $footNoteKeys = cdm_annotations_as_footnotekeys($description_element);
  // Source references as footnotes.
  foreach ($description_element->sources as $source) {
    if (_is_original_source_type($source)) {
      $fn_key = FootnoteManager::addNewFootnote(RenderHints::getFootnoteListKey(), theme('cdm_OriginalSource', array(
          'source' => $source,
          'doLink' => FALSE,
      )));
      // Ensure uniqueness of the footnote keys.
      cdm_add_footnote_to_array($footNoteKeys, $fn_key);
    }
  }
  // Sort and render footnote keys.
  $footnoteKeyListStr = '';
  asort($footNoteKeys);
  foreach ($footNoteKeys as $footNoteKey) {
    $footnoteKeyListStr .= theme('cdm_footnote_key', array('footnoteKey' => $footNoteKey, 'separator' => ($footnoteKeyListStr ? $separator : '')));
  }
  return $footnoteKeyListStr;
}

