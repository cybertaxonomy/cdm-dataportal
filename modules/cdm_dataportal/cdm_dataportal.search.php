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
 * dataportal. The form returned is populated with all necessary fields
 * for internal processing and has the textfield element $form['query']
 * which holds the query term.
 *
 * @param string $action_path
 *   The Drupal path to be put into the action url to which the form will
 *   be submitted.
 * @param string $search_webservice
 *   The cdm-remote webservice to be used, valid values are defined by
 *   the constants: FIXME.
 * @param string $query_field_default_value
 *   A default text for the query field
 * @param string $query_field_description
 *   The description text for the query field
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
  //
  //  $form['#process'] = array(
  //  $process => array(),
  //  );
  //
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
    // This causes the description to display also when hovering over
    // the textfield.
    // This is wanted behaviour for the simple seach but could
    // be disabled for the advances search.
    '#attributes' => array(
      'title' => $query_field_description,
    ),
    '#description' => $query_field_description,
    '#value' => $query_field_default_value,
    // '#description' => $query_field_description,
  );
  if(variable_get('cdm_dataportal_taxon_auto_suggest')){
      $form['query']['#autocomplete_path'] = 'cdm_dataportal/taxon/autosuggest////';
  }

    $form['search'] = array(
    '#weight' => 3,
    '#tree' => TRUE,
    // '#type' => $advanced_form ? 'fieldset': 'hidden',
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

function cdm_dataportal_taxon_autosuggest($classificationUuid = NULL, $areaUuid = NULL, $status = NULL, $string) {
  $matches = array();

  $queryParams = array();
  $queryParams['query'] = $string.'*';
  if((is_null($classificationUuid) || $classificationUuid=='') && isset($_SESSION['cdm']['taxonomictree_uuid'])){
    $classificationUuid = $_SESSION['cdm']['taxonomictree_uuid'];// if no classification uuid is set use the current one
  }
  if($classificationUuid){
    $queryParams['classificationUuid'] = $classificationUuid;
  }
  if($areaUuid){
    $queryParams['area'] = $areaUuid;
  }
  if($status){
    $queryParams['status'] = $status ;
  }
  $queryParams['pageNumber'] = '0';
  $queryParams['pageSize'] = '10';
  $queryParams['doTaxa'] = true;
  $queryParams['doSynonyms'] = true;
  $queryParams['doMisappliedNames'] = true;
  $queryParams['doTaxaByCommonNames'] = true;

  $search_results = cdm_ws_get(CDM_WS_TAXON_SEARCH, NULL, queryString($queryParams));
  foreach($search_results->records as $record){
      $titleCache = $record->entity->titleCache;
      preg_match('/(.*) sec.*/', $titleCache, $trimmedTitle); //remove sec reference
      $trimmedTitle = trim($trimmedTitle[1]);
      $matches[$trimmedTitle] = check_plain($trimmedTitle);
  }
  drupal_json_output($matches);
}


  /**
 * Creates a search form for searching on taxa.
 *
 * If advanced $advanced_form id TRUE the form will offer additional choices
 *
 * @param array $form
 *   A drupal form array
 * @param array $form_state
 *   The drupal form state passed as reference
 * @param bool $advanced_form
 *   default is FALSE
 * @param bool $classification_select
 *   set TRUE to offer a classification selector in the form - default is FALSE
 *   if only available in the advanced mode
 *
 * @return array
 *   the form array
 */
function cdm_dataportal_search_taxon_form($form, &$form_state, $advanced_form = FALSE, $classification_select = TRUE) {

  $query_field_default_value = (isset($_SESSION['cdm']['search']['query']) ? $_SESSION['cdm']['search']['query'] : '');

  if ($advanced_form || variable_get(SIMPLE_SEARCH_USE_LUCENE_BACKEND, FALSE)) {
    $search_service_endpoint = CDM_WS_PORTAL_TAXON_SEARCH;
  }
  else {
    $search_service_endpoint = CDM_WS_PORTAL_TAXON_FIND;
  }

  $form = cdm_dataportal_search_form_prepare(
    'cdm_dataportal/search/results/taxon',
    $search_service_endpoint,
    $query_field_default_value,
    t('Enter the name or part of a name you wish to search for.
      The asterisk  character * can be used as wildcard, but must not be used as first character.'),
      NULL
  );

  if (!$advanced_form) {
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

  $search_taxa_mode_settings = get_array_variable_merged(
    CDM_SEARCH_TAXA_MODE,
    CDM_SEARCH_TAXA_MODE_DEFAULT
  );
  $preset_do_taxa = $search_taxa_mode_settings['doTaxa'] !== 0;
  $preset_do_synonyms = $search_taxa_mode_settings['doSynonyms'] !== 0;
  $preset_do_taxa_by_common_names = $search_taxa_mode_settings['doTaxaByCommonNames'] !== 0;
  $preset_do_misapplied_names = $search_taxa_mode_settings['doMisappliedNames'] !== 0;

  if ($advanced_form) {

    // --- ADVANCED SEARCH FORM ---
    //

    // Get presets from settings.
    $preset_classification_uuid = get_current_classification_uuid();

    // Overwrite presets by user choice stored in session.
    if (isset($_SESSION['cdm']['search'])) {
      $preset_do_taxa = (isset($_SESSION['cdm']['search']['doTaxa']) ? 1 : 0);
      $preset_do_synonyms = (isset($_SESSION['cdm']['search']['doSynonyms']) ? 1 : 0);
      $preset_do_misapplied_names = (isset($_SESSION['cdm']['search']['doMisappliedNames']) ? 1 : 0);
      $preset_do_taxa_by_common_names = (isset($_SESSION['cdm']['search']['doTaxaByCommonNames']) ? 1 : 0);
      if (isset($_SESSION['cdm']['search']['tree'])) {
        $preset_classification_uuid = $_SESSION['cdm']['search']['tree'];
      }
    }

    if ($classification_select === TRUE) {
      $form['search']['tree'] = array(
        '#title' => t('Classification'),
        '#weight' => 1,
        '#type' => 'select',
        '#default_value' => $preset_classification_uuid,
        '#options' => cdm_get_taxontrees_as_options(TRUE),
        '#description' => t('A filter to limit the search to a specific classification. Choosing <em>--- ALL ---</em> will disable this filter.'),
      );
    }

    // General search parameters.
    $form['search']['doTaxa'] = array(
      '#weight' => 2,
      '#type' => 'checkbox',
      '#title' => t('Include') . ' ' . t('accepted taxa'),
      '#value' => $preset_do_taxa,
    );
    $form['search']['doSynonyms'] = array(
      '#weight' => 3,
      '#type' => 'checkbox',
      '#title' => t('Include') . ' ' . t('synonyms'),
      '#value' => $preset_do_synonyms,
    );
    $form['search']['doMisappliedNames'] = array(
      '#weight' => 4,
      '#type' => 'checkbox',
      '#title' => t('Include') . ' ' . t('misapplied names'),
      '#value' => $preset_do_misapplied_names,
    );
    $form['search']['doTaxaByCommonNames'] = array(
      '#weight' => 5,
      '#type' => 'checkbox',
      '#title' => t('Include') . ' ' . t('common names'),
      '#value' => $preset_do_taxa_by_common_names,
    );

    $area_term_dtos = cdm_ws_fetch_all(
      CDM_WS_DESCRIPTION_NAMEDAREAS_IN_USE,
      array('includeAllParents' => 'true')
    );

    // create map: term_uuid => term
    $term_map = array();
    foreach ($area_term_dtos as $term_dto) {
      $term_map[$term_dto->uuid] = $term_dto;
    }

    $term_tree = array();
    // mixed_vocabularies will contain the uuid vocabularies which
    // also contain terms of foreign vocabularies due to the term
    // hierarchy
    $mixed_vocabularies = array();

    // Build hierarchy of the terms regardless of the vocabulary.
    foreach ($term_map as $term_dto) {
      if (!empty($term_dto->partOfUuid)) {
        // Children.
        $parent =& $term_map[$term_dto->partOfUuid];
        if ($parent) {
          if (!isset($parent->children)) {
            $parent->children = array();
          }
          $parent->children[$term_dto->uuid] = $term_dto;
          if ($parent->vocabularyUuid != $term_dto->vocabularyUuid) {
            $mixed_vocabularies[$parent->vocabularyUuid] = $parent->vocabularyUuid;
          }
        }
      }
      else {
        // group root nodes by vocabulary
        if (!isset($term_tree[$term_dto->vocabularyUuid])) {
          $term_tree[$term_dto->vocabularyUuid] = array();
        }
        $term_tree[$term_dto->vocabularyUuid][$term_dto->uuid] = $term_dto;
      }
    }

    $show_area_filter = ! variable_get(CDM_SEARCH_AREA_FILTER_PRESET, '');

    if($show_area_filter){
      drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/search_area_filter.js');

      drupal_add_js('jQuery(document).ready(function() {
        jQuery(\'#edit-search-areas\').search_area_filter(\'#edit-search-areas-areas-filter\');
      });
      ', array('type' => 'inline'));

      $form['search']['areas'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter by distribution areas'),
        '#description' => t('The search will return taxa having distribution
        information for at least one of the selected areas.') . ' '
          .(count($term_tree) > 1 ? t('The areas are grouped
        by the vocabularies to which the highest level areas belong.') : ''),
      );
      $form['search']['areas']['areas_filter'] = array(
        '#type' => 'textfield',
        '#description' => t('Type to filter the areas listed below.'),
      );
      $vocab_cnt = 0;
      $areas_defaults = array();
      if (isset($_SESSION['cdm']['search']['area'])) {
        $areas_defaults = explode(',', $_SESSION['cdm']['search']['area']);
      }
      _add_js_resizable_element('.resizable-box', true);
      foreach ($term_tree as $vocab_uuid => $term_dto_tree) {
        $vocabulary = cdm_ws_get(CDM_WS_TERMVOCABULARY, array($vocab_uuid));
        $areas_options = term_tree_as_options($term_dto_tree);
        $form['search']['areas']['area'][$vocab_cnt++] = array(
          '#prefix' => '<strong>' . $vocabulary->representation_L10n
            . (isset($mixed_vocabularies[$vocab_uuid]) ? ' <span title="Contains terms of at least one other area vocabulary.">(' . t('mixed') . ')</span>': '')
            . '</strong><div class="resizable-container"><div class="resizable-box">',
          '#type' => 'checkboxes',
          '#default_value' => $areas_defaults,
          '#options' => $areas_options,
          '#suffix' => '</div></div>'
        );
      }
    }

  }
  else {
    // --- SIMPLE SEARCH FORM ---
    //

    // Overwrite presets by user choice stored in session.
    if (isset($_SESSION['cdm']['search'])) {
      $preset_do_misapplied_names = (isset($_SESSION['cdm']['search']['doMisappliedNames']) ? 1 : 0);
    }

    $form['search']['doTaxa'] = array(
      '#weight' => -2,
      '#type' => 'hidden',
      '#value' => $preset_do_taxa,
    );
    $form['search']['doSynonyms'] = array(
      '#weight' => -3,
      '#type' => 'hidden',
      '#value' => $preset_do_synonyms,
    );
    $form['search']['doMisappliedNames'] = array(
      '#weight' => -4,
      '#type' => 'checkbox',
      '#title' => t('Misapplied names'),
      '#value' => $preset_do_misapplied_names,
    );
    $form['search']['doTaxaByCommonNames'] = array(
      '#weight' => -5,
      '#type' => 'hidden',
      '#value' => $preset_do_taxa_by_common_names,
    );
  }

  return $form;
}

/**
 * Wrapper function for cdm_dataportal_search_taxon_form().
 *
 * This function makes ot possible possible to just pass the
 * correct $form_id 'cdm_dataportal_search_taxon_form_advanced' to
 * drupal_get_form like:
 * drupal_get_form('cdm_dataportal_search_taxon_form_advanced');
 *
 * @param array $form
 *   A drupal form array
 * @param array $form_state
 *   The drupal form state passed as reference
 *
 * @return array
 *   The form array
 */
function cdm_dataportal_search_taxon_form_advanced($form, &$form_state) {
  return cdm_dataportal_search_taxon_form($form, $form_state, TRUE);
}

/**
 * Form for searching taxa by the findByDescriptionElementFullText rest service.
 */
function cdm_dataportal_search_taxon_by_description_form() {
  $query_field_default_value = (isset($_SESSION['cdm']['search']['query']) ? $_SESSION['cdm']['search']['query'] : '');

  $form = cdm_dataportal_search_form_prepare(
    'cdm_dataportal/search/results/taxon',
    CDM_WS_PORTAL_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT,
    $query_field_default_value,
    t("Enter the text you wish to search for. The asterisk character * can be
        used as wildcard, but must not be used as first character. Terms can be combined with 'AND'. To search for a
        full phrase enclose the terms in parentheses. For more syntactical
        options please refer to the !link.",
      array(
        '!link' => l(
          t('Apache Lucene - Query Parser Syntax'),
          'http://lucene.apache.org/core/old_versioned_docs/versions/2_9_1/queryparsersyntax.html', array(
            'attributes' => array(
              'absolute' => TRUE,
              'html' => TRUE),
          )
        ),
      )
    ),
    NULL
  );

  $form['search']['tree'] = array(
    '#weight' => -1,
    '#type' => 'hidden',
    '#value' => get_current_classification_uuid(),
  );

  $form['search']['hl'] = array(
    '#weight' => -1,
    '#type' => 'hidden',
    '#value' => 1,
  );

  // Only available to admins:
  if (!isset($_SESSION['cdm']['search']['clazz'])) {
    $_SESSION['cdm']['search']['clazz'] = '';
  }
  if (module_exists("user") && user_access('administer')) {
    $form['search']['clazz'] = array(
      '#type' => 'select',
      '#title' => t('Limit to description item type'),
      '#default_value' => $_SESSION['cdm']['search']['clazz'],
      '#options' => cdm_descriptionElementTypes_as_option(TRUE),
    );
  }

  $profile_feature_tree = get_profile_feature_tree();
  $feature_options = _featureTree_nodes_as_feature_options($profile_feature_tree->root);
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
 * Processes the query parameters of the search form.
 *
 * Reads the query parameters from $_REQUEST and modifies and adds additional
 * query parameters if necessary.
 *
 *  - Filters $_REQUEST by a list of valid request parameters
 *  - modifies geographic_range parameters
 *  - adds taxon tree uuid if it is missing and if it should not be
 *    ignored (parameter value = 'IGNORE')
 *  - and more
 *
 * @param $search_endpoint string
 *    The web service endpoint which will be used for executing the search.
 *    Usually one of CDM_WS_PORTAL_TAXON_SEARCH, CDM_WS_PORTAL_TAXON_FIND,
 *    CDM_WS_PORTAL_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT.
 * @return array
 *   the processed request parameters submitted by the search form and
 *   also stores them in $_SESSION['cdm']['search']
 */
function cdm_dataportal_search_request($search_endpoint)
{

  $form_params = array();

  if (isset($_REQUEST['search']) && is_array($_REQUEST['search'])) {
    array_deep_copy($_REQUEST['search'], $form_params);
  }

  if (isset($_REQUEST['pager']) && is_array($_REQUEST['pager'])) {
    $form_params = array_merge($form_params, $_REQUEST['pager']);
  }

  $form_params['query'] = trim($_REQUEST['query']);

  if($search_endpoint == CDM_WS_PORTAL_TAXON_SEARCH){
    // lucene based taxon search always as phrase search: enclose it in "
    if(!str_beginsWith($form_params['query'], '"')){
      $form_params['query'] = '"' . $form_params['query'];
    }
    if(!str_endsWith($form_params['query'], '"')){
      $form_params['query'] = $form_params['query'] . '"' ;
    }
  }

  // --- handle geographic range
  // Split of geographic range.
  unset($form_params['areas']);

  $area_filter_preset = null;
  if (variable_get(CDM_SEARCH_AREA_FILTER_PRESET, '')) {
    $area_filter_preset = explode(',', variable_get(CDM_SEARCH_AREA_FILTER_PRESET, ''));
  }

  $area_uuids = array();
  if($area_filter_preset){
    $area_uuids = $area_filter_preset;
  }
  elseif (isset($_REQUEST['search']['areas']['area']) && is_array($_REQUEST['search']['areas']['area'])) {
    foreach ($_REQUEST['search']['areas']['area'] as $areas) {
      $area_uuids = array_merge($area_uuids, $areas);
    }
    // The area filter is limited to areas with non absent distribution status
    $presence_terms_options = cdm_vocabulary_as_option(UUID_PRESENCE_ABSENCE_TERM, null, FALSE, array('absenceTerm' => '/false/'));
    $presence_term_uuids = array_keys($presence_terms_options);
    $form_params['status'] = $presence_term_uuids;
  }
  if(count($area_uuids) > 0){
    $form_params['area'] = implode(',', $area_uuids);
  }

  // Store in session.
  $_SESSION['cdm']['search'] = $form_params;

  // ----------- further processing that must not be store in the session --------- //

  // Simple search will not submit a 'tree' query parameter,
  // so we add it here from what is stored in the session unless
  // SIMPLE_SEARCH_IGNORE_CLASSIFICATION is checked in the settings.
  if (!isset($form_params['tree']) && !variable_get(SIMPLE_SEARCH_IGNORE_CLASSIFICATION, 0)) {
    $form_params['tree'] = get_current_classification_uuid();
  }
  // If the 'NONE' classification has been chosen (advanced search)
  // delete the tree information to avoid unknown uuid exceptions in the
  // cdm service.
  if (isset($form_params['tree'])
    && ($form_params['tree'] == 'NONE' || !is_uuid($form_params['tree']))
  ) {
    // $form_params['ignore_classification'] =  TRUE;
    unset($form_params['tree']);
  }
  // else {
  //   $form_params['ignore_classification'] =  NULL;
  // }


  return $form_params;
}

/**
 * Provides the classification to which the last search has been limited to..
 *
 * This function should only be used after the cdm_dataportal_search_execute()
 * handler has been run, otherwise it will return the information from the last
 * search executed. The information is retrieved from
 * the $_SESSION variable:  $_SESSION['cdm']['search']['tree']
 *
 * @return object
 *   the CDM classification instance which has been used a filter for the
 *   last processed search
 *   or NULL, it it was on all classifications
 */
function cdm_dataportal_searched_in_classification() {

  $classification = &drupal_static(__FUNCTION__);

  if (!isset($classification)) {
    if (isset($_SESSION['cdm']['search']['tree'])) {
      $classification = cdm_ws_get(CDM_WS_PORTAL_TAXONOMY, ($_SESSION['cdm']['search']['tree']));
    }
    else {
      $classification = FALSE;
    }
  }

  return $classification !== FALSE ? $classification : NULL;
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
 * Sends a search request to the cdm server.
 *
 * The parameters to build the query are taken obtained by calling
 * cdm_dataportal_search_request() which reads the query parameters
 * from $_REQUEST and add additional query parameters if nessecary.
 *
 * @see cdm_dataportal_search_request()
 */
function cdm_dataportal_search_execute() {

  // Store as last search in session.
  $_SESSION['cdm']['last_search'] = $_SERVER['REQUEST_URI'];

  // Validate the search webservice parameter:
  if (!isset($_REQUEST['ws'])) {
    drupal_set_message(
      t("Invalid search, webservice parameter 'ws' is missing"), 'warning'
    );
    return NULL;
  }
  if (!cdm_dataportal_search_form_path_for_ws($_REQUEST['ws'])) {
    // Endpoint is unknown.
    drupal_set_message(
      t("Invalid search webservice parameter 'ws' given"), 'warning'
    );
    return NULL;
  }

  // Read the query parameters from $_REQUEST and add additional query
  // parameters if necessary.
  $request_params = cdm_dataportal_search_request($_REQUEST['ws']);

  $taxon_pager = cdm_ws_get($_REQUEST['ws'], NULL, queryString($request_params));

  return $taxon_pager;
}

/**
 * Transforms the termDTO tree into options array.
 *
 *   TermDto:
 *      - partOfUuid:
 *      - representation_L10n:
 *      - representation_L10n_abbreviatedLabel:
 *      - uuid:
 *      - vocabularyUuid:
 *      - children: array of TermDto
 *
 * The options array is suitable for drupal form API elements that
 * allow multiple choices.
 * @see http://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#options
 *
 * @param array $term_dto_tree
 *   a hierarchic array of CDM TermDto instances, with additional
 * 'children' field:
 * @param array $options
 *   Internally used for recursive calls
 * @param string $prefix
 *   Internally used for recursive calls
 *
 * @return array
 *   the terms in an array as options for a form element that allows
 *   multiple choices.
 */
function term_tree_as_options($term_dto_tree, &$options = array(), $prefix = '') {

  uasort($term_dto_tree, 'compare_terms_by_order_index');
  foreach ($term_dto_tree as $uuid => $dto) {
    $label = $prefix . '<span class="child-label">'
      .  $dto->representation_L10n
      . '</span><span class="child-label-abbreviated"> (' . $dto->representation_L10n_abbreviatedLabel . ')</span>';
    $options[$uuid] = $label;
    if (isset($dto->children) && is_array($dto->children)) {
      term_tree_as_options(
        $dto->children,
        $options, $prefix
          . '<span data-cdm-parent="' . $uuid . '" class="parent"></span>'
      );
    }
  }

  return $options;
}
