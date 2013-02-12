<?php
/**
 * @file
 * Page functions.
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
 * Creates a render array representing the ocurrences associetad with the $taxon.
 *
 * The resulting render array contains two elements:
 *  - 'map': A map showing all point locations of the occurences is availabale
 *  - 'specimen_list': the list of occurences prepated as table for theme_table()
 *
 * @param object $taxon
 *   A cdm Taxon object
 * @return
 *   A render array suitable for drupal_render()
 *
 */
function cdm_dataportal_taxon_page_specimens($taxon) {

  $render_array = array();
  RenderHints::pushToRenderStack('taxon_page_specimens');

  $relationship_choice = variable_get(CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS, unserialize(CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS_DEFAULT));
  $relationship_choice['direct'] = get_selection($relationship_choice['direct']);
  $relationship_choice['invers'] = get_selection($relationship_choice['invers']);

  $by_associatedtaxon_query = http_build_query(array(
      'relationshipsInvers' => implode(',', $relationship_choice['invers']),
      'relationships' => implode(',', $relationship_choice['direct']),
      'pageSize' => null // all hits in one page
      )
    );

  $pager = cdm_ws_get(CDM_WS_OCCURRENCE_BY_ASSOCIATEDTAXON,
      null,
      $by_associatedtaxon_query . '&taxonUuid=' . $taxon->uuid
  );

  $specimensOrObservations = array();
  if(isset($pager->records[0])){
    $specimensOrObservations =  $pager->records;
  }

  // order occurrences by date but types should be on top of the list
  $type_specimens = array();
  $other_occurrences = array();
  foreach ($specimensOrObservations as &$occurrence) {
    $typeDesignationsPager = cdm_ws_get(CDM_WS_OCCURRENCE . '/$0/specimenTypeDesignations', $occurrence->uuid);
    if (isset($typeDesignationsPager->count) && $typeDesignationsPager->count > 0) {
      $type_specimens[] = $occurrence;
    } else {
      $other_occurrences[] = $occurrence;
    }
  }
  $specimensOrObservations = array_merge($type_specimens, $other_occurrences);

  // Collect media (fieldObjectMedia, derivedUnitMedia) and add as a custom field
  // $occurrence->_fieldObjectMedia
  foreach ($specimensOrObservations as &$occurrence) {
    $occurrence->_fieldObjectMedia = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, array(
        $occurrence->uuid,
        'fieldObjectMedia',
    ));
    $occurrence->_derivedUnitMedia = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, array(
        $occurrence->uuid,
        'derivedUnitMedia',
    ));
  }

  // --- get map service HTTP query paramaters
  if (count($specimensOrObservations) > 0) {
    $occurrenceQuery = cdm_ws_get(CDM_WS_GEOSERVICE_OCCURRENCEMAP, $taxon->uuid,  $by_associatedtaxon_query);

    if( isset($occurrenceQuery->String) ) {
      $occurrenceQuery = $occurrenceQuery->String;

      $legendFormatQueryStr = null;
      if (variable_get('cdm_dataportal_map_openlayers', 1)) {
        $map_html = get_openlayers_map(variable_get('cdm_dataportal_geoservice_display_width', 680), variable_get('cdm_dataportal_geoservice_bounding_box', FALSE), $occurrenceQuery, NULL,
            $legendFormatQueryStr, variable_get('cdm_dataportal_geoservice_map_caption', ''));
      }
      else {
        // get_image_map($width, $occurrenceQuery = FALSE, $distributionQuery = FALSE, $legendFormatQuery = FALSE, $map_caption = FALSE )
        $map_html = get_image_map(variable_get('cdm_dataportal_geoservice_display_width', 680), variable_get('cdm_dataportal_geoservice_bounding_box', FALSE), $occurrenceQuery, NULL,
            $legendFormatQueryStr, variable_get('cdm_dataportal_geoservice_map_caption', ''));
      }
      $render_array['map'] = markup_to_render_array($map_html, 0);
    }
  }

  // --- generate the specimen list as table
  $specimen_table = array(
      '#theme' => 'table',
      '#weight' => 2,
      // prefix attributes and rows with '#' to let it pass to the theme function,
      // otherwise it is handled as child render array
      '#attributes' => array('class' => 'specimens'),
      '#rows' => array(),
  );

  if ($specimensOrObservations) {

    foreach ($specimensOrObservations as $specimenOrObservation) {

      $mediaList = array();
      if (is_array($specimenOrObservation->_fieldObjectMedia)) {
        $mediaList = array_merge($mediaList, $specimenOrObservation->_fieldObjectMedia);
      }
      if (is_array($specimenOrObservation->_derivedUnitMedia)) {
        $mediaList = array_merge($mediaList, $specimenOrObservation->_derivedUnitMedia);
      }

      // typelabel will contain the typeStatus
      $type_label = '';
      $typeDesignationPager = cdm_ws_get(CDM_WS_OCCURRENCE . '/$0/specimenTypeDesignations', $specimenOrObservation->uuid);
      if (isset($typeDesignationPager)) {
        $type_status = array();
        foreach ($typeDesignationPager->records as $typeDesignation) {
          if (isset($typeDesignation->typeStatus->representation_L10n)){
            $type_status[] = $typeDesignation->typeStatus->representation_L10n;
          }
        }
        $type_label = implode(', ', $type_status);
        if($type_label){
          $type_label .= ': ' ;
        }
      }

      // --- Specimen entry as dynamic label:
      //     -> Dynabox for the specimenOrObservation
      $gallery_name = $specimenOrObservation->uuid;

      $derived_unit_ws_request = cdm_compose_url(CDM_WS_OCCURRENCE, array( $specimenOrObservation->uuid));
      $label_html = cdm_dynabox(
          $specimenOrObservation->uuid,
          $type_label . $specimenOrObservation->titleCache,
          $derived_unit_ws_request,
          'cdm_specimenOrObservation', // the theme or compose function to use
          'Click for details',
          array('div','div'),
          'function(){ jQuery(\'#media_gallery_' . $gallery_name . '\').hide(); }', // open_callback
          'function(){ jQuery(\'#media_gallery_' . $gallery_name . '\').show(); }' // close_callback
      );

      // --- Render associated media.
      $gallery_html = '';
      if (count($mediaList) > 0) {
        $gallery_settings = getGallerySettings(CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME);
        $captionElements = array(
            '#uri' => t('open media'),
        );

        $gallery_html = theme('cdm_media_gallerie', array(
            'mediaList' => $mediaList,
            'galleryName' => $gallery_name,
            'maxExtend' => $gallery_settings['cdm_dataportal_media_maxextend'],
            'cols' => $gallery_settings['cdm_dataportal_media_cols'],
            'maxRows' => isset($gallery_settings['cdm_dataportal_media_maxRows']) ? isset($gallery_settings['cdm_dataportal_media_maxRows']) : null,
            'captionElements' => $captionElements,
            'mediaLinkType' => 'LIGHTBOX',
            'alternativeMediaUri' => NULL,
            'galleryLinkUri' => NULL,
        ));
      }

      $specimen_table['#rows'][] = array(
          // An array of table rows. Every row is an array of cells, or an associative array
          'data' => array(
              // Each cell can be either a string or an associative array
              $label_html . $gallery_html
          ),
          'class' =>  array(
              'descriptionElement',
              'descriptionElement_IndividualsAssociation'
          ),
      );
    }
  }

  $render_array['specimen_list'] = $specimen_table;

  RenderHints::popFromRenderStack();
  return $render_array;
}
