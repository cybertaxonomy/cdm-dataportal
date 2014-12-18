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
 * @defgroup compose Compose functions
 * @{
 * Functions which are composing Drupal render arays
 *
 * The cdm_dataporal module needs to compose rather complex render arrays from
 * the data returned by the CDM REST service. The compose functions are responsible
 * for creating the render arrays.
 *
 * All these functions are also implementations of the compose_hook() which is used
 * in the proxy_content() function.
 * @}
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
 *
 * @ingroup compose
 */
function compose_cdm_marker($marker) {

  $render_array = array(
      // ---- generic
      //  these entries should be common to all cdm enitiy render arrays
      '#theme' => 'cdm_marker', // TODO   add alternative theme funcitons: 'cdm_marker_' . marker.type.label
      '#attributes' => array('class' => html_class_attribute_ref($marker)),

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
 * Sorts an array of CDM IdentifiableSource instances by 1. by the
 * author teams familiy names and 2. by the publication date.
 *
 * @param unknown $sources
 *    The array of CDM IdentifiableSource instances
 * @param bool $do_theme if set TRUE the sources will be themed by theme_cdm_OriginalSource
 * @return multitype:
 */
function  oder_sources($sources, $do_theme = false){
    $sort_array = array();
    foreach ($sources as $source) {

      $order_key = '';

      // find the familynames
      if(isset($source->citation->uuid) && !isset($source->citation->authorship)){
        $authorteam = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $source->citation->uuid);

        $persons = array();
        if($authorteam->class == 'Team'){
          if(isset($authorteam->teamMembers)){
            $persons = $authorteam->teamMembers;
          }
        } else {
          $persons[] = $authorteam;
        }

        foreach($persons as $person){
          if(!empty($person->lastname)){
            $order_key .= $person->lastname;
          } else {
            $order_key .= $person->titleCache;
          }
        }
        if(empty($order_key)){
          $order_key = $authorteam->titleCache;
        }

      }
      $order_key = str_pad($order_key, 50);

      // add publication date to the key
      if(isset($source->citation->datePublished)){
        $order_key .= '_' . timePeriodAsOrderKey($source->citation->datePublished);
      } else {
        $order_key .= '_' . "0000";
      }

      // padd key until unique
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