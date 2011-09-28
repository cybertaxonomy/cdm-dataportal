<?php

define('DEFAULT_TAXONTREE_RANKLIMIT', '1b11c34c-48a8-4efa-98d5-84f7f66ef43a');//TODO Genus UUID
define('CDM_TAXONOMICTREE_UUID', 'cdm_taxonomictree_uuid');

define('CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE', 25);
define('CDM_DATAPORTAL_NOMREF_IN_TITLE', 1);
define('CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR', 0);
define('CDM_DATAPORTAL_ALL_FOOTNOTES', 0);
define('CDM_DATAPORTAL_ANNOTATIONS_FOOTNOTES', 0);
define('CDM_DATAPORTAL_LAST_VISITED_TAB_ARRAY_INDEX', 4);

/* annotationTypeKeys */
$annotationTypeKeys = array_keys( cdm_Vocabulary_as_option(UUID_ANNOTATION_TYPE) );
if(in_array(UUID_ANNOTATION_TYPE_TECHNICAL, $annotationTypeKeys)) {
  $annotationTypeKeys = array_flip($annotationTypeKeys);
  // technical annotation are off by default
  unset($annotationTypeKeys[UUID_ANNOTATION_TYPE_TECHNICAL]);
  $annotationTypeKeys = array_flip($annotationTypeKeys);
}
define('ANNOTATIONS_TYPES_AS_FOOTNOTES_DEFAULT', serialize($annotationTypeKeys));

/* taxonRelationshipTypes */
define('CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT', serialize(array(UUID_MISAPPLIED_NAME_FOR, UUID_INVALID_DESIGNATION_FOR)));


/* gallery variables */
$gallery_settings = array(
    "cdm_dataportal_show_taxon_thumbnails" => 1,
    "cdm_dataportal_show_synonym_thumbnails" => 0,
    "cdm_dataportal_show_thumbnail_captions" => 1,
    "cdm_dataportal_media_maxextend" => 120,
    "cdm_dataportal_media_cols" => 3,
    "cdm_dataportal_media_maxRows" => 1);


$taxon_tab_options = array(
  0 => 'General',
  1 => 'Synonymy',
  2 => 'Images',
  3 => 'Specimens',
  CDM_DATAPORTAL_LAST_VISITED_TAB_ARRAY_INDEX => 'Last visited tab',
);

define('EDIT_MAPSERVER_V1_URI', 'http://edit.br.fgov.be/edit_wp5/v1');
define('EDIT_MAPSERVER_V11_URI', 'http://edit.br.fgov.be/edit_wp5/v1.1');
define('DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP', 'distribution_textdata_on_top');

// --- Taxon profile settings --- /
define (LAYOUT_SETTING_PREFIX, 'layout_');
define (FEATURE_TREE_LAYOUT_DEFAULTS, serialize(
    array(
    'enabled'=> false,
    'enclosingTag' => 'ul',
    'entryEnclosingTag' => 'li',
    'glue' => ' '
    )
  ));


define('CDM_DATAPORTAL_DEFAULT_TAXON_TAB', serialize($taxon_tab_options));
define('CDM_DATAPORTAL_GALLERY_SETTINGS', serialize($gallery_settings));
define('CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME', 'specimen_gallery');
define('CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME', "description_gallery");
define('CDM_DATAPORTAL_MEDIA_GALLERY_NAME', "media_gallery");
define('CDM_DATAPORTAL_TAXON_MEDIA_GALLERY_NAME_TAB', "taxon_tab_media_gallery");
define('CDM_DATAPORTAL_SEARCH_GALLERY_NAME', "search_gallery");
define('CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS', 'cdm_dataportal_display_taxon_relationships');
define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS', 'cdm_dataportal_display_name_relations');
//define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_2', array("default" => t('Display all')));
define('CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT', 1);
define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_DEFAULT', 1);
define('CDM_TAXON_RELATIONSHIP_TYPES', 'cdm_taxon_relationship_types');
define('CDM_DATAPORTAL_DEFAULT_FEATURETREE_UUID', 'cdm_dataportal_featuretree_uuid');
define('CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID', 'cdm_dataportal_structdesc_featuretree_uuid');

function getGallerySettings($gallery_config_form_name){
  $default_values = unserialize(CDM_DATAPORTAL_GALLERY_SETTINGS);
  return variable_get($gallery_config_form_name, $default_values);
}

function get_default_taxon_tab($index = false) {

  global $user;
  $values = unserialize(CDM_DATAPORTAL_DEFAULT_TAXON_TAB);
  $user_tab_active = 'cdm_dataportal_' .$user->uid . '_default_tab_active';
  $user_tab = 'cdm_dataportal_' .$user->uid . '_default_tab';
  //get the user value
  $user_tab_on = variable_get($user_tab_active, false);
  if($user_tab_on){
    $user_value = variable_get($user_tab, 0);
    $index_value = $user_value;
  //get the system value
  }else{
    $system_value = variable_get('cdm_dataportal_default_tab', 0);
    $index_value = $system_value;
  }
  if (!index){
     return ($values[$index_value]);
  }else{
     return $index_value;
  }


  switch ($value){
    case 0:
      $res = 'General';
      break;
    case 1:
      $res = 'Synonymy';
      break;
    case 2:
      $res = 'Images';
      break;
    case 3:
      $res = 'Specimens';
      break;
    case 4:
      $res = 'last_visited_tab';
      break;
    default:
      $res = 'General';
  }
  return $res;
}

function cdm_dataportal_menu_admin($may_cache, &$items){

  if (!$may_cache) {

    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal',
      'title' => t('CDM Dataportal'),
      'description' => t('Setting for the CDM DataPortal'),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_general'),
      'type' => MENU_NORMAL_ITEM,
    );

    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/general',
      'title' => t('General'),
      'description' => t('Setting for the CDM DataPortal'),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_general'),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
    );

    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/cachesite',
      'title' => t('Cache'),
      'description' => t('Cache'),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_cache'),
      'weight' => 10,
      'type' => MENU_LOCAL_TASK,
    );

    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/geo',
      'title' => t('Geo & Map'),
      'description' => t('Geo & Map'),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_geo'),
      'weight' => 1,
      'type' => MENU_LOCAL_TASK,
    );

    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/layout',
      'title' => t('Layout'),
      'description' => t('Configure and adjust the layout of your DataPortal '),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_layout'),
      'weight' => 2,
      'type' => MENU_LOCAL_TASK,
    );

    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/layout/taxon',
      'title' => t('Taxon'),
      'description' => t('Configure and adjust the layout of your DataPortal '),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_layout_taxon'),
      'weight' => 1,
      'type' => MENU_LOCAL_TASK,
    );
/*
    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/layout/synonymy',
      'title' => t('Synonymy'),
      'description' => t('Configure and adjust the layout of your DataPortal '),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_layout_synonymy'),
      'weight' => 1,
      'type' => MENU_LOCAL_TASK,
    );

    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/layout/specimens',
      'title' => t('Specimens'),
      'description' => t('Configure and adjust the layout of your DataPortal '),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_layout_specimens'),
      'weight' => 1,
      'type' => MENU_LOCAL_TASK,
    );
*/
    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/layout/search',
      'title' => t('Search'),
      'description' => t('Configure and adjust the layout of your DataPortal '),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_layout_search'),
      'weight' => 2,
      'type' => MENU_LOCAL_TASK,
    );

    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/layout/media',
      'title' => t('Media'),
      'description' => t('Configure and adjust the layout of your DataPortal '),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_settings_layout_media'),
      'weight' => 3,
      'type' => MENU_LOCAL_TASK,
    );
/*   Path to banners configuration (DEFAULT THEME)
    $items[] = array(
      'path' => 'admin/settings/cdm_dataportal/layout/theme',
      'title' => t('Theme'),
      'description' => t('Configure the drupal theme of your DataPortal '),
      'access' => user_access('administer cdm_dataportal'),
      'callback' => 'drupal_get_form',
      'callback arguments' => array('cdm_dataportal_theming_form'),
      'weight' => 4,
      'type' => MENU_LOCAL_TASK,
        );
*/
  }


}

function cdm_help_general_cache(){
  $form = array();
  $form['cache_help'] = array(
    '#type' => 'fieldset',
  '#title' => t('Help'),
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  );
  $form['cache_help']['test'] = array('#value' => t('probando'));
  return drupal_render($form);
  $res = array();
  $res['default'] = drupal_render($help);
  return $res;
}

/**
 * Configures the settings form for the CDM-API module.
 *
 * @return Array Drupal settings form
 */
function cdm_settings_general(){

  $form['cdm_webservice'] = array(
      '#type' => 'fieldset',
      '#title' => t('CDM Server'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    '#description' => t('<em>CDM Server</em> makes possible the dialogue with
                         <em>CDM Data Portal</em> thanks to his web services.'),
  );

  $form['cdm_webservice']['cdm_webservice_url'] =  array(
    '#type' => 'textfield',
    '#title'         => t('CDM web service URL'),
    '#description'   => t('This is the ip address of the location of the CDM Web Server which contains
                           your collection database. The address must follow the format <em>"http://X:Y/Z"</em>
                           where "<em>X</em>" is the ip address of the machine where the server is running, "<em>Y</em>" is
                           the port number where the server is listening and "<em>Z</em>" the name of the database
                           where your collection is, e.g. <em>"http://160.45.63.201:8080/palmae"</em>'),
    '#default_value' => variable_get('cdm_webservice_url', NULL),
  );

  /** MOVED TO DATAPORTAL
  $form['cdm_webservice']['taxontree_ranklimit'] =  array(
    '#type'          => 'select',
    '#title'         => t('Rank of highest displayed taxon'),
    '#default_value' => variable_get('taxontree_ranklimit', DEFAULT_TAXONTREE_RANKLIMIT), //before DEFAULT_TAXONTREE_RANKLIMIT_UUID
    '#options'       => cdm_rankVocabulary_as_option(),
    '#description'   => t('The rank of the highest displayed taxon in the <em>taxontree</em>. When you explore
                           your collection, you can navigate it through a tree structure (<em>taxontree</em>). You can
                           select here which rank should be at the top level of the tree structure.'),
  );
  */
/*
  $form['cdm_webservice']['cdm_webservice_cache'] =  array(
    '#type' => 'checkbox',
    '#title'         => t('<b>Enable caching</b>'),
  '#options'     => cdm_help_general_cache(),
    '#default_value' => variable_get('cdm_webservice_cache', 1),
    '#description'   => t('When caching is enabled all single taxon sites are stored in an internal drupal cache doing
                           the portal response of taxa sites faster. This is possible because the sites are loaded from
                           the cache and are not created from scratch.
                           You can manage and find more information about the cache at the <a href="./?q=admin/settings/cdm_dataportal/cachesite">cache configuration site</a>.<br>' .
                         '<b>Note:</b> If taxa are modified by the editor or any other application the changes will be not
                         visible till the cache is erased. Therefore developers should deactived this feature when they
                         are working on the CDM Dataportal Module')
    );
*/
    $form['cdm_webservice']['cdm_webservice_debug'] =  array(
    '#type' => 'checkbox',
    '#title'         => t('<b>Debug CDM Web Service</b>'),
    '#default_value' => variable_get('cdm_webservice_debug', 1),
    '#description'   => t('When enabled is possible to see which web services from CDM Server have been called and its
                           results. A black box will appear at the top of the web site with the information.<br>' .
                          '<b>Note:</b> this is meanly a feature for developers.')
    );

    $form['cdm_webservice']['proxy'] = array(
      '#type' => 'fieldset',
      '#title' => t('Proxy'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE
    );

    $form['cdm_webservice']['proxy']['cdm_webservice_proxy_url'] =  array(
    '#type' => 'textfield',
    '#title'         => t('Proxy URL'),
    '#description'   => t('If this proxy url is set the cdm api tries
    to connect the web service over the given proxy server.
    Otherwise proxy usage is deactivated.'),
    '#default_value' => variable_get('cdm_webservice_proxy_url', false),
    );

    $form['cdm_webservice']['proxy']['cdm_webservice_proxy_port'] =  array(
    '#type' => 'textfield',
    '#title'         => t('Proxy port'),
    '#default_value' => variable_get('cdm_webservice_proxy_port', '80'),
    );

    $form['cdm_webservice']['proxy']['cdm_webservice_proxy_usr'] =  array(
    '#type' => 'textfield',
    '#title'         => t('Login'),
    '#default_value' => variable_get('cdm_webservice_proxy_usr', false),
    );

    $form['cdm_webservice']['proxy']['cdm_webservice_proxy_pwd'] =  array(
    '#type' => 'textfield',
    '#title'         => t('Password'),
    '#default_value' => variable_get('cdm_webservice_proxy_pwd', false),
    );

    //TODO: settings are still incomplete, compare with trunk/dataportal/inc/config_default.php.inc
    $form['cdm_dataportal'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxon Tree'),
      '#collapsible' => FALSE,
      '#collapsed' => TRUE,
      '#description' => t('<p>When you explore your collection, you can navigate it through a
                           tree structure also called <em>Taxon Tree</em>.</p><p>To be able to navigate through
                           your collection the
                           <a href="http://drupal.org/handbook/blocks">drupal block</a>
                           <em>CDM Taxon Tree</em> should be visible for users. Enable the block at
                           <a href="./?q=admin/build/block">Administer&#45&#62Site building&#45&#62Blocks</a></p>'),
    );

    $form['cdm_dataportal'][CDM_TAXONOMICTREE_UUID] = array(
      '#type' => 'select',
      '#title'         => t('Available classifications'),
      '#default_value' => variable_get(CDM_TAXONOMICTREE_UUID, false),
      '#options' => cdm_get_taxontrees_as_options(),
      '#description'   => t('Select the default taxa classification for your <em>taxon tree</em>,
                             the other classifications will be also available but with a manual user change.')
    );

    $form['cdm_dataportal']['taxontree_ranklimit'] =  array(
    '#type'          => 'select',
    '#title'         => t('Rank of highest displayed taxon'),
    '#default_value' => variable_get('taxontree_ranklimit', DEFAULT_TAXONTREE_RANKLIMIT), //before DEFAULT_TAXONTREE_RANKLIMIT_UUID
    '#options'       => cdm_rankVocabulary_as_option(),
    '#description'   => t('This is the rank of the highest displayed taxon in the <em>taxon tree</em>. You can
                           select here which rank should be at the top level of the tree structure.'),
  );

    return system_settings_form($form);
}


/**
 * LAYOUT settings
 * @return unknown_type
 */
function cdm_settings_layout(){

  //drupal_goto('admin/settings/cdm_dataportal/layout/taxon');
  $form = array();
/*
  // -- tabbed pages -- //
  $form['cdm_dataportal_taxonpage_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabbed taxon page'),
    '#default_value' => variable_get('cdm_dataportal_taxonpage_tabs', 1),
    '#description' => t('If selected split the taxon page into individual tabs for description, images, synonymy. If not the taxon data is renderized as a long single page without tabs.')
  );
*/
  $form['gen_layout'] = array(
    '#type' => 'fieldset',
      '#title' => t('Portal Layout'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
      '#description' => t('This settings contains the general configurations layout. If you want to configure the specific sites layout visit the respective configuration site for taxon, search or media.'),
  );

  //---- footnotes ---//
  $form['gen_layout']['footnotes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Footnotes'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    '#description' => t('Taxa data such authors, synonyms names, descriptions, media or distribution areas may have annotations or footnotes. When the footnotes are enabled
                         they will be visible (if they exist).'),
  );

  $form['gen_layout']['footnotes']['cdm_dataportal_all_footnotes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Do not show footnotes'),
      '#default_value' => variable_get('cdm_dataportal_all_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES),
      '#description' => t('Check this if you do not want to show any footnotes')
  );

  $form['gen_layout']['footnotes']['cdm_dataportal_annotations_footnotes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Do not show annotations footnotes'),
      '#default_value' => variable_get('cdm_dataportal_annotations_footnotes', CDM_DATAPORTAL_ANNOTATIONS_FOOTNOTES),
      '#description' => t('Check this if you do not want to show annotation footnotes')
  );

  $annotationTypeOptions = cdm_Vocabulary_as_option(UUID_ANNOTATION_TYPE);
  $form['gen_layout']['footnotes']['annotations_types_as_footnotes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Annotation types as footnotes'),
      '#description' => t('Only annotations of the selected type will be displayed as footnotes. You may want to turn \'technical annotations\' off. Untyped annotations will always be displayed.'),
      '#options' => $annotationTypeOptions,
      '#default_value' => variable_get('annotations_types_as_footnotes', unserialize(ANNOTATIONS_TYPES_AS_FOOTNOTES_DEFAULT))

  );

  //--- Advanced Search ---//
  $form['gen_layout']['asearch'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced search'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['gen_layout']['asearch']['cdm_dataportal_show_advanced_search'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show advanced search link'),
      '#default_value' => variable_get('cdm_dataportal_show_advanced_search', 1),
      '#description' => t('Check this box if the link to advanced search should be show below the search box.'),
    );

  return system_settings_form($form);
}

function cdm_dataportal_theming_form (){
    //--- Theme ---//
/*
    $form['cdm_dataportal_theming'] = array(
      '#type' => 'fieldset',
      '#title' => t('Theme Images'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
*/
  $form = array('#attributes' => array('enctype' => 'multipart/form-data'));

    $form['cdm_dataportal_theming_right_image'] = array(
        '#type' => 'file',
        '#title' => t('Select top right image'),
        '#description' => t('Maximum dimensions are %dimensions and the maximum size is %size kB.',
                            array('%dimensions' =>  '250x250', '%size' => '30')),
    );
    $form['cdm_dataportal_theming_middle_image'] = array(
        '#type' => 'file',
        '#title' => t('Select top middle image'),
    );
    $form['test'] = array(
        '#type' => 'textfield',
        '#title' => t('test')
    );

    //$form['gen_layout']['theme']['#submit'][] = 'settings_validate_theme_pictures';
    $form['cdm_dataportal_theming']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit')
    );

    return $form;
}

function cdm_dataportal_theming_form_submit (&$form, &$form_values){
  $validators = array();
  //destination path where the files/banners will be saved
  $dest = absolute_path_to_drupal() . '/' . path_to_theme() . '/images/banners';
  $dest = str_replace('/', DIRECTORY_SEPARATOR, $dest);
  //drupal_set_message($dest);

  //check if directory exists
  if (!file_exists($dest)){
        if(!mkdir($dest, 0777, true)){//TODO: add rights, which rights should I add?
            drupal_set_message('Fail uploading the files; the directory '
                               . $dest . ' could not be created.',
                               'warning');
        }
  }
  //check if files already exist
  //if (file_exists($dest)) {
  //}

  //save the files
    $file = file_check_upload('cdm_dataportal_theming_middle_image');
    if ($file){
        $file = file_save_upload($file, 'files');
        drupal_set_message($file->filepath);
        file_move($file->filepath, $dest);
    }else{
      drupal_set_message('Fail uploading the file, the file is not accepted.', 'warning');
    }
  //use banners in the selected theme
    //if (!copy($file, $file.'.bak')) {
    //    print ("failed to copy $file...<br>\n");
    //}

    //use the banners as default theme
}
/*
function cdm_settings_layout_synonymy(){
  / * ====== SYNONYMY ====== * /
  $form['synonymy'] = array(
      '#type' => 'fieldset',
      '#title' => t('Synonymy'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('This section covers the settings related to the taxon <b>synonymy</b> tab.'),
  );

  $form['synonymy']['cdm_dataportal_nomref_in_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show accepted taxon on top of the synonymy'),
    '#default_value' => variable_get('cdm_dataportal_nomref_in_title', CDM_DATAPORTAL_NOMREF_IN_TITLE),
    '#description' => t('If checked, the first homotypic taxon is a repetition of the accepted taxon most likely
                        with the full nomenclatural reference (depending on the currently chosen theme).')
  );

  $form['synonymy']['cdm_dataportal_display_is_accepted_for'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display <em>is accepted for ...</em> on taxon pages when coming from a synonym link.'),
    '#default_value' => variable_get('cdm_dataportal_display_is_accepted_for', CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR),
    '#description' => t('Check this if after doing a search and clicking on a synonym you want to see the "accept of" text for the accepted synonym.')
  );

  $form['synonymy']['name_relationships']['name_relationships_to_show'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display name relationships'),
    '#default_value' => variable_get('name_relationships_to_show', 0),
    '#options' => $nameRelationshipTypeOptions,
    '#description' => t('Select the name relationships you want to show for the accepted taxa.'),
  );

  $form['synonymy'][CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS] = array(
    '#type' => 'checkbox',
    '#title' => t('Show taxon relations ships of accepted taxon'),
    '#default_value' => variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT),
    '#description' => t('If this option is enabled the synonymy will show the below selected taxon relationships of accepted taxa.')
  );

  $taxonRelationshipTypeOptions = cdm_Vocabulary_as_option(UUID_TAXON_RELATIONSHIP_TYPE);
  $form['synonymy'][CDM_TAXON_RELATIONSHIP_TYPES] = array(
      '#type' => 'checkboxes',
      '#title' => t('Taxon relationship types'),
      '#description' => t('Only taxon relationships of the selected type will be displayed'),
      '#options' => $taxonRelationshipTypeOptions,
      '#default_value' => variable_get('CDM_TAXON_RELATIONSHIP_TYPES', unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT)),
      '#disabled' => !variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT)
  );

  return system_settings_form($form);

}
*/

function cdm_settings_layout_taxon(){
  $collapsed = false;
  $form = array();

  //--------- TABBED TAXON -------//
  $form['taxon_tabs'] = array(
    '#type' => 'fieldset',
      '#title' => t('Taxon tabs'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    '#description' => t('If tabbed taxon page is enabled the taxon profile will be splitted in four diferent tabs;
             General, Synonymy, Images and Specimens. If the taxon has no information for any of the tabs/sections such tab will be not displayed.'),
  );

    $form['taxon_tabs']['cdm_dataportal_taxonpage_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabbed taxon page'),
    '#default_value' => variable_get('cdm_dataportal_taxonpage_tabs', 1),
    '#description' => t('<p>If selected split the taxon page into individual tabs for description, images, synonymy and specimens.
                            If not the taxon data is renderized as a long single page without tabs.</p>')
  );

  $form['taxon_tabs']['cdm_dataportal_detault_tab'] =  array(
      '#type'          => 'select',
      '#title'         => t('Default tab to display'),
      '#default_value' => variable_get('cdm_dataportal_detault_tab', 0),
      '#options'       => unserialize(CDM_DATAPORTAL_DEFAULT_TAXON_TAB),
      '#description'   => t('<p>Select the default tab to display when visiting a taxon page. Only available if Tabbed Taxon Page is enable.</p>
              <strong>Note:</strong> After performing a search and clicking in any synonym, the taxon tab
              to be renderized will be the synonymy of the accepted taxon and not the above selected tab.'),
  );

  $form['cdm_dataportal_show_back_to_search_results'] = array(
      '#type' => 'checkbox',
        '#title' => t('Show <em>Back to search results</em> link at the taxon site.'),
        '#default_value' => variable_get('cdm_dataportal_show_back_to_search_results', 1),
        '#description' => t('<p>If checked the link to search results is rendererized at the top of the taxon site. Clicking on the link the last search performed is renderized again.</p>')
  );

  /* ======  TAXON_PROFILE ====== */

  $form['taxon_profile'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxon profile (tab)'),
      '#description' => t('<p>This section covers the setting related to the taxon profile tab, also known as the <strong>"General"</strong> tab.</p>'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );


  // ---- PROFILE PICTURE ----//
  $form['taxon_profile']['picture'] = array(
      '#type' => 'fieldset',
      '#title' => t('Profile Picture'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    '#description' => t('Select a profile picture for taxa. Like a facebook of plants.'),
  );

  $form['taxon_profile']['picture']['cdm_dataportal_show_default_image'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable profil picture'),
      '#default_value' => variable_get('cdm_dataportal_show_default_image', false),
      '#description'   => t('Show the profil picture.')
  );

  $options = cdm_rankVocabulary_as_option();
  array_unshift($options, '-- DISABLED --');
  $form['taxon_profile']['picture']['image_hide_rank'] =  array(
      '#type'          => 'select',
      '#title'         => t('Hide picture for taxa above'),
      '#default_value' => variable_get('image_hide_rank', '0'),
      '#options'       => $options,
      '#description'   => t('Select which rank of pictures should not have a profil picture.'),
  );
  //show picture
  $selectShowMedia = array(0 => "Show only taxon pictures",
  1 => "Show taxon and child taxa pictures");

  $form['taxon_profile']['picture']['cdm_dataportal_show_media'] = array(
      '#type' => 'select',
      '#title' => t('Available picture files'),
      '#default_value' => variable_get('cdm_dataportal_show_media', false),
      '#options' => $selectShowMedia,
      '#description'   => t('Show the profil pictures current taxon\'s children.')
  );

  //-- MEDIA THUMBNAILS --//
  $form_name = CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME;
  $form_title = 'Taxon Profile Images';
  $form_description = '<p>The different section in the taxon  profile can have images associated with them. These images are displayed in a gallery of thumbnails wich can be configuered here:</p>';
  $form['taxon_profile'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description);

  // ---- FEATURE TREE ---- //
  $form['taxon_profile']['taxon_profile'] = array(
  '#type' => 'fieldset',
     '#title' => t('Features'),
     '#collapsible' => TRUE,
     '#collapsed' => FALSE,
  '#description' => t('This section covers settings related to the taxon\'s <em>Feature Tree</em>. The <em>feature tree</em> are the taxon\'s
                        features such description, distribution, common names, etc. that drupal will render at his taxon profile page.'),
  );

  $form['taxon_profile']['feature_trees'][CDM_DATAPORTAL_DEFAULT_FEATURETREE_UUID] = array(
      '#type' => 'radios',
      '#title'         => t('Taxon profile sections'),
      '#default_value' => variable_get(CDM_DATAPORTAL_DEFAULT_FEATURETREE_UUID, UUID_DEFAULT_FEATURETREE),
      '#options' => cdm_get_featureTrees_as_options(TRUE),
      '#description'   => t('Select the Feature Tree to be displayed at the taxon profile. Click "Show Details" to see the Feature Tree elemets.'
      )
  );

  $form['taxon_profile']['feature_trees'][CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID] = array(
    '#type' => 'radios',
    '#title'         => t('Natural language representation of structured descriptions'),
    '#default_value' => variable_get(CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID, null),
    '#options' => cdm_get_featureTrees_as_options(),
    '#description'   => t('Taxon descriptions can be stored in a highly structured form.'.
      ' The feature tree selected here will be used to generate textual representation in natural language.'
      //.' If there is no applicable FeatureTree you can create a new one using the <a href="">FeatureTreeManager</a>'
    )
  );

  //---- LAYOUT PER FEATURE ---- //
  $feature_tree = cdm_ws_get(CDM_WS_FEATURETREES, variable_get(CDM_DATAPORTAL_DEFAULT_FEATURETREE_UUID, UUID_DEFAULT_FEATURETREE));

  if( isset($feature_tree->root->children) ){

    $form_feature_list_layout = array(
      '#title' => t('Taxon profile layout'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#type' => 'fieldset',
      '#description' => t('Will be availbale in a future release.'),
    );

    $feature_list_layout_settings_disabled = true;
    foreach( $feature_tree->root->children as $featureNode ){


      if( !$feature_list_layout_settings_disabled && isset($featureNode->feature) ) {

        $subform_id = LAYOUT_SETTING_PREFIX . $featureNode->feature->uuid; // must not exceed 45 characters !!!

        $settings = mixed_variable_get($subform_id, FEATURE_TREE_LAYOUT_DEFAULTS);


        $form_feature_list_layout[$subform_id] = array(
          '#tree' => TRUE,
           '#title' => $featureNode->feature->representation_L10n,
          '#collapsible' => FALSE,
          '#collapsed' => FALSE,
          '#type' => 'fieldset',
          '#description' => t('')
        );

        $form_feature_list_layout[$subform_id]['enabled'] = array(
          '#type' => 'checkbox',
           '#title' => t('Enable'),
          '#default_value' => $settings['enabled'],
          '#description' => t('Enable user defined layout for this feature')
        );

        $form_feature_list_layout[$subform_id]['enclosingTag'] = array(
          '#type' => 'textfield',
           '#title' => t('Enclosing tag'),
          '#disabled' => !$settings['enabled'],
          '#default_value' => $settings['enclosingTag'],
          '#description' => t('Default is: ') . "'<code>" . $systemDefaults['enclosingTag'] . "</code>'"
        );

        $form_feature_list_layout[$subform_id]['entryEnclosingTag'] = array(
          '#type' => 'textfield',
           '#title' => t('Entry enclosing tag'),
          '#disabled' => !$settings['enabled'],
          '#default_value' => $settings['entryEnclosingTag'],
          '#description' => t('Default is: ') . "'<code>". $systemDefaults['entryEnclosingTag'] ."</code>'"
        );

        $form_feature_list_layout[$subform_id]['glue'] = array(
          '#type' => 'textfield',
           '#title' => t('Glue'),
          '#disabled' => !$settings['enabled'],
          '#default_value' => $settings['glue'],
          '#description' => t('Default is: ') . "'<code>" . $systemDefaults['glue'] . "</code>'"
        );

      }

      $form['taxon_profile']['feature_list_layout'] = $form_feature_list_layout;
    }
  }


  //---- DISTRIBUTION LAYOUT ---- //

  $form['taxon_profile']['distribution_layout'] = array(
    '#title' => t('Distribution'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#type' => 'fieldset',
    '#description' => t('Select if you want to sort or not the distribution text located below the distribution map.'),
  );

  $form['taxon_profile']['distribution_layout']['distribution_sort'] =  array(
  '#type'          => 'radios',
  '#title'         => t('Sort'),
  '#default_value' => variable_get('distribution_sort', 'NO_SORT'),
  '#options' => array(
      'NO_SORT' => t('Standard (No sort)'),
      'HIDE_TDWG2' => t('Sorted without TDWG Level 2'),
    ));

   $form['taxon_profile']['distribution_layout'][DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP] =  array(
  '#type'          => 'checkbox',
  '#title'         => t('Show TextData elements on top of the map'),
  '#default_value' => variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0),
  '#description' => t('Check this if you want to appear all <code>TextData</code> elements on top of the map.
    Otherwise all <code>TextData</code> distribution elements will be listed below the other area elements.
    This option is useful if you need to have descriptive texts for each distribution map.'),
  );


/* ====== SYNONYMY ====== */
  $form['taxon_synonymy'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxon synonymy (tab)'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('This section covers the settings related to the taxon <b>synonymy</b> tab.####'),
  );

  $form['taxon_synonymy']['cdm_dataportal_nomref_in_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show accepted taxon on top of the synonymy'),
    '#default_value' => variable_get('cdm_dataportal_nomref_in_title', CDM_DATAPORTAL_NOMREF_IN_TITLE),
    '#description' => t('If checked, the first homotypic taxon is a repetition of the accepted taxon most likely
                        with the full nomenclatural reference (depending on the currently chosen theme).')
  );

  $form['taxon_synonymy']['cdm_dataportal_display_is_accepted_for'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display <em>is accepted for ...</em> on taxon pages when coming from a synonym link.'),
    '#default_value' => variable_get('cdm_dataportal_display_is_accepted_for', CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR),
    '#description' => t('Check this if after doing a search and clicking on a synonym you want to see the "accept of" text for the accepted synonym.')
  );

  $nameRelationshipTypeOptions = cdm_Vocabulary_as_option(UUID_NAME_RELATIONSHIP_TYPE);
  $form['synonymy']['name_relationships']['name_relationships_to_show'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display name relationships'),
    '#default_value' => variable_get('name_relationships_to_show', 0),
    '#options' => $nameRelationshipTypeOptions,
    '#description' => t('Select the name relationships you want to show for the accepted taxa.'),
  );


 $form['synonymy'][CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS] = array(
    '#type' => 'checkbox',
    '#title' => t('Show taxon relations ships of accepted taxon'),
    '#default_value' => variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT),
    '#description' => t('If this option is enabled the synonymy will show the below selected taxon relationships of accepted taxa.')
  );

   $taxonRelationshipTypeOptions = cdm_Vocabulary_as_option(UUID_TAXON_RELATIONSHIP_TYPE);
  $form['synonymy'][CDM_TAXON_RELATIONSHIP_TYPES] = array(
      '#type' => 'checkboxes',
      '#title' => t('Taxon relationship types'),
      '#description' => t('Only taxon relationships of the selected type will be displayed'),
      '#options' => $taxonRelationshipTypeOptions,
      '#default_value' => variable_get('CDM_TAXON_RELATIONSHIP_TYPES', unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT)),
      '#disabled' => !variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT)
  );

  // ====== SPECIMENS ====== //
  $form['taxon_specimens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxon specimens (tab)'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('This section covers the settings related to the taxon <b>specimens</b> tab.'),
  );
  $form_name = CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME;
  $form_title = 'Specimen media';
  $form_description = 'Specimens may have media which is displayed at the Specimen tab/section as a gallery.
   It is possible to configure the thumbnails gallery here, however for configuring how a single media should
   be displayed please go to <a href="./?q=admin/settings/cdm_dataportal/layout/media">Layout -&gt; Media</a></p>';
  $form['taxon_specimens'][] =
    cdm_dataportal_create_gallery_settings_form($form_name, $form_title, FALSE, $form_description);


  // --- MEDIA GALLERY ---- //
  $form_name = CDM_DATAPORTAL_TAXON_MEDIA_GALLERY_NAME_TAB;
  $form_title = 'Media gallery (tab)';
  $form_description = '<p>This section covers the settings related to the taxon <strong>media</strong> tab.
   Taxa may have media (usually images) and they are as thumbnails displayed. It is possible to configure
   the thumbnails gallery here, however for configuring how a single media should be displayed please go to
   <a href="./?q=admin/settings/cdm_dataportal/layout/media">Layout -&gt; Media</a></p>
   <p><strong>Note:</strong> These settings are only taken into account when the standard
   gallery viewer is selected at <a href="./?q=admin/settings/cdm_dataportal/layout/media">Layout -&gt; Media</a>.</p>';
  $form['taxon_media'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, TRUE, $form_description);


  return system_settings_form($form);
}

function cdm_settings_layout_search(){

  $form = array();

  $form['search_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxa Search'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('<p>The data portal allows the users to perform searchs.</p><p>To perform searchs
         the block <em>CDM Taxon Search</em> should be enabled and visible for users
         where they can write the text to be searched. You can find Drupal block configuration
         site at <a href="./?q=admin/build/block">Administer&#45&#62Site building&#45&#62Blocks</a></p> '),
  );

  $form['search_settings']['cdm_dataportal_search_items_on_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Results per page'),
    '#default_value' => variable_get('cdm_dataportal_search_items_on_page', CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE),
    '#description' => t('Number of results to display per page.')
  );

  // --- SEARCH TAXA GALLERY ---- //
  $items = variable_get('cdm_dataportal_search_items_on_page', CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE);
  $collapsed = FALSE;
  $form_name = CDM_DATAPORTAL_SEARCH_GALLERY_NAME;
  $form_title = 'Taxa Search thumbnails';
  $form_description = 'Search results may show thumbnails. ';
  $form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description);

  return system_settings_form($form);
}

function cdm_settings_layout_media(){

  $form = array();

  $form['media_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Media display settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    '#description' => t('This section covers the settings related to the taxa media, that is how each single media should be displayed.'),
      );

  $form['media_settings']['image_gallery_viewer'] =  array(
    '#type'          => 'select',
    '#title'         => t('Image viewer'),
    '#default_value' => variable_get('image_gallery_viewer', 'default'),
    '#options' => array('default' => t('Standard image viewer'),
                        'fsi' => t('FSI viewer (requires FSI server!)')),
  );

  // --- MEDIA GALLERY ---- //
  $form_name = CDM_DATAPORTAL_MEDIA_GALLERY_NAME;
  $form_title = 'Standard viewer';
  $form_description = '<p>Configure the standard image viewer.</p><p><strong>Note:</strong> the image viewer should selected otherwise settings are not taking into account.</p>';
  //$form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed);
  $form['media_settings'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description);


  return system_settings_form($form);
}


/**
 * GEOSERVICE and Map settings
 * @return unknown_type
 */
function cdm_settings_geo(){

  $form = array();

  /*
   * GEO SERVER
   */

  $form['geoserver'] = array(
    '#type' => 'fieldset',
    '#title' => t('Geo Server Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Configuration and selection of your geo server. The Geo Server is the responsible for generating the maps.'),
  );

  $form['geoserver']['edit_map_server'] = array(
    '#type' => 'select',
    '#title' => t('Geoservice access point URL'),
    '#default_value' => variable_get('edit_map_server', EDIT_MAPSERVER_V1_URI),
    '#options' => array(
        EDIT_MAPSERVER_V1_URI => 'EDIT Map Server v1',
        EDIT_MAPSERVER_V11_URI => 'EDIT Map Server v1.1',
        /*
        'http://edit.br.fgov.be/edit_wp5/v1/' => 'EDIT Map Server - Mirror 1',
        'http://edit.br.fgov.be/edit_wp5/v1/' => 'EDIT Map Server - Mirror 2',
         */
        'ALTERNATIVE' => '-- Alternative URL --'
        ),
    '#description' => t('Select the Map Server you want the data portal to connect.'
         . 'If you want to introduce a custom address just select the Alternative URL value and fill the field Geoservice'
         . 'Access Point - Alternative URL with the custem ip address.')
    );

    $form['geoserver']['edit_map_server_alternative'] = array(
      '#type' => 'textfield',
      '#title' => t('Geoservice access point - alternative URL'),
      '#default_value' => variable_get('edit_map_server_alternative', ''),
      '#description' => t('Alternative URL of a EDIT Map Service to be used by this portal. You must choose the option <i>-- Alternative URL --</i> in the chooser abofe to enable this url.')
    );


  /*
   *  MAP SETTINGS
   */

    $form['map_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Maps settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('Configuration of the renderized maps.'),
     );

    $form['map_settings']['cdm_dataportal_geoservice_display_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Maps size'),
      '#default_value' => variable_get('cdm_dataportal_geoservice_display_width', 390),
      '#description' => t('Choose the size of your Maps. A value of 500 means the size will be 500x500, a value of 300 means the size will be 300x300.')
    );

    $form['map_settings']['cdm_dataportal_geoservice_bounding_box'] = array(
      '#type' => 'textfield',
      '#title' => t('Fixed bounding box'),
      '#default_value' => variable_get('cdm_dataportal_geoservice_bounding_box', '-180,-90,180,90'),
      '#description' => t('Define surrounding of area to be displayed in maps. Use "-180,-90,180,90" for the whole world. Leave <strong>empty</strong> to let the map <strong>automatically zoom</strong> to the distribution area.')
    );

    $form['map_settings']['cdm_dataportal_geoservice_labels_on'] = array(
      '#type' => 'checkbox',
      '#title' => '<b>'.t('Display area labels').'</b>',
      '#default_value' => variable_get('cdm_dataportal_geoservice_labels_on', FALSE),
      '#description' => t('Check this if you like area names to be displayed in the maps. DOES IT WORKS???? ')
    );

    $form['map_settings']['cdm_dataportal_geoservice_map_caption'] = array(
      '#type' => 'textfield',
      '#title' => t('Map caption'),
      '#default_value' => variable_get('cdm_dataportal_geoservice_map_caption', ''),
      '#description' => t('Define a caption for the map.')
    );

    $form['map_settings']['cdm_dataportal_geoservice_distributionOpacity'] = array(
      '#type' => 'textfield',
      '#title' => t('Distribution layer opacity'),
      '#default_value' => variable_get('cdm_dataportal_geoservice_distributionOpacity', '0.5'),
      '#description' => t('Valid values range from 0.0 to 1.0. Value 1.0 means the distributions (the countries or regions) will
                           fully visible, while a value near to 0.0 will be not much visible.')
    );

    $form['map_settings']['cdm_dataportal_map_openlayers'] = array(
      '#type' => 'radios',
      '#title' => '<b>'.t('Map Viewer').'</b>',
      '#options' => array(1 => "OpenLayers dynamic mapviewer", 0 => "Plain image" ),
      '#default_value' => variable_get('cdm_dataportal_map_openlayers', 1),
      '#description' => t('You can choose from two different map viewers:<ul><li><em>OpenLayers</em> displays the maps in an interactive viewer which allows zooming and panning. If not enabled the maps will consist
                           on a static image. If enabled you can configure the default layer (background of your maps) below. Only one of
                           them will be renderized.</li><li><em>Plain image</em> displays the map as a plain non interactive image</li></ul>')
    );

    $openLayersEnabled = variable_get('cdm_dataportal_map_openlayers', 1) === 1;


    // --- Plain Image Settings --- //

    $form['map_image'] = array(
      '#type' => 'fieldset',
      '#title' => t('Plain image map settings'),
      '#collapsible' => TRUE,
      '#collapsed' => $openLayersEnabled,
      '#description' => 'The settings in this section are still expertimental '
        .'and can onyl be used with the EDIT map service version 1.1 or above.'
    );

    $edit_mapserver_version = getEDITMapServiceVersionNumber();
    if ($edit_mapserver_version < 1.1) {
      $form['map_image']['#description'] = '<div class="messages warning">' . t("The selected EDIT map service version has to small version number: $edit_mapserver_version") . '</div>'
      . $form['map_image']['#description'];
    }

    $form['map_image']['map_base_layer'] = array(
      '#type' => 'textfield',
      '#title' => t('Background layer'),
      '#default_value' => variable_get('map_base_layer', 'cyprusdivs'),
      '#description' => t('Background layer. for available layers inspect')
        . l(" deegree-csw ",  "http://edit2.br.fgov.be:8080/deegree-csw/md_search.jsp") . t('or')
        . l(" geoserver layers ", "http://edit.br.fgov.be:8080/geoserver/rest/layers")
    );

    $form['map_image']['map_bg_color'] = array(
      '#type' => 'textfield',
      '#title' => t('Background color'),
      '#default_value' => variable_get('map_bg_color', '1874CD')
    );

    $form['map_image']['map_base_layer_style'] = array(
      '#type' => 'textfield',
      '#title' => t('Background layer area style'),
      '#default_value' => variable_get('map_base_layer_style', 'ffffff,606060,,'), // only line color by now
      '#description' => t('Syntax: {Area fill color},{Area stroke color},{Area stroke width},{Area stroke dash style}')
    );


    // --- OpenLayers Settings --- //

    $form['openlayers'] = array(
      '#type' => 'fieldset',
      '#title' => t('OpenLayers settings'),
      '#collapsible' => TRUE,
      '#collapsed' => !$openLayersEnabled,
      '#description' => ''
    );

    if( !$openLayersEnabled ){
      $form['openlayers']['#description'] = '<div class="messages warning">' . t("The Openlayers viewer is curretnly not enabled! (see section Maps settings above )") . '</div>'
      . $form['openlayers']['#description'];
    }

    $baselayer_options = array(
      /*
       * NOTICE: must correspond to the layers defined in js/openlayers_,ap.js#getLayersByName()
       */
      'osgeo_vmap0' => "OpenLayers World", // EPSG:4326
      'metacarta_vmap0' => "Metacarta Vmap0" , // EPSG:4326, EPSG:900913
       // all others EPSG:900913
      'edit-vmap0_world_basic' => 'EDIT Vmap0',
      'edit-etopo1' => "ETOPO1 Global Relief Model",
      'osmarender' => 'OpenStreetMap',
      //'oam' => 'OpenAerialMap', // currently unavailable
      'gmap' => 'Google Streets',
      'gsat' => 'Google Satellite',
      'ghyb' => 'Google Hybrid',
      'veroad' => 'Virtual Earth Roads',
      'veaer' => 'Virtual Earth Aerial',
      'vehyb' => 'Virtual Earth Hybrid'
       //  ,
       //    'yahoo' => 'Yahoo Street',
       //    'yahoosat' => 'Yahoo Satellite',
       //    'yahoohyb' => 'Yahoo Hybrid'
    );

    $form['openlayers']['baselayers'] = array(
      '#type' => 'checkboxes_preferred',
      '#title' => t('Baser Layers'),
      '#options' => $baselayer_options,
      '#default_value' => variable_get('baselayers', array('metacarta_vmap0' => "metacarta_vmap0", 'PREFERRED' => 'metacarta_vmap0')),
      '#description' => t('Choose the baselayer layer you prefer to use as map background in the OpenLayers dynamic mapviewer.')
     );

  // cdm_dataportal_geoservice_showLayerSwitcher
  $form['openlayers']['cdm_dataportal_geoservice_showLayerSwitcher'] = array(
    '#type' => 'checkbox',
    '#title' => '<b>'.t('Show Layer Switcher').'</b>',
    '#default_value' => variable_get('cdm_dataportal_geoservice_showLayerSwitcher', TRUE),
    '#description' => t('The Layer Switcher control displays a table of contents for the map.  This allows the user interface to switch between BaseLasyers and to show or hide Overlays.  By default the switcher is shown minimized on the right edge of the map, the user may expand it by clicking on the handle.')
  );

  $localhostkey = 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ';
  $gmap_api_key = variable_get('gmap_api_key', 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ');
  $form['openlayers']['gmap_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Gogle maps API key'),
    '#default_value' => variable_get('gmap_api_key', $gmap_api_key),
    '#description' => t('If you want to use the Google Maps Layer a key is needed. If you need a key visit <a href="http://code.google.com/intl/en/apis/maps/signup.html">google maps api key</a>.<br>
         <b>Note:</b> The following key: <code>'.$localhostkey.'</code> is the default key for the localhost (127.0.0.1). The key in use is the one above this text.')
   );

  $form['cdm_dataportal_geoservice_map_legend'] = array(
     '#type' => 'fieldset',
     '#title' => t('Map legend'),
     '#collapsible' => TRUE,
     '#collapsed' => TRUE,
     '#description' => t('Configure the maps legend.')
  );

  $form['cdm_dataportal_geoservice_map_legend']['cdm_dataportal_geoservice_legend_on'] = array(
    '#type' => 'checkbox',
    '#title' => '<b>'.t('Display a map legend').'</b>',
    '#default_value' => variable_get('cdm_dataportal_geoservice_legend_on', TRUE),
    '#description' => t('Check this if you like a legend to be displayed with the maps.')
  );

  $form['cdm_dataportal_geoservice_map_legend']['cdm_dataportal_geoservice_legendOpacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Legend opacity'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legendOpacity', '0.5'),
    '#description' => t('Valid values range from 0.0 to 1.0. Value 1.0 means the legend will be fully visible, while a value near
                         to 0.0 will be not much visible.')
  );

  $form['cdm_dataportal_geoservice_map_legend']['cdm_dataportal_geoservice_legend_font_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Font size'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legend_font_size', 10),
    '#description' => t('Font size in pixels.')
  );

  $fontStyles = array(0 => "plane", 1 => "italic");
  $form['cdm_dataportal_geoservice_map_legend']['cdm_dataportal_geoservice_legend_font_style'] = array(
    '#type' => 'select',
    '#title' => t('Available font styles'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legend_font_style', false),
    '#options' => $fontStyles,
    '#description'   => t('Select a font style for the map legend.')
  );

  $form['cdm_dataportal_geoservice_map_legend']['cdm_dataportal_geoservice_legend_icon_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Legend icon width'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legend_icon_width', 35),
    '#description' => t('Legend icon width in pixels.')
  );
  $form['cdm_dataportal_geoservice_map_legend']['cdm_dataportal_geoservice_legend_icon_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Legend icon height'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legend_icon_height', 15),
    '#description' => t('Legend icon height in pixels.')
  );

        return system_settings_form($form);
}


function cdm_settings_cache(){

  $form = array();

  $form['cache_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Cache Settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    '#description' => t('<p>When caching is enabled all single taxon sites are stored in an internal drupal cache doing
                           the portal response of taxa pages faster. This is possible because the sites are loaded from
                           the cache and are not created from scratch.</p>'),
      );

  $form['cache_settings']['cdm_webservice_cache'] =  array(
    '#type'          => 'checkbox',
    '#title'         => t('<strong>Enable caching</strong>'),
  '#options'     => cdm_help_general_cache(),
    '#default_value' => variable_get('cdm_webservice_cache', 1),
    '#description'   => t('<p>Enable drupal to load taxa pages from the cache.</p>' .
                         '<p><strong>Note:</strong> If taxa are modified by the editor or any other application the changes will be not
                         visible till the cache is erased. Therefore developers should deactived this feature when they
                         are working on the CDM Dataportal Module.</p>')
    );

  $form['cache_settings']['cdm_run_cache'] = array(
    '#value' => cdm_view_cache_site()
  );

  return system_settings_form($form);
}

/**
 * @return walk and cache all taxon pages
 */
function cdm_view_cache_site(){

  $out = '';

  _add_js_progressbar();
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cache_all_taxa.js');

  $request_params = array();
  $request_params['query'] = '%';
  $request_params['tree'] = variable_get('cdm_taxonomictree_uuid', false); //cache only the dafault classification
  $request_params['doTaxa'] = 1;
  $request_params['doSynonyms'] = 0;
  $request_params['doTaxaByCommonNames'] = 0;

  $search_url = cdm_compose_url(CDM_WS_PORTAL_TAXON_FIND . ".json", null, queryString($request_params));
  $search_url = uri_uriByProxy($search_url);
  $taxon_page_url = url('cdm_dataportal/taxon/');


  $out .= t('<p><strong>Cache all taxon pages</strong></p>');
  $out .= '<p>When you lunch the cache process the cache is filled and ready to be enabled.<br/>
  Remember that when you load the taxa from the cache last changes on taxa will be not visible till you erase
  the cache and fill it again.</p>';
  $out .= '<p>Before  running the cache bot you have to empty the cache manually</p>';

  $out .= '<form id="cache_site">';
  $out .= '<div>'.t('This caching process may take long time and could cause heavy load on your server').'</div>';
  $out .= '<div id="progress"></div>';
  $out .= '<input type="hidden" name="searchTaxaUrl" value="'.$search_url.'"/>';
  $out .= '<input type="hidden" name="taxonPageUrl" value="'.$taxon_page_url.'"/>';
  $out .= '<input type="button" name="start" value="'.t('Start').'"/>';
  $out .= '<input type="button" name="stop" value="'.t('Stop').'"/>';
  $out .= '</form>';
  $out .= '</div>';
  //  foreach($taxonPager->records as $taxon){
  //    cdm_dataportal_taxon_view($uuid);
  //  }

  return $out;
}


/**
 * Implementation of hook_validate()
 *
 * @param $element
 */
function cdm_settings_validate($form_id, $form_values){

  if (!str_endsWith($form_values['cdm_webservice_url'], '/')) {
    //form_set_error('cdm_webservice_url', t("The URL to the CDM Web Service must end with a slash: '/'."));
    $form_values['cdm_webservice_url'] .= '/';
  }

  if($form_values['cdm_webservice_cache'] != variable_get('cdm_webservice_cache', 1)){
    cache_clear_all(NULL, 'cache_cdm_ws');
    // better clear secref_cache since i can not be sure if the cache has not be used during this response
    cdm_api_secref_cache_clear();
  }

}

function getEDITMapServiceURI(){

  if(variable_get('edit_map_server', false) == 'ALTERNATIVE'){
    return (variable_get('edit_map_server_alternative', false));
  } else if(variable_get('edit_map_server', false)) {
    return variable_get('edit_map_server', false);
  } else {
    return EDIT_MAPSERVER_V1_URI;
  }

}

/**
 *
 * @return float the version number of the currently selected edit mapserver as a float. Returns 0 on error
 */
function getEDITMapServiceVersionNumber() {

      $pattern = '/v(\d+\.\d+)/';

      $url = getEDITMapServiceURI();
      preg_match($pattern, $url, $matches, PREG_OFFSET_CAPTURE, 3);
      if( isset($matches[1]) ){
        $version = 1 + $matches[1][0] -1; // convert string to float
        return $version;
      } else {
        // report error
        drupal_set_message(" Invalid version number in EDIT map service URL: '" + variable_get('edit_map_server', EDIT_MAPSERVER_V1_URI) + "'", "waring");
        return 0;
      }
}

/**
 * Implementation of hook_elements()
 *
 * see http://drupal.org/node/37862 for an example
 */
function cdm_dataportal_elements() {
  $type['checkboxes_preferred'] = array(
    '#input' => TRUE,
    '#process' => array('expand_checkboxes_preferred' => array()),
    '#after_build' => array('checkboxes_preferred_after_build')
  );
  return $type;
}

/**
 * #process function for the custom form element type 'checkbox_preferred'
 */
function expand_checkboxes_preferred($element){

  // first of all create the checkboxes
  $element = expand_checkboxes($element);

  $children = element_children($element);
  $element['table_start'] = array(
    '#value' => '<table class="checkboxes_preferred"><tr><th></th><th>'.t('Enabled').'</th><th>'.t('Default').'</th></tr>',
    '#weight'=>-1
  );
  $weight = 0;
  foreach ($children as $key) {
    $odd_even = $weight % 4 == 0 ? 'odd' : 'even';
    $element[$key]['#weight'] = $weight;
    $element[$key]['#prefix'] = '<tr class="'.$odd_even.'"><td>'.t($element['#options'][$key]).'</td><td>';
    $element[$key]['#suffix'] = '</td>';
    unset($element[$key]['#title']);
    $weight += 2;
  }
  $weight = 0;

  if (count($element['#options']) > 0) {
    foreach ($element['#options'] as $key => $choice) {
      if (!isset($element[$key.'_preferred'])) {
        $element[$key.'_preferred'] = array(
        '#type' => 'radio',
        '#name' => $element['#parents'][0].'_preferred',
        '#return_value' => check_plain($key),
        '#default_value' => $element['#default_value_2'],
        '#attributes' => $element['#attributes'],
        '#parents' => $element['#parents'],
        '#spawned' => TRUE,
        '#weight' => $weight + 1,
        '#prefix' => '<td>',
        '#suffix' => '</td></tr>',
        //'#submit' => 'submit_checkboxes_preferred'
        );
      }
      $weight += 2;
    }
  }

  $element['table_end'] = array(
  //'#type'=>'value',
  '#value' => '</table>', '#weight'=>$weight++);
  return $element;
}


function theme_checkboxes_preferred($element){
  return theme('form_element',
  array(
      '#title' => $element['#title'],
      '#description' => $element['#description'],
      '#id' => $element['#id'],
      '#required' => $element['#required'],
      '#error' => $element['#error'],
  ),
  $element['#children']);
}

function checkboxes_preferred_after_build($form, &$form_values){

  $parent_id = $form['#parents'][0];

  if($_POST && count($_POST) > 0){
    // first pass of form processing
    $preferred_layer = $_POST[$parent_id.'_preferred'];
    $form['#value']['PREFERRED'] = $preferred_layer;
    $form_values[$parent_id] = $form['#value'];
  } else {
    // second pass of form processing
    $preferred_layer = $form['#value']['PREFERRED'];
  }

  // also set the chosen value (not sure if this is good drupal style ....)
  foreach( $children = element_children($form) as $key ){
    if($form[$key]['#type'] == 'radio'){
      $form[$key]['#value'] = $preferred_layer;
    }
  }
  // the default layer mus always be enabled
  $form[$preferred_layer]['#value'] = $preferred_layer;


  return $form;
}

