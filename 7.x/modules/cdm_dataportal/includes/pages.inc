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

  $by_associatedtaxon_query_parameters = array(
      'relationshipsInvers' => implode(',', $relationship_choice['invers']),
      'relationships' => implode(',', $relationship_choice['direct']),
  );

  if (isset($_REQUEST['pager']) && is_array($_REQUEST['pager'])) {
    $by_associatedtaxon_query_parameters = array_merge($by_associatedtaxon_query_parameters, $_REQUEST['pager']);
  }

  $by_associatedtaxon_query = http_build_query($by_associatedtaxon_query_parameters);

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
    $occurrence_query = cdm_ws_get(CDM_WS_GEOSERVICE_OCCURRENCEMAP, $taxon->uuid,  $by_associatedtaxon_query);

    if( isset($occurrence_query->String) ) {
      $occurrence_query = $occurrence_query->String;
      $legend_format_query = null;
      $distribution_query = NULL;

      $render_array['map'] = compose_map($occurrence_query, $distribution_query, $legend_format_query);
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
  $render_array['pager'] = markup_to_render_array(
      theme('cdm_pager', array(
          'pager' => $pager,
          'path' => $_REQUEST['q'],
          'parameters' => $_REQUEST,
      )),
      10 // weight
  );

  RenderHints::popFromRenderStack();
  return $render_array;
}


/**
 * TODO should this function really be a compose funtion?
 *     For a compose function must there alway be a theme function with the same name? (ak 8.8.2013)
 *
 * composes and returns an render array containing the components of the taxon profile tab:
 *  - 'taxon_profile_image'
 *  - 'taxon_description_feature_toc'
 *  - 'taxon_description_features'
 *
 *
 * @param array $variables
 *   An associative array containing:
 *   - taxon
 *   - mergedTrees
 *   - media
 *
 * @return array
 *   A Drupal reder array with the following elements:
 *     - 'taxon_profile_image'
 *     - 'taxon_description_feature_toc'
 *     - 'taxon_description_features'
 *
 * @ingroup compose
 */
function compose_cdm_taxon_page_profile($taxon, $merged_tree, $media) {

  $render_array = array();

  $taxon_profile_image_settings = variable_get(CDM_TAXON_PROFILE_IMAGE, unserialize(CDM_TAXON_PROFILE_IMAGE_DEFAULT));

  $hide_taxon_profile_image = FALSE;
  if (variable_get('image_hide_rank', '0') != '0' && isset($taxon->name->rank->uuid)) {
    $rankCompare = rank_compare($taxon->name->rank->uuid, variable_get('image_hide_rank', '-99'));
    $hide_taxon_profile_image = ($rankCompare > -1);
  }

  if ($taxon_profile_image_settings['show'] && !$hide_taxon_profile_image) {

    $representationPart = new stdClass();
    $attributes = array();
    if (isset($media[0]->representations[0]->parts[0])) {
      $representationPart = $media[0]->representations[0]->parts[0];
      $attributes['alt'] = $representationPart->uri;

      if(!empty($taxon_profile_image_settings['media_uri_query'])){
        $representationPart->uri = $representationPart->uri
        . (strpos($representationPart->uri, '?') !== FALSE ? '&' : '?')
        . $taxon_profile_image_settings['media_uri_query'];
      }
    }
    else {
      // show placeholder image instead
      if(!empty($taxon_profile_image_settings['custom_placeholder_image_on']) && !empty($taxon_profile_image_settings['custom_placeholder_image_fid'])){
        // use the user provided image
        $profile_image_file = file_load($taxon_profile_image_settings['custom_placeholder_image_fid']);
        $url = file_create_url($profile_image_file->uri);
        $image_info = image_get_info($profile_image_file->uri);
        $representationPart->width = $image_info['width'];
        $representationPart->height = $image_info['height'];
        $representationPart->uri = $url;
      } else {
        // use the hard coded default
        $representationPart->width = 184;
        $representationPart->height = 144;
        $representationPart->uri = base_path() .  drupal_get_path('module', 'cdm_dataportal') . '/images/no_picture.png';
      }
      $attributes['alt'] = "no image available";
    }

    $profile_image = theme('cdm_media_gallerie_image', array(
        'mediaRepresentationPart' => $representationPart,
        'maxExtend' => $taxon_profile_image_settings['maxextend'],
        'addPassePartout' => FALSE,
        'attributes' => $attributes,
    ));
    // NOTE: style="width:${maxextend}px' is needed for IE8 !!!
    $render_array['taxon_profile_image'] = markup_to_render_array('<div id="taxonProfileImage" style="width:' . $taxon_profile_image_settings['maxextend'] . 'px">' . $profile_image. '</div>', -101);
  }

  $toc = array(
      '#theme' => 'item_list',
      '#items' => cdm_feature_node_toc_items($merged_tree->root->children),
      '#title' => t('Content'),
      '#weight' => -100,
      '#suffix' => '</div>',
      '#prefix'=> '<div id="featureTOC">' // reusing id featureTOC even if this is semantically not correct
  );
  $render_array['taxon_description_feature_toc'] = $toc; //markup_to_render_array($toc_html);

  // Description.
  $render_array['taxon_description_features'] = markup_to_render_array(
      theme(
          'cdm_feature_nodes',
          array('mergedFeatureNodes' => $merged_tree->root->children, 'taxon' => $taxon)
      )
  );

  return $render_array;
}


/**
 * Manages the tab to be hidden in the taxon page.
 *
 * The tabs are identified by their last menu link path element:
 *  - description
 *  - synonymy
 *  - images
 *  - specimens
 *  - key
 *
 * Internally the tabs are stored in a static variable which is
 * managed by drupal_static().
 *
 * @param string $add_tab
 *   Optinal parameter. The given string will be added to the array of tabs
 *
 * @return
 *   The array of tabs
 */
function taxon_page_tabs_hidden($add_tab = NULL) {
  $tabs = &drupal_static(__FUNCTION__);

  if(!isset($tabs)){
    $tabs = array();
  }

  if (isset($add_tab) && !array_key_exists($add_tab, $tabs)) {
    $tabs[] = $add_tab;
  }

  return $tabs;
}

/**
 * Implements the hook_preprocess_HOOK() for theme_menu_local_tasks()
 *
 *  - Removes the tabs to be hidden, @see taxon_page_tabs_hidden()
 *  - Renames tabs according to the settings // TODO (this will replace the theme_cdm_taxonpage_tab() function !!!)
 *
 * @param array $variables
 *   The variables array
 */
function cdm_dataportal_preprocess_menu_local_tasks(&$variables) {

  $hidden_tabs = taxon_page_tabs_hidden();

  if (is_array($variables['primary'])) {
    foreach ($variables['primary'] as $key => &$element) {

      // 1. Remove the tabs to be hidden
      foreach ($hidden_tabs as $tab) {
        if ($element['#link']['path'] == 'cdm_dataportal/taxon/%/' . $tab) {
          // remove the tab
          unset($variables['primary'][$key]);
        }
      }
    }
  }
}



/**
 * Implements the hook_preprocess_HOOK() for theme_menu_local_task()
 *
 *
 * @param array $variables
 *   An associative array containing:
 *     - element: A render element containing:
 *          #link: A menu link array with 'title', 'href', and 'localized_options' keys.
 *          #active: A boolean indicating whether the local task is active.
 *
 */
function cdm_dataportal_preprocess_menu_local_task(&$variables) {

  $link = $variables['element']['#link'];
  if (preg_match('/cdm_dataportal\/.*\/refresh$/', $link['href'])) {
    $link['title'] = '<img class="refresh" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/view-refresh.png' . '" alt="' . check_plain($link['title']) . '" title="' . check_plain($link['title']) . '"/>';
    $link['localized_options']['html'] = TRUE;

    $variables['element']['#link'] = $link;
  }
}
