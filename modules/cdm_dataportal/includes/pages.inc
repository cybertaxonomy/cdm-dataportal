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

    // --- get map service HTTP query parameters
    if (count($specimensOrObservations) > 0) {
        $occurrence_queryDto = cdm_ws_get(CDM_WS_GEOSERVICE_OCCURRENCEMAP, $taxon->uuid,  $by_associatedtaxon_query);

      $map_visibility = variable_get(SPECIMEN_MAP_VISIBILITY, SPECIMEN_MAP_VISIBILITY_DEFAULT);
      if($map_visibility == 'always' ||
        variable_get(SPECIMEN_MAP_VISIBILITY, SPECIMEN_MAP_VISIBILITY_DEFAULT) == 'automatic' &&
          (isset($occurrence_queryDto->fieldUnitPoints[0]) || isset($occurrence_queryDto->derivedUnitPoints[0]))
        )
      {
        $occurrence_query = $occurrence_queryDto->occurrenceQuery;
        $legend_format_query = null;
        $distribution_query = NULL;
        $render_array['map'] = compose_map('specimens', $occurrence_query, $distribution_query, $legend_format_query, array());
      }
    }
    if(variable_get('cdm_dataportal_compressed_specimen_derivate_table')){

        //COMPRESSED SPECIMEN DERIVATE TABLE
        $associatedFieldUnitsQuery_parameters = array();
        if (isset($_REQUEST['pager']) && is_array($_REQUEST['pager'])) {
            $associatedFieldUnitsQuery_parameters = array_merge($associatedFieldUnitsQuery_parameters, $_REQUEST['pager']);
        }

        $by_associatedtaxon_query = http_build_query($associatedFieldUnitsQuery_parameters);
        $pagerFieldUnits = cdm_ws_get(CDM_WS_PORTAL_TAXON, array($taxon->uuid, 'associatedFieldUnits'), $by_associatedtaxon_query);
        // get icon images
        $expand_icon = font_awesome_icon_markup(
            'fa-plus-square-o',
            array(
                'alt'=>'Show details',
                'class' => array('expand_icon')
            )
        );
        $collapse_icon = font_awesome_icon_markup(
            'fa-minus-square-o',
            array(
                'alt'=>'Show details',
                'class' => array('collapse_icon')
            )
        );
        $detail_image_icon = '<img title="Detail Image" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/detail_image_derivate-16x16-32.png' . '"/>';
        $checked_box_icon = '<img src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/step_done.gif' . '"/>';
        $sequence_icon = '<img title="Molecular Data" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/sequence_derivate-16x16-32.png' . '"/>';
        $character_data_icon = '<img title="Character Data" src="' . base_path() . drupal_get_path('module', 'cdm_dataportal') . '/images/character_data_derivate-16x16-32.png' . '"/>';
        if (isset($pagerFieldUnits->records[0])) {
            $rowcount = 0;
            foreach ($pagerFieldUnits->records as $fieldUnit) {
                //get derivate hierarchy for the FieldUnit
                $derivateHierarchy = cdm_ws_get(CDM_WS_PORTAL_OCCURRENCE, array($fieldUnit->uuid, 'derivateHierarchy'), 'taxonUuid=' . $taxon->uuid);
                if($derivateHierarchy){
                    //summary row
                    $rows[] = array(
                        'data' => array(
                            array(
                                'data' => $expand_icon . $collapse_icon,
                                'class' => array('summary_row_cell', 'summary_row_icon', 'expand_column')
                            ),
                            array(
                                'data' => $derivateHierarchy->country,
                                'class' => array('summary_row_cell')
                            ),
                            array(
                                'data' => $derivateHierarchy->date,
                                'class' => array('summary_row_cell')
                            ),
                            array(
                                'data' => $derivateHierarchy->collection,
                                'class' => array('summary_row_cell')
                            ),
                            array(
                                'data' => $derivateHierarchy->herbarium,
                                'class' => array('summary_row_cell')
                            ),
                            array(
                                'data' => $derivateHierarchy->hasType? $checked_box_icon:"",
                                'class' => array('summary_row_cell', 'summary_row_icon')
                            ),
                            array(
                                'data' => $derivateHierarchy->hasSpecimenScan? $checked_box_icon:"",
                                'class' => array('summary_row_cell', 'summary_row_icon')
                            ),
                            array(
                                'data' => ($derivateHierarchy->hasDna? $sequence_icon :"")." "
                                    .($derivateHierarchy->hasDetailImage?$detail_image_icon:"")." "
                                    .($derivateHierarchy->hasCharacterData?$character_data_icon:""),
                                'class' => array('summary_row_cell', 'summary_row_icon')
                            )
                        ),
                        'id' => ('derivate_summary' . $rowcount), // summary row id
                        'class' => array('summary_row'),
                    );

                    //assemble field unit details
                    $detail_html = "";
                    // - taxon name
                    if (variable_get('cdm_dataportal_compressed_specimen_derivate_table_show_determined_as') && $derivateHierarchy->taxonName) {
                        $detail_html .= create_label("Associated with") . $derivateHierarchy->taxonName . "<br>";
                    }
                    // - protologue
                    if ($derivateHierarchy->protologue) {
                        //$detail_html .= "<strong>Protologue:</strong> " . $derivateHierarchy->protologue . "<br>"
                    }
                    // - citation
                    if ($derivateHierarchy->citation) {
                        $detail_html .= create_label("Citation") . $derivateHierarchy->citation . "<br>";
                    }
                    //assemble specimen details
                    if($derivateHierarchy->preservedSpecimenDTOs){
                        foreach($derivateHierarchy->preservedSpecimenDTOs as $preservedSpecimenDTO) {
                            $detail_html .= "<br>";
                            $detail_html .= render_cdm_specimen_page($preservedSpecimenDTO);
                        }
                    }
                    $detail_html .= "<br>";
                    //detail row resp. one BIG detail cell
                    $rows[] = array(
                        'data' => array(
                            array(
                                'data' => "", //empty first column
                                'class' => array('expand_column')
                            ),
                            array(
                                'data' => $detail_html,
                                'colspan' => 7,
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
            "#header" => array(
                array(
                    'data' => "",
                    'class' => array('expand_column')
                ),
                "Country", "Date", "Collector + collecting number", "Herbaria", "Type", "Scan", "Derivatives"),
            "#rows" => $rows,
            "#attributes" => array(
                "id" => $tableId,
                "border" => 2
            )
        );

        //add toggle functionality to derivate hierarchy table
        drupal_add_js_rowToggle("#".$tableId);

        $render_array['derivate_hierarchy_table'] = $derivateHierarchyTable;
        $render_array['pager'] = markup_to_render_array(
            theme('cdm_pager', array(
                'pager' => $pagerFieldUnits,
                'path' => $_REQUEST['q'],
                'parameters' => $_REQUEST
            )),
            10 // weight
        );
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
                if (isset($typeDesignationPager) and isset($typeDesignationPager->records)) {
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
                  'cdm_specimenOrObservation',
                  'Click for details',
                  array('div', 'div'),
                  array(),
                  null, // $content_element_selector
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
 
function create_html_link($link, $openInExternalWindow=false){
    $html = "";
    if($link->uri && $link->uri!=""){
        $html .= '<a  href="' . $link->uri . '"';
        if($openInExternalWindow){
            $html .= ' target="_blank"';
        }
        $html .= '>' . $link->linkText . '</a>';
    }
    else{
        $html .= $link->linkText;
    }
    return $html;
}

/**
 * Creates HTML links from the given link list concatenated by default by a comma.
 * @param $linkList the list with Link objects having "uri" and "linkText" as members
 * @return string the assembled HTML string containing the links
 */
function create_html_links($linkList, $openInExternalWindow=false, $separator=", ")
{
    $html = "";
    if ($linkList) {
        foreach ($linkList as $link) {
            $html .= create_html_link($link, $openInExternalWindow).$separator;
        }
        $html = rtrim($html, $separator);
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

    // --- GET TaxonNodeAgentRelations --- //
    $current_classification_uuid = get_current_classification_uuid();
    $taxon_node_agent_relations_pager = cdm_ws_get(CDM_WS_PORTAL_TAXON_TAXONNODEAGENTRELATIONS,
        array(
            $taxon->uuid,
            $current_classification_uuid,
        ),
        "pageSize=1&pageIndex=0"// we are only interested into the count so we are fetching only one item, o is not possible!
    );
    if (!$taxon_node_agent_relations_pager || $taxon_node_agent_relations_pager->count == 0){
        taxon_page_tabs_hidden('experts');
    }

    if ($tabsToDisplay["Synonymy"] == '0') {
    taxon_page_tabs_hidden('synonymy');
  }

  // -------------------------------------------- //

  if (variable_get('cdm_dataportal_display_is_accepted_for', CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR) && isset($_REQUEST['acceptedFor'])) {
    $render_array['accepted_for'] = markup_to_render_array(cdm_accepted_for($_REQUEST['acceptedFor']), $weight++);
  }

  // --- PAGE PART: DESCRIPTION --- //
  if ($page_part == 'description' || $page_part == 'all') {

    $merged_tree = merged_taxon_feature_tree($taxon);


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
    $default_image_gallery = 'default';
    $configured_image_gallery_viewer = variable_get('image_gallery_viewer', 'default');
    if($configured_image_gallery_viewer == $default_image_gallery || $configured_image_gallery_viewer == 'fsi'){
      // the fsi_gallery requires a flash plugin, in case the client browser is not supporting
      // flash we also need to provide an the default gallery as alternative
      $taxon_image_gallery = '<div id="default-gallery-viewer">' .
        call_user_func_array('taxon_image_gallery_' . $default_image_gallery, array(
          $taxon,
          $media,
        ))
        . '</div>';
    }
    if($configured_image_gallery_viewer == 'fsi'){
      $taxon_image_gallery .= '<div id="'. $configured_image_gallery_viewer .'-gallery-viewer">' .
        call_user_func_array('taxon_image_gallery_' . $configured_image_gallery_viewer, array(
          $taxon,
          $media,
        ))
       . '</div>';
    }
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

  // --- PAGE PART: EXPERTS --- //

  if (array_search('experts', taxon_page_tabs_hidden()) === FALSE && ($page_part == 'experts' || $page_part == 'all')) {
    $render_array['experts'] = array(
        '#prefix' => '<div id="experts" class="page-part">' . ($page_part == 'all' ? '<h2>' . t('Experts') . '</h2>' : ''),
        'content' => compose_cdm_taxon_page_experts($taxon), // returns render array
        '#suffix' => '</div>',
    );
  }

  // ------------------ END OF PARTS -------------- //

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
              'data' => l(t('@tab-name', array('@tab-name' => ucfirst($tab_key))), $_GET['q'], array('fragment' => $tab_key, 'query' => $http_request_params)),
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
 * TODO should this function really be a compose function?
 *     For a compose function must there always be a theme function with the same name? (ak 8.8.2013)
 *
 * composes and returns an render array containing the components of the taxon profile tab:
 *  - 'taxon_profile_image'
 *  - 'taxon_description_feature_toc'
 *  - 'taxon_description_features'
 *
 *
 * @param taxon
 * @param mergedTrees
 * @param media
 *
 * @return array
 *   A Drupal render array with the following elements:
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

      if (!empty($taxon_profile_image_settings['media_uri_query'])) {
        $representationPart->uri = $representationPart->uri
          . (strpos($representationPart->uri, '?') !== FALSE ? '&' : '?')
          . $taxon_profile_image_settings['media_uri_query'];
      }
    }
    else {
      if ($taxon_profile_image_settings['custom_placeholder_enabled']) {
        // show placeholder image instead
        if (!empty($taxon_profile_image_settings['custom_placeholder_image_on']) && !empty($taxon_profile_image_settings['custom_placeholder_image_fid'])) {
          // use the user provided image
          $profile_image_file = file_load($taxon_profile_image_settings['custom_placeholder_image_fid']);
          $url = file_create_url($profile_image_file->uri);
          $image_info = image_get_info($profile_image_file->uri);
          $representationPart->width = $image_info['width'];
          $representationPart->height = $image_info['height'];
          $representationPart->uri = $url;
        }
        else {
          // use the hard coded default
          $representationPart->width = 184;
          $representationPart->height = 144;
          $representationPart->uri = base_path() . drupal_get_path('module',
              'cdm_dataportal') . '/images/no_picture.png';
        }
        $attributes['alt'] = "no image available";
      }
    }

    if (isset($representationPart->uri)) {
      $profile_image = theme('cdm_media_gallerie_image', array(
        'mediaRepresentationPart' => $representationPart,
        'maxExtend' => $taxon_profile_image_settings['maxextend'],
        'addPassePartout' => FALSE,
        'attributes' => $attributes,
      ));
      // NOTE: style="width:${maxextend}px' is needed for IE8 !!!
      $render_array['taxon_profile_image'] = markup_to_render_array('<div id="taxonProfileImage" style="width:' . $taxon_profile_image_settings['maxextend'] . 'px">' . $profile_image . '</div>',
        -101);
    }
  }


  // Render the sections for each feature
  $render_array['taxon_description_features'] = compose_feature_blocks($merged_tree->root->childNodes, $taxon);

  $bibliography_settings = get_bibliography_settings();
  if($bibliography_settings['enabled'] == 1){
    $bibliography_block = feature_block(t('Bibliography'));
    $bibliography_block->content = FootnoteManager::renderFootnoteList('BIBLIOGRAPHY', '');

    if(!empty($bibliography_block->content)){
      $render_array['taxon_description_bibliography'] =  markup_to_render_array(
        theme('block',
          array(
            'elements' => array(
              '#block' => $bibliography_block,
              '#children' => $bibliography_block->content,
            )
          )
        ),
        100 // weight
      );
      cdm_toc_list_add_item('Bibliography', 'bibliography');
    }

  }

  // create the table of content
  $toc = array(
      '#theme' => 'item_list',
    '#items' => cdm_toc_list(),
      '#title' => t('Content'),
    '#weight' => -100,                  // move to the top
      '#suffix' => '</div>',
      '#prefix'=> '<div id="page-toc">'
  );
  $render_array['taxon_description_feature_toc'] = $toc;

  return $render_array;
}

/**
 * composes and returns an render array for the experts associated with the given taxon
 *
 * @param taxon
 *
 * @return array
 *   A Drupal render array for a table with the experts
 *
 * @ingroup compose
 */
function compose_cdm_taxon_page_experts($taxon){

  $render_array = array();
  if(!isset($taxon->uuid)){
    return $render_array;
  }

  $current_classification_uuid = get_current_classification_uuid();
  // TODO use cdm_ws_fetchall below but this failes! needs fix!
  $taxon_node_agent_relations = cdm_ws_get(CDM_WS_PORTAL_TAXON_TAXONNODEAGENTRELATIONS,
    array(
      $taxon->uuid,
      $current_classification_uuid
    )
  );

  $header = array(
    array('data' => t('Expert')),
    array('data' => t('Role'))
  );
  $rows = array();


  foreach($taxon_node_agent_relations->records as $taxon_node_agent_relation){


    $expert_role_id = $taxon_node_agent_relation->agent->uuid . '-' . $taxon_node_agent_relation->type->uuid;
    $expert_details_container_id = 'expert_details_' . $expert_role_id;

    $agent_label_markup = cdm_dynabox(
      'expert_' . $expert_role_id,
      $taxon_node_agent_relation->agent->titleCache,
      // specifying both ends of the relationship will return only one record in the pager
      cdm_compose_url(CDM_WS_PORTAL_AGENT,
         array($taxon_node_agent_relation->agent->uuid, 'taxonNodeAgentRelations'),
        'taxon_uuid=' . $taxon->uuid . '&relType_uuid=' . $taxon_node_agent_relation->type->uuid),
      'cdm_taxon_expert',
      'Click for details',
      array('div', 'div'),
      array(), // attributes
      '#' . $expert_details_container_id // $content_element_selector
    );

    // Expert and Role
    $rows[] = array(
      'data' => array(
        array(
          'data' => $agent_label_markup,
          'class' => array(html_class_attribute_ref($taxon_node_agent_relation->agent))
        ),
        array(
          'data' => $taxon_node_agent_relation->type->representation_L10n,
          'class' => array(html_class_attribute_ref($taxon_node_agent_relation->type))
        )
      )
    );
    // Agent details
    $rows[] = array(
      'data' => array(
        array(
          'data' => '<!-- expert_details_container -->',
          'id' => $expert_details_container_id,
          'colspan' => 2
        )
      )
    );

  }


  $render_array['experts_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );


  return $render_array;
}


/**
 * Manages the tabs to be hidden in the taxon page.
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
 *   Optional parameter. The given string will be added to the array of tabs
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

/* =================== block composition ===================== */

/**
 * Composes and returns an render array for the classification breadcrumbs of the given taxon.
 *
 * @param taxon
 *
 * @return array
 *   A Drupal render array for a table with the experts
 *
 * @ingroup compose
 */
function compose_classification_breadcrumbs($taxon_uuid) {

  _add_js_taxonomic_children('#classification-breadcrumbs .taxonomic-children-button');

  $render_array = array();

  $render_array['#theme'] = 'item_list';
  $render_array['#type'] = 'ul';
  $render_array['#attributes'] = array(
    'id' => 'classification-breadcrumbs',
    'class' => 'breadcrumbs inline',
  );

  $items = array();

  $parent_taxon_nodes = null;
  if($taxon_uuid){
    $parent_taxon_nodes = cdm_ws_taxonomy_pathFromRoot($taxon_uuid);
  }

  $classifications = cdm_ws_fetch_all(CDM_WS_PORTAL_TAXONOMY);
  // find current classification in list
  $classification = null;
  $current_classification_uuid = get_current_classification_uuid();
  foreach ($classifications as $classification){
    if($classification->uuid == $current_classification_uuid){
      break;
    }
  }

  $node_name = '';
  if(count($classifications) > 1 ){
    // need to add the current classification as first label

    $label = $classification->titleCache;
    if(strlen($label) > 20){
      $label = substr($label, 0, strpos($label, ' ', 15)) . '...';
    }
    $node_name = font_awesome_icon_markup('fa-th-list')  . ' ' . l($label, '#', array(
      'attributes' => array(
        'class' => 'taxonomic-children-button classification-chooser',
        'data-destination-uri' => drupal_get_destination(),
        'data-cdm-align-with' => array('prev')
      ),
      'html' => true
    ));
  }

  $rank_limit_uuid = variable_get(TAXONTREE_RANKLIMIT, TAXONTREE_RANKLIMIT_DEFAULT);

  $rank_separator = '<span> '
    . font_awesome_icon_markup('fa-chevron-right')
    . ' </span>';
  $more_children_icon = font_awesome_icon_markup('fa-sitemap fa-rotate-270');
  $more_children_label = '...';

  $items[] = $node_name;

  $more_children_for = null;
  if($parent_taxon_nodes){
    foreach ($parent_taxon_nodes as $node) {

      $is_first_item = count($items) == 0;
      $is_last_item = count($items) == count($parent_taxon_nodes);
      $node_name = cdm_dataportal_shortname_of($node);
      $path = path_to_taxon($node->taxonUuid);

      if($node->taxonomicChildrenCount > 0) {
        $more_children_for = $node->taxonUuid;
      } else {
        $more_children_for = null;
      }

      // 'fa-sitemap'

      $items[] =
        ($is_first_item ? '' : ' ')
        . $rank_separator
        . l(
          $node_name,
          $path,
          array(
            'attributes' => array(
              'class' => array('taxonomic-children-button'),
              'data-cdm-taxon-uuid' => array($node->taxonUuid),
              'data-cdm-classification-mode' => array('siblings'),
              'data-cdm-align-with' => array('prev')
            )
          )
        );
      }
    }

  // add more button to the end
  if(!$parent_taxon_nodes) {
    // not taxon focused yet, adding button to make  the root nodes available
    $items[] = '<span>'
      . $more_children_icon . '&nbsp;' .
      '<span class="taxonomic-children-button" data-classification-uuid="' . $current_classification_uuid
      . '" data-rank-limit-uuid="' . $rank_limit_uuid . '" data-cdm-align-with="prev"> ' . $more_children_label . '<span>'
      . '</span>';
  } else if($more_children_for){
    // last parent item has child taxon nodes
    $items[] = ' <span>'
      . $more_children_icon . '&nbsp;' .
      '<span class="taxonomic-children-button" data-cdm-taxon-uuid="' .$more_children_for
      . '" data-cdm-classification-mode="children" data-cdm-align-with="prev"> ' . $more_children_label . '</span>'
      . '</span>';

  }

  $render_array['#items'] = $items;

  return $render_array;
}


/**
 * @param $specimen_uuid
 * @return array
 *    The drupal render array for the page
 *
 * @ingroup compose
 */
function compose_cdm_specimen_page($specimen_uuid)
{
  drupal_set_title("Specimen Details");
  $specimen = cdm_ws_get(CDM_WS_PORTAL_OCCURRENCE, array($specimen_uuid, 'specimenDerivates'));

  $render_array = array();
  RenderHints::pushToRenderStack('specimen_page');

  $detail_html = render_cdm_specimen_page($specimen, true);
  $render_array['specimen_html'] = array(
    '#markup' => $detail_html
  );

  RenderHints::popFromRenderStack();
  return $render_array;
}

/**
 * @param $named_area_uuid
 * @return array
 *    The drupal render array for the page
 *
 * @ingroup compose
 */
function compose_cdm_named_area_page($named_area_uuid)
{

  $named_area = cdm_ws_get(CDM_WS_PORTAL_TERM, array($named_area_uuid));

  $render_array = array();
  RenderHints::pushToRenderStack('named_area_page');

  $groups = array();
  @_description_list_group_add($groups, t('Name') . ':', $named_area->representation_L10n);
  @_description_list_group_add($groups, t('IdInVocabulary') . ':', $named_area->idInVocabulary);
  if(isset($named_area->level)) {
    @_description_list_group_add($groups, t('Level') . ':', $named_area->level->representation_L10n);
  }

  $name_area_details_elements = array(
   // '#title' => $title,
    '#theme' => 'description_list',
    '#groups' => $groups,
    '#attributes' => array('class' => html_class_attribute_ref($named_area)),
  );

  $render_array[] = $name_area_details_elements;

  RenderHints::popFromRenderStack();
  return $render_array;
}
