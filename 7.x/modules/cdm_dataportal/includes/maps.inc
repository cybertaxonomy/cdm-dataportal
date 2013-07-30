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
 * @ingroup compose
 */
function compose_map($occurrence_query, $distribution_query, $legend_format_query, array $event_listeners = array()) {

  if (variable_get('cdm_dataportal_map_openlayers', 1)) {
    $map_html = get_openlayers_map(
        variable_get('cdm_dataportal_geoservice_display_width', 680),
        variable_get('cdm_dataportal_geoservice_bounding_box', FALSE),
        $occurrence_query,
        $distribution_query,
        $legend_format_query,
        variable_get('cdm_dataportal_geoservice_map_caption', ''),
        $event_listeners
    );
  }
  else {
    $map_html = get_image_map(
        variable_get('cdm_dataportal_geoservice_display_width', 680),
        variable_get('cdm_dataportal_geoservice_bounding_box', FALSE),
        $occurrence_query,
        $distribution_query,
        $legend_format_query,
        variable_get('cdm_dataportal_geoservice_map_caption', '')
    );
  }
  return markup_to_render_array($map_html, 0);
}

/**
 *
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
function _add_js_openlayers_map(array $event_listeners = array()) {

  _add_js_openlayers();

  drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/map/openlayers_map.js');
  drupal_add_js(drupal_get_path('module', 'cdm_dataportal') . '/js/map/openlayers_layers.js');

  $gmap_api_key = variable_get('gmap_api_key', 'ABQIAAAAFho6eHAcUOTHLmH9IYHAeBRi_j0U6kJrkFvY4-OX2XYmEAa76BTsyMmEq-tn6nFNtD2UdEGvfhvoCQ');

  $baseLayers = variable_get('baselayers', array());
  if (!is_array($baseLayers) || count($baseLayers) == 0) {
    $baseLayers = array(
        'metacarta_vmap0' => 'Metacarta Vmap0',
        'PREFERRED' => 'metacarta_vmap0',
    );
  }
  $layerNames = '';
  foreach ($baseLayers as $layerName => $layerLabel) {
    if ($layerName == 'PREFERRED') {
      $preferredLayer = $baseLayers['PREFERRED'];
    }
    else {
      $layerNames .= ($layerNames ? ', ' : '') . "'$layerName'";
    }
  }

  if (isset($baseLayers['gmap']) || isset($baseLayers['gsat']) || isset($baseLayers['ghyb'])) {
    // gmaps version 2 (needs api key)
    drupal_add_js('http://maps.google.com/maps?file=api&v=2&key=' . $gmap_api_key . '', array('type' => 'external'));
    // gmaps version 3 (does not need api key)
//     drupal_add_js('http://maps.google.com/maps?file=api&v=3&sensor=false', array('type' => 'external'));
    drupal_add_js('http://dev.virtualearth.net/mapcontrol/mapcontrol.ashx?v=6.1', array('type' => 'external'));
  }

  $event_listeners_js = '';
  $execute_handler = '';
  foreach($event_listeners as $event=>$js_callback){
    if($event == '#execute'){
      $execute_handler = 'map_container.each(function(){' . $js_callback . '();});';
    } else {
      $event_listeners_js .= ($event_listeners_js ? ",\n": "\n") .'"' . $event . '": ' . $js_callback;
    }
  }

  $edit_map_service = get_edit_map_service_settings();
  // window.onload - is executed when the document and images etc is fully loaded
  // Query(document).ready - is executed much earlier, when the DOM is loaded
  drupal_add_js("
          jQuery(document).ready(function() {
                window.onload = function () {
                  var map_container = jQuery('#openlayers_map').cdm_openlayers_map(
                   '" . $edit_map_service['base_uri'] . "',
                   '" . $edit_map_service['version'] . "',
                   {
                    legendPosition: 3,
                    boundingBox: '" . variable_get('cdm_dataportal_geoservice_bounding_box', FALSE) . "',
                    distributionOpacity: '" . variable_get('cdm_dataportal_geoservice_distributionOpacity', '0.5') . "',
                    legendOpacity: '" . variable_get('cdm_dataportal_geoservice_legendOpacity', '0.5') . "',
                    showLayerSwitcher: " . (variable_get('cdm_dataportal_geoservice_showLayerSwitcher', TRUE) ? 'true' : 'false') . ",
                    baseLayerNames: [" . $layerNames . "],
                    defaultBaseLayerName: '" . $preferredLayer . "'
                   }
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
function get_openlayers_map($width, $bounding_box = FALSE, $occurrenceQuery = FALSE, $distributionQuery = FALSE, $legendFormatQuery = FALSE, $map_caption = FALSE, array $event_listeners = array()) {

  _add_js_openlayers_map($event_listeners);

  $out = '<div id="openlayers">';
  $out .= '<div id="openlayers_map" class="smallmap"';
  if($width) {
    $out .= ' style="width: ' . $width . 'px; height:' . ($width / 2) . 'px"';
  }

  // Additional query parameters as set in the data portal admin section.
  $labels_on = variable_get('cdm_dataportal_geoservice_labels_on', 0);

  $openlayers_map_query_string = '&img=false&ms=' . $width
  . ($bounding_box ? '&bbox=' . $bounding_box : '')
  . ($labels_on ? '&label=' . $labels_on : '');

  if ($occurrenceQuery) {
    // @todo Fix $occurrenceQuery.
    //     $occurrenceQuery .= '&bbox=-180,-90,180,90';
    $occurrenceQuery .= '&l=v%3Aatbi%2Ce_w_0';
    // $occurrenceQuery .= '&l=v:e_w_0';
    // TODO add to cdm service?
    $occurrenceQuery .= '&legend=0';

    $out .= ' occurrenceQuery="' . $occurrenceQuery . '&' . $openlayers_map_query_string . '"';
  }

  if ($distributionQuery) {
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
function get_image_map($width, $bounding_box = FALSE, $occurrenceQuery = FALSE, $distributionQuery = FALSE, $legendFormatQuery = FALSE, $map_caption = FALSE) {

  // Additional query parameters as set in the data portal admin section.
  $labels_on = variable_get('cdm_dataportal_geoservice_labels_on', 0);

  $query_string = '&image=true&recalculate=false&legend=1&ms=' . $width
  . ($bounding_box ? '&bbox=' . $bounding_box : '')
  . ($labels_on ? '&label=' . $labels_on : '');

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
      $occurrenceQuery .= '&l=tdwg4&as=';

      $query_string .= "&" . $occurrenceQuery;
    }
    elseif ($distributionQuery) {
      $query_string .= "&" . $distributionQuery;
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

    $bgcolor_areaStyleId = "y";
    $baselayer_areaStyleId = "z";
    $bgcolor_layer = '';
    $additional_area_styles = array();

    // Background color:
    if (variable_get('map_bg_color', '')) {
      $bgcolor_layer = "background_gis:" . $bgcolor_areaStyleId;
      $additional_area_styles[] = $bgcolor_areaStyleId . ":" . variable_get('map_bg_color', '') . ",,";
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
    if (variable_get('map_base_layer', '')) {
      $query_string = str_replace($layer_param_token . "tdwg4", "$layer_param_token" . variable_get('map_base_layer', '') . ":" . $baselayer_areaStyleId, $query_string);
    }
    else {
      $query_string = str_replace($layer_param_token . "tdwg4", $layer_param_token . "tdwg4:" . $baselayer_areaStyleId . ",", $query_string);
    }

    if ($bgcolor_layer) {
      $query_string = str_replace($layer_param_token, $layer_param_token . $bgcolor_layer . ",", $query_string);
    }

    if (variable_get('map_base_layer_style', '')) {
      $additional_area_styles[] = $baselayer_areaStyleId . ":" . variable_get('map_base_layer_style', '');
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


