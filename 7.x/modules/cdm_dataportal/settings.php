<?php
/**
 * @file
 * CDM Dataportal settings.
 */

// TODO Genus UUID.
define('DEFAULT_TAXONTREE_RANKLIMIT', '1b11c34c-48a8-4efa-98d5-84f7f66ef43a');
define('CDM_TAXONOMICTREE_UUID', 'cdm_taxonomictree_uuid');

define('CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE', 25);
define('CDM_DATAPORTAL_NOMREF_IN_TITLE', 1);
define('CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR', 0);
define('CDM_DATAPORTAL_ALL_FOOTNOTES', 0);
define('CDM_DATAPORTAL_ANNOTATIONS_FOOTNOTES', 0);
define('CDM_DATAPORTAL_LAST_VISITED_TAB_ARRAY_INDEX', 999);

/* annotationTypeKeys */
$annotationTypeKeys = array_keys(cdm_Vocabulary_as_option(UUID_ANNOTATION_TYPE));
if (in_array(UUID_ANNOTATION_TYPE_TECHNICAL, $annotationTypeKeys)) {
  $annotationTypeKeys = array_flip($annotationTypeKeys);

  // Technical annotation are off by default.
  unset($annotationTypeKeys[UUID_ANNOTATION_TYPE_TECHNICAL]);
  $annotationTypeKeys = array_flip($annotationTypeKeys);
  // Additional value for the NULL case.
  $annotationTypeKeys[] = 'NULL_VALUE';
}
define('ANNOTATIONS_TYPES_AS_FOOTNOTES_DEFAULT', serialize($annotationTypeKeys));

/* taxonRelationshipTypes */
define('CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT', serialize(array(UUID_MISAPPLIED_NAME_FOR, UUID_INVALID_DESIGNATION_FOR)));

/* Gallery variables. */
$gallery_settings = array(
  "cdm_dataportal_show_taxon_thumbnails" => 1,
  "cdm_dataportal_show_synonym_thumbnails" => 0,
  "cdm_dataportal_show_thumbnail_captions" => 1,
  "cdm_dataportal_media_maxextend" => 120,
  "cdm_dataportal_media_cols" => 3,
  "cdm_dataportal_media_maxRows" => 1,
);

/* ---- MAP SETTING CONSTANTS ---- */
/**
 * @var array of URIs eg. http://edit.africamuseum.be"
 *   An options array
 */
define('EDIT_MAPSERVER_URI', serialize(
    array(
      'http://edit.africamuseum.be'=>'Primary (http://edit.africamuseum.be)',
      'http://edit.br.fgov.be'=>'Secondary (http://edit.br.fgov.be)',
    )
  )
);
define('EDIT_MAPSERVER_PATH', '/edit_wp5');
/**
 * @var array of versions eg. "v1.2"
 *   An options array
 */
define('EDIT_MAPSERVER_VERSION', serialize(
    array(
      'v1' => 'v1' ,
      'v1.1' => 'v1.1',
      'v1.2_dev' => 'v1.2_dev',
      'v1.2' => 'v1.2',
      'v1.3_dev' => 'v1.3_dev'
    )
  )
);
define('EDIT_MAPSERVER_URI_DEFAULT', 'http://edit.africamuseum.be');
define('EDIT_MAPSERVER_VERSION_DEFAULT', 'v1.2');

// --- Taxon profile settings --- /
define('LAYOUT_SETTING_PREFIX', 'layout_');
define('FEATURE_TREE_LAYOUT_DEFAULTS', serialize(
  array(
    'enabled' => FALSE,
    'enclosingTag' => 'ul',
    'entryEnclosingTag' => 'li',
    'glue' => ' ',
  )));

define('DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP', 'distribution_textdata_on_top');
define('CDM_TAXON_PROFILE_IMAGE', 'cdm_taxon_profile_image');
define('CDM_TAXON_PROFILE_IMAGE_DEFAULT', serialize(
    array(
        'show' => 0,
        'maxextend' => 184,
        'media_uri_query' => '',
        'custom_placeholder_image_on' => 0,
        'custom_placeholder_image_fid' => ''
    )
  )
);

/**
 * @todo document this function
 */
function get_taxon_tabs_list() {
  return array(
    0 => 'General',
    1 => 'Synonymy',
    2 => 'Images',
    3 => 'Specimens',
    4 => 'Keys',
  );
}

$taxon_tab_options = get_taxon_tabs_list();
$taxon_tab_options[CDM_DATAPORTAL_LAST_VISITED_TAB_ARRAY_INDEX] = 'Last visited tab';

define('CDM_DATAPORTAL_DEFAULT_TAXON_TAB', serialize($taxon_tab_options));

/**
 * @todo document this function.
 */
function get_taxon_options_list() {
  $taxon_tab_options = array_flip(get_taxon_tabs_list());
  foreach ($taxon_tab_options as $key => $value) {
    $taxon_tab_options[$key] = t($key);
  }
  return $taxon_tab_options;

}

define('CDM_PART_DEFINITIONS', 'cdm-part-definitions');
define('CDM_PART_DEFINITIONS_DEFAULT', serialize(
  array(
      'ZoologicalName' => array(
        'namePart' => array('name' => TRUE),
        'nameAuthorPart' => array('name' => TRUE),
        'referencePart' => array('authors' => TRUE),
        'microreferencePart' => array('microreference' => TRUE),
        'statusPart' => array('status' => TRUE),
        'descriptionPart' => array('description' => TRUE),
      ),
      'BotanicalName'=> array(
        'namePart' => array('name' => TRUE),
        'nameAuthorPart' => array('name' => TRUE, 'authors' => TRUE),
        'referencePart' => array('reference' => TRUE, 'microreference' => TRUE),
        'statusPart' => array('status' => TRUE),
        'descriptionPart' => array('description' => TRUE),
      ),
     '#DEFAULT' => array(
        'namePart' => array(
            'name' => TRUE
        ),
        'nameAuthorPart' => array(
            'name' => TRUE,
            'authors' => TRUE
        ),
        'referencePart' => array(
            'reference' => TRUE
        ),
        'microreferencePart' => array(
            'microreference' => TRUE,
        ),
        'statusPart' => array(
            'status' => TRUE,
        ),
        'descriptionPart' => array(
            'description' => TRUE,
        ),
      )
    )
  )
);
define('CDM_NAME_RENDER_TEMPLATES', 'cdm-name-render-templates');
define('CDM_NAME_RENDER_TEMPLATES_DEFAULT', serialize(
   array (
     'taxon_page_title,polytomousKey'=> array(
          'namePart' => array('#uri' => TRUE),
        ),
      'taxon_page_synonymy,related_taxon'=> array(
          'nameAuthorPart' => array('#uri' => TRUE),
          'referencePart' => TRUE,
          'statusPart' => TRUE,
          'descriptionPart' => TRUE,
        ),
      'acceptedFor,typedesignations,list_of_taxa' => array(
          'nameAuthorPart' => array('#uri' => TRUE),
          'referencePart' => TRUE,
        ),
      '#DEFAULT' => array(
          'nameAuthorPart' => array('#uri' => TRUE),
          'referencePart' => TRUE,
       )
    )
));

define('TAXONPAGE_VISIBILITY_OPTIONS_DEFAULT', serialize(get_taxon_options_list()));
define('CDM_DATAPORTAL_GALLERY_SETTINGS', serialize($gallery_settings));
define('CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME', 'specimen_gallery');
define('CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME', "description_gallery");
define('CDM_DATAPORTAL_MEDIA_GALLERY_NAME', "media_gallery");
define('CDM_DATAPORTAL_TAXON_MEDIA_GALLERY_NAME_TAB', "taxon_tab_media_gallery");
define('CDM_DATAPORTAL_SEARCH_GALLERY_NAME', "search_gallery");
define('CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS', 'cdm_dataportal_display_taxon_relationships');
define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS', 'cdm_dataportal_display_name_relations');
// define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_2', array("default" => t('Display all')));
define('CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT', 1);
define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_DEFAULT', 1);
/**
 * The drupal variable key for the array containing the uuids of the taxon relationship types to display in
 * the snonymy.
 *
 * @var string
 */
define('CDM_TAXON_RELATIONSHIP_TYPES', 'cdm_taxon_relationship_types');
/**
 * The drupal variable for the configuration of the information aggregation along
 * the taxon relation ships. The mapped arrayis associative and holds two elements:
 *    - direct: the uuids of the taxon relationship types to take into account in invers
 *      direction.
 *    - invers: the uuids of the taxon relationship types to take into account in direct
 *      direction.
 *
 * @var String
 */
define('CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS', 'cdm_aggregate_by_taxon_relationships');
define('CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS_DEFAULT', serialize(
    array(
        'direct'=>array(),
        'invers'=>array()
     )
   ));
define('CDM_PROFILE_FEATURETREE_UUID', 'cdm_dataportal_featuretree_uuid');
define('CDM_OCCURRENCE_FEATURETREE_UUID', 'cdm_occurrence_featuretree_uuid');
define('CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID', 'cdm_dataportal_structdesc_featuretree_uuid');

define('CDM_TAXON_MEDIA_FILTER', 'cdm_taxon_media_filter');
define('CDM_TAXON_MEDIA_FILTER_DEFAULT', serialize(
    array(
        'includeTaxonDescriptions' => 'includeTaxonDescriptions',
        'includeOccurrences' => 0,
        'includeTaxonNameDescriptions' => 0
     )
  ));

define('CDM_MAP_DISTRIBUTION', 'cdm_map_distribution');
define('CDM_MAP_DISTRIBUTION_DEFAULT', serialize(array(
  // needs to be merged with user setting by drupal_array_merge_deep()
  'width' => 512, // optimum size for OSM layers is 512
  'height' => 512 / 2, // optimum size for OSM layers 256
  'bbox' => '', // empty to allow automatic zooming to extend
  'show_labels' => FALSE,
  'caption' => '',
  'distribution_opacity' => '0.5',
  'map_type' => 1, //  1 = 'openlayers', 0 = 'image'
  'image_map' => array(
    'base_layer' => '', // none, formerly this was cyprusdivs
    'bg_color' => '1874CD',
    'layer_style' => 'ffffff,606060,,',
  ),
  'openlayers' => array(
    'base_layers' =>  array(
      // A layer MUST NOT BE SET in the defaults,
      // otherwise it can not be overidden by the user settings:
      // 'osgeo_vmap0' => 'osgeo_vmap0',
      // it is sufficient to define the preferred layer,
      // since it will automatically be enabled:
      'PREFERRED' => 'osgeo_vmap0'),
     'custom_wms_base_layer' => array(
            'name' => NULL,
            'url' => NULL,
            'params' =>  NULL,
            'projection' => NULL,
            'max_extent' => NULL,
            'units' => NULL
     ),
    'show_layer_switcher' => TRUE,
    'display_outside_max_extent' => FALSE
  ),
  'legend' => array(
    'show' => TRUE,
    'opacity' => '0.5',
    'font_size' => 10,
    'font_style' => FALSE,
    'icon_width' => 35,
    'icon_height' => 15
  )
)));

/**
 * Merges the named array variable with the array of defaults.
 *
 * @param string $variable_name
 *     The variable name
 * @param string | array $default
 *     The array containing the default values either as array or serialized as string.
 *     Unserialization is cared for if nessecary
 * @return array
 *     The merged array as returnd by drupal_array_merge_deep()
 */
function get_array_variable_merged($variable_name, $default){

    // unserialize if nessecary
    if(!is_array($default)){
      $default = unserialize($default);
    }
    $variable = variable_get($variable_name, array());
    return drupal_array_merge_deep($default, $variable);
}

/**
 * @todo document this function.
 */
function getGallerySettings($gallery_config_form_name) {
  return get_array_variable_merged($gallery_config_form_name, CDM_DATAPORTAL_GALLERY_SETTINGS);
}

/**
 * Returns the string representation of the default tab.
 *
 * @param bool $returnTabIndex
 *   Defaults to FALSE, if set true this function will return the index number
 *   of the default tab. (used to supply default values to form elements).
 */
function get_default_taxon_tab($returnTabIndex = FALSE) {

  global $user;
  $values = unserialize(CDM_DATAPORTAL_DEFAULT_TAXON_TAB);
  $user_tab_active = 'cdm_dataportal_' . $user->uid . '_default_tab_active';
  $user_tab = 'cdm_dataportal_' . $user->uid . '_default_tab';
  // Get the user value if the used has chosen to overwrite the system settings.
  $user_tab_on = variable_get($user_tab_active, FALSE);
  if ($user_tab_on) {
    $user_value = variable_get($user_tab, 0);
    $index_value = $user_value;
    // Get the system value.
  }
  else {
    $system_value = variable_get('cdm_dataportal_default_tab', 0);
    $index_value = $system_value;
  }

  // Return the index value or the string representation.
  if ($returnTabIndex) {
    return $index_value;
  }
  else {
    return ($values[$index_value]);
  }

}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cdm_dataportal_menu_admin(&$items) {
  // Display section on admin/config page.
  $items['admin/config/cdm_dataportal'] = array(
    'title' => 'CDM Dataportal',
    'description' => 'Settings for the CDM DataPortal.',
    'position' => 'right',
    'weight' => 10,
    'page callback' => 'system_admin_menu_block_page',
    'access arguments' => array('administer cdm_dataportal'),
    'file' => 'system.admin.inc',
    'file path' => drupal_get_path('module', 'system'),
  );
  $items['admin/config/cdm_dataportal/settings'] = array(
    'title' => 'Settings',
    'description' => 'Settings for the CDM DataPortal.',
    'weight' => 0,
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_general'),
    'access arguments' => array('administer cdm_dataportal'),
    'type' => MENU_NORMAL_ITEM,
  );
  $items['admin/config/cdm_dataportal/settings/general'] = array(
    'title' => 'General',
    'description' => 'General',
    'weight' => 0,
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_general'),
    'access arguments' => array('administer cdm_dataportal'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/cachesite'] = array(
    'title' => 'Cache',
    'description' => 'Cache',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_cache'),
    'weight' => 10,
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/geo'] = array(
    'title' => 'Geo & Map',
    'description' => 'Geo & Map',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_geo'),
    'weight' => 1,
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/layout'] = array(
    'title' => 'Layout',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout'),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/layout/taxon'] = array(
    'title' => 'Taxon',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout_taxon'),
    'weight' => 1,
    'type' => MENU_LOCAL_TASK,
  );
  /*
  $items[] = array(
  'path' => 'admin/config/cdm_dataportal/layout/synonymy',
  'title' => t('Synonymy'),
  'description' => t('Configure and adjust the layout of your DataPortal '),
  'access' => user_access('administer cdm_dataportal'),
  'callback' => 'drupal_get_form',
  'callback arguments' => array('cdm_settings_layout_synonymy'),
  'weight' => 1,
  'type' => MENU_LOCAL_TASK,
  );

  $items[] = array(
  'path' => 'admin/config/cdm_dataportal/layout/specimens',
  'title' => t('Specimens'),
  'description' => t('Configure and adjust the layout of your DataPortal '),
  'access' => user_access('administer cdm_dataportal'),
  'callback' => 'drupal_get_form',
  'callback arguments' => array('cdm_settings_layout_specimens'),
  'weight' => 1,
  'type' => MENU_LOCAL_TASK,
  );
  */
  $items['admin/config/cdm_dataportal/settings/layout/search'] = array(
    'title' => 'Search',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout_search'),
    'weight' => 2,
    'type' => MENU_LOCAL_TASK,
  );

  $items['admin/config/cdm_dataportal/settings/layout/media'] = array(
    'title' => 'Media',
    'description' => 'Configure and adjust the layout of your DataPortal ',
    'access arguments' => array('administer cdm_dataportal'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('cdm_settings_layout_media'),
    'weight' => 3,
    'type' => MENU_LOCAL_TASK,
  );

}

/**
 * @todo document this function.
 */
function cdm_help_general_cache() {
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
 * @return array
 *   Drupal settings form.
 */
function cdm_settings_general() {

  $form['cdm_webservice'] = array(
    '#type' => 'fieldset',
    '#title' => t('CDM Server'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('The <em>CDM Server</em> exposes data stored in a
        CDM data base to the web via RESTful web services and thus is the source of the data
        to be displayed by a CDM DataPotal.'),
  );

  $form['cdm_webservice']['cdm_webservice_url'] = array(
    '#type' => 'textfield',
    '#title' => t('CDM web service URL') . ':',
    '#description' => t('This is the URL to the CDM-Server exposing your data
      e.g. <em>"http://localhost:8080/cichorieae/"</em> The URL <strong>must end
      with a slash</strong> character!'),
    '#default_value' => variable_get('cdm_webservice_url', NULL),
  );

  $form['cdm_webservice']['cdm_webservice_debug'] = array(
    '#type' => 'markup',
    '#markup' => '<b>Debug CDM Web Service:</b> The web service debug box has completely been replaced '
      . 'by  the ' . l('CDM web service debug block', 'admin/structure/block/manage/cdm_api/cdm_ws_debug/configure')
      . '. It is usually only visible for administrative roles like <i>administrator</i> and <i>CDM admin</i>'
  );

  $form['cdm_webservice']['freetext_index'] = array(
    '#type' => 'fieldset',
    '#title' => t('Freetext index'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  // Check the cdmserver port number and display a waring if it is not port 80
  preg_match("#http[s]?://[0-9\p{L}\.]*:([0-9]*)/.*#u", variable_get('cdm_webservice_url', ''), $portNumberMatch, PREG_OFFSET_CAPTURE);
  if (isset($portNumberMatch[1]) && $portNumberMatch[1] != '80') {
    $form['cdm_webservice']['freetext_index']['message'] = array(
      '#markup' => "<div class=\"description\">"
      . t("The CDM web service URL contains a portnumber other than standart HTTP port 80: '!port'."
      . " Due to this the reindex and purge fuctions may not be working if there is a firewall in between you and the CDM Server."
      . " You may want to contact the maintainer of the according CDM Server in order to solve this problem.", array('!port' => $portNumberMatch[1][0]))
      . "</div>",
    );
  };

  $frontentURL = urlencode(variable_get('cdm_webservice_url', ''));
  $trigger_link_options = array(
    'attributes' => array(
      'class' => 'index-trigger',
    ),
  );
  $form['cdm_webservice']['freetext_index']['operations'] = array(
    '#markup' => "<div>" . t('Operations: !url1 !url2', array(
        '!url1' => l(t("Purge"), cdm_compose_url(CDM_WS_MANAGE_PURGE, NULL, 'frontendBaseUrl=' . $frontentURL), $trigger_link_options),
        '!url2' => l(t("Reindex"), cdm_compose_url(CDM_WS_MANAGE_REINDEX, NULL, 'frontendBaseUrl=' . $frontentURL), $trigger_link_options),
      ))
    . '<div id="index-progress"></div></div>',
  );
  _add_js_cdm_ws_progressbar(".index-trigger", "#index-progress");

  $form['cdm_webservice']['proxy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proxy'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['cdm_webservice']['proxy']['cdm_webservice_proxy_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy URL') . ':',
    '#description' => t('If this proxy url is set the cdm api tries
    to connect the web service over the given proxy server.
    Otherwise proxy usage is deactivated.'),
    '#default_value' => variable_get('cdm_webservice_proxy_url', FALSE),
  );

  $form['cdm_webservice']['proxy']['cdm_webservice_proxy_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxy port') . ':',
    '#default_value' => variable_get('cdm_webservice_proxy_port', '80'),
  );

  $form['cdm_webservice']['proxy']['cdm_webservice_proxy_usr'] = array(
    '#type' => 'textfield',
    '#title' => t('Login') . ':',
    '#default_value' => variable_get('cdm_webservice_proxy_usr', FALSE),
  );

  $form['cdm_webservice']['proxy']['cdm_webservice_proxy_pwd'] = array(
    '#type' => 'textfield',
    '#title' => t('Password') . ':',
    '#default_value' => variable_get('cdm_webservice_proxy_pwd', FALSE),
  );

  // TODO: settings are still incomplete, compare with
  // trunk/dataportal/inc/config_default.php.inc.
  $form['taxon_tree'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon Tree'),
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
    '#description' => t('<p>When you explore your collection, you can navigate
      it through a tree structure also called <em>Taxon Tree</em>.</p><p>To be
      able to navigate through your collection the
      <a href="http://drupal.org/handbook/blocks">drupal block</a>
      <em>CDM Taxon Tree</em> should be visible for users. Enable the block at
      <a href="./?q=admin/build/block">Administer&#45&#62Site building&#45&#62Blocks
      </a></p>'),
  );

  $form['taxon_tree'][CDM_TAXONOMICTREE_UUID] = array(
    '#type' => 'select',
    '#title' => t('Available classifications') . ':',
    '#default_value' => variable_get(CDM_TAXONOMICTREE_UUID, FALSE),
    '#options' => cdm_get_taxontrees_as_options(),
    '#description' => t('Select the default taxa classification for your
      <em>taxon tree</em>, the other classifications will be also available but
      with a manual user change.'),
  );

  $form['taxon_tree']['taxontree_ranklimit'] = array(
    '#type' => 'select',
    '#title' => t('Rank of highest displayed taxon') . ':',
     // Before DEFAULT_TAXONTREE_RANKLIMIT_UUID.
    '#default_value' => variable_get('taxontree_ranklimit', DEFAULT_TAXONTREE_RANKLIMIT),
    '#options' => cdm_rankVocabulary_as_option(),
    '#description' => t('This is the rank of the highest displayed taxon in the
      <em>taxon tree</em>. You can select here which rank should be at the top
      level of the tree structure.'),
  );

  $form['aggregation'] = array(
      '#type' => 'fieldset',
      '#title' => t('Aggregation of data'),
      '#collapsible' => FALSE,
      '#description' => t("This section covers the different aspects of aggregating information.
          <p>
          </p>"),

  );

  $form['aggregation'][CDM_TAXON_MEDIA_FILTER] = array(
      '#type' => 'checkboxes',
      '#title' => 'Taxon media filter',
      '#default_value' => variable_get(CDM_TAXON_MEDIA_FILTER, unserialize(CDM_TAXON_MEDIA_FILTER_DEFAULT)),
      '#options' => array(
          'includeTaxonDescriptions' => 'Media in taxon descriptions',
          'includeTaxonNameDescriptions' => 'Media in name descriptions',
          'includeOccurrences' => 'Media related to specimens and occurrences',
      ),
      '#description' => 'This filter configures which images should be taken into account.',
  );

  $form['aggregation']['notice'] = array(
      '#markup' => '<strong>NOTICE:</strong> The below aggregation settings can slow down the data portal, so you may want to sensibly apply these setting and you may also
          want to make use of the caching capabilities of the dataportal.',
  );

  $form['aggregation']['media_aggregation'] = array(
      '#type' => 'fieldset',
      '#title' => t('Media aggregation'),
      '#collapsible' => FALSE,
      '#collapsed' => TRUE,
      '#description' => t("The media aggregation is also affected by the settigs in \"<strong>Aggregation via taxon relationsships</strong>\" below."),

  );
  $form['aggregation']['media_aggregation']['cdm_images_include_children'] = array(
      '#type' => 'select',
      '#title' => t('Aggregation of taxon pictures') . ':',
      '#default_value' => variable_get('cdm_images_include_children', FALSE),
      '#options' => array(
          0 => "Show only pictures of the current taxon",
          1 => "Include pictures of taxonomic children",
      ),
      '#description' => t("Choose whether to include the images of the taxonomic children. This will affect the <em>Images</em> tab and image tumbnails like in the search results."),
  );

  $form['aggregation']['aggregate_by_taxon_relationships'][CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class'=>array('clearfix')),
      '#title' => t('Aggregation via taxon relationsships'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
      '#description' => t('Information on taxa will be aggregated along the below chosen
          taxon relation ships. This will affect images and occurrences (specimens).
          Taxon relation ships are directed and point form one taxon to another. The taxon
          relationships to be taken into accunt can therefore configured for the direct direction
          and for the inverse.'),
  );

  $taxonRelationshipTypeOptions = cdm_Vocabulary_as_option(UUID_TAXON_RELATIONSHIP_TYPE, '_cdm_relationship_type_term_label_callback');
  $aggregate_by_taxon_relationships = variable_get(CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS, unserialize(CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS_DEFAULT));

  $form['aggregation']['aggregate_by_taxon_relationships'][CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS]['direct'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Direct'),
      '#options' => $taxonRelationshipTypeOptions,
      '#default_value' => $aggregate_by_taxon_relationships['direct'],
  );
  $form['aggregation']['aggregate_by_taxon_relationships'][CDM_AGGREGATE_BY_TAXON_RELATIONSHIPS]['invers'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Invers'),
      '#options' => $taxonRelationshipTypeOptions,
      '#default_value' => $aggregate_by_taxon_relationships['invers'],
  );

  // ----------------------
  $form['cdm_js_devel_mode'] = array(
      '#type' => 'checkbox',
      '#title' => 'Java-script developer mode',
      '#default_value' => variable_get('cdm_js_devel_mode', FALSE),
      '#description' => 'In production envirionments the java script libraries
      the cdm_dataportal is making use of are compressed and optimized. This
      is good for performance but a caveat if you need to debug java-script. When the
      java-script developer mode is enabled the uncompressed and commented developer
      versions of java-script libraries will be used where possible.
      <br/><strong>Do not use this option in production!</strong>'
  );

  // Comment @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );

  return system_settings_form($form);
}


/**
 * LAYOUT settings
 * @return unknown_type
 *   todo
 */
function cdm_settings_layout() {

  $form = array();

  $form['about'] = array(
    '#markup' => '<h4>' . t('Portal Layout') . '</h4><p>' . t('This settings contains the general configurations
      layout. If you want to configure the specific sites layout visit the
      respective configuration site for taxon, search or media.') . '</p>',
  );

  // ---- footnotes --- //
  $form['footnotes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footnotes'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Taxa data such authors, synonyms names, descriptions,
      media or distribution areas may have annotations or footnotes. When the
      footnotes are enabled they will be visible (if they exist).'),
  );

  $form['footnotes']['cdm_dataportal_all_footnotes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not show footnotes'),
    '#default_value' => variable_get('cdm_dataportal_all_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES),
    '#description' => t('Check this if you do not want to show any footnotes'),
  );

  $form['footnotes']['cdm_dataportal_annotations_footnotes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not show annotations footnotes'),
    '#default_value' => variable_get('cdm_dataportal_annotations_footnotes', CDM_DATAPORTAL_ANNOTATIONS_FOOTNOTES),
    '#description' => t('Check this if you do not want to show annotation footnotes'),
  );

  $annotationTypeOptions = cdm_Vocabulary_as_option(UUID_ANNOTATION_TYPE);
  // Additional option for the NULL case.
  $annotationTypeOptions['NULL_VALUE'] = t('untyped');
  $form['footnotes']['annotations_types_as_footnotes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Annotation types as footnotes'),
    '#description' => t("Only annotations of the selected type will be displayed
       as footnotes. You may want to turn 'technical annotations' off."),
    '#options' => $annotationTypeOptions,
  );
  $annotationsTypesAsFootnotes = variable_get('annotations_types_as_footnotes', unserialize(ANNOTATIONS_TYPES_AS_FOOTNOTES_DEFAULT));
  if (!empty($annotationsTypesAsFootnotes)) {
    $form['footnotes']['annotations_types_as_footnotes']['#default_value'] = $annotationsTypesAsFootnotes;
  }

  // --- Advanced Search --- //
  $form['asearch'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced search'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
  );
  $form['asearch']['cdm_dataportal_show_advanced_search'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show advanced search link'),
      '#default_value' => variable_get('cdm_dataportal_show_advanced_search', 1),
      '#description' => t('Check this box if the link to advanced search should be show below the search box.'),
  );

  // ---- Taxon Name Rendering --- //
  $form['taxon_name'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxon name display'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('The display of taxon names is configured by two parts.'
          . 'The <srong>name render templates</strong> define the parts of the name to be displayed in the different areas of the data portal pages.'
          . 'The name parts are defined in the <stong>part definitions</strong>'),
  );

  drupal_add_js(
      'jQuery(document).ready( function( ) {
         // pretty print json
         var json_text = jQuery(\'#edit-' . CDM_PART_DEFINITIONS . '\').val();
         var obj = JSON.parse(json_text);
         jQuery(\'#edit-' . CDM_PART_DEFINITIONS . '\').val(JSON.stringify(obj, undefined, 2));

         json_text = jQuery(\'#edit-' . CDM_NAME_RENDER_TEMPLATES . '\').val();
         obj = JSON.parse(json_text);
         jQuery(\'#edit-' . CDM_NAME_RENDER_TEMPLATES . '\').val(JSON.stringify(obj, undefined, 2));
       });'
      ,array(
          'type' => 'inline',
          'scope' => 'footer'
      )
  );

  //TODO remove after all portals are using the settings
  $default_part_definitions = theme('get_partDefinition', array());
  if(!is_array($default_part_definitions)) {
    $default_part_definitions = unserialize(CDM_PART_DEFINITIONS_DEFAULT);
  }

  $form['taxon_name'][CDM_PART_DEFINITIONS] = array(
      '#type' => 'textarea',
      '#title' => t('Part definitions'),
      '#element_validate' => array('form_element_validate_json'),
      '#default_value' =>  json_encode(variable_get(CDM_PART_DEFINITIONS, $default_part_definitions)),
      '#description' => '
          <p>
          (Clearing the text area will reset it to the default)
          </p>
          <p>
           The part definitions define the specific parts of which a rendered taxon name plus additional information will consist.
          </p>
          <p>
           A full taxon name plus additional information can consist of the following elements:
          <ul>
             <li>name: the taxon name inclugin rank nbut without author</li>
             <li>authors:  The authors of a reference, also used in taxon names</li>
             <li>reference: the nomenclatural reference,</li>
             <li>microreference:  Volume, page number etc.</li>
             <li>status:  The nomenclatural status of a name</li>
             <li>description: name descriptions like protologues etc ...</li>
          </ul>
          </p>
          <p>
           These elements are combined in the part definitions array to from the specific parts to be rendered.
           (The taxon name "Lapsana communis L., Sp. Pl.: 811. 1753" shall be an example in the following)
           Usually the following parts are formed:
          <ul>
            <li>namePart: the name and rank (in example: "Lapsana communis")</li>
            <li>authorshipPart: the author (in example: "L.")</li>
            <li>nameAuthorPart: the combination of name and author part (in example: "Lapsana communis L.").</li>
               This is useful for zoological names where the authorshipPart belongs to the name and both should</li>
               be combined when a link to the taxon is rendered.</li>
            <li>referencePart: the nomencaltural reference (in example: "Sp. Pl. 1753")</li>
            <li>microreferencePart: usually the page number (in example ": 811.")</li>
            <li>statusPart: the nomenclatorical status</li>
            <li>descriptionPart: name descriptions like protologues etc ...</li>
          </ul>
          </p>
          <p>
           Each set of parts is dedicated to render a specific TaxonName type, the type names are used as keys for the
           specific parts part definitions:
          <ul>
            <li>BotanicalName</li>
            <li>ZoologicalName</li>
            <li>#DEFAULT: covers ViralNames and other NonViralNames
          </ul>
           An example:
          <pre>
           {
            "ZoologicalName": {
              "namePart": {
                "name": true
              },
              "referencePart": {
                "authors": true
              },
              "microreferencePart": {
                "microreference": true
              },
              "statusPart": {
                "status": true
              },
              "descriptionPart": {
                "description": true
              }
            },
            "BotanicalName": {
              "namePart": {
                "name": true,
                "authors": true
              },
              "referencePart": {
                "reference": true,
                "microreference": true
              },
              "statusPart": {
                "status": true
              },
              "descriptionPart": {
                "description": true
              }
            }
          }
           </pre>',
  );

  //TODO remove after all portals are using the settings
  $default_render_templates = theme('get_nameRenderTemplate', array());
  if(!is_array($default_render_templates)) {
    $default_render_templates = unserialize(CDM_NAME_RENDER_TEMPLATES_DEFAULT);
  }

  $form['taxon_name'][CDM_NAME_RENDER_TEMPLATES] = array(
      '#type' => 'textarea',
      '#title' => t('Name render templates'),
      '#element_validate' => array('form_element_validate_json'),
      '#default_value' =>  json_encode(variable_get(CDM_NAME_RENDER_TEMPLATES, $default_render_templates)),
      '#description' => '
          <p>
          (Clearing the text area will reset it to the default)
          </p>
          <p>
          The render templates array contains one or more name render templates to be used within the page areas identified by the
          render path. The render path is used as key of the array sub subelements whereas the name render template array is set as value.
          The following render Path keys are curretly recognized:
          <ul>
            <li>list_of_taxa:</li>
            <li>acceptedFor:</li>
            <li>taxon_page_synonymy</li>
            <li>typedesignations</li>
            <li>taxon_page_title</li>
            <li>polytomousKey</li>
            <li>na: name + authorship</li>
            <li>nar:name + authorship + reference</li>
            <li>#DEFAULT</li>
          </ul>
          A single render template can be used for multiple render paths. In this case the according key of the render templates
          array element should be a comma separated list of render paths, without any whitespace!.
          </p>
          <p>
          A render template is an associative array. The keys of this array are referring to the keys as defined in the part
          definitions array. See <a href="#edit-cdm-part-definitions">Part definitions</a> above for more information.
          <p>
          The value of the render template element must be set to TRUE in order to let this part being rendered.
          The namePart, nameAuthorPart and referencePart can also hold an associative array with a single
          element: array(\'#uri\' => TRUE). The value of the #uri element will be replaced by the according
          links if the paramters $nameLink or $refenceLink are given to the name render function
          (this is hard coded and cannot be configured here).',
  );

  // @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );

  $form['#submit'] = array('submit_json_as_php_array');
  // #json_elements especially defined for submit_json_as_php_array()
  $form['#json_elements'] = array(CDM_NAME_RENDER_TEMPLATES, CDM_PART_DEFINITIONS);
  return system_settings_form($form);
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description = '') {
  $form[$form_name] = array(
    '#type' => 'fieldset',
    '#title' => t($form_title),
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
    '#tree' => TRUE,
    '#description' => t($form_description),
  );

  $default_values = unserialize(CDM_DATAPORTAL_GALLERY_SETTINGS);
  $gallery_settings = variable_get($form_name, $default_values);
  // $test = variable_get('cdm_dataportal_search_items_on_page', CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE);
  if ($form_name == CDM_DATAPORTAL_SEARCH_GALLERY_NAME) {
    /*
    TODO: why cdm_dataportal_search_items_on_page does not save the value on $test???
    $form[$form_name]['cdm_dataportal_search_items_on_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Page Size'),
    '#default_value' => $test,
    '#description' => t('Number of Names to display per page in search results.')
    );
    */
    $form[$form_name]['cdm_dataportal_show_taxon_thumbnails'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show media thumbnails for accepted taxa'),
      '#default_value' => $gallery_settings['cdm_dataportal_show_taxon_thumbnails'],
    );

    $form[$form_name]['cdm_dataportal_show_synonym_thumbnails'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show media thumbnails for synonyms'),
      '#default_value' => $gallery_settings['cdm_dataportal_show_synonym_thumbnails'],
      '#description' => '',
    );
  }

  // $showCaption = variable_get('cdm_dataportal_findtaxa_show_thumbnail_captions', 0);
  $form[$form_name]['cdm_dataportal_show_thumbnail_captions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show captions under thumbnails'),
    '#default_value' => $gallery_settings['cdm_dataportal_show_thumbnail_captions'],
    '#description' => '',
  );

  $form[$form_name]['cdm_dataportal_media_maxextend'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail size') . ':',
    '#default_value' => $gallery_settings['cdm_dataportal_media_maxextend'],
    '#description' => t('Select the size of each individual thumbnail.'),
  );

  if ($form_name != CDM_DATAPORTAL_MEDIA_GALLERY_NAME) {
    $form[$form_name]['cdm_dataportal_media_cols'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of columns') . ':',
      '#default_value' => $gallery_settings['cdm_dataportal_media_cols'],
      '#description' => t('Group the thumbnails in columns: select how many
        columns the gallery should display.'),
    );
  }

  if ($form_name == CDM_DATAPORTAL_SEARCH_GALLERY_NAME) {
    $form[$form_name]['cdm_dataportal_media_maxRows'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of rows') . ':',
      '#default_value' => $gallery_settings['cdm_dataportal_media_maxRows'],
      '#description' => t('You can group the thumbnails in rows, select in how
        many rows should be the thumbnails grouped.<br/><strong>Note:</strong>
        If you want an unlimited number of rows please set to 0.'),
    );
  }

  return $form;
}

/**
 * @todo document this function.
 */
function cdm_settings_layout_taxon() {
  $collapsed = FALSE;
  $form = array();

  $form['#submit'][] = 'cdm_settings_layout_taxon_submit';

  $form['cdm_dataportal_show_back_to_search_results'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show <em>Back to search results</em> link at the taxon site.'),
      '#default_value' => variable_get('cdm_dataportal_show_back_to_search_results', 1),
      '#description' => t('<p>If checked the link to search results is rendered at
       the top of the taxon site. Clicking on the link the last search performed
       is rendered again.</p>'),
  );

  // --------- TABBED TAXON ------- //
  $form['taxon_tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon tabs'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('If tabbed taxon page is enabled the taxon profile will
      be splitted in four diferent tabs; General, Synonymy, Images and
      Specimens. If the taxon has no information for any of the tabs/sections
      such tab will be not displayed.'),
  );

  $form['taxon_tabs']['cdm_dataportal_taxonpage_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabbed taxon page'),
    '#default_value' => variable_get('cdm_dataportal_taxonpage_tabs', 1),
    '#description' => t('<p>If selected split the taxon page into individual
      tabs for description, images, synonymy and specimens. If not the taxon
      data is rendered as a long single page without tabs.</p>'),
  );

  $form['taxon_tabs']['cdm_taxonpage_tabs_visibility'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Tabs visibility options') . ':',
    '#default_value' => variable_get('cdm_taxonpage_tabs_visibility', get_taxon_options_list()),
    '#options' => get_taxon_options_list(),
    '#description' => t('Enable or disable Tabs in the Tabbed page display'),
  );

  $form['taxon_tabs']['cdm_dataportal_default_tab'] = array(
    '#type' => 'select',
    '#title' => t('Default tab to display') . ':',
    '#default_value' => variable_get('cdm_dataportal_default_tab', 0),
    '#options' => unserialize(CDM_DATAPORTAL_DEFAULT_TAXON_TAB),
    '#description' => t('<p>Select the default tab to display when visiting a
      taxon page. Only available if Tabbed Taxon Page is enable.</p>
      <strong>Note:</strong> After performing a search and clicking in any
      synonym, the taxon tab to be rendered will be the synonymy of the accepted
      taxon and not the above selected tab.'),
  );

  /* ======  TAXON_PROFILE ====== */
  $form['taxon_profile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon profile (tab)'),
    '#description' => t('<p>This section covers the settings related to the taxon
      profile tab, also known as the <strong>"General"</strong> tab.</p>'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // ---- PROFILE PICTURE ----//

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Taxon profile picture'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('This sections allows configuring the display of the so called taxon profile image which is displayed in the taxon profile tab.'),
  );

  //FIXME migrate variables:
  //  cdm_dataportal_show_default_image ---> CDM_TAXON_PROFILE_IMAGE['show']
  // FIXME
  //  eanable file modul in profile and in update

  $taxon_profile_image_settings = variable_get(CDM_TAXON_PROFILE_IMAGE, unserialize(CDM_TAXON_PROFILE_IMAGE_DEFAULT));
  /*
   * 'show' => 1,
   * 'maxextend' => 184,
   * 'media_uri_query' => ''
   * 'custom_placeholder_image_on' => 1,
   * 'custom_placeholder_image_fid' => ''
   */
  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable profil picture'),
    '#description' => t('Show the profil picture.'),
    '#default_value' => $taxon_profile_image_settings['show'],
  );

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['maxextend'] = array(
      '#type' => 'textfield',
      '#tree' => TRUE,
      '#title' => t('Profil picture maximum extend'),
      '#default_value' =>  $taxon_profile_image_settings['maxextend'],
      '#field_suffix' => 'px',
      '#maxlength' => 4,
      '#size' => 4,
      '#description' => t('The maximum extend in either dimension, width or height, of the profil picture in pixels.')
  );

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['media_uri_query'] = array(
      '#type' => 'textfield',
      '#tree' => TRUE,
      '#title' => t('Additional URI query parameter'),
      '#default_value' =>  $taxon_profile_image_settings['media_uri_query'],
      '#maxlength' => 1024,
      '#size' => 60,
      '#description' => t('Additional query parameters to be used when requesting for the '
          . 'profile image. E.g.: <code>width=400&height=300&quality=95&format=jpeg</code>.'
          . 'The query parameters will be appendend to the uri of the media representation part'
          . ' as stored in the cdm. The query parameter string must not start with a \'&\' or  \'?\'')
  );

  $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_on'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use a custom placeholder image'),
      '#description' => t("This image is shown as replacement if no image of the taxon is available."),
      '#default_value' => $taxon_profile_image_settings['custom_placeholder_image_on']
  );

  if($taxon_profile_image_settings['custom_placeholder_image_on'] == 1){
    $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_fid'] = array(
        '#type' => 'managed_file',
        '#title' => t('Custom placeholder image file'),
        '#progress_indicator' => 'bar',
        '#default_value' => $taxon_profile_image_settings['custom_placeholder_image_fid'],
    //       '#name' => 'custom_placeholder_image',
        '#upload_location' => 'public://' . CDM_TAXON_PROFILE_IMAGE .'/'
    );

    if($taxon_profile_image_settings['custom_placeholder_image_fid']){
      $profile_image_file = file_load($taxon_profile_image_settings['custom_placeholder_image_fid']);
      $url = file_create_url($profile_image_file->uri);
      $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['preview'] = array(
                '#type' => 'item',
                '#markup' => '<div class="image-preview"><img src="' . $url . '"/></div>',
      );
    }
  } else {
    $form['taxon_profile'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_fid'] = array(
        '#type' => 'hidden',
        '#default_value' => $taxon_profile_image_settings['custom_placeholder_image_fid']
    );
  }



  $options = cdm_rankVocabulary_as_option();
  array_unshift($options, '-- DISABLED --');
  $form['taxon_profile']['picture']['image_hide_rank'] = array(
    '#type' => 'select',
    '#title' => t('Hide profile picture for higher ranks') . ':',
    '#default_value' => variable_get('image_hide_rank', '0'),
    '#options' => $options,
    '#description' => t('The taxon profile picture will not be shown for taxa with rank higher that the selected.'),
  );

  // -- MEDIA THUMBNAILS -- //
  $form_name = CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME;
  $form_title = 'Taxon Profile Images';
  $form_description = '<p>The different section in the taxon  profile can have images associated with them. These images are displayed in a gallery of thumbnails wich can be configuered here:</p>';
  $form['taxon_profile'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description);

  // ---- FEATURE TREE ---- //
  $form['taxon_profile']['feature_trees'] = array(
    '#type' => 'fieldset',
    '#title' => t('Features'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t("This section covers settings related to the taxon's
      <em>Feature Tree</em>. The <em>feature tree</em> are the taxon's
      features such as description, distribution, common names, etc. that Drupal
      will render at the taxon profile page."),
  );
  $featureTrees = cdm_get_featureTrees_as_options(TRUE);
  $saved_uuid = variable_get(CDM_PROFILE_FEATURETREE_UUID, UUID_DEFAULT_FEATURETREE);
  if(!isset($featureTrees['options'][$saved_uuid])) {
    $saved_uuid = UUID_DEFAULT_FEATURETREE;
  }
  $form['taxon_profile']['feature_trees'][CDM_PROFILE_FEATURETREE_UUID] = array(
    '#type' => 'radios',
    '#title' => t('Taxon profile sections') . ':',
    '#default_value' => $saved_uuid,
    '#options' =>  $featureTrees['options'],
    '#pre_render' => array('form_pre_render_conditional_form_element', 'radios_prepare_options_suffix'),
    '#options_suffixes' => $featureTrees['treeRepresentations'],
    '#description' => t('Select the Feature Tree to be displayed at the taxon
      profile. Click "Show Details" to see the Feature Tree elements.'
    ),
  );
  $featureTrees = cdm_get_featureTrees_as_options();
  $saved_uuid = variable_get(CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID, UUID_DEFAULT_FEATURETREE);
  if(!isset($featureTrees['options'][$saved_uuid])) {
    $saved_uuid = NULL;
  }
  $form['taxon_profile']['feature_trees'][CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID] = array(
    '#type' => 'radios',
    '#title' => t('Natural language representation of structured descriptions') . ':',
    '#default_value' => $saved_uuid,
    '#options' => $featureTrees['options'],
    '#pre_render' => array('form_pre_render_conditional_form_element', 'radios_prepare_options_suffix'),
    '#options_suffixes' => $featureTrees['treeRepresentations'],
    '#description' => t('Taxon descriptions can be stored in a highly structured
      form. The feature tree selected here will be used to generate textual
      representation in natural language.'
    ),
  );

  // ---- LAYOUT PER FEATURE ---- //
  $feature_tree = get_profile_featureTree();
  if (isset($feature_tree->root->children)) {

    $form_feature_list_layout = array(
      '#title' => t('Taxon profile layout'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#type' => 'fieldset',
      '#description' => t('Will be available in a future release.'),
    );

    $feature_list_layout_settings_disabled = TRUE;
    foreach ($feature_tree->root->children as $featureNode) {

      if (!$feature_list_layout_settings_disabled && isset($featureNode->feature)) {
        // Must not exceed 45 characters !!!
        $subform_id = LAYOUT_SETTING_PREFIX . $featureNode->feature->uuid;

        $settings = mixed_variable_get($subform_id, FEATURE_TREE_LAYOUT_DEFAULTS);
        $systemDefaults = unserialize(FEATURE_TREE_LAYOUT_DEFAULTS);

        $form_feature_list_layout[$subform_id] = array(
          '#tree' => TRUE,
          '#title' => 'test' . $featureNode->feature->representation_L10n,
          '#collapsible' => FALSE,
          '#collapsed' => FALSE,
          '#type' => 'fieldset',
          '#description' => t(''),
        );

        $form_feature_list_layout[$subform_id]['enabled'] = array(
          '#type' => 'checkbox',
          '#title' => t('Enable'),
          '#default_value' => $settings['enabled'],
          '#description' => t('Enable user defined layout for this feature'),
        );

        $form_feature_list_layout[$subform_id]['enclosingTag'] = array(
          '#type' => 'textfield',
          '#title' => t('Enclosing tag'),
          '#disabled' => !$settings['enabled'],
          '#default_value' => $settings['enclosingTag'],
          '#description' => t('Default is: ') . "'<code>" . $systemDefaults['enclosingTag'] . "</code>'",
        );

        $form_feature_list_layout[$subform_id]['entryEnclosingTag'] = array(
          '#type' => 'textfield',
          '#title' => t('Entry enclosing tag'),
          '#disabled' => !$settings['enabled'],
          '#default_value' => $settings['entryEnclosingTag'],
          '#description' => t('Default is: ') . "'<code>" . $systemDefaults['entryEnclosingTag'] . "</code>'",
        );

        $form_feature_list_layout[$subform_id]['glue'] = array(
          '#type' => 'textfield',
          '#title' => t('Glue'),
          '#disabled' => !$settings['enabled'],
          '#default_value' => $settings['glue'],
          '#description' => t('Default is: ') . "'<code>" . $systemDefaults['glue'] . "</code>'",
        );

      }

      $form['taxon_profile']['feature_list_layout'] = $form_feature_list_layout;
    }
  }

  // ---- DISTRIBUTION LAYOUT ---- //
  $form['taxon_profile']['distribution_layout'] = array(
    '#title' => t('Distribution'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#type' => 'fieldset',
    '#description' => t('Select if you want to sort or not the distribution text
      located below the distribution map.'),
  );

  $form['taxon_profile']['distribution_layout']['distribution_sort'] = array(
    '#type' => 'radios',
    '#title' => t('Sort') . ':',
    '#default_value' => variable_get('distribution_sort', 'NO_SORT'),
    '#options' => array(
      'NO_SORT' => t('Standard (No sort)'),
      'HIDE_TDWG2' => t('Sorted without TDWG Level 2'),
    ),
  );

  $form['taxon_profile']['distribution_layout'][DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP] = array(
    '#type' => 'checkbox',
    '#title' => t('Show TextData elements on top of the map'),
    '#default_value' => variable_get(DISTRIBUTION_TEXTDATA_DISPLAY_ON_TOP, 0),
    '#description' => t('Check this if you want to appear all <code>TextData</code>
      elements on top of the map. Otherwise all <code>TextData</code>
      distribution elements will be listed below the other area elements.
      This option is useful if you need to have descriptive texts for each
      distribution map.'),
  );


  /* ====== SYNONYMY ====== */
  $form['taxon_synonymy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon synonymy (tab)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('This section covers the settings related to the taxon
      <strong>synonymy</strong> tab.'),
  );

  $form['taxon_synonymy']['cdm_dataportal_nomref_in_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show accepted taxon on top of the synonymy'),
    '#default_value' => variable_get('cdm_dataportal_nomref_in_title', CDM_DATAPORTAL_NOMREF_IN_TITLE),
    '#description' => t('If checked, the first homotypic taxon is a repetition
      of the accepted taxon most likely with the full nomenclatural reference
      (depending on the currently chosen theme).'),
  );

  $form['taxon_synonymy']['cdm_dataportal_display_is_accepted_for'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display <em>is accepted for ...</em> on taxon pages when
      coming from a synonym link.'),
    '#default_value' => variable_get('cdm_dataportal_display_is_accepted_for', CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR),
    '#description' => t('Check this if after doing a search and clicking on a
      synonym you want to see the "accept of" text for the accepted synonym.'),
  );

  $nameRelationshipTypeOptions = cdm_Vocabulary_as_option(UUID_NAME_RELATIONSHIP_TYPE);
  $form['taxon_synonymy']['name_relationships']['name_relationships_to_show'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display name relationships') . ':',
    '#default_value' => variable_get('name_relationships_to_show', 0),
    '#options' => $nameRelationshipTypeOptions,
    '#description' => t('Select the name relationships you want to show for the
      accepted taxa.'),
  );

  $form['taxon_synonymy'][CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS] = array(
    '#type' => 'checkbox',
    '#title' => t('Show taxon relations ships of accepted taxon'),
    '#default_value' => variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT),
    '#description' => t('If this option is enabled the synonymy will show the
      below selected taxon relationships of accepted taxa.'),
  );

  $taxonRelationshipTypeOptions = cdm_Vocabulary_as_option(UUID_TAXON_RELATIONSHIP_TYPE, '_cdm_relationship_type_term_label_callback');
  $form['taxon_synonymy'][CDM_TAXON_RELATIONSHIP_TYPES] = array(
    '#type' => 'checkboxes',
    '#title' => t('Taxon relationship types') . ':',
    '#description' => t('Only taxon relationships of the selected type will be
      displayed'),
    '#options' => $taxonRelationshipTypeOptions,
    '#default_value' => variable_get(CDM_TAXON_RELATIONSHIP_TYPES, unserialize(CDM_TAXON_RELATIONSHIP_TYPES_DEFAULT)),
    '#disabled' => !variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT),
  );

  // ====== SPECIMENS ====== //
  $form['taxon_specimens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxon specimens (tab)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('This section covers the settings related to the taxon
      <strong>specimens</strong> tab.'),
  );

  $featureTrees = cdm_get_featureTrees_as_options(TRUE);
  $saved_uuid = variable_get(CDM_OCCURRENCE_FEATURETREE_UUID, UUID_DEFAULT_FEATURETREE);
  if(!isset($featureTrees['options'][$saved_uuid])) {
    $saved_uuid = UUID_DEFAULT_FEATURETREE;
  }
  $form['taxon_specimens']['feature_trees'][CDM_OCCURRENCE_FEATURETREE_UUID] = array(
      '#type' => 'radios',
      '#title' => t('Specimen description feature tree') . ':',
      '#default_value' => $saved_uuid,
      '#options' =>  $featureTrees['options'],
      '#pre_render' => array('form_pre_render_conditional_form_element', 'radios_prepare_options_suffix'),
      '#options_suffixes' => $featureTrees['treeRepresentations'],
      '#description' => t('Select the feature tree to be used for displaying specimen descriptions. Click "Show Details" to see the Feature Tree elements.'
      ),
  );

  $form_name = CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME;
  $form_title = t('Specimen media');
  $form_description = t('Specimens may have media which is displayed at the
     Specimen tab/section as a gallery. It is possible to configure the
     thumbnails gallery here, however for configuring how a single media should
     be displayed please go to !url.</p>',
     array(
       '!url' => l(t('Layout -> Media'), 'admin/config/cdm_dataportal/settings/layout/media'),
     ));
  $form['taxon_specimens'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, FALSE, $form_description);

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

  // Comment @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );
  return system_settings_form($form);
}

/**
 * @todo document this function.
 */
function cdm_settings_layout_search() {

  $form = array();

  $form['search_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxa Search'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('<p>The data portal allows the users to perform searchs.</p><p>To perform searchs
         the block <em>CDM Taxon Search</em> should be enabled and visible for users
         where they can write the text to be searched. You can find Drupal block configuration
         site at <a href="./?q=admin/build/block">Administer&#45&#62Site building&#45&#62Blocks</a></p> '),
  );

  $form['search_settings']['simple_search_ignore_classification'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ignore the chosen classification in simple search'),
      '#default_value' => variable_get('simple_search_ignore_classification', TRUE),
      '#description' => t('The simple search, which can be executed via the search block,
          will by default search on all classifications. Remove the tick if you want your
          portal to search on the classification selected in the classification browser
          selector.'),
  );

  $form['search_settings']['cdm_dataportal_search_items_on_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Results per page') . ':',
    '#default_value' => variable_get('cdm_dataportal_search_items_on_page', CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE),
    '#description' => t('Number of results to display per page.'),
  );

  $form['search_settings']['cdm_search_taxa'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sets the default value of <em>Search for accepted taxa</em> in the advanced search form.'),
    '#default_value' => variable_get('cdm_search_taxa', 1),
    '#description' => t('<p>If checked the <em>Search for accepted taxa </em> option will be enabled in the advanced form.</p>'),
  );

  $form['search_settings']['cdm_search_synonyms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sets the default value of <em>Search for synonyms</em> in the advanced search form.'),
    '#default_value' => variable_get('cdm_search_synonyms', 1),
    '#description' => t('<p>If checked the <em>Search for synonyms</em> option will be enabled in the advanced form.</p>'),
  );

  $form['search_settings']['cdm_search_common_names'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sets the default value of <em>Search for common names</em> in the advanced search form.'),
    '#default_value' => variable_get('cdm_search_common_names', 0),
    '#description' => t('<p>If checked the <em>Search for common names</em> option will be enabled in the advanced form.</p>'),
  );

  $form['search_settings']['cdm_search_use_default_values'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sets use of default values in the advanced search form.'),
    '#default_value' => variable_get('cdm_search_use_default_values', 1),
    '#description' => t('<p>If checked the defqult values set abovewill be used for the search.</p>'),
  );


  // --- SEARCH TAXA GALLERY ---- //
  $items = variable_get('cdm_dataportal_search_items_on_page', CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE);
  $collapsed = FALSE;
  $form_name = CDM_DATAPORTAL_SEARCH_GALLERY_NAME;
  $form_title = 'Taxa Search thumbnails';
  $form_description = 'Search results may show thumbnails. ';
  $form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed, $form_description);

  // Comment @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );
  return system_settings_form($form);
}

/**
 * @todo document this function.
 */
function cdm_settings_layout_media() {

  $form = array();

  $form['media_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => 'This section covers layout settings for media pages.'
      . 'Further media related settings may be found under the taxon layout settings and on the general settings.',
  );

  $form['media_settings']['image_gallery_viewer'] = array(
    '#type' => 'select',
    '#title' => t('Image viewer') . ':',
    '#default_value' => variable_get('image_gallery_viewer', 'default'),
    '#options' => array(
      'default' => t('Standard image viewer'),
      'fsi' => t('FSI viewer (requires FSI server!)'),
    ),
  );

  // --- MEDIA GALLERY ---- //
  $form_name = CDM_DATAPORTAL_MEDIA_GALLERY_NAME;
  $form_title = 'Standard viewer';
  $form_description = '<p>Configure the standard image viewer.</p><p><strong>Note:</strong> the image viewer should selected otherwise settings are not taking into account.</p>';
  // $form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, $collapsed);
  $form['media_settings'][] = cdm_dataportal_create_gallery_settings_form($form_name, $form_title, FALSE, $form_description);

  // @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );
  return system_settings_form($form);
}

/**
 * GEOSERVICE and Map settings.
 */
function cdm_settings_geo($form, &$form_state) {

  $current_geoserver_settings = get_edit_map_service_settings();
  $map_distribution = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);

  $form = array();

  $dummy_distribution_query = NULL;
  if($map_distribution['map_type'] != 1){
    // we need to apply a dummy query since the map serice requires for image maps
    // at least as and ad to be defined
    $dummy_distribution_query = "as=a:339966&ad=tdwg1:a:1,2,3,4,5,6,7,8,9";
  }
  $form['map_preview'] = array(
      '#type' => 'fieldset',
      '#tree' => FALSE,
      '#title' => t('Map preview'),
      '#collapsible' => FALSE,
      '#description' => 'The preview of the map'
       . ($dummy_distribution_query != null ?
           ' may not be accurate in case if image maps, please check the map display in the taxon pages.':
           '.<br/>Hold down Strg and drag with your mouse to select a bbox to zoom to. <br/>The bbox of the visisble area of the map is always displayed below the map.')
  );
  $form['map_preview']['map'] = compose_map(NULL, $dummy_distribution_query, NULL,
      array(
          'move' => "this.cdmOpenlayersMap.printInfo",
          '#execute' => "this.cdmOpenlayersMap.printInfo"
      )
  );


  /*
   * GEO SERVER
   */
  $form['edit_map_server'] = array(
    '#type' => 'fieldset',
    '#tree' => true,
    '#title' => t('EDIT map service'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Configuration and selection of your geo server.
      The Geo Server is responsible for generating the maps.'),
  );

  $form['edit_map_server']['base_uri'] = array(
    '#type' => 'select',
    '#title' => t('EDIT map service') . ':',
    '#default_value' => $current_geoserver_settings['base_uri'],
    '#options' => unserialize(EDIT_MAPSERVER_URI),
    '#description' => t('Select the EDIT map server you want to use within your data portal.'),
  );
  $form['edit_map_server']['version'] = array(
      '#type' => 'select',
      '#title' => t('Version') . ':',
      '#default_value' => $current_geoserver_settings['version'],
      '#options' => unserialize(EDIT_MAPSERVER_VERSION),
      '#description' => t('The version of the EDIT map services'),
  );

  $localhostkey = 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ';
  $gmap_api_key = variable_get('gmap_api_key', 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ');
  $form['gmap_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Google maps API key') . ':',
      '#default_value' => variable_get('gmap_api_key', $gmap_api_key),
      '#description' => t('If you want to use the Google Maps Layer, a key is
      needed. If you need a key, visit
      <a href="http://code.google.com/intl/en/apis/maps/signup.html">google maps api key</a>.
      <br/><strong>Note:</strong> The following key: <code>!localhostkey</code>
      is the default key for the localhost (127.0.0.1).',
      array('!localhostkey' => $localhostkey)),
  );


  /*
   * MAP SETTINGS
   */

  $form[CDM_MAP_DISTRIBUTION] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Maps settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('General configuration for all map types.'),
  );

  /*
   * settings for the distribution map are used also for specimens map!!!!
   */

  $form[CDM_MAP_DISTRIBUTION]['width'] = array(
    '#type' => 'textfield',
    '#title' => 'Width',
    '#default_value' => $map_distribution['width'],
    '#maxlength' => 4,
    '#size' => 4,
    '#description' => 'Width of the map. To allow OSM baselayers to zoom out to the full extend of the world the map width must be
      a multiple of 256px since the osm tiles from tile.openstreetmap.org have a size of 256px x 256px and frational zoom
      levels are not possible in this case.',
  );
  $form[CDM_MAP_DISTRIBUTION]['height'] = array(
      '#type' => 'textfield',
      '#title' => 'Height',
      '#default_value' => $map_distribution['height'],
      '#maxlength' => 4,
      '#size' => 4,
      '#description' => 'Height of the map. Depending on the chosen base layer you may want to choose the height equal
      to the width or half of the width. Any other aspect ratio is also possible if desired.',
  );

  $form[CDM_MAP_DISTRIBUTION]['bbox'] = array(
    '#type' => 'textfield',
    '#title' => 'Bounding box',
    '#default_value' => $map_distribution['bbox'],
    '#description' => t('The bounding box defines the area to be initially displayed in maps.
      Use "-180,-90,180,90" for the whole world. Leave <strong>empty</strong>
      to let the map <strong>automatically zoom</strong> to the bounds enclosing the shown data.</p>
      <strong>TIP: </strong>You can use the map preview above to choose a bbox from the map. Maybe you need to change the map type to OpeLayers.
      Hold down Strg and drag with your mouse to select a bbox to zoom to. The bbox of the visisble area of the map is always displayed
      below the map from where you can copy the bbox string.</p>'),
  );

  $form[CDM_MAP_DISTRIBUTION]['show_labels'] = array(
    '#type' => 'checkbox',
    '#title' => 'Display area labels',
    '#default_value' => $map_distribution['show_labels'],
    '#description' => t('The map will show name labels of the areas'),
  );

  $form[CDM_MAP_DISTRIBUTION]['caption'] = array(
    '#type' => 'textfield',
    '#title' => 'Map caption',
    '#default_value' => $map_distribution['caption'],
    '#description' => t('The caption will be shown below the map.'),
  );

  $form[CDM_MAP_DISTRIBUTION]['distribution_opacity'] = array(
    '#type' => 'textfield',
    '#title' => 'Distribution layer opacity',
    '#default_value' => $map_distribution['distribution_opacity'],
    '#description' => t('Valid values range from 0.0 to 1.0. Value 1.0 means the distributions (the countries or regions) will
                           fully visible, while a value near to 0.0 will be not much visible.'),
  );

  $form[CDM_MAP_DISTRIBUTION]['map_type'] = array(
    '#type' => 'radios',
    '#title' => 'Map types',
    '#options' => array(
      1 => "OpenLayers dynamic map viewer",
      0 => "Plain image",
    ),
    '#default_value' => $map_distribution['map_type'],
    '#description' => 'Two different map types are available :
      <ul><li><em>OpenLayers</em>: Display the maps in an interactive viewer
      which allows zooming and panning. If enabled you can configure the default layer
      (background of your maps) below.</li>
      <li><em>Plain image</em>: The map will be static non interactive
      image.</li></ul>',
  );
  $open_layers_is_enabled = $map_distribution['map_type'] == 1;


  // --- Plain Image Settings --- //
  $form[CDM_MAP_DISTRIBUTION]['image_map'] = array(
    '#type' => 'fieldset',
    '#title' => 'Plain image map settings',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => $open_layers_is_enabled,
    '#description' => 'The settings in this section are still expertimental
      and can only be used with the EDIT map service version 1.1 or above.',
  );
  $edit_mapserver_version = get_edit_map_service_version_number();
  if ($edit_mapserver_version < 1.1) {
    $form[CDM_MAP_DISTRIBUTION]['image_map']['#description'] = '<div class="messages warning">' . t("The chosen EDIT map service version ($edit_mapserver_version) is too low, it must be at least 1.1") . '</div>'
      . $form[CDM_MAP_DISTRIBUTION]['image_map']['#description'];
  }

  $form[CDM_MAP_DISTRIBUTION]['image_map']['base_layer'] = array(
    '#type' => 'textfield',
    '#title' => 'Background layer',
    '#default_value' => $map_distribution['image_map']['base_layer'],
    '#description' => t('Background layer. For available layers inspect !url1 or !url2.', array(
      '!url1' => l('deegree-csw', 'http://edit.africamuseum.be:8080/deegree-csw/md_search.jsp'),
      '!url2' => l('geoserver layers', 'http://edit.africamuseum.be/geoserver/web/'),
    )),
  );

  $form[CDM_MAP_DISTRIBUTION]['image_map']['bg_color'] = array(
    '#type' => 'textfield',
    '#title' => 'Background color',
    '#default_value' => $map_distribution['image_map']['bg_color'],
  );

  $form[CDM_MAP_DISTRIBUTION]['image_map']['layer_style'] = array(
    '#type' => 'textfield',
    '#title' => 'Background layer style',
     // Only line color by now.
    '#default_value' => $map_distribution['image_map']['layer_style'],
    '#description' => 'Syntax: {Area fill color},{Area stroke color},{Area stroke width},{Area stroke dash style}',
  );


  // --- OpenLayers Settings --- //
  $form[CDM_MAP_DISTRIBUTION]['openlayers'] = array(
    '#type' => 'fieldset',
    '#title' => 'OpenLayers settings',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => !$open_layers_is_enabled,
    '#description' => '',
  );


  $form[CDM_MAP_DISTRIBUTION]['openlayers']['display_outside_max_extent'] = array(
      '#type' => 'checkbox',
      '#title' => 'Display outside max extent',
      '#default_value' => $map_distribution['openlayers']['display_outside_max_extent'],
      '#description' => t('Allows the map to display parts of the layers which are outside
         the max extent if the aspect ratio of the map and of the baselayer
         are not equal.'),
  );

  $form[CDM_MAP_DISTRIBUTION]['openlayers']['show_layer_switcher'] = array(
      '#type' => 'checkbox',
      '#title' => 'Show Layer Switcher',
      '#default_value' => $map_distribution['openlayers']['show_layer_switcher'],
      '#description' => 'The Layer Switcher control displays a table of contents
      for the map.  This allows the user interface to switch between
      base layers and to show or hide overlays.  By default the switcher is
      shown minimized on the right edge of the map, the user may expand it
      by clicking on the handle.',
  );

  if (!$open_layers_is_enabled) {
    $form[CDM_MAP_DISTRIBUTION]['openlayers']['#description'] = '<div class="messages warning">'
        . 'The Openlayers viewer is currently not enabled! (see section Maps settings above )</div>'
        . $form[CDM_MAP_DISTRIBUTION]['openlayers']['#description'];
  }

  $baselayer_options = array(
    /*
   NOTICE: must correspond to the layers defined in
   js/openlayers_,ap.js#getLayersByName()
   */
    'osgeo_vmap0' => "Metacarta Vmap0 (OSGeo server)", // EPSG:4326: EPSG:900913
    'metacarta_vmap0' => "Metacarta Vmap0 (MetaCarta Labs server)", // EPSG:4326, EPSG:900913
    // all others EPSG:900913
    // 'edit-vmap0_world_basic' => 'EDIT Vmap0',
    'edit-etopo1' => "ETOPO1 Global Relief Model",
    'mapnik' => 'OpenStreetMap',
    'mapquest_open' => "MapQuest",
    'mapquest_sat' => "MapQuest Sattelite",
//     'osmarender' => 'OpenStreetMap (Tiles@home)',
    'gmap' => 'Google Streets',
    'gsat' => 'Google Satellite',
    'ghyb' => 'Google Hybrid',
//     'veroad' => 'Virtual Earth Roads',
//     'veaer' => 'Virtual Earth Aerial',
//     'vehyb' => 'Virtual Earth Hybrid',
    // 'yahoo' => 'Yahoo Street',
    // 'yahoosat' => 'Yahoo Satellite',
    // 'yahoohyb' => 'Yahoo Hybrid',
     'custom_wms_base_layer_1' => 'Custom WMS base layer (needs to be manually configured below!)',
  );

  $form[CDM_MAP_DISTRIBUTION]['openlayers']['base_layers'] = array(
    '#type' => 'checkboxes_preferred',
    '#title' => 'Base Layers',
    '#options' => $baselayer_options,
    '#default_value' =>  $map_distribution['openlayers']['base_layers'],
    '#description' => 'Choose the baselayer layer you prefer to use as map background in the OpenLayers dynamic mapviewer.',
  );

  $form[CDM_MAP_DISTRIBUTION]['openlayers']['custom_wms_base_layer'] = array(
      '#type' => 'fieldset',
      '#title' => 'Custom WMS base layer',
      '#tree' => TRUE,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => 'Here you an define a custom wms layer as additional base layer.',
  );

  $form[CDM_MAP_DISTRIBUTION]['openlayers']['custom_wms_base_layer']['name'] = array(
      '#type' => 'textfield',
      '#title' => 'Layer name',
      // Only line color by now.
      '#default_value' => $map_distribution['openlayers']['custom_wms_base_layer']['name'],
      '#description' => 'A arbitrary name for the layer.',
  );
  $form[CDM_MAP_DISTRIBUTION]['openlayers']['custom_wms_base_layer']['url'] = array(
      '#type' => 'textfield',
      '#title' => 'WMS url',
      // Only line color by now.
      '#default_value' => $map_distribution['openlayers']['custom_wms_base_layer']['url'],
      '#description' => 'Base url for the WMS (e.g.  http://wms.jpl.nasa.gov/wms.cgi)'
  );
  $form[CDM_MAP_DISTRIBUTION]['openlayers']['custom_wms_base_layer']['params'] = array(
      '#type' => 'textarea',
      '#title' => 'WMS parameters',
      '#element_validate' => array('form_element_validate_json'),
      // Only line color by now.
      '#default_value' => $map_distribution['openlayers']['custom_wms_base_layer']['params'],
      '#description' => 'An javasript object with key/value pairs representing the GetMap query string parameters and parameter values, entered in valid JSON.'
  );
  $form[CDM_MAP_DISTRIBUTION]['openlayers']['custom_wms_base_layer']['projection'] = array(
      '#type' => 'textfield',
      '#title' => 'Projection',
      // Only line color by now.
      '#default_value' => $map_distribution['openlayers']['custom_wms_base_layer']['projection'],
      '#description' => 'The desired projection for the layer (e.g. EPSG:4326, EPSG:900913, EPSG:3857)'
  );
  $form[CDM_MAP_DISTRIBUTION]['openlayers']['custom_wms_base_layer']['max_extent'] = array(
      '#type' => 'textfield',
      '#title' => 'Maximum extent',
      // Only line color by now.
      '#default_value' => $map_distribution['openlayers']['custom_wms_base_layer']['max_extent'],
      '#description' => 'The maximum extent of the map as boundin box in the units of the map.'
  );
  $form[CDM_MAP_DISTRIBUTION]['openlayers']['custom_wms_base_layer']['units'] = array(
      '#type' => 'textfield',
      '#title' => 'Units',
      '#default_value' => $map_distribution['openlayers']['custom_wms_base_layer']['units'],
      '#description' => 'The layer map units.  Defaults to null.  Possible values are ‘degrees’ (or ‘dd’), ‘m’, ‘ft’, ‘km’, ‘mi’, ‘inches’.  Normally taken from the projection.  Only required if both map and layers do not define a projection, or if they define a projection which does not define units.'
  );

  /*
   * Map Legend
   */
  $form[CDM_MAP_DISTRIBUTION]['legend'] = array(
    '#type' => 'fieldset',
    '#title' => 'Map legend',
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => 'Configure the maps legend.',
  );

  $form[CDM_MAP_DISTRIBUTION]['legend']['show'] = array(
    '#type' => 'checkbox',
    '#title' => 'Display a map legend',
    '#default_value' => $map_distribution['legend']['show'],
    '#description' => 'Check this if you like a legend to be displayed with the maps.',
  );

  $form[CDM_MAP_DISTRIBUTION]['legend']['opacity'] = array(
    '#type' => 'textfield',
    '#title' => 'Legend opacity',
    '#default_value' => $map_distribution['legend']['opacity'],
    '#description' => 'Valid values range from 0.0 to 1.0. Value 1.0 means the legend will be fully visible, while a value near
                         to 0.0 will be not much visible.',
  );

  $form[CDM_MAP_DISTRIBUTION]['legend']['font_size'] = array(
    '#type' => 'textfield',
    '#title' => 'Font size',
    '#default_value' => $map_distribution['legend']['font_size'],
    '#description' => 'Font size in pixels.',
  );

  $fontStyles = array(
    0 => "plane",
    1 => "italic",
  );
  $form[CDM_MAP_DISTRIBUTION]['legend']['font_style'] = array(
    '#type' => 'select',
    '#title' => 'Available font styles',
    '#default_value' => $map_distribution['legend']['font_style'],
    '#options' => $fontStyles,
    '#description' => 'Select a font style for the map legend.',
  );

  $form[CDM_MAP_DISTRIBUTION]['legend']['icon_width'] = array(
    '#type' => 'textfield',
    '#title' => 'Icon width',
    '#default_value' => $map_distribution['legend']['icon_width'],
    '#description' => 'Legend icon width in pixels.',
  );
  $form[CDM_MAP_DISTRIBUTION]['legend']['icon_height'] = array(
    '#type' => 'textfield',
    '#title' => 'Icon height',
    '#default_value' => $map_distribution['legend']['icon_height'],
    '#description' => 'Legend icon height in pixels.',
  );

  // @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );

  return system_settings_form($form);
}

/**
 * @todo document this function.
 */
function cdm_settings_cache() {

  $form = array();

  $form['cache_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('<p>When caching is enabled all single taxon sites are
      stored in an internal drupal cache doing the portal response of taxa pages
      faster. This is possible because the sites are loaded from the cache and
      are not created from scratch.</p>'),
  );

  $form['cache_settings']['cdm_webservice_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Enable caching</strong>'),
    '#options' => cdm_help_general_cache(),
    '#default_value' => variable_get('cdm_webservice_cache', 1),
    '#description' => t('<p>Enable drupal to load taxa pages from the cache.</p>
       <p><strong>Note:</strong> If taxa are modified by the editor or any other
       application the changes will be not visible till the cache is erased.
       Therefore developers should deactived this feature when they are working
       on the CDM Dataportal Module.</p>'),
  );

  $form['cache_settings']['cdm_run_cache'] = array(
    '#markup' => cdm_view_cache_site(),
  );

  // @WA: D7 form api does not support reset buttons,
  // so to mimic the D5 reset button we add one like this.
  $form['actions']['reset'] = array(
    '#markup' => '<input id="reset" type="reset" class="form-submit" value="' . t('Reset to defaults') . '" />',
    '#weight' => 1000,
  );
  return system_settings_form($form);
}

/**
 * Walk and cache all taxon pages.
 */
function cdm_view_cache_site() {

  $out = '';

  drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/cache_all_taxa.js');

  $request_params = array();
  $request_params['class'] = "Taxon";

  $cdm_ws_page_taxa_url = cdm_compose_url(CDM_WS_TAXON . ".json", NULL, queryString($request_params));
  $cdm_ws_page_taxa_url = uri_uriByProxy($cdm_ws_page_taxa_url);
  $cdm_ws_page_taxa_url = rtrim($cdm_ws_page_taxa_url, '/');

  $out .= t('<p><strong>Cache all taxon pages</strong></p>');
  $out .= '<p>When you launch the cache process the cache is filled and ready to be enabled.<br/>
  Remember that when you load the taxa from the cache last changes on taxa will be not visible till you erase
  the cache and fill it again.</p>';
  $out .= '<p>Before  running the cache bot you have to empty the cache manually.</p>';

  $out .= '<div>' . t('This caching process may take long time and could cause heavy load on your server') . '</div>';
  $out .= '<div id="progress"></div>';

  // Comment @WA: A form within a form is not valid html and not needed here.
  // Also, it would be recommended just to include this part of the form in the
  // rest of the form array in cdm_settings_cache().
  // $out .= '<form id="cache_site">';
  $out .= '<input type="hidden" name="pageTaxaUrl" value="' . $cdm_ws_page_taxa_url . '"/>';
  $out .= '<input type="hidden" name="taxonPageUrl" value="' . url('cdm_dataportal/taxon/') . '"/>';
  $out .= '<input type="button" name="start" value="' . t('Start') . '"/>';
  $out .= '<input type="button" name="stop" value="' . t('Stop') . '"/>';
  // $out .= '</form>';
  $out .= '</div>';
  /*
  foreach($taxonPager->records as $taxon){
    cdm_dataportal_taxon_view($uuid);
  }
  */
  return $out;
}


function cdm_settings_layout_taxon_submit($form, &$form_state){
  if (isset($form_state['values'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_fid'])) {
    $file = file_load($form_state['values'][CDM_TAXON_PROFILE_IMAGE]['custom_placeholder_image_fid']);
    if(is_object($file)){
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'cdm_dataportal', CDM_TAXON_PROFILE_IMAGE, 0);
    }
  }
}

/**
 * Form validation handler for ?
 *
 * Comment @WA: currently this handler is not used.
 * It seems partly for cdm_settings_general en partly for cdm_settings_cache?
 * Then you can change the name into cdm_settings_general_validate() or
 * cdm_settings_cache_validate().
 * But cdm_api_secref_cache_clear is not doing anything (yet)?
 */
function cdm_settings_validate($form, &$form_state) {

  if (!str_endsWith($form_state['values']['cdm_webservice_url'], '/')) {
    // form_set_error('cdm_webservice_url', t("The URL to the CDM Web Service must end with a slash: '/'."));
    $form_state['values']['cdm_webservice_url'] .= '/';
  }

  if ($form_state['values']['cdm_webservice_cache'] != variable_get('cdm_webservice_cache', 1)) {
    cache_clear_all(NULL, 'cache_cdm_ws');
    // Better clear secref_cache since I can not be sure if the cache has not
    // be used during this response.
    cdm_api_secref_cache_clear();
  }
}

/**
 * Returns an associative array of the currently chosen settings for the EDIT map service or the defaults as
 * specified in EDIT_MAPSERVER_URI_DEFAULT and EDIT_MAPSERVER_VERSION_DEFAULT:
 *  - base_uri: the protocol and host part , e.g.: http://edit.africamuseum.be
 *  - version: the version, e.g.: v1.1
 *
 * @return array
 *    An associative array of the currently chosen settings for the EDIT map service or the defaults.
 */
function get_edit_map_service_settings() {

  $settings = variable_get('edit_map_server', array(
      'base_uri' => EDIT_MAPSERVER_URI_DEFAULT,
      'version' => EDIT_MAPSERVER_VERSION_DEFAULT
      )
  );
  // replace old non tree like settings by default
  // TODO to be removed after release 3.1.5
  if(!is_array($settings)){
    variable_del('edit_map_server');
    return get_edit_map_service_settings();
  }

  return $settings;
}

/**
 * Returns the full edit map service URI e.g.: http://edit.africamuseum.be/edit_wp5/v1.1
 *
 * @return string
 *   The full edit map service URI e.g.: http://edit.africamuseum.be/edit_wp5/v1.1
 */
function get_edit_map_service_full_uri() {
  $settings = get_edit_map_service_settings();
  return $settings['base_uri'] . EDIT_MAPSERVER_PATH .  '/' . $settings['version'];
}


/**
 * Returns the version number of the currently selected edit mapserver as a float
 *
 * @return float
 *   The version number of the currently selected edit mapserver as a float.
 *   Returns 0 on error.
 */
function get_edit_map_service_version_number() {

  $pattern = '/v([\d\.]+).*$/';

  $settings = get_edit_map_service_settings();
  preg_match($pattern, $settings['version'], $matches, PREG_OFFSET_CAPTURE);
  if (isset($matches[1])) {
    // Convert string to float.
    $version = 1 + $matches[1][0] - 1;
    return $version;
  }
  else {
    // Report error.
    drupal_set_message(t(" Invalid EDIT map service version number: '!version'",
        array(
          '!version' => $settings['version'],
          'warning')
        )
      );
    return 0;
  }
}

/**
 * Returns the array of selected items in a options array
 *
 * @param array $options
 *   An options array as generated by a form element like checkoxes, select ...,
 */
function get_selection($options) {
  $selection = array();
  foreach ($options as $key=>$val) {
    if (!empty($val)) {
      $selection[] = $val;
    }
  }
  return $selection;
}


/**
 * Implements hook_element_info().
 *
 * Allows modules to declare their own Form API element types and specify their default values.
 *
 * @see http://api.drupal.org/api/drupal/modules!system!system.api.php/function/hook_element_info/7
 */
function cdm_dataportal_element_info() {
  $type['checkboxes_preferred'] = array(
    '#input' => TRUE,
    '#process' => array('checkboxes_preferred_expand'),
    '#after_build' => array('checkboxes_preferred_after_build'),
    '#theme' => array('checkboxes_preferred'),
    // '#theme_wrapper' => array('form_element'),
  );
  return $type;
}

/**
 * #process callback function for the custom form element type 'checkbox_preferred'
 *
 *
 */
function checkboxes_preferred_expand($element, &$form_state, $form) {

  // First of all create checkboxes for each of the elements
  $element = form_process_checkboxes($element);

  // compose the element name
  $parents = array();
  array_deep_copy($element['#parents'], $parents);
  $parents[count($parents) -1 ] .= '_preferred';
  $element_name = $parents[0];
  for ($i=1; $i < count($parents); $i++){
    $element_name .= '[' . $parents[$i] . ']';
  }

  $children = element_children($element);

  $element['table_start'] = array(
    '#markup' => '<table class="checkboxes_preferred"><tr><th></th><th>' . t('Enabled') . '</th><th>' . t('Default') . '</th></tr>',
    '#weight' => -1,
  );

  // prepare first part each of the table rows which contains the row label
  $weight = 0;
  foreach ($children as $key) {
    $odd_even = $weight % 4 == 0 ? 'odd' : 'even';
    $element[$key]['#weight'] = $weight;
    $element[$key]['#prefix'] = '<tr class="' . $odd_even . '"><td>' . t($element['#options'][$key]) . '</td><td>';
    $element[$key]['#suffix'] = '</td>';
    unset($element[$key]['#title']);
    $weight += 2;
  }
  $weight = 0;

  // add a radio button to each of the checkboxes, the
  // check boxes have already been created at the beginning
  // of this function
  if (count($element['#options']) > 0) {
    foreach ($element['#options'] as $key => $choice) {
      if (!isset($element[$key . '_preferred'])) {
        $element[$key . '_preferred'] = array(
          '#type' => 'radio',
          '#name' => $element_name,
          '#return_value' => check_plain($key),
          '#default_value' => empty($element['#default_value_2']) ? NULL : $element['#default_value_2'],
          '#attributes' => $element['#attributes'],
          '#parents' => $element['#parents'],
          // '#spawned' => TRUE,
          '#weight' => $weight + 1,
          '#prefix' => '<td>',        // add a prefix to start a new table cell
          '#suffix' => '</td></tr>',  // add a prefix to close the tabel row
        );
      }
      $weight += 2;
    }
  }

  // end the table
  $element['table_end'] = array(
    '#markup' => '</table>',
    '#weight' => $weight++,
  );

  return $element;
}

/**
 * Theme function for the custom form field 'checkboxes_preferred'.
 */
function theme_checkboxes_preferred($variables) {
  $element = $variables['element'];
  $out = '<div id="edit-baselayers-wrapper" class="form-item">';
  $out .= '<label for="edit-baselayers">' . $element['#title'] . '</label>';
  $out .= drupal_render_children($element);
  $out .= '<div class="description">' . $element['#description'] . '</div>';
  $out .= '</div>';
  return $out;
}

/**
 * Callback for checkboxes preferred for widget which will
 * be called after the form or element is built. The call
 * back is configured in the form element by setting it as
 * #after_build parameter.
 *
 * @see http://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#after_build
 *
 * @param $element
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *   This includes the current persistent storage data for the form.
 *   Additional information, like the sanitized $_POST data,
 *   is also accumulated here in $form_state['input']
 *
 * @return the modified form array
 */
function checkboxes_preferred_after_build($element, &$form_state) {

  $parent_id = $element['#parents'][count($element['#parents']) - 1];

  if ($_POST && count($_POST) > 0) {
    // TODO use  $form_state['input'] instead of POST !!!
    // First pass of form processing.
    $parents = array();
    array_deep_copy($element['#parents'], $parents);
    $parents[count($parents) - 1] .= '_preferred';
    $preferred_layer = drupal_array_get_nested_value($_POST, $parents);
    $element['#value']['PREFERRED'] = $preferred_layer;
//     $form_state[$parent_id] = $element['#value'];
//     $form_state['values']['baselayers'] = $element['#value'];
    $form_state_element_values = &drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    $form_state_element_values = $element['#value'];
  }
  else {
    // Second pass of form processing.
    $preferred_layer = $element['#value']['PREFERRED'];
  }

  // Also set the chosen value (not sure if this is good Drupal style ....).
  foreach ($children = element_children($element) as $key) {
    if (!empty($element[$key]['#type']) && $element[$key]['#type'] == 'radio') {
      $element[$key]['#value'] = $preferred_layer;
    }
  }
  // The default layer must always be enabled.
  $element[$preferred_layer]['#value'] = $preferred_layer;

  return $element;
}

function radios_prepare_options_suffix(&$elements){

  $childrenKeys = element_children($elements);
  foreach($childrenKeys as $key){
    if(!is_array($elements[$key]['#theme_wrappers'])){
      $elements[$key]['#theme_wrappers'] = array();
    }
    if(isset($elements['#options_suffixes'][$key])){
      $elements[$key]['#theme_wrappers'][] = 'radio_options_suffix';
      $elements[$key]['#options_suffix'] = $elements['#options_suffixes'][$key];
    }
  }
  return $elements;

}

/**
 * TODO
 * @param unknown $variables
 */
function theme_radio_options_suffix($variables) {
  $element = $variables['element'];
  if(isset($element['#options_suffix'])) {
    $element['#children'] .= $element['#options_suffix'];
  }
  return $element['#children'];
}


/**
 * Element validate callback for text field and arrays containing json.
 *
 * @param $element
 *   The form element to validate
 * @param $form_state
 *   A keyed array containing the current state of the form.
 * @param $form
 *   Nested array of form elements that comprise the form.
 */
function form_element_validate_json($element, &$form_state, $form) {
   if (!empty($element['#value'])) {
     json_decode($element['#value']);
     if(json_last_error() != JSON_ERROR_NONE){
       form_error($element, t('The form element %title contains invalid JSON. You can check the syntax with ', array('%title' => $element['#title'])) . l('JSONLint', 'http://jsonlint.com/'));
     }
   }
}

/**
 * Form submission handler for textareas and textfields containing JSON.
 *
 * The contained JSON will be converted into an php array
 * or object and will be stored in the variables as such.
 *
 * @see http://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#submit
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 */
function submit_json_as_php_array($form, &$form_state) {
  if (is_array($form['#json_elements'])) {
    foreach ($form['#json_elements'] as $element){
      if (trim($form_state['values'][$element])) {
        $form_state['values'][$element] = (array) json_decode($form_state['values'][$element]);
      } else {
        $form_state['values'][$element] = NULL;
      }
    }
  }
}
