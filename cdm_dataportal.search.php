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
 * @todo document ths function.
 */
function cdm_dataportal_search_taxon_form($form, &$form_state, $advancedForm = FALSE) {
  global $theme_key;

  $tdwg_level_select = (isset($_SESSION['cdm']['search']['tdwg_level_select']) ? $_SESSION['cdm']['search']['tdwg_level_select'] : 2);
  $selected_areas = (isset($_SESSION['cdm']['search']['area']) ? $_SESSION['cdm']['search']['area'] : FALSE);

  $query_field_default_value = (isset($_SESSION['cdm']['search']['query']) ? $_SESSION['cdm']['search']['query'] : '');

  $form = cdm_dataportal_search_form_prepare('cdm_dataportal/search/results/taxon', CDM_WS_PORTAL_TAXON_FIND, $query_field_default_value, t('Enter the name or part of a name you wish to search for. The asterisk  character * can always be used as wildcard.'), NULL);

  if (!$advancedForm) {
    $form['query']['#size'] = 20;
  }

  $form['search']['tree'] = array(
    '#weight' => -1,
    '#type' => 'hidden',
    '#value' => get_taxonomictree_uuid_selected(),
  );

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

  if ($advancedForm) {

    // Get presets from settings.
    $preset_doTaxa = variable_get('cdm_search_doTaxa', 1);
    $preset_doSynonyms = variable_get('cdm_search_doSynonyms', 1);
    $preset_doTaxaByCommonNames = variable_get('cdm_search_doTaxaByCommonNames', 0);
    $preset_doMisappliedNames = variable_get('cdm_search_doMisappliedNames', 1);
    $preset_UseDefaults = variable_get('cdm_search_use_default_values', 1);

    // Overwrite presets by user choice stored in session.
    if (isset($_SESSION['cdm']['search']) && !$preset_UseDefaults) {
      $preset_doTaxa = (isset($_SESSION['cdm']['search']['doTaxa']) ? 1 : 0);
      $preset_doSynonyms = (isset($_SESSION['cdm']['search']['doSynonyms']) ? 1 : 0);
      $preset_doMisappliedNames = (isset($_SESSION['cdm']['search']['doMisappliedNames']) ? 1 : 0);
      $preset_doTaxaByCommonNames = (isset($_SESSION['cdm']['search']['doTaxaByCommonNames']) ? 1 : 0);
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
      '#value' => $preset_doTaxaByCommonNames,
    );

    // Geographic Range.
    $form['search']['geographic_range'] = array(
      '#type' => 'fieldset',
      '#weight' => 5,
      '#tree' => TRUE,
      '#title' => t('Geographic range'),
    );

    $form['search']['geographic_range']['tdwg_level_select'] = array(
      '#type' => 'radios',
      '#title' => t('Select a TDWG distribution level and code'),
      '#default_value' => $tdwg_level_select,
      '#options' => array(
        t('TDWG level-1, i.e. a continent'),
        t('TDWG level-2'),
        t('TDWG level-3, i.e. a country'),
        t('TDWG level-4'),
      ),
    );
    $tdwg[1] = cdm_ws_get(CDM_WS_TDWG_LEVEL, '1');
    $tdwg[2] = cdm_ws_get(CDM_WS_TDWG_LEVEL, '2');
    $tdwg[3] = cdm_ws_get(CDM_WS_TDWG_LEVEL, '3');
    $tdwg[4] = cdm_ws_get(CDM_WS_TDWG_LEVEL, '4');

    $tdwg_js = '';
    foreach ($tdwg as $key => $tdwg_level) {
      $tdwgOptions = array();
      $tdwgOptionsSelected = array();
      foreach ($tdwg_level as $area) {
        $representation = $area->representations[0];
        $tdwgOptions[$representation->abbreviatedLabel] = $area->representation_L10n;
        if (is_array($selected_areas) && in_array($representation->abbreviatedLabel, $selected_areas)) {
          // $area->uuid;
          $tdwgOptionsSelected[] = $representation->abbreviatedLabel;
        }
      }
      asort($tdwgOptions);
      $form['search']['geographic_range']['tdwg_level_' . $key] = array(
        '#type' => 'select',
        '#title' => t('TDWG level') . ' ' . $key,
        '#default_value' => $tdwgOptionsSelected,
        '#multiple' => TRUE,
        '#options' => $tdwgOptions,
      );
      $tdwg_js .= "$('#edit-search-geographic-range-tdwg-level-$key').parent()" . ($tdwg_level_select + 1 == $key ? '.show()' : '.hide()') . ";\n";
    }

    drupal_add_js(
    "jQuery(document).ready(function($){
      $(\"input[name='search[geographic_range][tdwg_level_select]']\").change(
        function(){
          var selectId = $(\"input[name='search[geographic_range][tdwg_level_select]']:checked\").val();
          var i;
          for(i = 0; i < 4; i++){
          
            if(selectId == i){
              $('#edit-search-geographic-range-tdwg-level-' + (i + 1) ).parent().fadeIn('slow');
              $('#edit-search-geographic-range-tdwg-level-' + (i + 1)).children().removeAttr('selected');
            } else {
              $('#edit-search-geographic-range-tdwg-level-' + (i + 1)).parent().fadeOut('slow');
              $('#edit-search-geographic-range-tdwg-level-' + (i + 1)).children().removeAttr('selected');
            }
          }
        }
      );

    $tdwg_js
    });",
    array('type' => 'inline'));
  }
  else {
    $preset_doTaxa = variable_get('cdm_search_doTaxa', 1);
    $preset_doSynonyms = variable_get('cdm_search_doSynonyms', 1);
    $preset_doTaxaByCommonNames = variable_get('cdm_search_doTaxaByCommonNames', 0);
    $preset_doMisappliedNames = variable_get('cdm_search_doMisappliedNames', 1);
    $preset_UseDefaults = variable_get('cdm_search_use_default_values', 1);

    // Overwrite presets by user choice stored in session.
    if (isset($_SESSION['cdm']['search']) && !$preset_UseDefaults) {
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
  $featureTree->titleCache; if(is_array($featureTree->root->children) &&
  count($featureTree->root->children) > 0){ // render the hierarchic tree
  structure $treeDetails = '<div class="featuretree_structure">'
  //._featureTree_elements_toString($featureTree->root)
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
 * Filters $_REQUEST by a list of valid request parameters and also sets
 * defaults if required.
 * returns the processed request parameters submitted by the search form and
 * also stores them
 * in $_SESSION['cdm']['search']
 */
function cdm_dataportal_search_form_request() {
  $form_params = array();
  if (is_array($_REQUEST['search'])) {
    array_deep_copy($_REQUEST['search'], $form_params);
  }
  $form_params['query'] = trim($_REQUEST['query']);

  // --- handle geographic range
  // Split of geographic range.
  if (isset($_REQUEST['search']['geographic_range']) && is_array($_REQUEST['search']['geographic_range'])) {
    $geographicRange = $_REQUEST['search']['geographic_range'];
    // Remove from form.
    unset($form_params['geographic_range']);
    $form_params['tdwg_level_select'] = $geographicRange['tdwg_level_select'];
    for ($i = 1; $i < 5; $i++) {
      if (isset($geographicRange['tdwg_level_' . $i])) {
        $form_params['area'] = $geographicRange['tdwg_level_' . $i];
      }
    }
  }

  // Store in session.
  $_SESSION['cdm']['search'] = $form_params;

  return $form_params;
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
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cdm_dataportal_search_execute() {

  // Store as last search in session.
  $_SESSION['cdm']['last_search'] = $_SERVER['REQUEST_URI'];

  // Validate the search webservice parameter:
  if (!cdm_dataportal_search_form_path_for_ws($_REQUEST['ws'])) {// Check is ws.
    // Endpoint is unknown.
    drupal_set_message(t('Invalid search webservice parameter given'));
    return NULL;
  }

  $request_params = cdm_dataportal_search_form_request();
  $taxonPager = cdm_ws_get($_REQUEST['ws'], NULL, queryString($request_params));

  return $taxonPager;
}
