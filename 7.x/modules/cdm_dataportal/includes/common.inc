<?php
/**
 * @file
 * Functions for dealing with CDM entities from the package model.common
 *
 * @copyright
 *   (C) 2007-2012 EDIT
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
 * Compose an render array from a CDM Marker object.
 *
 * compose_hook() implementation
 *
 * @param object $marker
 *   CDM instance of type Marker
 * @return array
 *   A drupal render array
 */
function compose_cdm_marker($marker) {

  $render_array = array(
      // ---- generic
      //  these entries should be common to all cdm enitiy render arrays
      '#theme' => 'cdm_marker', // TODO   add alternative theme funcitons: 'cdm_marker_' . marker.type.label
      '#attributes' => array('class' => html_class_atttibute_ref($marker)),

      // ---- individual
      '#label' => $marker->markerType->representation_L10n . ': ' . (($marker->flag !== TRUE ? t('yes') : t('no'))),
  );

  return $render_array;
}

/**
 * Checks if the given $cdm_entitiy has a marker the type references by the
 * $marker_type_uuid and returns TRUE if a matching marker has been found.
 *
 * @param CdmEntity $cdm_entitiy
 * @param string $marker_type_uuid
 */
function cdm_entity_has_marker($cdm_entitiy, $marker_type_uuid) {
  if(isset($cdm_entitiy->markers[0]) && !is_uuid($marker_type_uuid)){
    foreach ($cdm_entitiy->markers as $marker) {
      if(isset($marker->markerType) && $marker->markerType->uuid == $marker_type_uuid){
        return TRUE;
      }
    }
  }
  return FALSE;
}