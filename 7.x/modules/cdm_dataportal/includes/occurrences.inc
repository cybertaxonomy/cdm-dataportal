<?php
/**
 * @file
 * Functions for dealing with CDM entities of type SpeciemenOrOccurrences
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
 * Compose an render array from a CDM DerivedUnitFacade object.
 *
 * compose_hook() implementation
 *
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function compose_cdm_derivedUnitFacade($derivedUnitFacade) {

  static $excludeFields = array(
      'class',
      'titleCache',
      'derivedUnitMedia',
  );

  $groups = array();
  if (is_object($derivedUnitFacade)) {
    foreach (get_object_vars($derivedUnitFacade) as $field => $value) {
      if (!in_array($field, $excludeFields) && ($value && (!is_object($value) || isset($value->class)))) {
        switch ($field) {

          case 'locality':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value->text);
            break;

          case 'country':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value->representation_L10n);
            break;

          case 'collector':
            $sub_dl_groups = array();
            @_description_list_group_add($sub_dl_groups, 'code', $value->code, 1);
            @_description_list_group_add($sub_dl_groups, 'codeStandard',  $value->codeStandard, 2);
            @_description_list_group_add($sub_dl_groups, 'institute', $value->institute, 3);
            @_description_list_group_add($sub_dl_groups, 'townOrLocation', $value->townOrLocation, 4);

            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                array(
                    array('#markup' => $value->titleCache),
                    array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                )
            );
            break;

          case 'collection':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value->titleCache);
            break;

          case 'exactLocation':
            $sub_dl_groups = array();
            @_description_list_group_add($sub_dl_groups, 'errorRadius', $value->errorRadius, 1);
//             @_description_list_group_add($sub_dl_groups, 'longitude', $value->longitude, 2);
//             @_description_list_group_add($sub_dl_groups, 'latitude', $value->latitude, 3);

            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                array(
                    array('#markup' => $value->sexagesimalString),
                    array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                )
            );
            break;

          case 'getGatheringPeriod':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), timePeriodToString($value));
            break;

          default:
            _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value);
        }

      }
    } // END of loop over $derivedUnitFacade fields

    $render_array = array(
        '#theme' => 'description_list',
        '#groups' => $groups,
    );

  }
  return $render_array;
}

/**
 * Compose an render array from a CDM DerivedUnitFacade object.
 *
 *
 *
 * compose_hook() implementation
 *
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function compose_cdm_specimenOrObservation($specimenOrObservation, &$derivatives = array()) {

  static $excludeFields = array(
      'class',
      'derivedFrom',
      'derivationEvents',
      'titleCache',
      'derivedUnitMedia',
      'created',
      'updated',
      'uuid'
  );

  static $init_strategy_json = NULL;
  if (!$init_strategy_json) {
    $init_array = array(
        "derivedFrom.derivatives",
        "derivedFrom.originals",
    );
    $init_strategy_json = json_encode($init_array);
  }

  if (is_object($specimenOrObservation)) {

    // request again for deeper initialization
    $specimenOrObservation = cdm_ws_get("portal/" . CDM_WS_OCCURRENCE, $specimenOrObservation->uuid);

    $title = (count($derivatives) > 0 ? 'derived from ' : '')
      . $specimenOrObservation->class . ': '
      .  $specimenOrObservation->titleCache;

    $groups = array();
    // --- add initialized fields
    foreach (get_object_vars($specimenOrObservation) as $field => $value) {
      if (!in_array($field, $excludeFields) && ($value && (!is_object($value) || isset($value->class)))) {
        switch ($field) {

          case 'locality':
            break;

          case 'country':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value->representation_L10n);
            break;

          case 'collector':
            $sub_dl_groups = array();
            @_description_list_group_add($sub_dl_groups, 'code', $value->code, 1);
            @_description_list_group_add($sub_dl_groups, 'codeStandard',  $value->codeStandard, 2);
            @_description_list_group_add($sub_dl_groups, 'institute', $value->institute, 3);
            @_description_list_group_add($sub_dl_groups, 'townOrLocation', $value->townOrLocation, 4);

            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                array(
                    array('#markup' => $value->titleCache),
                    array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                )
            );
            break;

          case 'determinations':
              $sub_dl_groups = array();
              @_description_list_group_add($sub_dl_groups, 'taxon', $value->taxon->titleCache, 1);

              @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                  array(
                      array('#markup' => $value->titleCache),
                      array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                  )
              );
              break;

          case 'sources':
              $dd_array = array();
//               foreach ($value as $reference) {
//                 $dd_array[] =
//               }
//               @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
//                   array(
//                       array('#markup' => $value->titleCache),
//                       array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
//                   )
//               );
              break;

          case 'collection':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value->titleCache);
            break;

          case 'exactLocation':
            $sub_dl_groups = array();
            @_description_list_group_add($sub_dl_groups, 'errorRadius', $value->errorRadius, 1);
            @_description_list_group_add($sub_dl_groups, 'longitude', $value->longitude, 2);
            @_description_list_group_add($sub_dl_groups, 'latitude', $value->latitude, 3);

            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                array(
                    array('#markup' => $value->sexagesimalString),
                    array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                )
            );
            break;

          case 'getGatheringPeriod':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), timePeriodToString($value));
            break;

          default:
            if(is_object($value) || is_array($value)){
              drupal_set_message("Unhandled type in compose_cdm_specimenOrObservation() for field " . $field, "warning");
            } else {
              _description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value);
            }

        }

      }
    } // END of loop over $derivedUnitFacade fields

//     // --- retrieve and add additional fields

//     switch ($specimenOrObservation->class) {
//       case 'LivingBeing':
//       case 'Observation':
//       case 'DerivedUnit':

//         break;

//      case 'Specimen':
//      case 'Fossil':
//      case 'DnaSample':
//        // TODO
//        // - preservation: PreservationMethod
//        // - PreservationMethod
//         break;
//     }


    $occurrence_elements = array(
        '#title' => $title,
        '#theme' => 'description_list',
        '#groups' => $groups,
    );

    $derivatives[] = $occurrence_elements;

    // --- recurse into originals
    $derivationEvent = cdm_ws_get(
        CDM_WS_OCCURRENCE,
        array($specimenOrObservation->uuid, 'derivedFrom'),
        queryString(array('init' => $init_strategy_json))
      );
    if (isset($derivationEvent->originals)) {
      foreach ($derivationEvent->originals as $original) {
        compose_cdm_specimenOrObservation($original, $derivatives);
      }
    }

  } // END of $specimenOrObservation exists

  return $derivatives;
}
