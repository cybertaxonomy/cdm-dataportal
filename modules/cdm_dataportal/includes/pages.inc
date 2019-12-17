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
 * Composes a render array representing the ocurrences associetad with the $taxon.
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
 * @ingroup Compose
 *
 */
function compose_cdm_taxon_page_specimens($taxon) {

    $render_array = array();
    RenderHints::pushToRenderStack('taxon_page_specimens');

    $fieldUnitDTOs = null;
    $specimensOrObservations = null;
    if (variable_get('cdm_dataportal_specimen_derivate_tree')){
      // get fieldUnitDTOs
      $fieldUnitDTOs = cdm_ws_get(CDM_WS_TAXON_FIELDUNIT_DTOS, array( $taxon->uuid));
      $fieldUnitDTOs = order_fieldUnitDtos_by_date_and_type($fieldUnitDTOs);
    } else {
      // $specimensOrObservations = cdm_ws_get(CDM_WS_TAXON, array( $taxon->uuid, 'specimensOrObservations'));
      // extend by associated taxas occurrences
      $relationship_filter_query_parameters = relationship_filter_query_parameters();

      if (isset($_REQUEST['pager']) && is_array($_REQUEST['pager'])) {
        $relationship_filter_query_parameters = array_merge($relationship_filter_query_parameters, $_REQUEST['pager']);
      }
      $by_associatedtaxon_query = http_build_query($relationship_filter_query_parameters);
      $pager = cdm_ws_get(CDM_WS_OCCURRENCE_BY_ASSOCIATEDTAXON,
        null,
        $by_associatedtaxon_query . '&taxonUuid=' . $taxon->uuid
      );
      $specimensOrObservations = array();
      if (isset($pager->records[0])) {
        $specimensOrObservations = $pager->records;
      }
      // cdm_ws_get(CDM_WS_OCCURRENCE_FIELDUNIT_DTO_BY_ASSOCIATEDTAXON,
      // null,
      // $by_associatedtaxon_query . '&taxonUuid=' . $taxon->uuid
      // );

      // $specimensOrObservations = array();
      // if(isset($pager->records[0])){
      //    $specimensOrObservations =  $pager->records;
      // }
      // Collect media (fieldObjectMedia, derivedUnitMedia) and add as a custom field
      foreach ($specimensOrObservations as &$occurrence) {
        $occurrence->_fieldObjectMedia = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, array(
          $occurrence->uuid,
          'fieldObjectMediaDTO',
        ));
        $occurrence->_derivedUnitMedia = cdm_ws_get(CDM_WS_DERIVEDUNIT_FACADE, array(
          $occurrence->uuid,
          'derivedUnitMedia',
        ));
      }
      $specimensOrObservations = order_specimens_or_observations_by_date_and_type($specimensOrObservations);
    }

    // --- get map service HTTP query parameters
    if (count($specimensOrObservations) > 0 || $fieldUnitDTOs > 0) {
        $render_array['map'] = occurrence_map_query_parameters($taxon);
    }

    // -------------------------------------------------------

    if(variable_get('cdm_dataportal_compressed_specimen_derivate_table')){

        //COMPRESSED SPECIMEN DERIVATE TABLE
        $associatedFieldUnitsQuery_parameters = array();
        if (isset($_REQUEST['pager']) && is_array($_REQUEST['pager'])) {
            $associatedFieldUnitsQuery_parameters = array_merge($associatedFieldUnitsQuery_parameters, $_REQUEST['pager']);
        }

        $by_associatedtaxon_query = http_build_query($associatedFieldUnitsQuery_parameters);
        $pager_field_units = cdm_ws_get(CDM_WS_PORTAL_TAXON,
            array($taxon->uuid, 'associatedFieldUnits'),
            $by_associatedtaxon_query . '&pageSize=' . variable_get('cdm_dataportal_compressed_specimen_derivate_table_page_size')
        );

        if (isset($pager_field_units->records[0])) {
          $field_unit_uuids = array();
          foreach ($pager_field_units->records as $field_unit) {
            $field_unit_uuids[] = $field_unit->uuid;
          }

          $render_array['derivate_hierarchy_table'] = compose_compressed_specimen_derivate_table($field_unit_uuids);
        }

        $render_array['pager'] = markup_to_render_array(
            theme('cdm_pager', array(
                'pager' => $pager_field_units,
                'path' => $_REQUEST['q'],
                'parameters' => $_REQUEST
            )),
            10 // weight
        );
    }
    else if(variable_get('cdm_dataportal_specimen_derivate_tree')){
      $render_array['specimen_list'] = compose_specimen_table_top_down($fieldUnitDTOs);
      $render_array['specimen_list']['#weight'] = 2;
    } else {
      $specimen_table = compose_specimens_table_bottom_up($specimensOrObservations);

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
 * @return array
 *
 * TODO move to cdm_dataportal.module or api.module?
 */
function relationship_filter_query_parameters()
{
  $relationship_choice = variable_get(CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS, unserialize(CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS_DEFAULT));
  $relationship_choice['direct'] = get_selection($relationship_choice['direct']);
  $relationship_choice['invers'] = get_selection($relationship_choice['invers']);

  $by_associatedtaxon_query_parameters = array(
    'relationshipsInvers' => implode(',', $relationship_choice['invers']),
    'relationships' => implode(',', $relationship_choice['direct']),
  );
  return $by_associatedtaxon_query_parameters;
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

  $synonymy_as_tab = variable_get(CDM_SYNONYMY_AS_TAB, CDM_SYNONYMY_AS_TAB_DEFAULT) === 1;
  if(!$synonymy_as_tab){
    unset($tabsToDisplay["Synonymy"]);
    // the synonymy is located in the general part in this case
    if($page_part == 'synonymy'){
      $page_part = 'description';
    }
  }

  $media = _load_media_for_taxon($taxon);


  if (!isset($media[0]) || ($tabsToDisplay["Images"] == '0')) {
    taxon_page_tabs_hidden_add('images');
  }

  // --- GET specimensOrObservations --- //
  $specimensOrObservations = cdm_ws_get(CDM_WS_TAXON, array( $taxon->uuid, 'specimensOrObservationsCount'));

  $specimensOrObservationsCount = $specimensOrObservations != null ? $specimensOrObservations->result : 0;
  if ($specimensOrObservationsCount == 0 || ($tabsToDisplay["Specimens"] == '0')) {
    taxon_page_tabs_hidden_add('specimens');
  }

  // --- GET polytomousKeys --- //
  $polytomousKeysPager = cdm_ws_get(CDM_WS_POLYTOMOUSKEY, NULL, "findByTaxonomicScope=$taxon->uuid");
  $identificationKeyCount = 0;
  if ($polytomousKeysPager) {
    $identificationKeyCount += $polytomousKeysPager->count;
  }
  if ($identificationKeyCount == 0 || ($tabsToDisplay["Keys"] == '0')) {
    taxon_page_tabs_hidden_add('keys');
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
      taxon_page_tabs_hidden_add('experts');
  }

  if (!isset($tabsToDisplay["Synonymy"]) || $tabsToDisplay["Synonymy"] == '0') {
    taxon_page_tabs_hidden_add('synonymy');
  }

  // -------------------------------------------- //

  if (variable_get('cdm_dataportal_display_is_accepted_for', CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR) && isset($_REQUEST['acceptedFor'])) {
    $render_array['accepted_for'] = markup_to_render_array(cdm_accepted_for($_REQUEST['acceptedFor']), $weight++);
  }

  // --- PAGE PART: DESCRIPTION --- //
  if (!taxon_page_tabs_hidden_check('description') && ($page_part == 'description' || $page_part == 'all')) {

    $merged_tree = merged_taxon_feature_tree($taxon);


    $render_array['general'] = compose_cdm_taxon_page_profile($taxon, $merged_tree, $media, !$synonymy_as_tab);
    $render_array['general']['#weight'] = $weight++;
    $render_array['general']['#prefix'] = '<div id="general" class="page-part">';
    $render_array['general']['#suffix'] = '</div>';
  }

  // --- PAGE PART: IMAGES --- //
  if (!taxon_page_tabs_hidden_check('images') && ($page_part == 'images' || $page_part == 'all')) {
    $images_html = '<div id="images" class="page-part">';
    if ($page_part == 'all') {
      $images_html .= '<h2>' . t(cdm_taxonpage_tab_label('Images')) . '</h2>';
    }
    // Get the image gallery as configured by the admin.
    $default_image_gallery = 'default';
    $configured_image_gallery_viewer = variable_get('image_gallery_viewer', 'default');
    $taxon_image_gallery = '<div class="error">No image gallery viewer configured</div>';
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
  if (!taxon_page_tabs_hidden_check('synonymy') && (($page_part == 'synonymy' || $page_part == 'all') && $synonymy_as_tab)) {
    $synonymy_html = '<div id="synonymy" class="page-part">';
    if ($page_part == 'all') {
      $synonymy_html .= '<h2>' . t(cdm_taxonpage_tab_label('Synonymy')) . '</h2>';
    }
    $addAcceptedTaxon = variable_get('cdm_dataportal_nomref_in_title', CDM_DATAPORTAL_NOMREF_IN_TITLE);

    $synonym_a = compose_cdm_taxon_page_synonymy($taxon, $addAcceptedTaxon);
    $synonymy_html .= drupal_render($synonym_a);

    $synonymy_html .= '</div>';
    $render_array['synonymy'] = markup_to_render_array($synonymy_html, $weight++);

  }

  // --- PAGE PART: SPECIMENS --- //
  if (!taxon_page_tabs_hidden_check('specimens') && ($specimensOrObservationsCount > 0 && ($page_part == 'specimens' || $page_part == 'all'))) {
    $render_array['specimens'] = array(
        '#prefix' => '<div id="specimens" class="page-part">' . ($page_part == 'all' ? '<h2>' . t(cdm_taxonpage_tab_label('Specimens')) . '</h2>' : ''),
        'content' => compose_cdm_taxon_page_specimens($taxon), // returns render array
        '#suffix' => '</div>',
    );
  }

  // --- PAGE PART: KEYS --- //
  if(!taxon_page_tabs_hidden_check('keys')){
    if ($identificationKeyCount == 1 && $page_part == 'keys'){
      drupal_goto(path_to_key($polytomousKeysPager->records[0]->class, $polytomousKeysPager->records[0]->uuid));
    }
    else if ($identificationKeyCount > 0 && ($page_part == 'keys' || $page_part == 'all')) {
      $keys_html = '<div id="keys" class="page-part">';
      if ($page_part == 'all') {
        $keys_html .= '<h2>' . t(cdm_taxonpage_tab_label('Keys')) . '</h2>';
      }
      $keys_html .= theme('cdm_block_IdentificationKeys', array('taxonUuid' => $taxon->uuid));
      $keys_html .= '</div>';
      $render_array['keys'] = markup_to_render_array($keys_html, $weight++);
    }
  }

  // --- PAGE PART: EXPERTS --- //

  if (!taxon_page_tabs_hidden_check('experts') && ($page_part == 'experts' || $page_part == 'all')) {
    $render_array['experts'] = array(
        '#prefix' => '<div id="experts" class="page-part">' . ($page_part == 'all' ? '<h2>' . t(cdm_taxonpage_tab_label('Experts')) . '</h2>' : ''),
        'content' => compose_cdm_taxon_page_experts($taxon), // returns render array
        '#suffix' => '</div>',
    );
  }

  // ------------------ END OF PARTS -------------- //

  // adjust weights of page and toc elements according to the settings
  $taxontabs_weights = get_array_variable_merged(CDM_TAXONPAGE_TAB_WEIGHT, CDM_TAXONPAGE_TAB_WEIGHT_DEFAULT);
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
              'data' => l(t(cdm_taxonpage_tab_label(ucfirst($tab_key))), $_GET['q'], array('fragment' => $tab_key, 'query' => $http_request_params)),
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
 * @param object taxon
 * @param object $merged_tree
 * @param object media
 * @param bool $add_synonymy
 *
 * @return array
 *   A Drupal render array with the following elements:
 *     - 'taxon_profile_image'
 *     - 'taxon_description_feature_toc'
 *     - 'taxon_description_features'
 *
 * @throws Exception
 *
 * @ingroup compose
 */
function compose_cdm_taxon_page_profile($taxon, $merged_tree, $media, $add_synonymy) {

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
    if (isset($media[0])) {
      // due to a bug the portal/taxon/{uuid}/media service only delivers a filtered media object
      // which only contains the thumbnail representation even if the height and width filters are not set.
      // -->
      $preferred_media = cdm_ws_get(CDM_WS_MEDIA, $media[0]->uuid);
      $preferred_representations = cdm_preferred_media_representations($preferred_media, array(
        'image/jpg',
        'image/jpeg',
        'image/png',
        'image/gif',
      ),
        $taxon_profile_image_settings['maxextend'],
        $taxon_profile_image_settings['maxextend']
      );
      if(count($preferred_representations) > 0){

        $representation = array_shift($preferred_representations);
        $representationPart = $representation->parts[0];
        $attributes['alt'] = $representationPart->uri;

        if (!empty($taxon_profile_image_settings['media_uri_query'])) {
          $representationPart->uri = $representationPart->uri
            . (strpos($representationPart->uri, '?') !== FALSE ? '&' : '?')
            . $taxon_profile_image_settings['media_uri_query'];
        }
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
      $profile_image = cdm_media_gallerie_image($representationPart, $taxon_profile_image_settings['maxextend'], FALSE, $attributes);
      // NOTE: style="width:${maxextend}px' is needed for IE8 !!!
      $max_extend_with = $taxon_profile_image_settings['maxextend'] . 'px';
      $render_array['taxon_profile_image'] = markup_to_render_array('<div id="taxonProfileImage" style="width:' . $max_extend_with . '">' . $profile_image . '</div>',
        -101);
    }
  }

  if($add_synonymy){
    $synonymy_a = compose_cdm_taxon_page_synonymy($taxon, true);
    $synonymy_a['#weight'] = -102;
    $render_array['synonymy'] = $synonymy_a;
  }

//  $pseudo_feature_block_toc_items = array();


  // Render the sections for each real feature
  $feature_block_list = make_feature_block_list($merged_tree->root->childNodes, $taxon);

  // >>>>>>>>>>>>>>>>>>> PSEUDO FEATURES >>>>>>>>>>>>>>>>>>>
  $pseudo_feature_weights = get_array_variable_merged(CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS, CDM_PSEUDO_FEATURE_BLOCK_WEIGHTS_DEFAULT);
  // Bibliography
  $bibliography_settings = get_bibliography_settings();
  if($bibliography_settings['enabled'] == 1){
    $bibliography_markup = FootnoteManager::renderFootnoteList(PSEUDO_FEATURE_BIBLIOGRAPHY, '');
    if($bibliography_markup) {
      $feature_bibliography = make_pseudo_feature('Bibliography', PSEUDO_FEATURE_BIBLIOGRAPHY);
      $bibliography_item = markup_to_render_array($bibliography_markup);
      $bibliography_block = feature_block(t('Bibliography'), $feature_bibliography);
      $bibliography_block->content = array();
      $bibliography_block->content[] = compose_feature_block_wrap_elements(array($bibliography_item), $feature_bibliography);

      $feature_block_list[$pseudo_feature_weights[PSEUDO_FEATURE_BIBLIOGRAPHY]] = $bibliography_block;
      cdm_toc_list_add_item('Bibliography', 'bibliography', null, false, $pseudo_feature_weights[PSEUDO_FEATURE_BIBLIOGRAPHY]);
    }
  }

  // Descriptions (aggregated)
  $descriptionTypes = array();
  $descriptionTypes['descriptionTypes'] = ("AGGREGATED_STRUC_DESC");
  $aggregatedDescriptions = cdm_ws_fetch_all(CDM_WS_PORTAL_TAXON . '/' . $taxon->uuid . '/descriptions', $descriptionTypes);
  if (isset($aggregatedDescriptions) and !empty($aggregatedDescriptions)) {
      // if($feature_block_list) ....TODO
      $feature_description = make_pseudo_feature('Descriptions (aggregated)', PSEUDO_FEATURE_AGGREGATION_DESCRIPTIONS);
      $description_item = '';
      foreach ($aggregatedDescriptions as $description) {
        $description_item .= render_cdm_description($description, TRUE);
      }
      $description_block = feature_block(t('Descriptions (aggregated)'), $feature_description);
      $description_block->content = [];
      $description_block->content[] = compose_feature_block_wrap_elements([$description_item], $feature_description);

      $feature_block_list[$pseudo_feature_weights[PSEUDO_FEATURE_AGGREGATION_DESCRIPTIONS]] = $description_block;
      cdm_toc_list_add_item('Descriptions (aggregated)', 'aggregation_descriptions', null, false, $pseudo_feature_weights[PSEUDO_FEATURE_AGGREGATION_DESCRIPTIONS]);


  }

  // sort by weight
  ksort($feature_block_list);
  $render_array['taxon_description_features'] = _block_get_renderable_array($feature_block_list);

/*  // update TOC
  if ($pseudo_feature_block_toc_items){
    foreach ($pseudo_feature_block_toc_items as $label=>$fragment){
      cdm_toc_list_add_item($label, $fragment);
    }
  }
*/
  // <<<<<<<<<<<<<<<<<<< PSEUDO FEATURES <<<<<<<<<<<<<<<<<<<

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
 * Renders the link which will lead to the specimen detail page
 * @param object $specimen
 *    the cdm specimen entity which will be linked to
 *
 * @return string
 *     the markup for the link
 */
function render_cdm_specimen_link($specimen) {
  $path = path_to_specimen($specimen->uuid);
  $attributes['class'][] = html_class_attribute_ref($specimen);
  return $specimen->titleCache.icon_link($path);
}

/**
 * Returns HTML containing the synonymy for the accepted taxon.
 *
 * Shows the whole synonymy for the accepted taxon.
 * The synonymy list is headed by the complete scientific name
 * of the accepted taxon with nomenclatural reference.
 *
 * @param object $taxon
 * @param boolean $add_accepted_taxon
 *
 * @return array
 *  Drupal render array for the synonymy
 *
 * @throws Exception
 *
 * @ingroup compose
 */
function compose_cdm_taxon_page_synonymy($taxon, $add_accepted_taxon) {

  RenderHints::pushToRenderStack('taxon_page_synonymy');

  // footnote key for the homotypic group and accepted taxon,
  // both should have the same footnote key
  RenderHints::setFootnoteListKey(RenderHints::getRenderPath());

  $synomymie = cdm_ws_get(CDM_WS_PORTAL_TAXON_SYNONYMY, array($taxon->uuid));

  $out = '';

  // Render accepted taxon.
  //
  // foonotes of the accepted taxon will be rendered in the homotypic group section
  // even if there are not synonyms in the homotypic group
  // homotypic group and accepted taxon should have the same footnote key
  $referenceUri = '';
  if ($add_accepted_taxon) {
    // remember the last part of the render path
    $synonymy_render_path = RenderHints::getRenderPath();
    // set new render path for the accepted taxon so
    // it can be styled differently via the name render part definitions
    RenderHints::pushToRenderStack('accepted_taxon');
    $accepted_name = '';
    if (variable_get(CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE, 0)) {
      $label = variable_get(CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE_LABEL, CDM_SYNONYMY_ACCEPTED_TAXON_SEC_SEPARATE_LABEL_DEFAULT);
      $accepted_name .= '<div class="secReference"><span class="label">' . t($label) . ':</span> ' . $taxon->sec->titleCache . '</div>';
    }
    if (isset($taxon->name->nomenclaturalReference)) {
      $referenceUri = url(path_to_reference($taxon->name->nomenclaturalReference->uuid));
    }

    $accepted_name .= '<div class="accepted-name">';
    $accepted_name .= render_taxon_or_name($taxon, NULL, $referenceUri);

    $name_relations = cdm_name_relationships_for_taxon($taxon);
    $name_relationships = compose_name_relationships_inline($name_relations, $taxon->name->uuid, $taxon->uuid);
    // Render relationships of accepted name.
    if(isset($name_relationships['list']['items'][0])){
      $accepted_name .= ' ' . drupal_render($name_relationships);
    }

    // handle annotations of the name and taxon
    $special_annotations_array = array();
    $special_annotations_array[] = $taxon->name;
    $special_annotations_array[] = $taxon;
    $accepted_name .= theme('cdm_annotations_as_footnotekeys', array(
        'cdmBase_list' => $special_annotations_array,
        'footnote_list_key' => $synonymy_render_path . '-annotations')
    );
    $accepted_name .= '</div>';
    RenderHints::popFromRenderStack();
  }

  // --- Render homotypic synonymy group
  if (!empty($accepted_name)) {
    $out .= $accepted_name;
  }

  // Render the homotypicSynonymyGroup including the type information.
  $out .= theme(
    'cdm_homotypicSynonymyGroup',
    array(
      'synonymList' => $synomymie->homotypicSynonymsByHomotypicGroup,
      'accepted_taxon_name_uuid' => $taxon->name->uuid
    )
  );


  // Render accepted taxon heterotypic synonymy groups.
  if ($synomymie->heterotypicSynonymyGroups) {
    foreach ($synomymie->heterotypicSynonymyGroups as $homotypicalGroup) {
      $out .= theme('cdm_heterotypicSynonymyGroup', array('homotypicalGroup' => $homotypicalGroup));
    }
  }
  // Render taxon relationships.
  if (variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT)) {
    $taxonRelationshipsDTO = cdm_ws_get(CDM_WS_PORTAL_TAXON_RELATIONS_DTO, $taxon->uuid);
    $out .= cdm_taxonRelationships($taxonRelationshipsDTO, $taxon);
  }

  RenderHints::popFromRenderStack();

  return markup_to_render_array($out);
}


/**
 * composes and returns an render array for the experts associated with the given taxon
 *
 * @param object taxon
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
function taxon_page_tabs_hidden_add($add_tab = NULL) {
  $tabs = &drupal_static('taxon_page_tabs_hidden');

  if(!isset($tabs)){
    $tabs = array();
  }

  if (isset($add_tab) && !array_key_exists($add_tab, $tabs)) {
    $tabs[] = $add_tab;
  }

  return $tabs;
}

/**
 * Manages the tabs to be hidden in the taxon page.
 *
 * The tabs names are identified by their last menu link path element:
 *  - description
 *  - synonymy
 *  - images
 *  - specimens
 *  - key
 *
 * Internally the tabs are stored in a static variable which is
 * managed by drupal_static().
 *
 * @param string $tabname
 *   The name of the tab to check
 *
 * @return boolean
 *   True if the tab or section is to be hidden
 */
function taxon_page_tabs_hidden_check($tabname) {

  $tabs = &drupal_static('taxon_page_tabs_hidden');

  if(!isset($tabs)){
    $tabs = array();
  }

  return array_search($tabname, $tabs) !== FALSE;
}

/**
 * Implements the hook_preprocess_HOOK() for theme_menu_local_tasks()
 *
 *  - Removes the tabs to be hidden, @see taxon_page_tabs_hidden_add()
 *  - Renames tabs according to the settings // TODO (this will replace the theme_cdm_taxonpage_tab() function !!!)
 *
 * @param array $variables
 *   The variables array
 */
function cdm_dataportal_preprocess_menu_local_tasks(&$variables) {

  $hidden_tabs = taxon_page_tabs_hidden_add();

  if (!variable_get(CDM_SEARCH_BLAST_ENABLED)) {
      if (is_array($variables['primary'])) {
          foreach ($variables['primary'] as $key => &$element) {
              if ($element['#link']['path'] == 'cdm_dataportal/search/blast') {
                  // remove the tab
                  unset($variables['primary'][$key]);
              }

          }
      }
  }
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
          '<span class="' . html_class_attribute_ref($node) . '">' . $node_name . '</span>',
          $path,
          array(
            'attributes' => array(
              'class' => array('taxonomic-children-button'),
              'data-cdm-taxon-uuid' => array($node->taxonUuid),
              'data-cdm-classification-mode' => array('siblings'),
              'data-cdm-align-with' => array('prev')
            ),
            'html' => true
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
  RenderHints::pushToRenderStack('specimen_page');

  $specimen_table = array(
      '#theme' => 'table',
      '#weight' => 2,
      // prefix attributes and rows with '#' to let it pass toF the theme function,
      // otherwise it is handled as child render array

    '#attributes' => array('class' => 'specimens'),
      '#rows' => array(),
      '#prefix' => '<div id="specimens">',
      '#suffix' => '</div>',


  );
  $specimen = compose_cdm_specimen_or_observation($specimen_uuid, true);
  $render_array = array(
          '#theme' => 'item_list',
          '#items' => array($specimen),
          '#type' => 'ul');
  $output = drupal_render($render_array);

  $specimen_table['#rows'][] = array(
          // An array of table rows. Every row is an array of cells, or an associative array
          'data' => $specimen,
          'class' =>  array(
              'descriptionElement',
              'descriptionElement_IndividualsAssociation'
          ),
  );


 // $detail_html = compose_cdm_specimen_or_observation($specimen_uuid, true);
//    $render_array['markup'] = array(
//        '#prefix' => '<div id="specimens" class="page">',
//        'content' => $specimen_table,
//        '#suffix' => '</div>',
//    );
    $render_array['specimen_table'] =  $specimen_table;

  RenderHints::popFromRenderStack();
  return $specimen; // $render_array;
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


/**
 * Returns a drupal render array for a single reference page.
 *
 * Composes a page with all data on a single reference.
 *
 * @param string $uuid
 *   An uuid for a cdm reference.
 *
 * @return array
 *  A drupal render array
 *
 * @throws Exception
 *
 * @ingroup compose
 */
function compose_cdm_reference_page($uuid) {

  $pathelement = "reference_page";
  RenderHints::pushToRenderStack($pathelement);
  $reference = cdm_ws_get(CDM_WS_REFERENCE, $uuid);
  if (!$reference) {
    drupal_set_title(t('Reference does not exist'), PASS_THROUGH);
    return "";
  }
  if (isset($reference->titleCache)) {
    drupal_set_title($reference->titleCache, PASS_THROUGH);
  }

  $field_order = array(
    "title",
    "abbrevTitle",
    // "titleCache" abbrevTitleCache
    // "citation",
    "authorship",
    "editor",
    "publisher",
    "placePublished",
    "datePublished",
    "year",
    "edition",// Class Book.
    "volume",// Class Article.
    "seriesPart",
    "inReference",
    "nomRefBase", // Class BookSection, Book, Article.
    "pages",// Class Article.
    "series",// Class Article, PrintSeries.
    "school",// Class Thesis.
    "institution",// Class Report.
    "organization",// Class Proceedings.
    "nextVersion",
    "previousVersion",
    "isbn",// Class Book.
    "issn",// Class Journal.
    "doi",
    "uri"
  );

  $table_rows = array();

  if (!isset($reference->authorship)) {
    $authorship = cdm_ws_get(CDM_WS_REFERENCE_AUTHORTEAM, $reference->uuid);
    $reference->authorship = isset($authorship->titleCache) ? $authorship->titleCache : '';
  }

  if (!isset($reference->inReference)) {
    $reference->inReference = cdm_ws_get(CDM_WS_REFERENCE, array(
      $reference->uuid,
      "inReference",
    ));
  }

  foreach ($field_order as $fieldname) {

    if (isset($reference->$fieldname)) {

      if ($fieldname == "datePublished") {
        $period = $reference->$fieldname;
        $datePublished = timePeriodToString($period);
        if (isset($datePublished) && $datePublished != '') {
          $table_rows[] = array(
            t("Date published"),
            $datePublished,
          );
        }
      }
      elseif ($fieldname == "doi" && is_object($reference->doi)) {
        $table_rows[] = array(
          t('@fieldname', array('@fieldname' => ucfirst(strtolower($fieldname)))),
          cdm_doi($reference->doi, false)
        );
      }
      elseif ($fieldname == "uri" && isset($reference->uri) && $reference->uri) {
        $table_rows[] = array(
          t('@fieldname', array('@fieldname' => ucfirst(strtolower($fieldname)))),
          cdm_external_uri($reference->uri, false)
        );
      }
      elseif (is_object($reference->$fieldname)) {
        if ($fieldname == "authorship") {
          $dump = $reference->$fieldname;
          $teammembers = "teamMembers";
          $team = $dump->$teammembers;
          $nameArray = array();

          foreach ($team as $member) {
            if (strlen($member->lastname) > 0) {
              $nname = $member->lastname;
              $name = $nname;
              if (strlen($member->firstname) > 0) {
                $vname = $member->firstname;
                $name = $vname . " " . $nname;
              }
              $nameArray[] = $name;
            }
            else {
              if (strlen($member->titleCache) > 0) {
                $nameArray[] = $member->titleCache;
              }
            }
          }
          $value = join($nameArray, ", ");
        }
        elseif ($fieldname == "inReference") {
          $type = $reference->$fieldname->type;
          $value = l($reference->$fieldname->titleCache, path_to_reference($reference->$fieldname->uuid));
          switch ($type) {
            case "Book":
              $fieldname = "in book";
              break;
            case "Journal":
              $fieldname = "in journal";
              break;
            case "Proceedings":
              $fieldname = "in proceedings";
              break;
          }
        }
        else {
          $value = $reference->$fieldname->titleCache;
        }


        if (isset($value) && $value != '') {
          $table_rows[] = array(
            t('@fieldname', array('@fieldname' => ucfirst(strtolower($fieldname)))),
            $value,
          );
        }

      }
      else {
        if (isset($reference->$fieldname) && $reference->$fieldname != '') {
          $table_rows[] = array(
            t('@fieldname', array('@fieldname' => ucfirst(strtolower($fieldname)))),
            $reference->$fieldname,
          );
        }
      }
    }
  }

  $out = theme_table(array(
    'header' => array(),
    'rows' => $table_rows,
    'attributes' => array(
      'class' => html_class_attribute_ref($reference)
    ),
    'caption' => NULL,
    'colgroups' => NULL,
    'sticky' => NULL,
    'empty' => NULL,
  ));

  if(isset($reference->referenceAbstract)){
    $out .= '<h2 class="block-title">Abstract</h2><div class="abstract">' . $reference->referenceAbstract . '</div>';
  }


  // Annotations below the table
  $annotations = cdm_fetch_visible_annotations($reference);
  $out .= theme("cdm_annotations", array('annotations' => $annotations));

  $registration_working_set = cdm_ws_get("registrationWorkingSetDTO", array($uuid));
  if($registration_working_set && count($registration_working_set->registrationDTOs) > 0){
    $out .= "<h3>Nomenclatural acts:</h3><div class=\"cdm-item-list registration-item-list\">";
    foreach($registration_working_set->registrationDTOs as $registration_dto){
      if($registration_dto->status == "PUBLISHED"){
        $registration_render_a = compose_registration_dto_compact($registration_dto, 'citation');
        $registration_render_a["#prefix"] = "<div class=\"item item-registration\">";
        $registration_render_a["#suffix"] = "</div>";
        $out .= drupal_render($registration_render_a);
      }
    }
    $out .= "</div>";
  }

  RenderHints::popFromRenderStack();

  return markup_to_render_array($out);
}

/**
 * Provides the the label string for taxon page tabs.
 *
 * The $tabname as passed to the method will be returned if no override
 * label is configured in the settings.
 */
function cdm_taxonpage_tab_label($tabname) {
  static $taxon_tabs_labels = null;
  if($taxon_tabs_labels == null){
    $taxon_tabs_labels = get_array_variable_merged(CDM_TAXONPAGE_TAB_LABELS, CDM_TAXONPAGE_TAB_LABELS_DEFAULT);
  }
  $tabname_key = strtolower($tabname);
  if(isset($taxon_tabs_labels[$tabname_key]) && $taxon_tabs_labels[$tabname_key]){
    return $taxon_tabs_labels[$tabname_key];
  }
  return $tabname;
}
