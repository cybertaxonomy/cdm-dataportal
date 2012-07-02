<?php
// $Id$

/*
 * @file
 * cdm_dataportal.search.php
 *
 * search related functions
 *
 * Copyright (C) 2007 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 */



/**
 * prepares a form array for general purpose search functiality in the dataportal.
 * The form returned is populated with all nessecary fields for interla processing
 * and has the textfield element $form['query'] with holds the query term,
 * the sub tree array can be extended to contain additinal search parameters.
 *
 * @param - $action_path the drupal path to be put into the action url to which the form will be submitted.
 * @param - $process the value for #process, defaults to  'cdm_dataportal_search_process'
 * @param - $query_field_default_value
 * @param - $query_field_description
 */
function cdm_dataportal_search_form_prepare($action_path, $process = null, $query_field_default_value, $query_field_description){

  if($process == null){
    $process = 'cdm_dataportal_search_process';
  }
  $form['#method'] = 'get';
  $form['#process'] = array($process => array());
  $form['#action'] = url($action_path, NULL, NULL, true);

  $form['query'] = array(
            '#weight' => 0,
            '#type' => 'textfield',
            '#size' => 68,
            '#attributes' => array('title' => $query_field_description),
            '#value' => $query_field_default_value,
  );

  $form['search'] = array(
          '#weight' => 3,
          '#tree' => true,
  //'#type' => $advancedForm ? 'fieldset': 'hidden',
          '#title' => t('Options')
  );

  // clean URL get forms breaks if we don't give it a 'q'.
  if (!(bool)variable_get('clean_url', '0')) {
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
          '#value' => t('Search')
  );

  return $form;
}

/**
 * @param - $advancedForm
 * @return
 */
function cdm_dataportal_search_taxon_form($advancedForm = false){
  global $theme_key;

  $tdwg_level_select =  (isset($_SESSION['cdm']['search']['tdwg_level_select']) ? $_SESSION['cdm']['search']['tdwg_level_select'] : 2);
  $selected_areas =  (isset($_SESSION['cdm']['search']['area']) ? $_SESSION['cdm']['search']['area'] : false);

  if(isset($_SESSION['cdm']['search']) && !$preset_UseDefaults) {
    $query_field_default_value = (isset($_SESSION['cdm']['search']['query']) ? $_SESSION['cdm']['search']['query'] : '');
  }

  $form = cdm_dataportal_search_form_prepare(
    'cdm_dataportal/search/results/taxon',
  null,
  $query_field_default_value,
  t('Enter the name or part of a name you wish to search for. The asterisk  character * can always be used as wildcard')
  );

  if(!$advancedForm) {
    $form['query']['#size'] = 20;
  }

  $form['search']['tree'] = array(
    '#weight' => -1,
    '#type' => 'hidden',
    '#value' => get_taxonomictree_uuid_selected()
  );


  $form['search']['pageSize'] = array(
      '#weight' => -1,
      '#type' => 'hidden',
      '#value' => variable_get('cdm_dataportal_search_items_on_page', 25)
  );

  $form['search']['pageNumber'] = array(
      '#weight' => -1,
      '#type' => 'hidden',
      '#value' => 0
  );

  if($advancedForm){

    // get presets from settings
    $preset_doTaxa = variable_get('cdm_search_doTaxa', 1);
    $preset_doSynonyms = variable_get('cdm_search_doSynonyms', 1);
    $preset_doTaxaByCommonNames = variable_get('cdm_search_doTaxaByCommonNames', 0);
    $preset_doMisappliedNames = variable_get('cdm_search_doMisappliedNames', 1);
    $preset_UseDefaults = variable_get('cdm_search_use_default_values', 1);

    // overwrite presets by user choice stored in session
    if(isset($_SESSION['cdm']['search']) && !$preset_UseDefaults) {
      $preset_doTaxa = (isset($_SESSION['cdm']['search']['doTaxa']) ? 1 : 0);
      $preset_doSynonyms = (isset($_SESSION['cdm']['search']['doSynonyms']) ? 1 : 0);
      $preset_doMisappliedNames = (isset($_SESSION['cdm']['search']['doMisappliedNames']) ? 1 : 0);
      $preset_doTaxaByCommonNames = (isset($_SESSION['cdm']['search']['doTaxaByCommonNames']) ? 1 : 0);
    }
    // general search parameters

    $form['search']['doTaxa'] = array(
      '#weight' => 2,
      '#type' => 'checkbox',
      '#title' => t('Search for accepted taxa'),
      '#value' => $preset_doTaxa
    );
    $form['search']['doSynonyms'] = array(
      '#weight' => 3,
      '#type' => 'checkbox',
      '#title' => t('Search for synonyms'),
      '#value' => $preset_doSynonyms
    );
    $form['search']['doMisappliedNames'] = array(
          '#weight' => 4,
          '#type' => 'checkbox',
          '#title' => t('Search for misapplied names'),
          '#value' => $preset_doMisappliedNames
    );
    $form['search']['doTaxaByCommonNames'] = array(
      '#weight' => 5,
      '#type' => 'checkbox',
      '#title' => t('Search for common names'),
      '#value' => $preset_doTaxaByCommonNames
    );

    // Geographic Range
    $form['search']['geographic_range'] = array(
      '#type' => 'fieldset',
      '#weight' => 5,
      '#tree' => true,
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
    t('TDWG level-4')
    )
    );
    $tdwg[1] = cdm_ws_get(CDM_WS_TDWG_LEVEL, '1');
    $tdwg[2] = cdm_ws_get(CDM_WS_TDWG_LEVEL, '2');
    $tdwg[3] = cdm_ws_get(CDM_WS_TDWG_LEVEL, '3');
    $tdwg[4] = cdm_ws_get(CDM_WS_TDWG_LEVEL, '4');

    $tdwg_js = '';
    foreach($tdwg as $key=>$tdwg_level){
      $tdwgOptions = array();
      $tdwgOptionsSelected = array();
      foreach($tdwg_level as $area){
        $representation = $area->representations[0];
        $tdwgOptions[$representation->abbreviatedLabel] = $area->representation_L10n;
        if(is_array($selected_areas) && in_array($representation->abbreviatedLabel, $selected_areas)){
          $tdwgOptionsSelected[] = $representation->abbreviatedLabel; //$area->uuid;
        }
      }
      asort($tdwgOptions);
      $form['search']['geographic_range']['tdwg_level_'.$key] = array(
        '#type' => 'select',
        '#title'         => t('TDWG level').' '.$key,
        '#default_value' => $tdwgOptionsSelected,
        '#multiple' => TRUE,
        '#options' => $tdwgOptions
      );
      $tdwg_js .= "$('#edit-search-geographic-range-tdwg-level-$key').parent()".($tdwg_level_select + 1 == $key ?  '.show()' : '.hide()'). ";\n";
    }

    drupal_add_js(
    "$(document).ready(function(){

      $(\"input[@name='search[geographic_range][tdwg_level_select]']\").change(
        function(event){
          var selectId = $(\"input[@name='search[geographic_range][tdwg_level_select]']:checked\").val();
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
    'inline');

  } else {
    $preset_doTaxa = variable_get('cdm_search_doTaxa', 1);
    $preset_doSynonyms = variable_get('cdm_search_doSynonyms', 1);
    $preset_doTaxaByCommonNames = variable_get('cdm_search_doTaxaByCommonNames', 0);
    $preset_doMisappliedNames = variable_get('cdm_search_doMisappliedNames', 1);
    $preset_UseDefaults = variable_get('cdm_search_use_default_values', 1);

    // overwrite presets by user choice stored in session
    if(isset($_SESSION['cdm']['search']) && !$preset_UseDefaults) {
      $preset_doMisappliedNames = (isset($_SESSION['cdm']['search']['doMisappliedNames']) ? 1 : 0);
    }

    $form['search']['doTaxa'] = array(
        '#weight' => -2,
        '#type' => 'hidden',
        '#value' => $preset_doTaxa
    );
    $form['search']['doSynonyms'] = array(
        '#weight' => -3,
        '#type' => 'hidden',
        '#value' => $preset_doSynonyms
    );
    $form['search']['doMisappliedNames'] = array(
        '#weight' => -4,
        '#type' => 'checkbox',
        '#title' => t('Misapplied names'),
        '#value' => $preset_doMisappliedNames
    );
    $form['search']['doTaxaByCommonNames'] = array(
        '#weight' => -5,
        '#type' => 'hidden',
        '#value' => $preset_doTaxaByCommonNames
    );
  }

  return $form;
}

function cdm_dataportal_search_taxon_form_advanced(){
  return cdm_dataportal_search_taxon_form(true);
}

/**
 * Search form for the searching taxa by the findByDescriptionElementFullText rest service.
 */
function cdm_dataportal_search_taxon_by_description_form() {

  $form = cdm_dataportal_search_form_prepare(
      'cdm_dataportal/search/results/taxon',
  null,
  $query_field_default_value,
  t('Enter the text you wish to search for. The asterisk character * can always be used as wildcard. For more syntactial elements please refer to the lucene query syntax reference.')
  );

  return $form;

}

/**
 * Filters $_REQUEST by a list of valid request parameters and also sets defaults if required.
 * returns the processed request parameters submitted by the search form and also stores them
 * in $_SESSION['cdm']['search']
 */
function cdm_dataportal_search_form_request(){


  $form_params = array();
  array_deep_copy($_REQUEST['search'], $form_params);
  $form_params['query'] =  trim($_REQUEST['query']);

  // --- handle geographic range
  // split of  geographic range
  if(is_array($_REQUEST['search']['geographic_range'])){
    $geographicRange = $_REQUEST['search']['geographic_range'];
    // remove from form
    unset($form_params['geographic_range']);
    $form_params['tdwg_level_select'] = $geographicRange['tdwg_level_select'];
    for($i = 1; $i < 5; $i++){
      if(isset($geographicRange['tdwg_level_'.$i])){
        $form_params['area'] = $geographicRange['tdwg_level_'.$i];
      }
    }
  }

  // store in session
  $_SESSION['cdm']['search'] = $form_params;

  return $form_params;
}


/**
 * Implementation #process method call, see form_builder()
 * <p>
 * Removes Drupal internal form elements from query
 * @param $form
 * @return unknown_type
 */
function cdm_dataportal_search_process($form) {
  unset($form['form_id']);
  unset($form['form_token']);
  return $form;
}
