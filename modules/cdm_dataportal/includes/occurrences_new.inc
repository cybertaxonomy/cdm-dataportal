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
 * Composes the view on specimens and occurrences as derivate tree
 * starting from the field unit including all derivatives.
 *
 * @param array $root_unit_dtos
 *   list of SpecimenOrObservationDTOs
 *
 * @return array
 *   The Drupal render array
 *
 * @ingroup compose
 * @see CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE
 *
 */
function compose_specimen_table_top_down_new(array $root_unit_dtos) {
  // add icons
  $expand_icon = font_awesome_icon_markup(
    'fa-plus-square-o',
    [
      'alt' => 'Show details',
      'class' => ['expand_icon'],
    ]
  );
  $collapse_icon = font_awesome_icon_markup(
    'fa-minus-square-o',
    [
      'alt' => 'Show details',
      'class' => ['collapse_icon'],
    ]
  );

  $derivation_tree = derived_units_tree($root_unit_dtos);

  $render_array = [];
  $render_array['derived-unit-tree'] = $derivation_tree;

  _add_js_derivation_tree('.derived-unit-tree');

  return $render_array;
}

/**
 * Creates the root levels and trees for all subordinate derivatives.
 *
 * See derived_units_sub_tree()
 *
 * @param array $root_unit_dtos
 *     list of SpecimenOrObservationDTOs
 *
 * @return array
 *    An array which can be used in render arrays to be passed to the
 * theme_table() and theme_list().
 */
function derived_units_tree(array $root_unit_dtos) {

  $root_items = [];
  //we need one more item to contain the items of one level (fieldunit, derivate data etc.)
  foreach ($root_unit_dtos as &$sob_dto) {
    $field_unit_dto_render_array = compose_cdm_specimen_or_observation_dto_details_grid($sob_dto);
    $root_item = [
      '#prefix' => '<div class="derived-unit-tree">',
      '#suffix' => '</div>',
      '#type' => 'container',
      '#attributes' => [
        'class' => [
          'derived-unit-item derived-unit-tree-root',
          html_class_attribute_ref($sob_dto),
        ],
      ],
      'div-container' => [
        'root-item-and-sub-tree' => [
          markup_to_render_array(derived_units_tree_node_header($sob_dto)
            . '<div class="unit-content-wrapper">' // allows to apply the borders between .derived-unit-tree-root and .unit-content
            . '<div class="unit-content">' . drupal_render($field_unit_dto_render_array) . '</div>'
            . '</div>'),
        ],
      ],

    ];
    if (isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0) {
      usort($sob_dto->derivatives, 'compare_specimen_or_observation_dtos');
      // children are displayed in a nested list.
      $root_item['div-container']['root-item-and-sub-tree'][] = derived_units_sub_tree($sob_dto->derivatives);
    }
    $root_items[] = $root_item;
  }

  return $root_items;
}

/**
 * @param array $unit_dtos
 *
 * @return array
 */
function derived_units_sub_tree(array $unit_dtos) {

  $list_items = derived_units_as_list_items($unit_dtos);

  $derivation_tree = [
    '#theme' => 'item_list',
    '#type' => 'ul',
    '#attributes' => [
      'class' => CDM_SPECIMEN_LIST_VIEW_MODE_OPTION_DERIVATE_TREE . ' derived-unit-sub-tree',
    ],
    '#items' => $list_items,
  ];
  return $derivation_tree;
}

/**
 * Creates render array items for FieldUnitDTO or DerivedUnitDTO.
 *
 * @param array $root_unit_dtos
 *     list of SpecimenOrObservationDTOs
 *
 * @return array
 *    An array which can be used in render arrays to be passed to the
 * theme_table() and theme_list().
 */
function derived_units_as_list_items(array $root_unit_dtos) {

  $list_items = [];
  //we need one more item to contain the items of one level (fieldunit, derivate data etc.)
  foreach ($root_unit_dtos as &$sob_dto) {
    $item = [];
    $item['class'] = ['derived-unit-item ', html_class_attribute_ref($sob_dto)];
    // data" element of the array is used as the contents of the list item
    $item['data'] = [];
    $units_render_array = compose_cdm_specimen_or_observation_dto_details_grid($sob_dto);
    $item['data'] = derived_units_tree_node_header($sob_dto)
      . '<div class="unit-content">' . drupal_render($units_render_array) . '</div>';
    if (isset($sob_dto->derivatives) && sizeof($sob_dto->derivatives) > 0) {
      usort($sob_dto->derivatives, 'compare_specimen_or_observation_dtos');
      // children are displayed in a nested list.
      $item['children'] = derived_units_as_list_items($sob_dto->derivatives);
    }
    $list_items[] = $item;
  }

  return $list_items;
}

/**
 * @param $sob_dto
 *
 * @return string
 */
function derived_units_tree_node_header($sob_dto) {
  $link =  cdm_internal_link(path_to_specimen($sob_dto->uuid), null);
  return '<div class="unit-header"><div class="unit-label">' . $sob_dto->label . '<span class="page-link">' . $link . '</span></div></div>';
}

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
 * @param bool $compact_mode
 *   Currently unused, but added for compatibility with
 *   compose_cdm_specimen_or_observation($specimen_or_observation,
 *   $isSpecimen_page = false, &$derivatives = null)
 * @param array $derivatives
 *   the render array which contains the compositions of the derivatives
 *   of the supplied $specimenOrObservation
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO
 *
 * @ingroup compose
 */
function compose_cdm_specimen_or_observation_dto_details_grid($sob_dto, $compact_mode = FALSE, &$derivatives = NULL) {
  $render_array = [];
  if (!$sob_dto) {
    return $render_array;
  }
  if ($sob_dto->type == 'FieldUnit') {
    $render_array['field-unit'] = compose_cdm_field_unit_dto_details($sob_dto, $compact_mode);
    if (isset($sob_dto->gatheringEvent)) {
      $render_array['gathering'] = compose_cdm_gathering_dto_details($sob_dto->gatheringEvent, $compact_mode);
    }
  } else {
    if(isset($sob_dto->derivationEvent)){
      $render_array['derivation-event'] = compose_cdm_derivation_event($sob_dto->derivationEvent);
    }
    $render_array['storage'] = compose_cdm_derived_unit_dto_storage_details($sob_dto, $compact_mode);
    $render_array['type-designations'] = compose_cdm_unit_dto_type_designations($sob_dto, $compact_mode);
    if($sob_dto->type == 'DnaSample'){
      // FIXME:
      $render_array['dna-sample'] = compose_cdm_derived_unit_dto_sequences($sob_dto, $compact_mode);
    } else if($sob_dto->type == 'MediaSpecimen' && isset_not_empty($sob_dto->mediaSpecimen)) {
      $render_array['media-specimens'] = cdm_sob_dto_media_table($sob_dto->mediaSpecimen, $sob_dto, 'Media Specimens');
    }
  }
  if(isset_not_empty($sob_dto->determinedNames)){
    $render_array['determinations'] = compose_cdm_sob_dto_determinations($sob_dto, $compact_mode);
  }
  if(isset_not_empty($sob_dto->listOfMedia)){
    $render_array['media'] = cdm_sob_dto_media_table($sob_dto->listOfMedia, $sob_dto, 'Media');;
  }

  return $render_array;
}
/**
 * Data rows with details for a SpecimenOrObservationDTO to be used in a table
 * @param object $sob_dto
 *   the CDM SpecimenOrObservationDTO
 * @param bool $compact_mode
 *   Currently unused,
 *
 * @return array
 *  The data rows
 */
function sob_dto_details_rows($sob_dto, $compact_mode = FALSE) {

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
 * @param bool $compact_mode
 *   Currently unused,
 *
 * @return array
 *  The render array for the DerivationEventDTO
 *
 * @ingroup compose
 */
function compose_cdm_derivation_event($derivation_event_dto, $compact_mode = FALSE){

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
 * @param bool $compact_mode
 *   Currently unused,
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO
 *
 * @ingroup compose
 */
function compose_cdm_field_unit_dto_details($fu_dto, $compact_mode = FALSE) {

  $table_row_data = sob_dto_details_rows($fu_dto, $compact_mode);

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
 * @param bool $compact_mode
 *   Currently unused,
 *
 * @return array
 *  The render array for the DerivedUnitDTO
 *
 * @ingroup compose
 */
function  compose_cdm_derived_unit_dto_storage_details($du_dto, $compact_mode = FALSE) {

  $table_row_data = sob_dto_details_rows($du_dto, $compact_mode);

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
      render_taxon_or_name($taxon_name, path_to_name($taxon_name->uuid)));
  }
  if (isset($du_dto->exsiccatum)) {
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
  if (isset($du_dto->preservationMethod)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('preservationMethod'),
      $du_dto->preservationMethod);
  }
  if (isset_not_empty($du_dto->characterData)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      cdm_occurrence_field_name_label('characterData'),
      icon_link(path_to_specimen($du_dto->uuid), '', FALSE), false);
  }

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
 * @param bool $compact_mode
 *   Currently unused,
 *
 * @return array
 *  The render array for the GatheringDTO
 *
 * @ingroup compose
 */
function compose_cdm_gathering_dto_details($gathering_dto, $compact_mode = FALSE) {

  $table_row_data = [];
  if (isset_not_empty($gathering_dto->date)) {
    $table_row_data[] = cdm_sob_dto_table_row(cdm_occurrence_field_name_label('date'),
      partialToDate($gathering_dto->date));
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
 * @param bool $compact_mode
 *   Currently unused,
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO.determinedNames
 *
 * @ingroup compose
 */
function compose_cdm_sob_dto_determinations($sob_dto, $compact_mode = FALSE) {

  $table_row_data = [];

  foreach ($sob_dto->determinedNames as $name) {
    $taxon_name = cdm_ws_get(CDM_WS_PORTAL_NAME, $name->uuid);
    $table_row_data[] = cdm_sob_dto_table_row(NULL, render_taxon_or_name($taxon_name, url(path_to_name($taxon_name->uuid))));
  }

  $label = count($table_row_data) > 1 ? t("Identifications") : t("Identification");
  return cdm_sob_dto_table($label, $table_row_data, $sob_dto, 1);
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
 * @param bool $compact_mode
 *   Currently unused,
 *
 * @return array
 *  The render array for the SpecimenOrObservationDTO.determinedNames
 *
 * @ingroup compose
 */
function compose_cdm_unit_dto_type_designations($unit_dto, $compact_mode = FALSE) {

  $table_row_data = [];

  if (isset_not_empty($unit_dto->specimenTypeDesignations)) {
    $table_row_data[] = cdm_sob_dto_table_row(
      NULL,
      render_specimen_typedesignation_dto($unit_dto->specimenTypeDesignations));
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
  $gallery_markup = compose_cdm_media_gallerie(array(
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
 * @param bool $compact_mode
 *   Currently unused,
 *
 * @return array
 *  The render array for the DerivedUnitDTO.sequences.
 *
 * @ingroup compose
 *
 * TODO see  #3347 (services and REST service controller for molecular classes implemented)
 */
function compose_cdm_derived_unit_dto_sequences($unit_dto, $compact_mode = FALSE) {

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
