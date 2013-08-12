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

    if (hasFeatureNodeDescriptionElements($node)) {

      $featureRepresentation = isset($node->feature->representation_L10n) ? $node->feature->representation_L10n : 'Feature';
      //TODO HACK to implement images for taxa, should be removed.
      if ($node->feature->uuid != UUID_IMAGE) {
        $items[] = array(
            l(
                theme('cdm_feature_name', array('feature_name' => $featureRepresentation)),
                $_GET['q'],
                array(
                    'attributes' => array('class' => array('toc')),
                    'fragment' => generalizeString($featureRepresentation),
                    'query' => $http_request_params
                )
            )
        );
      }
    }
  }

  $items = module_invoke_all('cdm_feature_node_toc_items_alter', $items);

  return $items;
}