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
function compose_cdm_specimenOrObservation($specimenOrObservation) {


}
