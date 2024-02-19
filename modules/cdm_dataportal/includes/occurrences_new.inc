<?php
/**
 * @file
 * Functions for dealing with CDM entities of type SpecimenOrOccurrences
 * This file contains new functions which are to replace functions in
 *   occurrences.inc and should be merged with the latter one.
 *
 * @see http://www.mozilla.org/MPL/MPL-1.1.html
 *
 * @copyright
 *   (C) 2007-2021 EDIT
 *   European Distributed Institute of Taxonomy
 *   http://www.e-taxonomy.eu
 *
 *   The contents of this module are subject to the Mozilla
 *   Public License Version 1.1.
 * @author
 *   - Andreas Kohlbecker <a.kohlbecker@BGBM.org>
 */



/**
 * Compose grid of details tabled for a CDM SpecimenOrObservationDTO
 *
 * The resulting render array will contain separate DOM block level elements
 * with header line for the various kind of information like, location,
 * gathering, specimen/observation, DNA, etc.
 *
 *
 * Subordinate derivatives will not be included. For showing the derivation
 * hierarchy see methods like derived_units_sub_tree()
 *
 * compose_hook() implementation
 *
 * @param object $sob_dto
 *   the CDM FieldUnitDTO or DerivedUnitDTO to compose
 *   the render array for.
 * @param bool $full_details
 *  Activates the display of additional data like: annotations, markers, ...
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO
 *
 * @ingroup compose
 */
function compose_cdm_specimen_or_observation_dto_details_grid($sob_dto, $full_details = FALSE) {
  $render_array = [];
  if (!$sob_dto) {
    return $render_array;
  }
  if ($sob_dto->type == 'FieldUnit') {
    $render_array['field-unit'] = compose_cdm_field_unit_dto_details($sob_dto, $full_details);
    if (isset($sob_dto->gatheringEvent)) {
      $render_array['gathering'] = compose_cdm_gathering_dto_details($sob_dto->gatheringEvent, $full_details);
    }
  } else {
    if(isset($sob_dto->derivationEvent)){
      $render_array['derivation-event'] = compose_cdm_derivation_event($sob_dto->derivationEvent);
    }
    $render_array['storage'] = compose_cdm_derived_unit_dto_storage_details($sob_dto, $full_details);
    $render_array['type-designations'] = compose_cdm_unit_dto_type_designations($sob_dto, $full_details);
    if($sob_dto->type == 'DnaSample'){
      // FIXME:
      $render_array['dna-sample'] = compose_cdm_derived_unit_dto_sequences($sob_dto, $full_details);
    } else if($sob_dto->type == 'MediaSpecimen' && isset_not_empty($sob_dto->mediaSpecimen)) {
      $render_array['media-specimens'] = cdm_sob_dto_media_table($sob_dto->mediaSpecimen, $sob_dto, 'Media Specimens');
    } else if ($sob_dto->characterData){
        $render_array['character-data'] = cdm_sob_dto_character_data_table($sob_dto->characterData, $sob_dto, 'Character Data');
    }
  }
  if(isset_not_empty($sob_dto->determinations)){
    $render_array['determinations'] = compose_cdm_sob_dto_determinations($sob_dto, $full_details);
  }
  if(isset_not_empty($sob_dto->listOfMedia)){
    $render_array['media'] = cdm_sob_dto_media_table($sob_dto->listOfMedia, $sob_dto, 'Media');;
  }

  if($full_details){
    // Annotations and Sources
    RenderHints::setAnnotationsAndSourceConfig([
      'sources_as_content' => TRUE,
      'annotations_as_content' => TRUE,
      'link_to_name_used_in_source' => TRUE,
      'link_to_reference' => FALSE,
      'add_footnote_keys' => FALSE,
      'bibliography_aware' => FALSE
    ]);
    $annotations_and_sources = handle_annotations_and_sources($sob_dto);
    if ($annotations_and_sources->hasSourceReferences()) {
      $render_array['sources'] = compose_cdm_sob_dto_sources($sob_dto, $annotations_and_sources);
    }

    if(count($annotations_and_sources->getAnnotations()) > 0){
      $render_array['annotations'] = compose_cdm_sob_dto_annotations($sob_dto, $annotations_and_sources);
    }
      // Markers
    $markers = cdm_ws_get(CDM_WS_PORTAL_OCCURRENCE, [$sob_dto->uuid, 'markers']);
    if(isset_not_empty($markers->records)){
      $render_array['markers'] = compose_cdm_sob_dto_markers($sob_dto, $markers);
    }
  }

  return $render_array;
}
/**
 * Data rows with details for a SpecimenOrObservationDTO to be used in a table
 * @param object $sob_dto
 *   the CDM SpecimenOrObservationDTO
 * @param bool $full_details
 *   Currently unused,
 *
 * @return array
 *  The data rows
 */
function sob_dto_details_rows($sob_dto, $full_details = FALSE) {

  $table_row_data = [];

  if (isset_not_empty($sob_dto->recordBase)) {
    $label = cdm_term_representation($sob_dto->recordBase);
    if($label == 'Dna Sample'){
      $label == 'DNA Sample';
    }
    $table_row_data[0] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('recordBase'),
      $label);
  }
  if (isset_not_empty($sob_dto->kindOfUnit)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('kindOfUnit'),
      cdm_term_representation($sob_dto->kindOfUnit));
  }
  if (isset_not_empty($sob_dto->preferredStableUri)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('preferredStableUri'),
      cdm_external_uri($sob_dto->preferredStableUri, false));
  }
  if (isset_not_empty($sob_dto->sex)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('sex'),
      cdm_term_representation($sob_dto->sex));
  }
  if (isset_not_empty($sob_dto->lifeStage)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('lifeStage'),
      cdm_term_representation($sob_dto->lifeStage));
  }

  return $table_row_data;
}

/**
 * Compose a table of details for a DerivationEventDTO.
 *
 * compose_hook() implementation
 *
 * @param object $derivation_event_dto
 *   the CDM DerivationEventDTO
 * @param bool $full_details
 *   Currently unused,
 *
 * @return array
 *  The render array for the DerivationEventDTO
 *
 * @ingroup compose
 */
function compose_cdm_derivation_event($derivation_event_dto, $full_details = FALSE){

  $details_array = [];
  // $markup = t("Derivation") . ': ';
  $markup = ucfirst(cdm_term_representation($derivation_event_dto->eventType));
  if(isset_not_empty($derivation_event_dto->actor)){
    $details_array[] = $derivation_event_dto->actor;
  }
  if(isset_not_empty($derivation_event_dto->institute)){
    $details_array[] = $derivation_event_dto->institute;
  }
  if(isset_not_empty($derivation_event_dto->timePeriod)){
    $details_array[] = timePeriodToString($derivation_event_dto->timePeriod);
  }
  $details_markup = join(', ', $details_array);
  if($details_markup){
    $markup .= ': ' . $details_markup;
  }
  $render_array = [
    '#type' => 'container',
    '#attributes' => [
      'class' => [
        'derivation-event',
        html_class_attribute_ref($derivation_event_dto)
      ],
      'style' => [
        'grid-column-start: span 2'
      ]
    ],
    'content' => markup_to_render_array($markup)
  ];
  return $render_array;
}

/**
 * Compose a table of details for a FieldUnitDTO.
 *
 * The resulting DOM block level element will have a header line and details.
 *
 * compose_hook() implementation
 *
 * @param object $fu_dto
 *   the CDM FieldUnitDTO
 * @param bool $full_details
 *   Currently unused,
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO
 *
 * @ingroup compose
 */
function compose_cdm_field_unit_dto_details($fu_dto, $full_details = FALSE) {

  $table_row_data = sob_dto_details_rows($fu_dto, $full_details);

  if (isset_not_empty($fu_dto->individualCount)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('individualCount'), $fu_dto->individualCount);
  }
  if (isset_not_empty($fu_dto->definition)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('definition'), $fu_dto->definition);
  }
  if (isset_not_empty($fu_dto->fieldNumber)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('fieldNumber'), $fu_dto->fieldNumber);
  }
  if (isset_not_empty($fu_dto->primaryCollector)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('primaryCollector'), $fu_dto->primaryCollector);
  }
  if (isset_not_empty($fu_dto->fieldNotes)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('fieldNotes'), $fu_dto->fieldNotes);
  }

  $heading = $table_row_data[0]['data'][1];
  unset($table_row_data[0]);
  return cdm_sob_dto_table(t($heading), $table_row_data, $fu_dto, 2);
}

/**
 * Compose details table for a DerivedUnitDTO.
 *
 * The resulting DOM block level element will have a header line and details.
 *
 * compose_hook() implementation
 *
 * @param object $du_dto
 *   the CDM DerivedUnitDTO
 * @param bool $full_details
 *   Currently unused,
 *
 * @return array
 *  The render array for the DerivedUnitDTO
 *
 * @ingroup compose
 */
function  compose_cdm_derived_unit_dto_storage_details($du_dto, $full_details = FALSE) {

  $table_row_data = sob_dto_details_rows($du_dto, $full_details);
    //this is already shown in titleCache

    if (isset($du_dto->accessionNumber)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('accessionNumber'),
      $du_dto->accessionNumber);
  }
  if (isset($du_dto->collection)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('collection'),
      render_collection_dto($du_dto->collection));
  }
  if (isset($du_dto->storedUnder)) {
    $taxon_name = cdm_ws_get(CDM_WS_PORTAL_NAME, array($du_dto->storedUnder->uuid));
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('storedUnder'),
      render_taxon_or_name($taxon_name, url(path_to_name($taxon_name->uuid))));
  }
  if (isset_not_empty($du_dto->exsiccatum)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('exsiccatum'),
      $du_dto->exsiccatum);
  }

  if (isset($du_dto->catalogNumber)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('catalogNumber'),
      $du_dto->catalogNumber);
  }

  if (isset($du_dto->barcode)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('barcode'),
      $du_dto->barcode);
  }

  if (isset_not_empty($du_dto->status)) {
    $table_row_data[] = compose_cdm_sob_dto_status(
      $du_dto);
  }
  if (isset_not_empty($du_dto->preservationMethod)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('preservationMethod'),
      $du_dto->preservationMethod);
  }

  /* TODO
  @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('medium'), $value->medium, NULL, 1);
  @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('temperature'), $value->temperature, NULL, 2);
  @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('definedMaterialOrMethod'), $value->definedMaterialOrMethod, NULL, 3);
*/
 // if (isset_not_empty($du_dto->characterData)) {
 // $detail_html = "";
 //   foreach ($du_dto->characterData as $characterStatePair) {
 //       if (isset_not_empty($characterStatePair->key) && isset_not_empty($characterStatePair->value)) {
//          $detail_html .= "<i>" . $characterStatePair->key . "</i>:" . $characterStatePair->value;
//          $detail_html .= "<br>";
//        }
//    }
//    $table_row_data[] = cdm_sob_dto_table_row(
//    cdm_occurrence_field_name_label('characterData'),$detail_html);
      //icon_link(path_to_specimen($du_dto->uuid), '', FALSE), false);
//}

  $heading = $table_row_data[0]['data'][1];
  unset($table_row_data[0]);

  return cdm_sob_dto_table(t($heading), $table_row_data, $du_dto, 1);
}

/**
 * Compose an render array from a CDM GatheringDTO.
 *
 * The resulting DOM block level element will have a header line and details.
 *
 * compose_hook() implementation
 *
 * @param object $gathering_dto
 *   the CDM GatheringDTO object
 * @param bool $full_details
 *   Currently unused,
 *
 * @return array
 *  The render array for the GatheringDTO
 *
 * @ingroup compose
 */
function compose_cdm_gathering_dto_details($gathering_dto, $full_details = FALSE) {

  $table_row_data = [];
  if (isset_not_empty($gathering_dto->date)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('date'),$gathering_dto->date);
      //partialToDate($gathering_dto->date));
  }
  if (isset_not_empty($gathering_dto->collector)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('collector'),
      $gathering_dto->collector);
  }
  if (isset_not_empty($gathering_dto->description)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('description'),
      $gathering_dto->description);
  }
  if (isset_not_empty($gathering_dto->locality)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('locality'),
      $gathering_dto->locality);
  }
  if (isset_not_empty($gathering_dto->country)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('country'),
      $gathering_dto->country);
  }
  if (isset_not_empty($gathering_dto->collectingMethod)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('collectingMethod'),
      $gathering_dto->collectingMethod);
  }
  if (isset($gathering_dto->exactLocation)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('exactLocation'),
      render_point($gathering_dto->exactLocation));
  }
  if (isset($gathering_dto->absoluteElevation)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('absoluteElevation'),
      statistical_values_from_gathering_event($gathering_dto, 'absoluteElevation'));
  }
  if (isset($gathering_dto->distanceToGround) && $gathering_dto->distanceToGround > 0) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('distanceToGround'),
      statistical_values_from_gathering_event($gathering_dto, 'distanceToGround'));
  }
  if (isset($gathering_dto->distanceToWaterSurface) && $gathering_dto->distanceToWaterSurface > 0) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('distanceToWaterSurface'),
      statistical_values_from_gathering_event($gathering_dto, 'distanceToWaterSurface'));
  }
  if (isset_not_empty($gathering_dto->collectingAreas)) {
    $area_representations = [];
    foreach ($gathering_dto->collectingAreas as $area) {
      // $area_representations[] = l($area->representation_L10n, path_to_named_area($area->uuid));
      $area_representations[] = $area;
    }
    if (!empty($area_representations)) {
      $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('collectingAreas'),
        implode(', ', $area_representations)
      );
    }
  }

  return cdm_sob_dto_table(t("Gathering & Location"), $table_row_data, $gathering_dto, 1);
}

/**
 * Compose an render array from the SpecimenOrObservationDTO.determinedNames.
 *
 * The resulting DOM block level element will have a header line and details.
 *
 * compose_hook() implementation
 *
 * @param object $sob_dto
 *   the CDM SpecimenOrObservationDTO
 * @param bool $full_details
 *   Currently unused,
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO.determinedNames
 *
 * @ingroup compose
 */
function compose_cdm_sob_dto_determinations($sob_dto, $full_details = FALSE) {

  $table_row_data = [];

  foreach ($sob_dto->determinations as $det_event) {
    // FIXME use $det_event->determination->taggedText instead of loading the entity
    if($det_event->determination->type == "TaxonName") {
      $link_path = path_to_name($det_event->determination->uuid);
      $taxon_or_name = cdm_ws_get(CDM_WS_PORTAL_NAME, [$det_event->determination->uuid]);
    } else {
      $link_path = path_to_taxon($det_event->determination->uuid, 'general');
      $taxon_or_name = cdm_ws_get(CDM_WS_PORTAL_TAXON, [$det_event->determination->uuid]);
    }
    $time_period_string = NULL;
    if (isset($det_event->timePeriod)) {
      $time_period_string = timePeriodToString($det_event->timePeriod);
    }
    $markup = render_taxon_or_name($taxon_or_name, url($link_path));
    if (isset($det_event->modifier)) {
      $markup .= cdm_term_representation($det_event->modifier);
    }
    if ($time_period_string) {
      $markup .= ', ' . $time_period_string;
    }
    if (isset($det_event->actor)) {
      $markup .= ', ' . $det_event->actor;
    }
    // FXIME not yet supported by the DTO!
    if (isset($det_event->description)) {
      $markup .= ', ' . $det_event->description;
    }
    $row = cdm_sob_dto_table_row(NULL, $markup);
    $row['#weight'] = isset_numerical($det_event->isPreferred) ? '0' : ($time_period_string ? $time_period_string : '1');
    $table_row_data[] = $row;
  }

  $label = count($table_row_data) > 1 ? t("Identifications") : t("Identification");
  return cdm_sob_dto_table($label, $table_row_data, $sob_dto, 1);
}

/**
 * Compose an render array from the SpecimenOrObservationDTO.determinedNames.
 *
 * The resulting DOM block level element will have a header line and details.
 *
 * compose_hook() implementation
 *
 * @param \AnnotationsAndSources $annotations_and_sources
 *   see
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO.determinedNames
 *
 * @ingroup compose
 */
function compose_cdm_sob_dto_sources($sob_dto, $annotations_and_sources) {

  $table_row_data = [];

  foreach ($annotations_and_sources->getSourceReferences() as $source_ref) {
    $table_row_data[] = cdm_sob_dto_table_row(NULL, $source_ref);
  }

  $label = count($table_row_data) > 1 ? t("Sources") : t("Source");
  return cdm_sob_dto_table($label, $table_row_data, $sob_dto, 1);
}

/**
 * @param $sob_dto
 * @param \AnnotationsAndSources $annotations_and_sources
 *
 * @return array|null
 */
function compose_cdm_sob_dto_annotations($sob_dto, $annotations_and_sources) {

  $table_row_data = [];

  foreach ($annotations_and_sources->getAnnotations() as $annotation_text) {
    $table_row_data[] = cdm_sob_dto_table_row(NULL, $annotation_text);
  }

  $label = count($table_row_data) > 1 ? t("Annotations") : t("Annotation");
  return cdm_sob_dto_table($label, $table_row_data, $sob_dto, 1);
}

function compose_cdm_sob_dto_markers($sob_dto, $markers) {

  $table_row_data = [];
  foreach ($markers->records as $marker) {
    $table_row_data[] = cdm_sob_dto_table_row($marker->markerType->representation_L10n, ($marker->flag !== TRUE ? t('yes') : t('no')));
  }

  $label = count($table_row_data) > 1 ? t("Markers") : t("Marker");
  return cdm_sob_dto_table($label, $table_row_data, $sob_dto, 1);
}
/**
 * @param $sob_dto
 *
 * @return array|null
 */
function cdm_sob_dto_character_data_table($sob_dto){
    $table_row_data = [];
    if (!empty($sob_dto) ) {
        foreach ($sob_dto as $characterStatePair){
            if (isset_not_empty($characterStatePair->key) && isset_not_empty($characterStatePair->value)) {
                $table_row_data[] = cdm_sob_dto_table_row($characterStatePair->key, $characterStatePair->value);
            }
        }
        //$detail_html = "";
        //foreach ($sob_dto as $characterStatePair) {
        //    $detail_html .= "<i>" . $characterStatePair->key . "</i>:" . $characterStatePair->value;
        //    $detail_html .= "; ";
        //}

        //$row = cdm_sob_dto_table_row(NULL, $detail_html);
        //$row['#weight'] = '0' ;

        //$table_row_data[0] = $row;
    }


    return cdm_sob_dto_table('Character Data', $table_row_data, $sob_dto, 1);
}

function compose_cdm_sob_dto_status($sob_dto) {

  $status_string ="";
  if (isset_not_empty($sob_dto->status))
  foreach ($sob_dto->status as $status) {
    $status_string =  $status_string . $status->label;
  }

  $label =  t("Status");
  return cdm_sob_dto_table_row($label, $status_string);
}

/**
 * Compose an render array from the DerivedUnitDTO.specimenTypeDesignations.
 *
 * The resulting DOM block level element will have a header line and details.
 *
 * compose_hook() implementation
 *
 * @param object $unit_dto
 *   the CDM DerivedUnitDTO
 * @param bool $full_details
 *   Currently unused,
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO.determinedNames
 *
 * @ingroup compose
 */
function compose_cdm_unit_dto_type_designations($unit_dto, $full_details = FALSE) {

  $table_row_data = [];

  if (isset_not_empty($unit_dto->specimenTypeDesignations)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      NULL,
      render_specimen_typedesignation_dto($unit_dto->specimenTypeDesignations, false, 'div', 'div'));
  }

  return cdm_sob_dto_table(t('Type designations'), $table_row_data, $unit_dto, 5, 2);
}

/**
 * @param $listOfMedia
 * @param $sob_dto
 * @param $heading
 *
 * @return array|null
 */
function cdm_sob_dto_media_table(array $listOfMedia, $sob_dto, $heading) {
  $table_row_data = [];
  $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);
  $captionElements = array(
    'title',
    '#uri' => t('open media'),
  );
  $gallery_markup = compose_cdm_media_gallery(array(
    'mediaList' => $listOfMedia,
    'galleryName' => $sob_dto->uuid,
    'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
    'cols' => $gallery_settings['cdm_dataportal_media_cols'],
    'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
    'captionElements' => $captionElements,
    'mediaLinkType' => 'LIGHTBOX',
    'alternativeMediaUri' => NULL,
    'galleryLinkUri' => NULL,
    'showCaption' => true
  ));
  $table_row_data[] = cdm_sob_dto_table_row(NULL, $gallery_markup);
  $grid_col_span = count($listOfMedia) > 1 ? 2 : NULL;
  return cdm_sob_dto_table(t($heading), $table_row_data, $sob_dto, 20, $grid_col_span);
}

/**
 * Compose an render array from the DerivedUnitDTO.sequences.
 *
 * The resulting DOM block level element will have a header line and details.
 *
 * compose_hook() implementation
 *
 * @param object $unit_dto
 *   the CDM DerivedUnitDTO
 * @param bool $full_details
 *   Currently unused,
 *
 * @return array
 *  The render array for the DerivedUnitDTO.sequences.
 *
 * @ingroup compose
 *
 * TODO see  #3347 (services and REST service controller for molecular classes implemented)
 */
function compose_cdm_derived_unit_dto_sequences($unit_dto, $full_details = FALSE) {

  $table_row_data = [];
  if(isset_not_empty($unit_dto->sequences)){
    foreach ($unit_dto->sequences as $sequence) {
      if (isset($sequence->geneticAccessionNumber)) {
        $table_row_data[] = cdm_sob_dto_table_row(
          cdm_occurrence_field_name_label('geneticAccessionNumber'),
          $sequence->geneticAccessionNumber);
      }
      // TODO ....
    }
  }

  /* TODO
case 'dnaQuality':
            $sub_dl_groups = array();

            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('purificationMethod'), $value->purificationMethod, NULL, 1);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('ratioOfAbsorbance260_230'), $value->ratioOfAbsorbance260_230, NULL, 2);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('ratioOfAbsorbance260_280'), $value->ratioOfAbsorbance260_280, NULL, 3);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('concentration'), $value->concentration, NULL, 4);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('concentrationUnit'), $value->concentrationUnit, NULL, 4);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('qualityTerm'), $value->qualityTerm, NULL, 4);
            @_description_list_group_add($sub_dl_groups, cdm_occurrence_field_name_label('qualityCheckDate'), $value->qualityCheckDate, NULL, 4);

            if (is_array($sub_dl_groups) && sizeof($sub_dl_groups)>0) {
              @_description_list_group_add($groups, cdm_occurrence_field_name_label($field),
                array(
                  array('#markup' => $value->titleCache),
                  array('#theme' => 'description_list', '#groups' => $sub_dl_groups)
                )
              );
            }
            break;
*/

  return cdm_sob_dto_table(t('Storage'), $table_row_data, $unit_dto, 1);
}

/**
 * Creates a form array for showing details of SpecimenOrObservationDTO in a
 * tabular form with heading.
 *
 * @param $table_heading
 * @param array $table_row_data
 * @param $sob_dto
 * @param $weight
 *  The weight determining the order of the drupal render element
 * @param $grid_col_span
 *  The number of grid colums the table should
 *
 * @return array|null
 */
function cdm_sob_dto_table($table_heading, array $table_row_data, $sob_dto, $weight = NULL, $grid_col_span = 1) {

  if (count($table_row_data) > 0) {
    $wrapper_style = '';
    if($grid_col_span > 1){
      $wrapper_style = ' style="grid-column-start: span ' . $grid_col_span. ';"';
    }
    $sob_table = [
      '#theme' => 'table',
      '#prefix' => '<div class="table-wrapper"' . $wrapper_style . '>',
      '#suffix' => '</div>',
      '#header' => [
        [
          'data' => $table_heading,
          'colspan' => 2,
        ],
      ],
      "#rows" => $table_row_data,
      "#attributes" => [
        "class" => [
          'details-table',
          'details-table-' . normalize_to_class_attribute($table_heading),
          html_class_attribute_ref($sob_dto)
        ],
      ],
    ];
    if ($weight) {
      $sob_table['#weight'] = $weight;
    }
    return $sob_table;
  }
  return NULL;
}

function cdm_sob_dto_table_row($label, $value) {
  if ($value) {
    if($label) {
      return [
        'data' => [
          [
            'data' => str_replace(':', '', $label),
            'class' => [
              'label',
            ],
          ],
          $value,
        ],
        'no_striping' => TRUE,
      ];
    } else {
      // value spanning two columns
      return [
        'data' => [
          [
            'data' => $value,
            'colspan' => 2,
          ]
        ],
        'no_striping' => TRUE,
      ];
    }
  }
  return NULL;
}



/**
 * Compose an render array from a CDD SpecimenOrObservation entity.
 *
 * compose_hook() implementation
 *
 * @param object $sob_dto
 *   the CDM FieldUnit or DerivedUnit to compose
 *   the render array for.
 *
 * @return array
 *   the supplied render array $derivatives to which the composition of the supplied
 *   $specimenOrObservation has been added to
 *
 * @ingroup compose
 */
function compose_cdm_specimen_or_observation_new($sob_dto)
{

  $render_array = [];
  if (is_object($sob_dto)) {

    RenderHints::pushToRenderStack('specimen_or_observation');
    $footnote_list_key = $sob_dto->type . '-' . $sob_dto->uuid;
    RenderHints::setFootnoteListKey($footnote_list_key);

    $render_array['sob_details_grid'] = [
      '#type' => 'container',
      '#attributes' => [
        'class' => 'derived-unit-details-grid'
      ],
        //'children' => compose_cdm_specimen_or_observation_dto_details_grid($sob_dto, true)
        'children' => compose_cdm_specimen_or_observation_dto_details_grid($sob_dto)
    ];;

    $derivation_tree_roots = cdm_ws_get(CDM_WS_OCCURRENCE_ROOT_UNIT_DTOS, [$sob_dto->uuid]);
    $render_array['derivation_tree_header'] = markup_to_render_array("<h3>Derivation tree</h3>");
    $derivationTreeComposer = new DerivationTreeComposer($derivation_tree_roots);
    $derivationTreeComposer->setWithDetails(false);
    $derivationTreeComposer->setFocusedUnitUuid($sob_dto->uuid);
    $render_array['derivation_tree'] = $derivationTreeComposer->compose();

    /*   TODO Extensions
    // TODO: filter by using visible_extensions_sorted()
    // TODO: treat as top level element, see https://dev.e-taxonomy.eu/redmine/issues/2985#note-23
    $extensions = cdm_ws_fetch_all(CDM_WS_PORTAL_OCCURRENCE . '/'  . $sob_dto->uuid . '/extensions', array($sob_dto->uuid));
    if ($extensions && count($extensions)) {

      $extensions_render_array = compose_extensions($extensions);
      @_description_list_group_add($groups, t('Extensions') . ':',
        $extensions_render_array,
        '', 100);
    }
    */

    $render_array['footnotes'] = markup_to_render_array(render_footnotes($footnote_list_key));


  } // END of $specimenOrObservation exists

  return $render_array;
}

