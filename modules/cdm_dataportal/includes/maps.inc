<?php
/**
 * @file
 * Functions for dealing maps
 *
 * @copyright
 *   (C) 2007-2013 EDIT
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
 * Compose an render array for distribution and occurrence
 * maps.
 *
 * The map can either be a plain image or a dynamic open layers map
 * depending on the settings
 *
 * compose_hook() implementation
 *
 * @param $map_id
 * @param string $occurrence_query
 * @param string $distribution_query
 * @param string $legend_format_query
 * @param array $event_listeners
 *   An associative array of with OpenLayers.Map event names as key and corresponding js callbacks.
 *   In addition to the event names '#execute' as key is also allowed.
 *   Valid events are:
 *      - move
 *      - moveend
 *      - zoomend
 *      - changelayer
 *      - changebaselayer
 *      - #execute:
 *            force execution of the given callback after registration of the event handlers
 *   see http://dev.openlayers.org/apidocs/files/OpenLayers/Map-js.html#OpenLayers.Map.events for more
 * @param bool $resizable
 *    only possible for openlayers_map
 * @param string $force_map_type
 *   Can be used to override the map_type setting stored in the settings variable CDM_MAP_DISTRIBUTION
 *   - 1: openlayers_map
 *   - 0: image_map
 * @return array A drupal render array
 * A drupal render array
 * @ingroup compose
 */
function compose_map($map_id, $occurrence_query = NULL, $distribution_query = NULL, $legend_format_query = NULL, array $event_listeners = array(), $resizable = false, $force_map_type = NULL) {

    $map_settings = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);

    if($force_map_type === NULL){
      $force_map_type = $map_settings['map_type'];
    }

    if ($force_map_type == 1) {
      _add_jquery_ui();
      $map_html = cdm_map_openlayers(
        $map_id,
        $occurrence_query,
        $distribution_query,
        $legend_format_query,
        $map_settings['caption'],
        $event_listeners,
        $resizable
      );
    }
     else {
      $map_height = round($map_settings['image_map']['width'] / (float)$map_settings['aspect_ratio']);
      $map_html = cdm_map_plain_image(
        $map_settings['image_map']['width'],
        $map_height,
        $occurrence_query,
        $distribution_query,
        $legend_format_query,
        $map_settings['caption']
      );
     }
  return markup_to_render_array($map_html);
}

/**
 * Adds the javascript for a openlayers map to the page as well as all javascript libs.
 *
 *
 * @param $map_id
 * @param $map_settings
 *   The map settings array as retrieved by e.g. get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);
 * @param array $event_listeners
 *   An associative array of with OpenLayers.Map event names as key and corresponding js callbacks.
 *   In addition to the event names '#execute' as key is also allowed.
 *   Valid events are:
 *      - move
 *      - moveend
 *      - zoomend
 *      - changelayer
 *      - changebaselayer
 *      - #execute:
 *            force execution of the given callback after registration of the event handlers
 *   see http://dev.openlayers.org/apidocs/files/OpenLayers/Map-js.html#OpenLayers.Map.events for more
 * @param bool $resizable
 *   The map is made resizable when set to true
 */
function _add_js_openlayers_map($map_id, $map_settings, array $event_listeners = array(), $resizable = false) {

  font_awesome_icon_markup(); // no icon specified, only used to add the font and styles

  _add_js_openlayers();

  $edit_map_service = get_edit_map_service_settings();

  drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/map/openlayers_map.js',
    array(
      'type' => 'file',
      'group' => JS_DEFAULT, //  module-layer JavaScript.
      'weight' => 0,
      'cache' => TRUE,
      'preprocess' => FALSE
  ));
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/map/openlayers_layers.js',
    array(
      'type' => 'file',
      'group' => JS_DEFAULT,
       'weight' => 1, // after openlayers_map.js
      'cache' => TRUE,
      'preprocess' => FALSE
  ));

  $cdm_openlayers_options = array(
      'legendPosition'  => '3',
      'boundingBox' => $map_settings['bbox'],
      'aspectRatio' => $map_settings['aspect_ratio'],
      'distributionOpacity' => $map_settings['distribution_opacity'],
      'legendOpacity' => $map_settings['legend']['opacity'],
      'showLayerSwitcher' => $map_settings['openlayers']['show_layer_switcher']  ==  1,
      'displayOutsideMaxExtent' => $map_settings['openlayers']['display_outside_max_extent'] == 1,
      'resizable' => $resizable
//       'imgPath' => drupal_get_path('module', 'cdm_dataportal') . '/js/map/OpenLayers-2.13.1/img/' // path to the control icons
      // if no baseLayerNames or defaultBaseLayerName are not defined
      // the defaults in cdm_openlayers.js will be used
  );

  // --- setting the base layer options
  if (is_array($map_settings['openlayers']['base_layers']) && count($map_settings['openlayers']['base_layers']) > 0) {

    $layer_names = $map_settings['openlayers']['base_layers'];

    foreach($layer_names as $name){
      if(str_beginsWith($name, 'g')){
        if( isset($map_settings['openlayers']['google_maps_api_key']) && strlen($map_settings['openlayers']['google_maps_api_key']) == 39) {
          // google layer detected
          drupal_add_js("https://maps.googleapis.com/maps/api/js?key=" . $map_settings['openlayers']['google_maps_api_key'] . "&callback=initMap", 'external');
        } else {
          drupal_set_message('A Google Maps layer is configured but the API key is either missing or invalid. 
          Please set your Google Maps API key in the '  . l('Geo & Map Settings', 'admin/config/cdm_dataportal/settings/geo') .'.', 'warning');
        }
      }
    }

    $cdm_openlayers_options['baseLayerNames'] = array_values($layer_names);

    if($layer_names['PREFERRED']){
      $cdm_openlayers_options['defaultBaseLayerName'] = $layer_names['PREFERRED'];
      unset($layer_names['PREFERRED']); // why is this needed?
      if(!array_search($cdm_openlayers_options['defaultBaseLayerName'], $cdm_openlayers_options['baseLayerNames'])){
        // the default layer must also  be in the list of base layers
        $cdm_openlayers_options['baseLayerNames'][] = $cdm_openlayers_options['defaultBaseLayerName'];
      }
    }

  }

  // --- custom wms base layer
  $map_settings['openlayers']['custom_wms_base_layer']['params'] = json_decode($map_settings['openlayers']['custom_wms_base_layer']['params']);
  $cdm_openlayers_options['customWMSBaseLayerData'] = $map_settings['openlayers']['custom_wms_base_layer'];

  // --- eventhandlers
  $event_listeners_js = '';
  $execute_handler = '';
  foreach($event_listeners as $event=>$js_callback){
    if($event == '#execute'){
      $execute_handler = 'map_container.each(function(){' . $js_callback . '();});';
    } else {
      $event_listeners_js .= ($event_listeners_js ? ",\n": "\n") .'"' . $event . '": ' . $js_callback;
    }
  }

  $mapserver_base_uri = $edit_map_service['base_uri'];
  $is_https = isset($_SERVER['HTTPS']) && strtolower($_SERVER['HTTPS']) == 'on';
  $http_protocol = $is_https ? 'https' : 'http';
  $mapserver_base_uri = preg_replace('/^https?:/', $http_protocol . ':', $mapserver_base_uri);

  // window.onload - is executed when the document and images etc is fully loaded
  // Query(document).ready - is executed much earlier, when the DOM is loaded
  drupal_add_js("
          jQuery(document).ready(function() {
                jQuery(window).load(function () {
                  var map_container = jQuery('#openlayers-map-" . $map_id . "').cdm_openlayers_map(
                   '" . $mapserver_base_uri . "',
                   '" . $edit_map_service['version'] . "',
                   " .  json_encode($cdm_openlayers_options) . "
                );
                map_container.each(function(){
                        this.cdmOpenlayersMap.registerEvents({" . "
                        " . $event_listeners_js . "
                        });
                });
                " . $execute_handler . "
        });
      });
    ", array('type' => 'inline'));

}


/**
 * Creates markup for an openlayers based dynamic map.
 *
 * @param string $bounding_box
 * @param string $occurrenceQuery
 * @param string $distributionQuery
 * @param string $legendFormatQuery
 * @param string $map_caption
 * @param array $event_listeners
 *   An associative array of with OpenLayers.Map event names as key and corresponding js callbacks.
 *   In addition to the event names '#execute' as key is also allowed.
 *   Valid events are:
 *      - move
 *      - moveend
 *      - zoomend
 *      - changelayer
 *      - changebaselayer
 *      - #execute:
 *            force execution of the given callback after registration of the event handlers
 *   see http://dev.openlayers.org/apidocs/files/OpenLayers/Map-js.html#OpenLayers.Map.events for more
 *
 * @return String
 *    The markup for the map
 */
function cdm_map_openlayers($map_id, $occurrenceQuery = FALSE, $distributionQuery = FALSE,
                            $legendFormatQuery = FALSE, $map_caption = FALSE, array $event_listeners = array(),
                            $resizable = false) {

  $map_settings = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);

  if($map_id == NULL){
    $map_id = sha1($occurrenceQuery . $distributionQuery);
  }

  _add_js_openlayers_map($map_id, $map_settings, $event_listeners, $resizable);

  $out = '<div id=""openlayers-container-' . $map_id . '" class="openlayers-container openlayers_width ui-widget-content" style="width: 100%;">';
  $out .= '<div id="openlayers-map-' . $map_id . '" class="smallmap" style="width:100%; height:100%; margin: 10px;"';

  // Additional query parameters as set in the data portal admin section.
  $labels_on = $map_settings['show_labels'];

  // need to set the ms parameter to some value in order to satisfy the
  // map service even if this value should not be required:
  $width = 512;

  $openlayers_map_query_string = '&img=false&ms=' . $width
  . ($labels_on ? '&label=' . $labels_on : '');

  if ($occurrenceQuery) {
    // @todo Fix $occurrenceQuery.
    //     $occurrenceQuery .= '&bbox=-180,-90,180,90';
    $occurrenceQuery .= '&l=v%3Aatbi%2Ce_w_0'; // TODO why are we using v:atbi,e_w_0 as layer ???
    // $occurrenceQuery .= '&l=v:e_w_0';
    // TODO add to cdm service?
    $occurrenceQuery .= '&legend=0';

    $out .= ' occurrenceQuery="' . $occurrenceQuery . '&' . $openlayers_map_query_string . '"';
  }

  if ($distributionQuery) {
    //HACK for testing (this must be done in js)
//     $distributionQuery .= "&layer=em_tiny_jan2003&dest_projection_epsg=7777777";
    $out .= ' distributionQuery="' . $distributionQuery . '&' . $openlayers_map_query_string . '"';
  }

  if ($legendFormatQuery) {
    $out .= ' legendFormatQuery="' . $legendFormatQuery . '"';
  }

  $out .= '></div></div>';

  // Showing map caption.
  if ($map_caption) {
    $out .= '<div class="distribution_map_caption">' . $map_caption . '</div>';
  }
  return $out;
}


/**
 * Composes the render array for a distribution map using the given distribution query string.
 *
 * The distribution map can either be a plain image or a dynamic open layers map
 * depending on the settings.
 *
 * compose_hook() implementation
 *
 * @param string $query_string
 *    An EDIT map services distribution query string
 *
 * @return array
 *    A drupal render array
 *
 * Similar compose function compose_map()
 *
 * @ingroup compose
 */
function compose_distribution_map($query_string) {

  $fontStyles = array(
      0 => "plane",
      1 => "italic",
  );

  if (!$query_string) {
    // The $query_string is empty if there are no distribution areas defined.
    return null;
  }

  /* ------ choose the display mode, either openlayers or static image ------ */

  $map_settings = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);

  if ($map_settings['map_type'] == 1) {

    /* =========== display distributions using the openlayers map viewer =========== */

    $legendFormatQueryStr = "format=image" . urlencode('/') . "png"
      . "&TRANSPARENT=TRUE"
      . "&WIDTH=" . $map_settings['legend']['icon_width']
      . "&HEIGHT=" . $map_settings['legend']['icon_height']
      // TODO why is the layer=topp:tdwg_level_4 parameter needed at all here??
      // AK: i think the tdwg_level_4 is used as place holder and will be replaced later on
      // => search for "tdwg_level_4" in the code
      . "&layer=topp" . urlencode(':') . "tdwg_level_4"
      . "&LEGEND_OPTIONS=forceLabels" . urlencode(':') . "on"
      . ";fontStyle" . urlencode(':') . $fontStyles[$map_settings['legend']['font_style']]
      . ";fontSize" . urlencode(':') .  $map_settings['legend']['font_size']
      . "&SLD=";

    /*$out .= cdm_map_openlayers(
        $map_settings['bbox'],
        NULL,
        $query_string,
        $legendFormatQueryStr,
        $map_settings['caption']
    );
    */
  }
  else {
    $legendFormatQueryStr = '';
    /*
        cdm_map_plain_image(
            $map_settings['image_map']['width'],
            $map_settings['image_map']['height'],
            $map_settings['bbox'],
            NULL,
            $query_string,
            $legendFormatQueryStr,
            $map_settings['caption']
        );
    */
  }
  $out = compose_map('distribution', NULL, $query_string, $legendFormatQueryStr);

  return $out;
}




/**
 * Composes the markup for a plain image map.
 *
 * @param int $width
 * @param string $occurrenceQuery
 * @param string $distributionQuery
 * @param string $legendFormatQuery
 * @param string $map_caption
 *
* @return String
 *    rendered html
 */
function cdm_map_plain_image($width, $height= NULL, $occurrenceQuery = FALSE, $distributionQuery = FALSE,
                             $legendFormatQuery = FALSE, $map_caption = FALSE) {

  $map_settings = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);

  $baselayer_name = $map_settings['image_map']['base_layer'];
  if(empty($baselayer_name)){
    $baselayer_name = "earth";
  }

  $query_string = '&img=true&recalculate=false&ms=' . $width . ($height ? ',' . $height : '')
  // Additional query parameters as set in the data portal admin section.
  . ($map_settings['bbox'] ? '&bbox=' . $map_settings['bbox'] : '')
  . ($map_settings['show_labels'] ? '&label=' . $map_settings['show_labels'] : '');

  if ($map_caption) {
    $query_string .= '&mlp=3&mc_s=Georgia,15,blue&mc=' . $map_caption;
  }

  if (get_edit_map_service_version_number() >= 1.1) {

    // Either occurrence or distribution - combined maps will be possible
    // in the future.
    if ($occurrenceQuery) {
      // @todo Fix $occurrenceQuery.
      $occurrenceQuery = str_replace("&image=false", "", $occurrenceQuery);
      // $occurrenceQuery .= '&l=v%3Aatbi%2Ce_w_0';

      // Will be replaced below.. HACK!!!
      $occurrenceQuery .= '&l=' . $baselayer_name . '&as=';

      $query_string .= "&" . $occurrenceQuery;
    }
    elseif ($distributionQuery) {
      $query_string .= '&l=' . $baselayer_name . "&" .$distributionQuery;
    }

    // Apply Plain Image map settings special for version >= 1.1.
    /*
    example : title=a:Naturalized++non-invasive
    &ad=cyprusdivs:bdcode:a:5&as=a:ff9900,,0.1,&l=tdwg4
    &ms=500&bbox=32,34,35,36&img=true&legend=1&mlp=3
    &mc_s=Georgia,15,blue&mc=&recalculate=false

    http://edit.br.fgov.be/edit_wp5/v1/rest_gen.php?
    l=background_gis:b,cyprusdivs&ad=cyprusdivs%3Abdcode%3Aa%3A8%2C4
    &as=a%3A339966%2C%2C0.1%2C|b:0000ff,,
    &bbox=32%2C34%2C35%2C36&img=true&legend=1&mc=&mc_s=Georgia%2C15%2Cblue
    &mlp=3&ms=500&recalculate=false&title=a%3Aindigenous
    */

    $map_service_script_name = "rest_gen.php";

    $bgcolor_areaStyleId = "Y";
    $baselayer_areaStyleId = "Z";
    $bgcolor_layer = '';
    $additional_area_styles = array();

    // Background color:
    if ($map_settings['image_map']['bg_color'] ) {
      $bgcolor_layer = "background_gis:" . $bgcolor_areaStyleId;
      $additional_area_styles[] = $bgcolor_areaStyleId . ":" . $map_settings['image_map']['bg_color'] . ",,";
    }

    // TODO HACK to replace the default base layer which currently is tdwg4 !!!
    // only needed for distribution maps.
    if (strpos($query_string, "?l=") !== FALSE) {
      $layer_param_token = "?l=";
    }
    else {
      $layer_param_token = "&l=";
    }
    if (strpos($query_string, "?as=") !== FALSE) {
      $areystyle_param_token = "?as=";
    }
    else {
      $areystyle_param_token = "&as=";
    }
    if ($map_settings['image_map']['base_layer']) {
      $query_string = str_replace($layer_param_token .$baselayer_name, "$layer_param_token" . $map_settings['image_map']['base_layer'] . ":" . $baselayer_areaStyleId, $query_string);
    }
    else {
      $query_string = str_replace($layer_param_token . $baselayer_name, $layer_param_token . $baselayer_name . ":" . $baselayer_areaStyleId . ",", $query_string);
    }

    if ($bgcolor_layer) {
      $query_string = str_replace($layer_param_token, $layer_param_token . $bgcolor_layer . ",", $query_string);
    }

    if ($map_settings['image_map']['layer_style']) {
      $additional_area_styles[] = $baselayer_areaStyleId . ":" . $map_settings['image_map']['layer_style'];
    }

    if(isset($map_settings['projection'])){
      $query_string .= "&srs=" . $map_settings['projection'];
    }

    if(isset($map_settings['legend']['show']) && $map_settings['legend']['show']){
      $query_string .= "&legend=1";
    }

    foreach ($additional_area_styles as $as) {
      $query_string = str_replace($areystyle_param_token, $areystyle_param_token . $as . "|", $query_string);
    }

  }
  else {
    // Pre 1.1. version of map service.
    if ($occurrenceQuery) {

      $map_service_script_name = "point.php";

      // Fix $occurrenceQuery.
      $occurrenceQuery = str_replace("&image=false", "", $occurrenceQuery);
      // $occurrenceQuery .= '&l=v%3Aatbi%2Ce_w_0';
      $occurrenceQuery .= '&l=v:e_w_0';
      $query_string .= "&" . $occurrenceQuery;
    }
    elseif ($distributionQuery) {
      $query_string .= "&" . $distributionQuery;
      $map_service_script_name = "areas.php";
    }
  }

  $mapUri = url(get_edit_map_service_full_uri() . '/' . $map_service_script_name . '?' .  $query_string);
  $out = '<img class="distribution_map" src="' . $mapUri . '" alt="Map" />';
  // Showing map caption.
  if ($map_caption) {
    $out .= '<div class="distribution_map_caption">' . $map_caption . '</div>';
  }

  return $out;
}


