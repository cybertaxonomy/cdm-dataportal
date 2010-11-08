<?php

define('DEFAULT_TAXONTREE_RANKLIMIT', '1b11c34c-48a8-4efa-98d5-84f7f66ef43a');//TODO Genus UUID
define('CDM_TAXONOMICTREE_UUID', 'cdm_taxonomictree_uuid');

define('CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE', 25);
define('CDM_DATAPORTAL_NOMREF_IN_TITLE', 1);
define('CDM_DATAPORTAL_DISPLAY_IS_ACCEPTED_FOR', 0);
define('CDM_DATAPORTAL_ALL_FOOTNOTES', 0);
define('CDM_DATAPORTAL_ANNOTATIONS_FOOTNOTES', 0);


/* gallery variables */
$gallery_settings = array(
    "cdm_dataportal_show_taxon_thumbnails" => 1,
    "cdm_dataportal_show_synonym_thumbnails" => 0,
    "cdm_dataportal_show_thumbnail_captions" => 1,
    "cdm_dataportal_media_maxextend" => 120,
    "cdm_dataportal_media_cols" => 3,
    "cdm_dataportal_media_maxRows" => 1);
/**
 * default settings for all gallerys
 * @var unknown_type
 */
define('CDM_DATAPORTAL_GALLERY_SETTINGS', serialize($gallery_settings));
define('CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME', 'specimen_gallery');
define('CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME', "description_gallery");
define('CDM_DATAPORTAL_MEDIA_GALLERY_NAME', "media_gallery");
define('CDM_DATAPORTAL_SEARCH_GALLERY_NAME', "search_gallery");
define('CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS', 'cdm_dataportal_display_taxon_relationships');
define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS', 'cdm_dataportal_display_name_relations');
//define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_2', array("default" => t('Display all')));
define('CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT', 1);
define('CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_DEFAULT', 1);
define('CDM_DATAPORTAL_DEFAULT_FEATURETREE_UUID', 'cdm_dataportal_featuretree_uuid');
define('CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID', 'cdm_dataportal_structdesc_featuretree_uuid');

function getGallerySettings($gallery_config_form_name){
  $default_values = unserialize(CDM_DATAPORTAL_GALLERY_SETTINGS);
  return variable_get($gallery_config_form_name, $default_values);
}

/**
 * Generate main administration form.
 *
 * @return
 *   An array containing form items to place on the module settings page.
 */
function cdm_dataportal_settings(){

  return cdm_dataportal_settings_general();
}

/**
 * Configures the settings form for the CDM-API module.
 *
 * @return Array Drupal settings form
 */
function cdm_dataportal_settings_general(){

  $form['cdm_webservice'] = array(
      '#type' => 'fieldset',
      '#title' => t('CDM Web Service'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
  );

  $form['cdm_webservice']['cdm_webservice_url'] =  array(
    '#type' => 'textfield',
    '#title'         => t('CDM Web Service URL'),
    '#description'   => t('The URL of CDM Webservice which delivers the data to be published.'),
    '#default_value' => variable_get('cdm_webservice_url', NULL),
  );

  $form['cdm_webservice']['taxontree_ranklimit'] =  array(
    '#type'          => 'select',
    '#title'         => t('Rank of highest displayed taxon'),
    '#default_value' => variable_get('taxontree_ranklimit', DEFAULT_TAXONTREE_RANKLIMIT), //before DEFAULT_TAXONTREE_RANKLIMIT_UUID
    '#options'       => cdm_rankVocabulary_as_option(),
    '#description'   => t('The rank of the highest displayed taxon in the taxontree.'),
  );

  $form['cdm_webservice']['cdm_webservice_cache'] =  array(
    '#type' => 'checkbox',
    '#title'         => t('Enable Caching'),
    '#default_value' => variable_get('cdm_webservice_cache', 1),
    '#description'   => t('Enable caching of webservice responses on simple requests, '
    .'that is requests which only have one parameter generally a UUID or a concatenation of UUIDs')
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
    '#title'         => t('Proxy Port'),
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

    $form['cdm_webservice']['cdm_webservice_debug'] =  array(
    '#type' => 'checkbox',
    '#title'         => t('Debug CDM Web Service'),
    '#default_value' => variable_get('cdm_webservice_debug', 1),
    '#description'   => t('Enable CDM Web Service debugging messages. Only visible for the super administrator or for users having the permission <em>administer cdm_api</em>!')
    );

    //TODO: settings are still incomplete, compare with trunk/dataportal/inc/config_default.php.inc
    $form['cdm_dataportal'] = array(
      '#type' => 'fieldset',
      '#title' => t('CDM DataPortal'),
      '#collapsible' => FALSE,
      '#collapsed' => TRUE,
    );

    $form['cdm_dataportal'][CDM_TAXONOMICTREE_UUID] = array(
      '#type' => 'select',
      '#title'         => t('Available classifications'),
      '#default_value' => variable_get(CDM_TAXONOMICTREE_UUID, false),
      '#options' => cdm_get_taxontrees_as_options(),
      '#description'   => t('Select the default classification to be used.')
    );

    return system_settings_form($form);
}



/**
 * LAYOUT settings
 * @return unknown_type
 */
function cdm_dataportal_settings_layout(){

  $form = array();
  /*
   $form['cdm_taxonname_type'] = array(
   '#type' => 'select',
   '#title'         => t('Taxon name type'),
   '#default_value' => variable_get('cdm_taxonname_type', 'BotanicalName'),
   '#options' => array( 'BotanicalName'=>t('BotanicalName'), 'ZoologicalName'=>t('ZoologicalName')),
   '#description'   => t('')
   );
   */

  $form['cdm_dataportal_taxonpage_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabbed taxon page'),
    '#default_value' => variable_get('cdm_dataportal_taxonpage_tabs', 1),
    '#description' => t('Split the taxon page into individual tabs for description, images, synonymy')
  );

  //---- footnotes ---//
  $form['footnotes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Footnotes'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );

  $form['footnotes']['cdm_dataportal_all_footnotes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Do not show footnotes'),
      '#default_value' => variable_get('cdm_dataportal_all_footnotes', CDM_DATAPORTAL_ALL_FOOTNOTES),
      '#description' => t('Check this if you do not want to show any footnotes')
  );

  $form['footnotes']['cdm_dataportal_annotations_footnotes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Do not show annotations footnotes'),
      '#default_value' => variable_get('cdm_dataportal_annotations_footnotes', CDM_DATAPORTAL_ANNOTATIONS_FOOTNOTES),
      '#description' => t('Check this if you do not want to show annotation footnotes')
  );


  //---- SYNONYMY ----//
  $form['synonymy'] = array(
      '#type' => 'fieldset',
      '#title' => t('Synonymy'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
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

  $name_relationships_terms = cdm_ws_get(CDM_WS_TERMVOCABULARY, UUID_NAME_RELATIONSHIP_TYPE);
  $name_rel_options = array();
  //$name_rel_options['default'] = 'Show all';
  foreach ($name_relationships_terms->terms as $element){
    $name_rel_options[$element->uuid] = t('Show "' . $element->representation_L10n_abbreviated . '" relationships');
  }

  $name_relationships_form['name_relationships_to_show']= array(
  '#type' => 'checkboxes',
  '#title' => t('Display name relationships'),
  '#default_value' => variable_get('name_relationships_to_show', 0),
  '#options' => $name_rel_options,
  '#description' => t('Select the name relationships you want to show for the accepted taxa.'),
  );

  $form['synonymy']['name_relationships'] = $name_relationships_form;
/*
  $form['synonymy'][CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS] = array(
    '#type' => 'checkbox',
    '#title' => t('Show name relations of accepted taxa on taxon page'),
    '#default_value' => variable_get(CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_NAME_RELATIONSHIPS_DEFAULT),
    //'#description' => t('Check this if you want the synonymy list to show all the name relationships where other names implies the accepted taxa.')
    '#description' => t('Check this if you want the synonymy list to show all the name relationships of accepted taxa.')
  );
*/
  $form['synonymy'][CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS] = array(
    '#type' => 'checkbox',
    '#title' => t('Show taxon relations of accepted taxa on taxon page'),
    '#default_value' => variable_get(CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS, CDM_DATAPORTAL_DISPLAY_TAXON_RELATIONSHIPS_DEFAULT),
    '#description' => t('Check this if you want the synonymy list to show the <em>"Misapplied Name for"</em> and <em>"Invalid Designation for"</em> relationships of accepted taxa.')
  );

/*
  $form['synonymy']['cdm_dataportal_name_relations_skiptype_basionym'] = array(
    '#type' => 'checkbox',
    '#title' => t('Exclude the basionym relationship type from the taxon page'),
    '#default_value' => variable_get('cdm_dataportal_name_relations_skiptype_basionym', 1),
    '#description' => t('')
  );
*/

  /*
   $form['cdm_dataportal_descriptions_separated'] = array(
   '#type' => 'checkbox',
   '#title' => t('Separate Descriptions'),
   '#default_value' => variable_get('cdm_dataportal_descriptions_separated', 0),
   '#description' => t('By default corresponding elements of different descriptions are joined together'
   .' into a common section per feature (i.e. type of description).'
   .' Check this box to allow displaying all descriptions separately.')
   );
   */
  //------------------ FEATURE TREE --------------------//

  $form['cdm_dataportal']['taxon_profile'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxon profile'),
      '#description'   => t('This section covers setting related to the taxon profile tab, also known as the <strong>"General"</strong> tab.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );

  $form['cdm_dataportal']['taxon_profile'][CDM_DATAPORTAL_DEFAULT_FEATURETREE_UUID] = array(
      '#type' => 'radios',
      '#title'         => t('Taxon profile elements'),
      '#default_value' => variable_get(CDM_DATAPORTAL_DEFAULT_FEATURETREE_UUID, UUID_DEFAULT_FEATURETREE),
      '#options' => cdm_get_featureTrees_as_options(TRUE),
      '#description'   => t('Select a FeatureTree to specify the elements to be displayd in the taxon profile.'
      //.' If there is no applicable FeatureTree you can create a new one using the <a href="">FeatureTreeManager</a>'
  )
  );

  $form['cdm_dataportal']['taxon_profile'][CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID] = array(
      '#type' => 'radios',
      '#title'         => t('Natural language representation of structured descriptions'),
      '#default_value' => variable_get(CDM_DATAPORTAL_STRUCTURED_DESCRIPTION_FEATURETREE_UUID, null),
      '#options' => cdm_get_featureTrees_as_options(),
      '#description'   => t('Taxon descriptions can be stored in a highly structured form.'.
        ' The feature tree selected here will be used to generate textual representation in natural language.'
        //.' If there is no applicable FeatureTree you can create a new one using the <a href="">FeatureTreeManager</a>'
  )
  );

  //---- IMAGES ----//
  $form['images'] = array(
      '#type' => 'fieldset',
      '#title' => t('Images'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );
  $options = cdm_rankVocabulary_as_option();
  array_unshift($options, '-- DISABLED --');
  $form['images']['image_hide_rank'] =  array(
      '#type'          => 'select',
      '#title'         => t('Hide Images for Taxa above'),
      '#default_value' => variable_get('image_hide_rank', '0'),
      '#options'       => $options,
      '#description'   => t(''),
  );
  //show media
  $selectShowMedia = array(0 => "Show only taxon media",
  1 => "Show taxon and child taxon media");
  $form['images']['cdm_dataportal_show_media'] = array(
      '#type' => 'select',
      '#title' => t('Available media files'),
      '#default_value' => variable_get('cdm_dataportal_show_media', false),
      '#options' => $selectShowMedia,
      '#description'   => t('Select if a taxon should show only his media or also child media.')
  );
  $selectShowMedia = array(0 => "Show only taxon media",
  1 => "Show taxon and child taxon media");
  $form['images']['cdm_dataportal_show_default_image'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show default image'),
      '#default_value' => variable_get('cdm_dataportal_show_default_image', false),
      '#description'   => t('Select if the taxon profile page should display the default image if no image is available for the chosen taxon.')
  );

  //------------------ SEARCH --------------------//
  $form['search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );
  $form['search']['cdm_dataportal_search_items_on_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Page Size'),
    '#default_value' => variable_get('cdm_dataportal_search_items_on_page', CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE),
    '#description' => t('Number of Names to display per page in search results.')
  );

  // --- SEARCH TAXA GALLERY ---- //
  $items = variable_get('cdm_dataportal_search_items_on_page', CDM_DATAPORTAL_SEARCH_ITEMS_ON_PAGE);
  $collapsed = TRUE;
  $form_name = CDM_DATAPORTAL_SEARCH_GALLERY_NAME;
  $form_tittle = 'Search Taxa';
  $form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_tittle, $collapsed);

  // --- FEATURE DESCRIPTION GALLERY ---- //
  $form_name = CDM_DATAPORTAL_DESCRIPTION_GALLERY_NAME;
  $form_tittle = 'Description elements gallery';
  $form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_tittle, $collapsed);

  // --- CDM_DATAPORTAL_SPECIMEN_GALLERY --- //
  $form_name = CDM_DATAPORTAL_SPECIMEN_GALLERY_NAME;
  $form_tittle = 'Speciment media gallery';
  $form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_tittle, $collapsed);


  // --- MEDIA GALLERY ---- //
  $form_name = CDM_DATAPORTAL_MEDIA_GALLERY_NAME;
  $form_tittle = 'Media gallery';
  $form[] = cdm_dataportal_create_gallery_settings_form($form_name, $form_tittle, $collapsed);

  $form['image_gallery_viewer'] =  array(
    '#type'          => 'select',
    '#title'         => t('Image Gallery Viewer'),
    '#default_value' => variable_get('image_gallery_viewer', 'default'),
    '#options' => array(
        'default' => t('Standart image gallery'),
        'fsi' => t('FSI viewer (requires FSI server!)'),
  ));
  // variable_get("imageviewer", "default")

  //-- DISTRIBUTION LAYOUT --//
  $form['distribution_layout'] = array(
        '#title' => t('Distribution layout'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#type' => 'fieldset',
  );

  $form['distribution_layout']['distribution_sort'] =  array(
    '#type'          => 'radios',
    '#title'         => t('Sort'),
    '#default_value' => variable_get('distribution_sort', 'NO_SORT'),
    '#options' => array(
        'NO_SORT' => t('Standard (No sort)'),
        'HIDE_TDWG2' => t('Sorted without TDWG Level 2'),
  ));

  return system_settings_form($form);
}


/**
 * GEOSERVICE and Map settings
 * @return unknown_type
 */
function cdm_dataportal_settings_geo(){

  $form = array();

  $form['edit_map_server'] = array(
    '#type' => 'select',
    '#title' => t('Geoservice Access Point URL'),
    '#default_value' => variable_get('edit_map_server', 'http://edit.br.fgov.be/edit_wp5/v1/'),
    '#options' => array(
	      'http://edit.br.fgov.be/edit_wp5/v1/' => 'EDIT Map Server',
      /*
        'http://edit.br.fgov.be/edit_wp5/v1/' => 'EDIT Map Server - Mirror 1',
        'http://edit.br.fgov.be/edit_wp5/v1/' => 'EDIT Map Server - Mirror 2',
       */
	      'ALTERNATIVE' => '-- Alternative URL --'
	    ),
    '#description' => t('Base URL of the geoservice to be used by this portal')
  );

  $form['edit_map_server_alternative'] = array(
    '#type' => 'textfield',
    '#title' => t('Geoservice Access Point - Alternative URL'),
    '#default_value' => variable_get('edit_map_server_alternative', ''),
    '#description' => t('Alternative URL of a EDIT Map Service to be used by this portal. You must choose the option <i>-- Alternative URL --</i> in the chooser abofe to enable this url.')
  );

  $form['cdm_dataportal_geoservice_display_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Geoservice Display Width'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_display_width', 390),
    '#description' => t('Width of the image generated by geoservice')
  );

  $form['cdm_dataportal_geoservice_bounding_box'] = array(
    '#type' => 'textfield',
    '#title' => t('Fixed Geoservice Bounding Box'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_bounding_box', '-180,-90,180,90'),
    '#description' => t('Define urrounding of area to be displayed in maps. Use "-180,-90,180,90" for the whole world. Leave <strong>empty</strong> to let the map <strong>automatically zoom</strong> to the distribution area.')
  );

  $form['cdm_dataportal_geoservice_labels_on'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Country Labels'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_labels_on', FALSE),
    '#description' => t('Check this if you like country names to be displayed in the maps. ')
  );

  $form['cdm_dataportal_geoservice_map_caption'] = array(
    '#type' => 'textfield',
    '#title' => t('Map Caption'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_map_caption', ''),
    '#description' => t('Define a caption for the map.')
  );

  $form['cdm_dataportal_map_openlayers'] = array(
    '#type' => 'checkbox',
    '#title' => t('OpenLayers Viewer'),
    '#default_value' => variable_get('cdm_dataportal_map_openlayers', 1),
    '#description' => t('Display the maps in an interactive viewer which allows zooming and panning.')
  );


  // --- OpenLayers Settings --- //

  $form['openlayers'] = array(
      '#type' => 'fieldset',
      '#title' => t('OpenLayers Settings'),
      '#collapsible' => FALSE,
      '#collapsed' => !variable_get('cdm_dataportal_map_openlayers', 1)
  );

  $baselayer_options = array(
    /*
     * NOTICE: must correspond to the layers defined in js/openlayers_,ap.js#getLayersByName()
     */
    'osgeo_vmap0' => "OpenLayers World", // EPSG:4326
    'metacarta_vmap0' => "Metacarta Vmap0" , // EPSG:4326, EPSG:900913
    // all others EPSG:900913 ...
    'osmarender' => 'OpenStreetMap',
    'oam' => 'OpenAerialMap',
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
    '#description' => t('')
  );

  // cdm_dataportal_geoservice_showLayerSwitcher
  $form['openlayers']['cdm_dataportal_geoservice_showLayerSwitcher'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show LayerSwitcher'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_showLayerSwitcher', TRUE),
    '#description' => t('The LayerSwitcher control displays a table of contents for the map.  This allows the user interface to switch between BaseLasyers and to show or hide Overlays.  By default the switcher is shown minimized on the right edge of the map, the user may expand it by clicking on the handle.')
  );

  $localhostkey = 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ';
  $gmap_api_key = variable_get('gmap_api_key', 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ');
  $form['openlayers']['gmap_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Gogle Maps API Key'),
    '#default_value' => variable_get('gmap_api_key', $gmap_api_key),
    '#description' => t('Gogle Maps API Key, the key set by default <code>'.$localhostkey.'</code> is a key for the localhost = 127.0.0.1 The key in use is the one above this text.')
  );

  $form['openlayers']['cdm_dataportal_geoservice_distributionOpacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Distribution Layer Opacity'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_distributionOpacity', '0.5'),
    '#description' => t('Valid values range from 0.0 to 1.0. You can choose to let the underlying layers shine through if you select a value < 1.0. A value of 1.0 will cause a full opacity of the ditribution layer.')
  );

  $form['openlayers']['cdm_dataportal_geoservice_legendOpacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Legend Opacity'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legendOpacity', '0.5'),
    '#description' => t('Valid values range from 0.0 to 1.0. You can choose to let the layers shine through the legend if you select a value < 1.0. A value of 1.0 will cause a full opacity of the legend.')
  );

  $form['cdm_dataportal_geoservice_map_legend'] = array(
      '#type' => 'fieldset',
      '#title' => t('Map Legend'),
      '#collapsible' => FALSE,
      '#collapsed' => TRUE,
  );

  $form['cdm_dataportal_geoservice_map_legend']['cdm_dataportal_geoservice_legend_on'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a map legend'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legend_on', TRUE),
    '#description' => t('Check this if you like a legend to be displayed with the maps. ')
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
    '#title' => t('Icon width'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legend_icon_width', 35),
    '#description' => t('Icon width in pixels.')
  );

  $form['cdm_dataportal_geoservice_map_legend']['cdm_dataportal_geoservice_legend_icon_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon height'),
    '#default_value' => variable_get('cdm_dataportal_geoservice_legend_icon_height', 15),
    '#description' => t('Icon height in pixels.')
  );

  return system_settings_form($form);
}

/**
 * @return walk and cache all taxon pages
 */
function cdm_dataportal_view_cache_site(){

  _add_js_progressbar();

  drupal_add_js(drupal_get_path('module', 'cdm_dataportal').'/js/cache_all_taxa.js');

  $out = '';

  $request_params = array();
  $request_params['query'] = '%';
  $request_params['tree'] = variable_get('cdm_taxonomictree_uuid', false); //cache only the dafault classification
  $request_params['doTaxa'] = 1;
  $request_params['doSynonyms'] = 0;
  $request_params['doTaxaByCommonNames'] = 0;
  $search_url = cdm_compose_url(CDM_WS_PORTAL_TAXON_FIND, null, queryString($request_params));
  $search_url = uri_uriByProxy($search_url);
  $taxon_page_url = url('cdm_dataportal/taxon/');

  $out .= '<form id="cache_site">';
  $out .= '<br /><h4>'.t('Cache all taxon pages').'</h4>';
  $out .= 'Before  running the cache bot you have to empty the cache manually:<ul>'
  .'<li>Navigate to "Site Configuration -> General"</li>'
  .'<li>Uncheck "Enable Caching" checkbox</li>'
  .'<li>Check "Enable Caching" checkbox</li>'
  .'</ul>';
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
function cdm_dataportal_settings_validate($form_id, $form_values){

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
  } else {
  	return variable_get('edit_map_server', false);
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

