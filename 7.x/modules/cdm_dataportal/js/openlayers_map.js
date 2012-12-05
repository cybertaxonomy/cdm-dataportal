function CdmOpenlayersMap(mapElement, mapserverBaseUrl, options){

  var legendImgSrc = null;

  var map = null;

  var dataBounds = null;

  var baseLayers = [];
  var defaultBaseLayer;

  var defaultControls = [
           new OpenLayers.Control.PanZoom(),
           new OpenLayers.Control.Navigation({zoomWheelEnabled: false, handleRightClicks:true, zoomBoxKeyMask: OpenLayers.Handler.MOD_CTRL})
         ];
  /*
   * EPSG:4326 and EPSG:900913 extends
   */
  var mapExtend_4326 = new OpenLayers.Bounds(-180, -90, 180, 90);
  var mapExtend_900913 = new OpenLayers.Bounds(-20037508.34, -20037508.34, 20037508.34, 20037508.34);

  var mapOptions={
      EPSG900913: {
        controls: defaultControls,
        maxExtent: mapExtend_900913,
        //maxResolution: (mapExtend_900913.getWidth() / options.displayWidth),
        maxResolution: (mapExtend_900913.getHeight() / (options.displayWidth / 2)),
        //maxResolution:156543.0339 * 20,
        units:'m',
        restrictedExtent: mapExtend_900913,
        projection: new OpenLayers.Projection("EPSG:900913")
      },
      EPSG4326: {
        controls: defaultControls,
        maxExtent: mapExtend_4326,
        maxResolution: (mapExtend_4326.getWidth() / options.displayWidth),
        units:'degrees',
        restrictedExtent: mapExtend_4326,
        projection: new OpenLayers.Projection("EPSG:4326")
      }
  };


  var dataProj = new OpenLayers.Projection("EPSG:4326");

  var dataLayerOptions = {
      maxExtent: mapExtend_900913,
      isBaseLayer: false,
      displayInLayerSwitcher: true
  };

  var layerByNameMap = {
      tdwg1: 'topp:tdwg_level_1',
      tdwg2: 'topp:tdwg_level_2',
      tdwg3: 'topp:tdwg_level_3',
      tdwg4: 'topp:tdwg_level_4'
  };

  /**
   *
   */
  this.init = function(){

    createLayers(options.baseLayerNames, options.defaultBaseLayerName);

     //HACK !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(defaultBaseLayer.projection == mapOptions.EPSG900913.projection){
      options.minZoom = 1;
    }

    initOpenLayers();



    // -- Distribution Layer --
    var mapServiceRequest;
    var distributionQuery = mapElement.attr('distributionQuery');

    if(distributionQuery !== undefined){
      if(typeof legendPosition == 'number'){
        distributionQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + options.legendPosition);
      }

      distributionQuery = mergeQueryStrings(distributionQuery, 'callback=?');
      var legendFormatQuery = mapElement.attr('legendFormatQuery');
      if(legendFormatQuery !== undefined){
        legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
      }

      mapServiceRequest = mapserverBaseUrl + '/areas.php?' + distributionQuery;

      jQuery.ajax({
          url: mapServiceRequest,
          dataType: "jsonp",
          success: function(data){
            addDataLayer(data);
          }
        });
    }

    // -- Occurrence Layer --
    var occurrenceQuery = mapElement.attr('occurrenceQuery');
    if(occurrenceQuery !== undefined){
//      if(typeof legendPosition == 'number'){
//        occurrenceQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + options.legendPosition);
//      }

      occurrenceQuery = mergeQueryStrings(occurrenceQuery, 'callback=?');
//      var legendFormatQuery = mapElement.attr('legendFormatQuery');
//      if(legendFormatQuery !== undefined){
//        legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
//      }

      mapServiceRequest = mapserverBaseUrl + '/points.php?' + occurrenceQuery;

      jQuery.ajax({
          url: mapServiceRequest,
          dataType: "jsonp",
          success: function(data){
            addDataLayer(data);
          }
        });
    }


  };

  /**
   * Initialize the Openlayers viewer with the base layer
   */
  var initOpenLayers = function(){

    // instantiate the openlayers viewer
    if(defaultBaseLayer.projection == mapOptions.EPSG900913.projection){
      map = new OpenLayers.Map('openlayers_map', mapOptions.EPSG900913);
    } else {
      map = new OpenLayers.Map('openlayers_map', mapOptions.EPSG4326);
    }

    //add the base layers
    map.addLayers(baseLayers);
    map.setBaseLayer(defaultBaseLayer);

    if(options.showLayerSwitcher === true){
      map.addControl(new OpenLayers.Control.LayerSwitcher({'ascending':false}));
    }

    // zoom to the required area
    var boundsStr = (typeof options.boundingBox == 'string' && options.boundingBox.length > 6 ? options.boundingBox : '-180, -90, 180, 90');
    var zoomToBounds = OpenLayers.Bounds.fromString( boundsStr );
    map.zoomToExtent(zoomToBounds.transform(dataProj, map.getProjectionObject()), false);
    if(map.getZoom() > options.maxZoom){
      map.zoomTo(options.maxZoom);
    } else if(map.getZoom() < options.minZoom){
      map.zoomTo(options.minZoom);
    }

    // adjust height of openlayers container div
    jQuery('#openlayers').css('height', jQuery('#openlayers #openlayers_map').height());

  };

  /**
   * add a distribution layer
   */
  var addDataLayer = function(mapResponseObj){

    var layer;
    // add additional layers, get them from
    // the mapResponseObj
    if(mapResponseObj !== undefined){
      if(mapResponseObj.points_sld !== undefined){
        // it is a response from the points.php
        //TODO points_sld should be renamed to sld in response + fill path to sld should be given

        layer = new OpenLayers.Layer.WMS.Untiled(
            'points',
            "http://193.190.116.6:8080/geoserver/wms/wms",
            {layers: 'topp:rest_points' ,transparent:"true", format:"image/png"},
            dataLayerOptions );

        var sld = mapResponseObj.points_sld;
        if(sld.indexOf("http://") !== 0){
          sld = "http://edit.br.fgov.be/synthesys/www/v1/sld/" + sld;
        }

        layer.params.SLD = sld;
        map.addLayers([layer]);

      } else {
        // it is a response from the areas.php
        for ( var i in mapResponseObj.layers) {
        var layerData = mapResponseObj.layers[i];

          layer = new OpenLayers.Layer.WMS.Untiled(
              layerData.tdwg,
              mapResponseObj.geoserver + "/wms",
              {layers: layerByNameMap[layerData.tdwg] ,transparent:"true", format:"image/png"},
              dataLayerOptions );
          layer.params.SLD = layerData.sld;
          layer.setOpacity(options.distributionOpacity);
          map.addLayers([layer]);

        }

      }

      // zoom to the required area
      if(mapResponseObj.bbox !== undefined){
        var newBounds =  OpenLayers.Bounds.fromString( mapResponseObj.bbox );
        if(dataBounds !== null){
          dataBounds.extend(newBounds);
        } else if(newBounds !== undefined){
          dataBounds = newBounds;
        }
        map.zoomToExtent(dataBounds.transform(dataProj, map.getProjectionObject()), false);
        if(map.getZoom() > options.maxZoom){
          map.zoomTo(options.maxZoom);
        } else if(map.getZoom() < options.minZoom){
          map.zoomTo(options.minZoom);
        }
      }


      if(options.legendPosition !== undefined && mapResponseObj.legend !== undefined){
        var legendSrcUrl = mapResponseObj.geoserver + legendImgSrc + mapResponseObj.legend;
        addLegendAsElement(legendSrcUrl);
        //addLegendAsLayer(legendSrcUrl, map);
      }
    }

  };

  /**
   *
   */
  var addLegendAsElement= function(legendSrcUrl){

    mapElement.after('<div class="openlayers_legend"><img src="' + legendSrcUrl + '"></div>');
    mapElement.next('.openlayers_legend').css('opacity', options.legendOpacity).find('img').load(function () {
      jQuery(this).parent()
        .css('position', 'relative')
        .css('z-index', '1002')
        .css('top', -mapElement.height())
        .css('left', mapElement.width()- jQuery(this).width())
        .width(jQuery(this).width());
    });
  };


  var addLegendAsLayer= function(legendSrcUrl, map){
    var w, h;

    // 1. download imge to find height and width
    mapElement.after('<div class="openlayers_legend"><img src="' + legendSrcUrl + '"></div>');
    mapElement.next('.openlayers_legend').css('display', 'none').css('opacity', options.legendOpacity).find('img').load(function () {

      w = mapElement.next('.openlayers_legend').find('img').width();
      h = mapElement.next('.openlayers_legend').find('img').height();
      mapElement.next('.openlayers_legend').remove();

//      createLegendLayer();
//      // 2. create the Legend Layer
      //TODO createLegendLayer as inner fiinction seems like an error
//      var createLegendLayer = function(){
//
//
//        var legendLayerOptions={
//            maxResolution: '.$maxRes.',
//            maxExtent: new OpenLayers.Bounds(0, 0, w, h)
//        };
//
//        var legendLayer = new OpenLayers.Layer.Image(
//            'Legend',
//            legendSrcUrl,
//            new OpenLayers.Bounds(0, 0, w, h),
//            new OpenLayers.Size(w, h),
//            imageLayerOptions);
//      };
    });


  };

  /**
   * merge 2 Url query strings
   */
   var mergeQueryStrings = function(queryStr1, queryStr2){
    if(queryStr1.charAt(queryStr1.length - 1) != '&'){
      queryStr1 += '&';
    }
    if(queryStr2.charAt(0) == '&'){
      return queryStr1 + queryStr2.substr(1);
    } else {
      return queryStr1 + queryStr2;
    }
  };

  /**
   *
   */
  var createLayers = function( baseLayerNames, defaultBaseLayerName){
    //var baseLayers = new Array();
    for(var i = 0; i <  baseLayerNames.length; i++) {
      //var layerName in baseLayerNames ){
      baseLayers[i] = getLayersByName(baseLayerNames[i]);
      // set the default base layer
      if(baseLayerNames[i] == defaultBaseLayerName){
        defaultBaseLayer = baseLayers[i];
      }
    }

    //return baseLayers;
  };


  /**
   *
   */
  var getLayersByName = function(layerName){

    var baseLayer;

    switch(layerName){

      case 'metacarta_vmap0':
        /**
         * NOTE: labs.metacarta.com is currently unavailable
         *
         * Available Projections:
         *     EPSG:900913
         *     EPSG:4326
         */
        baseLayer = new OpenLayers.Layer.WMS(
              "Metacarta Vmap0",
              "http://labs.metacarta.com/wms/vmap0",
              {layers: 'basic', format:"png"},
              {
              maxExtent: mapExtend_4326,
              isBaseLayer: true,
              displayInLayerSwitcher: true
            }
        );
      break;

      case 'osgeo_vmap0':
        /**
         * Available Projections:
         *     EPSG:4269
              EPSG:4326
              EPSG:900913
         */
        baseLayer = new OpenLayers.Layer.WMS(
              "Metacarta Vmap0",
              "http://vmap0.tiles.osgeo.org/wms/vmap0",
              {layers: 'basic', format:"png"},
              {
            maxExtent: mapExtend_900913,
            isBaseLayer: true,
            displayInLayerSwitcher: true
          }
        );
      break;

      case 'edit-etopo1':
        baseLayer = new OpenLayers.Layer.WMS(
              "ETOPO1 Global Relief Model",
              "http://edit.br.fgov.be:8080/geoserver/wms",
              {layers: 'topp:color_etopo1_ice_full', format:"image/png"},
              {
            maxExtent: mapExtend_900913,
            isBaseLayer: true,
            displayInLayerSwitcher: true
          }
        );
      break;

      case 'edit-vmap0_world_basic':
        baseLayer = new OpenLayers.Layer.WMS(
              "EDIT Vmap0",
              "http://edit.br.fgov.be:8080/geoserver/wms",
              {layers: 'vmap0_world_basic', format:"image/png"},
              {
            maxExtent: mapExtend_900913,
            isBaseLayer: true,
            displayInLayerSwitcher: true
          }
        );
      break;

      // create Google Mercator layers
      case 'gmap':
        baseLayer = new OpenLayers.Layer.Google(
                  "Google Streets",
                  {'sphericalMercator': true}
              );
      break;


      case 'gsat':
        baseLayer = new OpenLayers.Layer.Google(
                  "Google Satellite",
                  {type: G_SATELLITE_MAP, 'sphericalMercator': true, numZoomLevels: 22}
              );
      break;

      case 'ghyb':
        baseLayer = new OpenLayers.Layer.Google(
                  "Google Hybrid",
                  {type: G_HYBRID_MAP, 'sphericalMercator': true}
              );
      break;

      case 'veroad':
        baseLayer = new OpenLayers.Layer.VirtualEarth(
                  "Virtual Earth Roads",
                  {'type': VEMapStyle.Road, 'sphericalMercator': true}
              );
      break;

      case 'veaer':
        baseLayer = new OpenLayers.Layer.VirtualEarth(
                  "Virtual Earth Aerial",
                  {'type': VEMapStyle.Aerial, 'sphericalMercator': true}
              );
      break;

      case 'vehyb':
        baseLayer = new OpenLayers.Layer.VirtualEarth(
                  "Virtual Earth Hybrid",
                  {'type': VEMapStyle.Hybrid, 'sphericalMercator': true}
              );
      break;

      case 'yahoo':
        baseLayer = new OpenLayers.Layer.Yahoo(
                  "Yahoo Street",
                  {'sphericalMercator': true}
              );
      break;

      case 'yahoosat':
        baseLayer = new OpenLayers.Layer.Yahoo(
                  "Yahoo Satellite",
                  {'type': YAHOO_MAP_SAT, 'sphericalMercator': true}
              );
      break;

      case 'yahoohyb':
         rebaseLayer = new OpenLayers.Layer.Yahoo(
                  "Yahoo Hybrid",
                  {'type': YAHOO_MAP_HYB, 'sphericalMercator': true}
              );
      break;

      case 'mapnik':
        baseLayer = new OpenLayers.Layer.OSM();
      break;

      case 'oam':
        baseLayer = new OpenLayers.Layer.XYZ(
                "OpenAerialMap",
                "http://tile.openaerialmap.org/tiles/1.0.0/openaerialmap-900913/${z}/${x}/${y}.png",
                {
                    sphericalMercator: true
                }
            );
      break;

      case 'osmarender':
        baseLayer = new OpenLayers.Layer.OSM(
                "OpenStreetMap (Tiles@Home)",
                "http://tah.openstreetmap.org/Tiles/tile/${z}/${x}/${y}.png"
               );
      break;

    };

    return baseLayer;
  };

}


(function($){

  $.fn.cdm_openlayers_map = function(mapserverBaseUrl, options) {

    var opts = $.extend({},$.fn.cdm_openlayers_map.defaults, options);

    return this.each(function(){

      var openlayers_map = new CdmOpenlayersMap($(this), mapserverBaseUrl, opts);
      openlayers_map.init();

     }); // END each

  }; // END cdm_openlayers_map

})(jQuery);

(function($){
$.fn.cdm_openlayers_map.defaults = {  // set up default options
    legendPosition:  null,      // 1,2,3,4,5,6 = display a legend in the corner specified by the number
    displayWidth: 400,
    distributionOpacity: 0.75,
    legendOpacity: 0.75,
    boundingBox: null,
    showLayerSwitcher: false,
    baseLayerNames: ["osgeo_vmap0"],
    defaultBaseLayerName: 'osgeo_vmap0',
    maxZoom: 4,
    minZoom: 0
};
})(jQuery);

