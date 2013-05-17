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

/**
 * Sorts an array of CDM IdentifiableSource instances by 1. publication date and 2. by the
 * author team.
 *
 * @param unknown $sources
 *    The array of CDM IdentifiableSource instances
 * @param bool $do_theme if set TRUE the sources will be themed by theme_cdm_OriginalSource
 * @return multitype:
 */
function  oder_sources($sources, $do_theme = false){
    $sort_array = array();
    foreach ($sources as $source) {
      $order_key = "0000";
      if(isset($source->citation->datePublished)){
        $order_key = timePeriodAsOrderKey($source->citation->datePublished);
      }
      if(!empty($source->citation->authorTeam->titleCache)){
        $order_key .= $source->citation->authorTeam->titleCache;
      }
      while(array_key_exists($order_key, $sort_array)){
        $order_key .= "_";
      }
      if($do_theme) {
        $sort_array[$order_key] = theme('cdm_OriginalSource', array('source' => $source));
      } else {
        $sort_array[$order_key] = $source;
      }
    }
    ksort($sort_array);
    return array_values ($sort_array);
}