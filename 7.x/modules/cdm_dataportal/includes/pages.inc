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

    if(variable_get('cdm_dataportal_compressed_specimen_derivate_table')){

        //COMPRESSED SPECIMEN DERIVATE TABLE
        $fieldUnits = cdm_ws_get(CDM_WS_PORTAL_TAXON, array($taxon->uuid, 'associatedFieldUnits'));
        $detail_image_icon = '<img title="Detail Image" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/detail_image_derivate-16x16-32.png' . '"/>';
        $checked_box_icon = '<img src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/step_done.gif' . '"/>';
        $sequence_icon = '<img title="Molecular Data" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/sequence_derivate-16x16-32.png' . '"/>';
        if ($fieldUnits) {
            $rowcount = 0;
            foreach ($fieldUnits as $fieldUnit) {
                //get derivate hierarchy for the FieldUnit
                $derivateHierarchy = cdm_ws_get(CDM_WS_PORTAL_OCCURRENCE, array($fieldUnit->uuid, 'derivateHierarchy'), 'taxonUuid=' . $taxon->uuid);
                if($derivateHierarchy){
                    //summary row
                    $rows[] = array(
                        'data' => array(
                            $derivateHierarchy->country,
                            $derivateHierarchy->date,
                            $derivateHierarchy->collection,
                            $derivateHierarchy->herbarium,
                            array(
                                'data' => $derivateHierarchy->hasType? $checked_box_icon:"",
                                'class' => array('checkBoxCell')
                            ),
                            array(
                                'data' => $derivateHierarchy->hasSpecimenScan? $checked_box_icon:"",
                                'class' => array('checkBoxCell')
                            ),
                            ($derivateHierarchy->hasDna? $sequence_icon :"") . ($derivateHierarchy->hasDetailImage?$detail_image_icon:"")
                        ),
                        'id' => ('derivate_summary' . $rowcount), // summary row id
                        'class' => array('summary_row'),
                    );

                    //assemble detail html
                    $taxonName = $derivateHierarchy->taxonName;
                    $protologue = $derivateHierarchy->protologue;
                    $citation = $derivateHierarchy->citation;
                    $types = "";
                    foreach($derivateHierarchy->types as $typeStatus => $accessionList){
                        $types .= $typeStatus;
                        foreach($accessionList as $accessionNumber){
                            $types .= " (" . $accessionNumber . ") ";
                        }
                    }
                    $specimenScans = createLinks($derivateHierarchy->specimenScans);
                    $molecularData = createLinks($derivateHierarchy->molecularData);
                    $detailImages = createLinks($derivateHierarchy->detailImages);

                    $detail_html = "";
                    if($taxonName){
                        $detail_html .= "<b>Name:</b> " . $taxonName . "<br>";
                    }
                    if($protologue){
                        //$detail_html .= "<b>Protologue:</b> " . $protologue . "<br>"
                    }
                    if($citation){
                        $detail_html .= "<b>Citation:</b> " . $citation . "<br>";
                    }
                    if($types){
                        $detail_html .= "<b>Types:</b> " . $types . "<br>";
                    }
                    if($specimenScans){
                        $detail_html .= "<b>Specimen Scans:</b> " . $specimenScans . "<br>";
                    }
                    if($molecularData){
                        $detail_html .= "<b>Molecular Data:</b> " . $molecularData . "<br>";
                    }
                    if($detailImages){
                        $detail_html .=  "<b>Detail Images:</b> " . $detailImages . "<br>";;
                    }

                    //detail row resp. one BIG detail cell
                    $rows[] = array(
                        'data' => array(
                            array(
                                'data' => $detail_html,
                                'colspan' => 8,
                            ),
                        ),
                        'id' => ('derivate_details' . $rowcount),//details row ID
                        'class' => array('detail_row'),
                    );
                    $rowcount++;
                }
            }
        }

        $tableId = "derivate_hierarchy_table";
        $derivateHierarchyTable = array(
            "#theme" => "table",
            "#weight" => 2,
            "#header" => array("Country", "Date", "Collection", "Herbaria", "Type", "Image", "Derivates"),
            "#rows" => $rows,
            "#attributes" => array(
                "id" => $tableId,
                "border" => 2
            )
        );

        //add toggle functionality to derivate hierarchy table
        drupal_add_js_rowToggle("#".$tableId);

        $render_array['derivate_hierarchy_table'] = $derivateHierarchyTable;
    }
    else{
        //BOTTOM-UP-SPECIMEN-TABLE
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
                    array(),
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
    }
    RenderHints::popFromRenderStack();
    return $render_array;
}

/**
 * Creates HTML links from the given pair list concatenated by a white space.
 * @param $linkTextPairList the list with pairs which hold the uri as "first" and uriText as "second"
 * @return string the assembled HTML string containing the links
 */
function createLinks($linkTextPairList)
{
    $html = "";
    if ($linkTextPairList) {
        foreach ($linkTextPairList as $linkTextPair) {
            $html .= '<a href="' . $linkTextPair->first . '">' . $linkTextPair->second . "</a>";
        }
    }
    return $html;
}
/**
 * Composes a taxon page which can consist of multiple parts like
 * 'General', 'Synonymy', 'Images', 'Keys'. These parts can be displayed
 * as tabs or as sections of a single page.
 *
 * It is headed by the name of the accepted taxon without author and reference.
 *
 * @param $taxon
 *   The CDM Taxon Instance to compose the page for.
 * @param $page_part
 *   Name of the part to display, valid values are:
 *    - 'description' -  for the general part
 *    - 'images'
 *    - 'synonymy'
 *    - 'keys'
 *    - 'all'
 *
 * @return array
 *   A drupal render array
 *
 * @ingroup compose
 */
function compose_cdm_taxon_page($taxon, $page_part = 'description') {

  global $theme;

  // we better cache here since drupal_get_query_parameters has no internal static cache variable
  $http_request_params = drupal_get_query_parameters();

  // add all mandatory js sources
  _add_js_footnotes();


  $render_array = array();
  $weight = 0; // the weight for the render array elements

  $tabsToDisplay = variable_get('cdm_taxonpage_tabs_visibility', unserialize(TAXONPAGE_VISIBILITY_OPTIONS_DEFAULT));

  $page_part = variable_get('cdm_dataportal_taxonpage_tabs', 1) ? $page_part : 'all';

  $media = _load_media_for_taxon($taxon);


  if (!isset($media[0]) || ($tabsToDisplay["Images"] == '0')) {
    taxon_page_tabs_hidden('images');
  }

  // --- GET specimensOrObservations --- //
  $specimensOrObservations = cdm_ws_get(CDM_WS_TAXON, array( $taxon->uuid, 'specimensOrObservations'));

  $specimensOrObservationsCount = is_array($specimensOrObservations) ? count($specimensOrObservations) : 0;
  if ($specimensOrObservationsCount == 0 || ($tabsToDisplay["Specimens"] == '0')) {
    taxon_page_tabs_hidden('specimens');
  }

  // --- GET polytomousKeys --- //
  $polytomousKeysPager = cdm_ws_get(CDM_WS_POLYTOMOUSKEY, NULL, "findByTaxonomicScope=$taxon->uuid");
  $identificationKeyCount = 0;
  if ($polytomousKeysPager) {
    $identificationKeyCount += $polytomousKeysPager->count;
  }
  if ($identificationKeyCount == 0 || ($tabsToDisplay["Keys"] == '0')) {
    taxon_page_tabs_hidden('keys');
  }

  if ($tabsToDisplay["Synonymy"] == '0') {
    taxon_page_tabs_hidden('synonymy');
  }

  // -------------------------------------------- //

  $render_array['back_to_search'] = markup_to_render_array(theme('cdm_back_to_search_result_button'), -103);

  if (variable_get('cdm_dataportal_display_is_accepted_for', CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR) && isset($_REQUEST['acceptedFor'])) {
    $render_array['accepted_for'] = markup_to_render_array(theme('cdm_acceptedFor', array('acceptedFor' => $_REQUEST['acceptedFor'])), $weight++);
  }

  // --- PAGE PART: DESCRIPTION --- //
  if ($page_part == 'description' || $page_part == 'all') {

    // 1. fetch descriptions_by_featuretree but exclude the distribution feature
    $merged_tree = cdm_ws_descriptions_by_featuretree(get_profile_featureTree(), $taxon->uuid, array(UUID_DISTRIBUTION));


    // 2. find the distribution feature node
    $distribution_node =& cdm_feature_tree_find_node($merged_tree->root->childNodes, UUID_DISTRIBUTION);

    if ( $distribution_node) {
      // 3. get the distributionInfoDTO
      $query_parameters = cdm_distribution_filter_query();
      $query_parameters['part'] = array('mapUriParams');
      if(variable_get('distribution_sort', 'NO_SORT') != 'NO_SORT'){
        $query_parameters['part'][] = 'tree';
      } else {
        $query_parameters['part'][] = 'elements';
      }
      $query_parameters['omitLevels'] = array(UUID_NAMEDAREALEVEL_TDWGLEVEL_2);
      $customStatusColorsJson = variable_get(DISTRIBUTION_STATUS_COLORS, null);
      if($customStatusColorsJson){
        $query_parameters['statusColors'] = $customStatusColorsJson;
      }

      $distribution_info_dto = cdm_ws_get(CDM_WS_PORTAL_DESCRIPTION_DISTRIBUTION_INFO_FOR, $taxon->uuid, queryString($query_parameters));
      // 4. get distribution TextData is there are any
      $distribution_text_data = cdm_ws_fetch_all(CDM_WS_DESCRIPTIONELEMENT_BY_TAXON,
        array(
            'taxon' => $taxon->uuid,
            'type'=>'TextData',
            'features' => UUID_DISTRIBUTION
          )
      );

      // 5. put all disitribution data into the distribution feature node
      if($distribution_text_data //if text data exists
      || ($distribution_info_dto && isset($distribution_info_dto->tree) && $distribution_info_dto->tree->rootElement->numberOfChildren>0) // OR if tree element has distribution elements
      || ($distribution_info_dto && !empty($distribution_info_dto->elements))) {  // OR if DTO has distribution elements
        $distribution_node->descriptionElements = array('#type' => 'DTO');
        if($distribution_text_data){
          $distribution_node->descriptionElements['TextData'] = $distribution_text_data;
        }
        if($distribution_info_dto){
          $distribution_node->descriptionElements['DistributionInfoDTO'] = $distribution_info_dto;
        }
      }
    }

    $render_array['general'] = compose_cdm_taxon_page_profile($taxon, $merged_tree, $media);
    $render_array['general']['#weight'] = $weight++;
    $render_array['general']['#prefix'] = '<div id="general" class="page-part">';
    $render_array['general']['#suffix'] = '</div>';
  }

  // --- PAGE PART: IMAGES --- //
  if (array_search('images', taxon_page_tabs_hidden()) === FALSE && ($page_part == 'images' || $page_part == 'all')) {
    $images_html = '<div id="images" class="page-part">';
    if ($page_part == 'all') {
      $images_html .= '<h2>' . t('Images') . '</h2>';
    }
    // Get the image gallery as configured by the admin.
    $taxon_image_gallery = call_user_func_array('taxon_image_gallery_' . variable_get('image_gallery_viewer', 'default'), array(
        $taxon,
        $media,
    ));
    $images_html .= $taxon_image_gallery;
    $images_html .= '</div>';
    $render_array['images'] = markup_to_render_array($images_html, $weight++);
  }

  // --- PAGE PART: SYNONYMY --- //
  if (($page_part == 'synonymy' || $page_part == 'all')) {
    $synonymy_html = '<div id="synonymy" class="page-part">';
    if ($page_part == 'all') {
      $synonymy_html .= '<h2>' . t('Synonymy') . '</h2>';
    }
    $addAcceptedTaxon = variable_get('cdm_dataportal_nomref_in_title', CDM_DATAPORTAL_NOMREF_IN_TITLE);
    $synonymy_html .= theme('cdm_taxon_page_synonymy', array('taxon' => $taxon, 'addAcceptedTaxon' => $addAcceptedTaxon));

    $synonymy_html .= '</div>';
    $render_array['synonymy'] = markup_to_render_array($synonymy_html, $weight++);

  }

  // --- PAGE PART: SPECIMENS --- //
  if ($specimensOrObservationsCount > 0 && ($page_part == 'specimens' || $page_part == 'all')) {
    $render_array['specimens'] = array(
        '#prefix' => '<div id="specimens" class="page-part">' . ($page_part == 'all' ? '<h2>' . t('Specimens') . '</h2>' : ''),
        'content' => cdm_dataportal_taxon_page_specimens($taxon), // returns render array
        '#suffix' => '</div>',
    );
  }

  // --- PAGE PART: KEYS --- //
  if ($identificationKeyCount == 1 && $page_part == 'keys'){
    drupal_goto(path_to_key($polytomousKeysPager->records[0]->class, $polytomousKeysPager->records[0]->uuid));
  }
  else if ($identificationKeyCount > 0 && ($page_part == 'keys' || $page_part == 'all')) {
    $keys_html = '<div id="keys" class="page-part">';
    if ($page_part == 'all') {
      $keys_html .= '<h2>' . t('Keys') . '</h2>';
    }
    $keys_html .= theme('cdm_block_IdentificationKeys', array('taxonUuid' => $taxon->uuid));
    $keys_html .= '</div>';
    $render_array['keys'] = markup_to_render_array($keys_html, $weight++);
  }


  // adjust weights of page and toc elements according to the settings
  $taxontabs_weights = get_array_variable_merged('cdm_taxonpage_tabs_weight', CDM_TAXONPAGE_TAB_WEIGHT_DEFAULT);
  foreach($taxontabs_weights as $tab_key => $weight){
    if(isset($render_array[$tab_key])){
      $render_array[$tab_key]['#weight'] = $weight;
    }
  }


  // set up the TOC for the pages which contain all pageparts
  if($page_part == 'all') {

    asort($taxontabs_weights);
    foreach(array_keys($taxontabs_weights) as $tab_key){
      if(isset($render_array[$tab_key])){
        if($tab_key != 'general'){
          // add entry for page part
          $toc_elements[] = array(
              'data' => l(t(ucfirst($tab_key)), $_GET['q'], array('fragment' => $tab_key, 'query' => $http_request_params)),
              'class' => array('page-part-toc-item-' . $tab_key)
          );
        } else {
          // add content of profile part instead
          if(isset($render_array['general'])) {
            // in case all tabs are shown at once the feature tocs
            // should be integrated into the tabs toc as sub list
            // and the profile image should be on top of the page
            if(isset($render_array['general']['taxon_description_feature_toc'])){;
            foreach ($render_array['general']['taxon_description_feature_toc']['#items'] as $profile_toc_item){
              $toc_elements[] = $profile_toc_item;
            }
            unset($render_array['general']['taxon_description_feature_toc']);
            }
          }
        }
      }
    }

    // move profile image in page structure
    if(isset($render_array['general']['taxon_profile_image'])){
      $render_array['profile_image'] = $render_array['general']['taxon_profile_image'];
      $render_array['profile_image']['#weight'] = -100;
      unset($render_array['general']['taxon_profile_image']);
    }

    // finally add the table of contents to the render array
    $render_array['toc'] = array(
        '#theme' => 'item_list',
        '#items' => $toc_elements,
        '#title' => t('Content'),
        '#weight' => -101,
        '#suffix' => '</div>',
        '#prefix'=> '<div id="page-toc">'
    );
  }


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
      '#items' => cdm_feature_node_toc_items($merged_tree->root->childNodes),
      '#title' => t('Content'),
      '#weight' => -100,
      '#suffix' => '</div>',
      '#prefix'=> '<div id="page-toc">'
  );
  $render_array['taxon_description_feature_toc'] = $toc; //markup_to_render_array($toc_html);

  // Render the sections for each feature
  $render_array['taxon_description_features'] = markup_to_render_array(
      theme(
          'cdm_feature_nodes',
          array('mergedFeatureNodes' => $merged_tree->root->childNodes, 'taxon' => $taxon)
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
