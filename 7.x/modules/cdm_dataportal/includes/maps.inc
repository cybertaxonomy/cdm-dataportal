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
 *
 * @return array
 *    A drupal render array
 *
 * Similar compose function compose_distribution_map()
 *
 * @ingroup compose
 */
function compose_map($occurrence_query = NULL, $distribution_query = NULL, $legend_format_query = NULL, array $event_listeners = array()) {

  $map_settings = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);
  if ($map_settings['map_type'] == 1) {
    $map_html = get_openlayers_map(
        $map_settings['width'],
        $map_settings['height'],
        $map_settings['bbox'],
        $occurrence_query,
        $distribution_query,
        $legend_format_query,
        $map_settings['caption'],
        $event_listeners
    );
  }
  else {
    $map_html = get_image_map(
        $map_settings['width'],
        $map_settings['height'],
        $map_settings['bbox'],
        $occurrence_query,
        $distribution_query,
        $legend_format_query,
        $map_settings['caption']
    );
  }
  return markup_to_render_array($map_html, 0);
}

/**
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
 */
function _add_js_openlayers_map($map_settings, array $event_listeners = array()) {

  _add_js_openlayers();

  $edit_map_service = get_edit_map_service_settings();

  $gmap_api_key = variable_get('gmap_api_key', 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ');

  drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/map/openlayers_map.js');
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/map/openlayers_layers.js');

  $cdm_openlayers_options = array(
      'legendPosition'  => '3',
      'boundingBox' => $map_settings['bbox'],
      'distributionOpacity' => $map_settings['distribution_opacity'],
      'legendOpacity' => $map_settings['legend']['opacity'],
      'showLayerSwitcher' => $map_settings['openlayers']['show_layer_switcher']  ==  1,
      'displayOutsideMaxExtent' => $map_settings['openlayers']['display_outside_max_extent'] == 1,
//       'imgPath' => drupal_get_path('module', 'cdm_dataportal') . '/js/map/OpenLayers-2.13.1/img/' // path to the control icons
      // if no baseLayerNames or defaultBaseLayerName are not defined
      // the defaults in cdm_openlayers.js will be used
  );

  // --- setting the base layer options
  if (is_array($map_settings['openlayers']['base_layers']) && count($map_settings['openlayers']['base_layers']) > 0) {

    $layer_names = $map_settings['openlayers']['base_layers'];

    if(isset($layer_names['PREFERRED'])){
      $cdm_openlayers_options['defaultBaseLayerName'] = $layer_names['PREFERRED'];
      unset($layer_names['PREFERRED']);
    }

    $cdm_openlayers_options['baseLayerNames'] = array_values($layer_names);

    if (isset($layer_names['gmap']) || isset($layer_names['gsat']) || isset($layer_names['ghyb'])) {
      // gmaps version 2 (needs api key)
      drupal_add_js('http://maps.google.com/maps?file=api&v=2&key=' . $gmap_api_key . '', array('type' => 'external'));
      // gmaps version 3 (does not need api key)
  //     drupal_add_js('http://maps.google.com/maps?file=api&v=3&sensor=false', array('type' => 'external'));
      drupal_add_js('http://dev.virtualearth.net/mapcontrol/mapcontrol.ashx?v=6.1', array('type' => 'external'));
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

//   // combine keys and values
//   foreach($cdm_openlayers_options as $key=>&$val){
//     $val = $key . ": " . $val;
//   };

  // window.onload - is executed when the document and images etc is fully loaded
  // Query(document).ready - is executed much earlier, when the DOM is loaded
  drupal_add_js("
          jQuery(document).ready(function() {
                window.onload = function () {
                  var map_container = jQuery('#openlayers_map').cdm_openlayers_map(
                   '" . $edit_map_service['base_uri'] . "',
                   '" . $edit_map_service['version'] . "',
                   " .  json_encode($cdm_openlayers_options) . "
                );
                map_container.each(function(){
                        this.cdmOpenlayersMap.registerEvents({" . $event_listeners_js . "});
                });
                " . $execute_handler . "
        };
      });
    ", array('type' => 'inline'));

}


/**
 * @todo Enter description here ...
 *
 * @param unknown_type $width
 * @param unknown_type $bounding_box
 * @param unknown_type $occurrenceQuery
 * @param unknown_type $distributionQuery
 * @param unknown_type $legendFormatQuery
 * @param unknown_type $map_caption
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
 *    rendered html
 */
function get_openlayers_map($width, $height, $bounding_box = FALSE, $occurrenceQuery = FALSE, $distributionQuery = FALSE, $legendFormatQuery = FALSE, $map_caption = FALSE, array $event_listeners = array()) {

  $map_settings = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);

  _add_js_openlayers_map($map_settings, $event_listeners);

  $out = '<div id="openlayers">';
  $out .= '<div id="openlayers_map" class="smallmap"';
  if($width) {
    if(!$height){
      $height = $width / 2;
    }
    $out .= ' style="width: ' . $width . 'px; height:' . $height . 'px"';
  }

  // Additional query parameters as set in the data portal admin section.
  $labels_on = $map_settings['show_labels'];

  $openlayers_map_query_string = '&img=false&ms=' . $width
  . ($bounding_box ? '&bbox=' . $bounding_box : '')
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
    // FIXME: replace <br> by according css style.
    $out .= '<div class="distribution_map_caption">' . $map_caption . '</div>' . '<br />';
    $out .= '</div>';
  }
  return $out;
}


/**
 * Composes the render array for a distribution map of the given taxon.
 *
 * The distribution map can either be a plain image or a dynamic open layers map
 * depending on the settings.
 *
 * compose_hook() implementation
 *
 * @param $taxon
 *   The CDM Taxon instance to create the distribution map for.
 * @return array
 *    A drupal render array
 *
 * Similar compose function compose_map()
 *
 * @ingroup compose
 */
function compose_distribution_map($taxon, $query_string) {

  $out = '';
  $settings = get_edit_map_service_settings();

  $fontStyles = array(
      0 => "plane",
      1 => "italic",
  );

//   // Query cdm server for map service uri parameters.
//   $query_string = cdm_ws_get(CDM_WS_GEOSERVICE_DISTRIBUTIONMAP, $taxon->uuid, queryString(cdm_distribution_filter_query()));
//   $query_string = $query_string->String;
  $out .= "<!-- map_data_parameters:" . print_r($query_string, TRUE) . " -->";
  if (!$query_string) {
    // The $query_string is empty if there are no distribution areas defined.
    return;
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

    $out .= get_openlayers_map(
        $map_settings['width'],
        $map_settings['height'],
        $map_settings['bbox'],
        NULL,
        $query_string,
        $legendFormatQueryStr,
        $map_settings['caption']
    );
  }
  else {
    $legendFormatQueryStr = '';
    $out = get_image_map(
        $map_settings['width'],
        $map_settings['height'],
        $map_settings['bbox'],
        NULL,
        $query_string,
        $legendFormatQueryStr,
        $map_settings['caption']
    );
  }
  return markup_to_render_array($out);
}




/**
 * @todo Enter description here ...
 *
 * @param unknown_type $width
 * @param unknown_type $bounding_box
 * @param unknown_type $occurrenceQuery
 * @param unknown_type $distributionQuery
 * @param unknown_type $legendFormatQuery
 * @param unknown_type $map_caption
 *
* @return String
 *    rendered html
 */
function get_image_map($width, $height= NULL, $bounding_box = FALSE, $occurrenceQuery = FALSE, $distributionQuery = FALSE, $legendFormatQuery = FALSE, $map_caption = FALSE) {

  $map_settings = get_array_variable_merged(CDM_MAP_DISTRIBUTION, CDM_MAP_DISTRIBUTION_DEFAULT);

  $baselayer_name = $map_settings['image_map']['base_layer'];
  if(empty($baselayer_name)){
    $baselayer_name = "earth";
  }

  $query_string = '&image=true&recalculate=false&ms=' . $width . ($height ? ',' . $height : '')
  // Additional query parameters as set in the data portal admin section.
  . ($bounding_box ? '&bbox=' . $bounding_box : '')
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
    // FIXME: replace <br> by according css style.
    $out .= '<div class="distribution_map_caption">' . $map_caption . '</div>' . '<br />';
    $out .= '</div>';
  }

  return $out;
}


