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