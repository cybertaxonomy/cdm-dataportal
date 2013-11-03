<?php
/**
 * @file
 * Search related functions.
 */

/**
 * Returns a Drupal path to a search form for a CDM webservice.
 *
 * For a given CDM webservice end-point, the drupal page path to the
 * according search form is returned.
 * cdm webservice end points are defined in constant variables like:
 * <code>CDM_WS_PORTAL_TAXON_FIND</code> and
 * <code>CDM_WS_PORTAL_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT</code>
 *
 * @param string $ws_endpoint
 *   The cdm webservice endpoint for which to find the search form path.
 *
 * @return string
 *   The Drupal path found.
 */
function cdm_dataportal_search_form_path_for_ws($ws_endpoint) {
  static $form_ws_map = array(
    CDM_WS_PORTAL_TAXON_FIND => "cdm_dataportal/search",
    CDM_WS_PORTAL_TAXON_SEARCH => "cdm_dataportal/search",
    CDM_WS_PORTAL_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT => "cdm_dataportal/search/taxon_by_description",
  );
  return $form_ws_map[$ws_endpoint];
}

/**
 * Prepares a form array for a general purpose search form.
 *
 * The form is used for general purpose search functionality in the
 * dataportal. The form returned is populated with all nessecary fields
 * for internal processing and has the textfield element $form['query']
 * which holds the query term.
 * he sub tree array can be extended to contain additional search parameters.
 *
 * @param string $action_path
 *   The Drupal path to be put into the action url to which the form will
 *   be submitted.
 * @param string $search_webservice
 *   The cdm-remote webservice to be used, valid values are defined by
 *   the constants: FIXME.
 * @param string $query_field_default_value
 * @param string $query_field_description
 * @param string $process
 *   The value for #process, if NULL (default), 'cdm_dataportal_search_process'
 *   is used.
 *
 * @return array
 *   The prepared form array.
 */
function cdm_dataportal_search_form_prepare($action_path, $search_webservice, $query_field_default_value, $query_field_description, $process = NULL) {
  if ($process == NULL) {
    $process = 'cdm_dataportal_search_process';
  }
  $form['#method'] = 'get';
  /*
  $form['#process'] = array(
    $process => array(),
  );
  */
  $form['#action'] = url($action_path, array(
    'absolute' => TRUE,
  ));

  $form['ws'] = array(
    '#type' => 'hidden',
    '#value' => $search_webservice,
    '#name' => 'ws',
  );

  $form['query'] = array(
    '#weight' => 0,
    '#type' => 'textfield',
    '#size' => 68,
  // Comment @WA: this causes the description to display also when hovering over
  // the textfield.
  // I think this should be removed, although it is currently there in
  // D5 portals.
    '#attributes' => array(
      'title' => $query_field_description,
    ),
    '#description' => $query_field_description,
    '#value' => $query_field_default_value,
    // '#description' => $query_field_description,
  );

  $form['search'] = array(
    '#weight' => 3,
    '#tree' => TRUE,
    // '#type' => $advancedForm ? 'fieldset': 'hidden',
    '#title' => t('Options'),
  );

  // Clean URL get forms breaks if we don't give it a 'q'.
  if (!(bool) variable_get('clean_url', '0')) {
    $form['search']['q'] = array(
      '#type' => 'hidden',
      '#value' => $action_path,
      '#name' => 'q',
    );
  }

  $form['submit'] = array(
    '#weight' => 5,
    '#type' => 'submit',
    '#name' => '',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * Creates a search form for searching on taxa.
 *
 * If advanced $advancedForm id TRUE the form will offer additinal choices
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 * @param bool $advancedForm
 *    default is FALSE
 * @param bool $classificationSelect
 *    set TRUE to offer a classifiaction selector in the form - default is FALSE
 *    if only available in the advanced mode
 *
 * @return
 *   the form array
 *
 */
function cdm_dataportal_search_taxon_form($form, &$form_state, $advancedForm = FALSE, $classificationSelect = TRUE) {
  global $theme_key;

  $tdwg_level_select = (isset($_SESSION['cdm']['search']['tdwg_level_select']) ? $_SESSION['cdm']['search']['tdwg_level_select'] : 2);
  $selected_areas = (isset($_SESSION['cdm']['search']['area']) ? $_SESSION['cdm']['search']['area'] : FALSE);
  $query_field_default_value = (isset($_SESSION['cdm']['search']['query']) ? $_SESSION['cdm']['search']['query'] : '');



  if ($advancedForm) {
    $form = cdm_dataportal_search_form_prepare('cdm_dataportal/search/results/taxon', CDM_WS_PORTAL_TAXON_SEARCH, $query_field_default_value, t('Enter the name or part of a name you wish to search for. The asterisk  character * can always be used as wildcard.'), NULL);
  } else {
    // using CDM_WS_PORTAL_TAXON_SEARCH in all cases, for testing or the origial CDM_WS_PORTAL_TAXON_FIND for production
    $form = cdm_dataportal_search_form_prepare('cdm_dataportal/search/results/taxon', CDM_WS_PORTAL_TAXON_FIND, $query_field_default_value, t('Enter the name or part of a name you wish to search for. The asterisk  character * can always be used as wildcard.'), NULL);
//     $form = cdm_dataportal_search_form_prepare('cdm_dataportal/search/results/taxon', CDM_WS_PORTAL_TAXON_SEARCH, $query_field_default_value, t('Enter the name or part of a name you wish to search for. The asterisk  character * can always be used as wildcard.'), NULL);

    $form['query']['#size'] = 20;
  }

  $form['search']['pageSize'] = array(
    '#weight' => -1,
    '#type' => 'hidden',
    '#value' => variable_get('cdm_dataportal_search_items_on_page', 25),
  );

  $form['search']['pageNumber'] = array(
    '#weight' => -1,
    '#type' => 'hidden',
    '#value' => 0,
  );

  $search_taxa_mode_settings = get_array_variable_merged(CDM_SEARCH_TAXA_MODE, CDM_SEARCH_TAXA_MODE_DEFAULT);
  $preset_doTaxa = $search_taxa_mode_settings['doTaxa'] !== 0;
  $preset_doSynonyms = $search_taxa_mode_settings['doSynonyms'] !== 0;
  $preset_doTaxaByCommonNames = $search_taxa_mode_settings['doTaxaByCommonNames'] !== 0;
  $preset_doMisappliedNames = $search_taxa_mode_settings['doMisappliedNames'] !== 0;

  if ($advancedForm) {

    // --- ADVANCED SEARCH FORM ---
    //

    // Get presets from settings.
    $preset_classification_uuid = get_taxonomictree_uuid_selected();

    // Overwrite presets by user choice stored in session.
    if (isset($_SESSION['cdm']['search'])) {
      $preset_doTaxa = (isset($_SESSION['cdm']['search']['doTaxa']) ? 1 : 0);
      $preset_doSynonyms = (isset($_SESSION['cdm']['search']['doSynonyms']) ? 1 : 0);
      $preset_doMisappliedNames = (isset($_SESSION['cdm']['search']['doMisappliedNames']) ? 1 : 0);
      $preset_doTaxaByCommonNames = (isset($_SESSION['cdm']['search']['doTaxaByCommonNames']) ? 1 : 0);
      if (isset($_SESSION['cdm']['search']['tree'])) {
        $preset_classification_uuid = $_SESSION['cdm']['search']['tree'];
      }
    }


   if ($classificationSelect === TRUE) {
      $form['search']['tree'] = array(
        '#title' => t('Classification'),
        '#weight' => 1,
        '#type' => 'select',
        '#default_value' => get_taxonomictree_uuid_selected(),
        '#options' => cdm_get_taxontrees_as_options(TRUE),
        '#description' => t('A filter to limit the search to a specific classification.')
      );
   }

    // General search parameters.
    $form['search']['doTaxa'] = array(
      '#weight' => 2,
      '#type' => 'checkbox',
      '#title' => t('Search for accepted taxa'),
      '#value' => $preset_doTaxa,
    );
    $form['search']['doSynonyms'] = array(
      '#weight' => 3,
      '#type' => 'checkbox',
      '#title' => t('Search for synonyms'),
      '#value' => $preset_doSynonyms,
    );
    $form['search']['doMisappliedNames'] = array(
      '#weight' => 4,
      '#type' => 'checkbox',
      '#title' => t('Search for misapplied names'),
      '#value' => $preset_doMisappliedNames,
    );
    $form['search']['doTaxaByCommonNames'] = array(
      '#weight' => 5,
      '#type' => 'checkbox',
      '#title' => t('Search for common names'),
      '#value' => $preset_doTaxaByCommonNames
    );

    $areas_options = cdm_terms_as_options(cdm_ws_fetch_all(CDM_WS_DESCRIPTION_NAMEDAREAS_IN_USE));
    $areas_defaults = array();
    if(isset($_SESSION['cdm']['search']['area'])){
      $areas_defaults = explode(',', $_SESSION['cdm']['search']['area']);
    }
    $form['search']['area'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Filter by distribution areas'),
      '#default_value' =>  $areas_defaults,
      '#options' => $areas_options,
      '#description' => t('Check one or multiple areas to filter by distribution.')
    );

  } else {
    // --- SIMPLE SEARCH FORM ---
    //

    // Overwrite presets by user choice stored in session.
    if (isset($_SESSION['cdm']['search'])) {
      $preset_doMisappliedNames = (isset($_SESSION['cdm']['search']['doMisappliedNames']) ? 1 : 0);
    }

    $form['search']['doTaxa'] = array(
      '#weight' => -2,
      '#type' => 'hidden',
      '#value' => $preset_doTaxa,
    );
    $form['search']['doSynonyms'] = array(
      '#weight' => -3,
      '#type' => 'hidden',
      '#value' => $preset_doSynonyms,
    );
    $form['search']['doMisappliedNames'] = array(
      '#weight' => -4,
      '#type' => 'checkbox',
      '#title' => t('Misapplied names'),
      '#value' => $preset_doMisappliedNames,
    );
    $form['search']['doTaxaByCommonNames'] = array(
      '#weight' => -5,
      '#type' => 'hidden',
      '#value' => $preset_doTaxaByCommonNames,
    );
  }

  return $form;
}
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cdm_dataportal_search_taxon_form_advanced($form, &$form_state) {
  return cdm_dataportal_search_taxon_form($form, $form_state, TRUE);
}

/**
 * Search form for the searching taxa by the findByDescriptionElementFullText
 * rest service.
 */
function cdm_dataportal_search_taxon_by_description_form() {
  $query_field_default_value = (isset($_SESSION['cdm']['search']['query']) ? $_SESSION['cdm']['search']['query'] : '');

  $form = cdm_dataportal_search_form_prepare(
      'cdm_dataportal/search/results/taxon',
      CDM_WS_PORTAL_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT,
      $query_field_default_value,
      t("Enter the text you wish to search for. The asterisk character * can be
        used as wildcard. Terms can be combined with 'AND'. To search for a
        full phrase enclose the terms in parentheses. For more syntactial
        options please refer to the !link.", array(
          '!link' => l(t('Apache Lucene - Query Parser Syntax'), 'http://lucene.apache.org/core/old_versioned_docs/versions/2_9_1/queryparsersyntax.html', array(
             'attributes' => array('absolute' => TRUE, 'html' => TRUE))),
        )),
      NULL
      );

  $form['search']['tree'] = array(
    '#weight' => -1,
    '#type' => 'hidden',
    '#value' => get_taxonomictree_uuid_selected(),
  );

  $form['search']['hl'] = array(
    '#weight' => -1,
    '#type' => 'hidden',
    '#value' => 1,
  );

  // Only avaiable to admins:
  if (!isset($_SESSION['cdm']['search']['clazz'])) {
    $_SESSION['cdm']['search']['clazz'] = '';
  }
  if (module_exists("user") && user_access('administer')) {
    $form['search']['clazz'] = array(
      '#type' => 'select',
      '#title' => t('Limit to DescriptionElement type'),
      '#default_value' => $_SESSION['cdm']['search']['clazz'],
      '#options' => cdm_descriptionElementTypes_as_option(TRUE),
    );
  }

  /*
  see cdm_get_featureTrees_as_options() ... $treeRepresentation =
  $featureTree->titleCache; if(is_array($featureTree->root->childNodes) &&
  count($featureTree->root->childNodes) > 0){ // render the hierarchic tree
  structure $treeDetails = '<div class="featuretree_structure">'
  //.cdm_featureTree_elements_toString($featureTree->root)
  .theme('featureTree_hierarchy', $featureTree->uuid) .'</div>'; $form =
  array(); $form['featureTree-'.$featureTree->uuid] = array( '#type' =>
  'fieldset', '#title' => t('Show details'), '#collapsible' => TRUE,
  '#collapsed' => TRUE, );
  $form['featureTree-'.$featureTree->uuid]['details'] =
  array('#value'=>$treeDetails); $treeRepresentation .= drupal_render($form);
  */

  $profile_featureTree = get_profile_featureTree();
  $feature_options = _featureTree_nodes_as_feature_options($profile_featureTree->root);
  if (isset($_SESSION['cdm']['search']['features'])) {
    $form['search']['features'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Limit to selected features'),
      '#default_value' => $_SESSION['cdm']['search']['features'],
      '#options' => $feature_options,
    );
  }
  else {
    $form['search']['features'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Limit to selected features'),
      '#options' => $feature_options,
    );
  }
  return $form;
}

/**
 * Reads the query parameters from $_REQUEST and modifies and adds additional query parameters if nessecary.
 *
 *  - Filters $_REQUEST by a list of valid request parameters
 *  - modifies geographic_range parameters
 *  - adds taxon tree uuid if it is missing and if it should not be
 *    ignored (parameter value = 'IGNORE')
 *  - and more
 *
 *
 * @return
 *   the processed request parameters submitted by the search form and
 *   also stores them in $_SESSION['cdm']['search']
 */
function cdm_dataportal_search_form_request() {

  $form_params = array();

  if (isset($_REQUEST['search']) && is_array($_REQUEST['search'])) {
    array_deep_copy($_REQUEST['search'], $form_params);
  }

  if (isset($_REQUEST['pager']) && is_array($_REQUEST['pager'])) {
    $form_params = array_merge($form_params, $_REQUEST['pager']);
  }

  $form_params['query'] = trim($_REQUEST['query']);

  // --- handle geographic range
  // Split of geographic range.
  unset($form_params['area']);
  if (isset($_REQUEST['search']['area']) && is_array($_REQUEST['search']['area'])) {
    $form_params['area'] = implode(',', $_REQUEST['search']['area']);
  }

  // simple search will not submit a 'tree' query parameter, so we add it here from
  // what is stored in the session unless 'simple_search_ignore_classification'
  // is checked in the settings
  if (!isset($form_params['tree']) && !variable_get('simple_search_ignore_classification', 1)) {
    $form_params['tree'] = get_taxonomictree_uuid_selected();
  }
  // if the 'NONE' classification has been chosen (adanced search) delete the tree information
  // to avoid unknown uuid exceptions in the cdm service
  if (isset($form_params['tree']) && ($form_params['tree'] == 'NONE' || ! is_uuid($form_params['tree']))) {
//     $form_params['ignore_classification'] =  TRUE;
    unset($form_params['tree']);
  }
//   else {
//     $form_params['ignore_classification'] =  NULL;
//   }

  // Store in session.
  $_SESSION['cdm']['search'] = $form_params;

  return $form_params;
}

/**
 * Provides the classification the last search has been run on if any.
 *
 * This function should only be used after the cdm_dataportal_search_execute() handler has been run,
 * otherwise it will return the infomation from the last search executed. The information is retrieved from
 * the $_SESSION variable:  $_SESSION['cdm']['search']['tree']
 *
 * @return
 *    the CDM classification instance which has been used a filter for the last processed search
 *    or NULL, it it was on all classifications
 */
function cdm_dataportal_searched_in_classification() {

  $classification = &drupal_static(__FUNCTION__);

  if (!isset($classification)) {
    if (isset($_SESSION['cdm']['search']['tree'])) {
      $classification = cdm_ws_get(CDM_WS_PORTAL_TAXONOMY, ($_SESSION['cdm']['search']['tree']));
    } else {
      $classification = FALSE;
    }
  }

  return $classification !== FALSE ?  $classification : NULL;
}

/**
 * Removes Drupal internal form elements from query.
 */
function cdm_dataportal_search_process($form, &$form_state) {
  unset($form['form_id']);
  unset($form['form_token']);
  return $form;
}

/**
 * Sends a search request at the cdm web server.
 *
 * The parameters to build the query are taken obtained by calling
 * cdm_dataportal_search_form_request() which reads the query parameters
 * from $_REQUEST and add additional query parameters if nessecary.
 *
 * @see cdm_dataportal_search_form_request()
 *
 */
function cdm_dataportal_search_execute() {

  // Store as last search in session.
  $_SESSION['cdm']['last_search'] = $_SERVER['REQUEST_URI'];

  // Validate the search webservice parameter:
  if (!isset($_REQUEST['ws'])) {// Check is ws.
    // Endpoint is unknown.
    drupal_set_message(t('webservice parameter \'ws\' missing'), 'warning');
    return NULL;
  }
  if (!cdm_dataportal_search_form_path_for_ws($_REQUEST['ws'])) {// Check is ws.
    // Endpoint is unknown.
    drupal_set_message(t('Invalid search webservice parameter  \'ws\' given'), 'warning');
    return NULL;
  }

  // read the query parameters from $_REQUEST and add additional query parameters if nessecary.
  $request_params = cdm_dataportal_search_form_request();

  $taxonPager = cdm_ws_get($_REQUEST['ws'], NULL, queryString($request_params));

  return $taxonPager;
}
