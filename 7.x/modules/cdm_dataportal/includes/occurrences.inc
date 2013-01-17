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

/*
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
*/
  return $render_array;
}

/**
 * Compose an render array from a CDM DerivedUnitFacade object.
 *
 * compose_hook() implementation
 *
 * @param object $specimenOrObservation
 *   the CDM instance of type SpecimenOrObservation to compose
 *   the render array for
 * @param array $derivatives
 *   the render array which contains the compositions of the derivatives
 *   of the supplied $specimenOrObservation
 *
 * @return array
 *   the supplied render array $derivatives to which the composition of the supplied
 *   $specimenOrObservation has been added to
 */
function compose_cdm_specimenOrObservation($specimenOrObservation, &$derivatives = null) {

  static $excludeFields = array(
      'derivationEvents',
      'titleCache',
      'derivedUnitMedia',
      'created',
      'updated',
      'class',
//       'uuid'
  );

//   if(user_is_logged_in()) {
//     unset($excludeFields['uuid']);
//   }

  if (!isset($derivatives)) {
    $derivatives = array();
  }

  $descriptions = null;
  $derivedFrom = null;

  if (is_object($specimenOrObservation)) {

    // request again for deeper initialization
    $specimenOrObservation = cdm_ws_get("portal/" . CDM_WS_OCCURRENCE, $specimenOrObservation->uuid);

    $type_label = $specimenOrObservation->class;
    // collect typeStatus as label
    if (isset($specimenOrObservation->specimenTypeDesignations)) {
      $type_status = array();
      foreach ($specimenOrObservation->specimenTypeDesignations as $typeDesignation) {
        if (isset($typeDesignation->typeStatus->representation_L10n)){
          $type_status[] = $typeDesignation->typeStatus->representation_L10n;
        }
      }
      $type_label = implode(', ', $type_status);
    }

    $title = $type_label . ': ' . $specimenOrObservation->titleCache;

    $groups = array();
    // --- add initialized fields
    foreach (get_object_vars($specimenOrObservation) as $field => $value) {
      if (!in_array($field, $excludeFields) && ($value && (!is_object($value) || isset($value->class)))) {
        switch ($field) {

          /* ---- java.lang.Object --- */
          case 'class':
            if($value != '' /* FieldObservation' */){
              @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value);
            }
          break;

          /* ---- SpecimenOrObservationBase --- */
          case 'sex':
          case 'lifeStage':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $value->representation_L10n);
            break;

          case 'definition':
            // TODO
            break;

          case 'specimenTypeDesignations':
              @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), theme('cdm_typedesignations', array('typeDesignations' => $value)));
            break;

          case 'determinations':
            $dd_elements = array();
            $glue = ', ';

            foreach  ($value as $determinationEvent){
              $timeperiod_string = NULL;
              if (isset($determinationEvent->timeperiod)) {
                $timeperiod_string = timePeriodToString($determinationEvent->timeperiod);
              }
              $weight = isset($determinationEvent->preferred) && $determinationEvent->preferred == 1 ? '0' : ($timeperiod_string ? $timeperiod_string : '1');
              // check key exists
              while (array_key_exists($weight, $dd_elements)) {
                $weight .= '0';
              }
              $taxon_name = cdm_ws_get(CDM_WS_TAXON . '/$0/name', $determinationEvent->taxon->uuid);
              $taxon_html = theme('cdm_taxonName',
                  array(
                      'taxonName' => $taxon_name,
                      'nameLink' => path_to_taxon($determinationEvent->taxon->uuid),
                  )
              );
              $dd_elements[$weight] = $taxon_html;
              if (isset($determinationEvent->modifier)) {
                $dd_elements[$weight] .= cdm_term_representation($determinationEvent->modifier);
              }
              if ($timeperiod_string) {
                $dd_elements[$weight] .= $glue . $timeperiod_string;
              }
              if (isset($determinationEvent->actor->titleCache)) {
                $dd_elements[$weight] .= $glue . $determinationEvent->actor->titleCache;
              }
              if (isset($determinationEvent->description)) {
                $dd_elements[$weight] .= $glue . $determinationEvent->description;
              }
            }
            ksort($dd_elements);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('determinations'), $dd_elements);
            break;

          case 'descriptions':
            $descriptions = $value;
            $occurrence_featureTree = cdm_get_occurrence_featureTree();
            $dd_elements = array();

            foreach ($value as $description) {
              $description = cdm_ws_get(CDM_WS_PORTAL_DESCRIPTION, $description->uuid);
              $elements_by_feature = _mergeFeatureTreeDescriptions($occurrence_featureTree->root->children, $description->elements);
              $rendered_description = theme(
                 'cdm_feature_nodes',
                 array('mergedFeatureNodes' => $elements_by_feature)
              );
              $description_render_elements = array();
//               $description_render_elements[] = array('#markup' => '<h4>'. $description->titleCache . '</h4>');
              $description_render_elements[] = array('#markup' => $rendered_description);
              $dd_elements[] = $description_render_elements;
            }

            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements);
            break;

          case 'sources':
              $dd_elements = array();
              foreach ($value as $identifiable_source) {
                $dd_elements[] = theme('cdm_OriginalSource', array('source' => $identifiable_source));
              }
              @_description_list_group_add($groups, cdm_occurrence_field_name_label($field), $dd_elements);
              break;


          /* ---- DerivedUnitBase --- */
          case 'derivedFrom':
            $derivedFrom = $value;
            break;

          case 'collection':
            $sub_dl_groups = array();
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('code'), $value->code, 1);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('codeStandard'),  $value->codeStandard, NULL, 2);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('institute'), $value->institute, NULL, 3);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('townOrLocation'), $value->townOrLocation, NULL, 4);
            // TODO "superCollection"
            // TODO may have media

            @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                array(
                    array('#markup' => $value->titleCache),
                    array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                )
            );
            break;

            case 'storedUnder':
              @_description_list_group_add($groups, cdm_occurrence_field_name_label('storedUnder'), theme('cdm_taxonName', $value));
              break;


          /* ---- Specimen --- */
          // TODO preservation
          // TODO exsiccatum


          /* ---- FieldObservation --- */
          case 'gatheringEvent':
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('collector'), $value->actor->titleCache);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('timeperiod'), timePeriodToString($value->timeperiod));
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('description'), $value->description);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('locality'), $value->locality);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('country'), $value->country->representation_L10n);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('collectingMethod'), $value->collectingMethod);
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('absoluteElevation'), $value->absoluteElevation, ' m');
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('absoluteElevationError'), $value->absoluteElevationError, ' m');
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToGround'), $value->distanceToGround, ' m');
            @_description_list_group_add($groups, cdm_occurrence_field_name_label('distanceToWaterSurface'), $value->distanceToWaterSurface, ' m');
            if (isset($value->collectingAreas)) {
              $area_representations = array();
              foreach($value->collectingAreas as $area) {
                $area_representations[] = $area->representation_L10n;
              }
              @_description_list_group_add($groups, cdm_occurrence_field_name_label('collectingAreas'), implode(', ', $area_representations));
            }
            if (isset($value->exactLocation)) {
              $sub_dl_groups = array();
              @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('errorRadius'), $value->exactLocation->errorRadius, ' m', 1);
              @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('longitude'), $value->exactLocation->longitude, '°', 2);
              @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('latitude'), $value->exactLocation->latitude, '°', 3);
              if (isset($value->exactLocation->referenceSystem)) {
                @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('referenceSystem'), $value->exactLocation->referenceSystem->representation_L10n, '', 4);
              }

              @_description_list_group_add($groups, cdm_occurrence_field_name_label('exactLocation'),
                  array(
                      array('#markup' => $value->exactLocation->sexagesimalString),
                      array(
                          '#theme' => 'description_list',
                          '#groups' => $sub_dl_groups
                      ),
                  )
              );
            }
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


    $occurrence_elements = array(
        '#title' => $title,
        '#theme' => 'description_list',
        '#groups' => $groups,
        '#attributes' => array('class' => html_class_atttibute_ref($specimenOrObservation)),
    );

    $derivatives[] = $occurrence_elements;

    // --- recurse into originals
    if (!isset($derivedFrom)) {
      $derivedFrom = cdm_ws_get(
          CDM_WS_OCCURRENCE,
          array($specimenOrObservation->uuid, 'derivedFrom')
        );
    }

    if (isset($derivedFrom)) {
      if (isset($derivedFrom->originals)) {
        $derived_from_label = t('derived');
        if(isset($derivedFrom->type)){
          $derived_from_label = $derivedFrom->representation_L10n;
        }
        $derivatives[] = array(
            '#markup' => '<div>' . $derived_from_label . ' ' . t('from') . ': </div>',
        );
        foreach ($derivedFrom->originals as $original) {
          compose_cdm_specimenOrObservation($original, $derivatives);
        }
      }
    }

  } // END of $specimenOrObservation exists

  return $derivatives;
}
