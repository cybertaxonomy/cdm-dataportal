//see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

/**
 * Expected dom structure:
 *  '<div class="ahah-content" rel="'.$cdm_proxy_url.'"><span class="loading">Loading ....</span></div>';
 */
(function($, document, window, undefined) {

  $.fn.cdm_openlayers_map = function(mapserverBaseUrl, mapserverVersion, options) {

    var opts = $.extend({},$.fn.cdm_openlayers_map.defaults, options);

    return this.each(function(){
      this.cdmOpenlayersMap = new CdmOpenLayers.Map($(this), mapserverBaseUrl, mapserverVersion, opts);
      this.cdmOpenlayersMap.init();
     }); // END each

  }; // END cdm_openlayers_map

})(jQuery, document, window, undefined);

(function($){
    $.fn.cdm_openlayers_map.defaults = {  // set up default options
        legendPosition:  null,      // 1,2,3,4,5,6 = display a legend in the corner specified by the number
        distributionOpacity: 0.75,
        legendOpacity: 0.75,
        boundingBox: "-180,-90,180,90",
        showLayerSwitcher: false,
        baseLayerNames: ["osgeo_vmap0"],
        defaultBaseLayerName: 'osgeo_vmap0',
        maxZoom: 4,
        minZoom: 0
    };
})(jQuery);



/**************************************************************************
 *                          CdmOpenLayers
 **************************************************************************/
(function() {

/**
 * The CdmOpenLayers namespace definition
 */
window.CdmOpenLayers  = (function () {

    var projections = {
            epsg_4326: new OpenLayers.Projection("EPSG:4326"),
            epsg_900913: new OpenLayers.Projection("EPSG:900913"),
            epsg_3857:  new OpenLayers.Projection("EPSG:3857")
    };
    var mapExtends = {
        epsg_4326: new OpenLayers.Bounds(-180, -90, 180, 90),
        epsg_900913: new OpenLayers.Bounds(-180, -90, 180, 90),
        epsg_3857: new OpenLayers.Bounds(-180, -90, 180, 90)
    };
    // transform epsg_900913 to units meter
    mapExtends.epsg_900913.transform(projections.epsg_4326, projections.epsg_900913);
    mapExtends.epsg_3857.transform(projections.epsg_4326, projections.epsg_3857);

    // make public by returning an object
    return {
            projections: projections,
            mapExtends: mapExtends,
            getLayerByName: function(layerName){} // initially empty fuction, will be populated by openlayers_layers.js
// TODO remove below lines before release
//            availableLayers: {
//
//                layers: {}, // initially empty, will be populated by openlayers_layers.js
//
//                createAllLayers: function() {
//                    for (var key in this.layers) {
//                        if (this.layers.hasOwnProperty(key)){
//                            var layer = this.layers[key];
//                            layer.create();
//                        };
//                    };
//                }
//            }
    };

})(); // end of namespace definition for CdmOpenLayers

  /**
   * The CdmOpenLayers.Map constructor
   * @param mapElement
   * @param mapserverBaseUrl
   * @param mapserverVersion
   * @param options
   * @returns
   */
window.CdmOpenLayers.Map = function(mapElement, mapserverBaseUrl, mapserverVersion, options){

    var mapServicePath = '/edit_wp5';

    // firebug console stub (avoids errors if firebug is not active)
    if(typeof console === "undefined") {
        console = { log: function() { } };
    }


    var legendImgSrc = null;

    var map = null;

    var infoElement = null;

    var dataBounds = null;

    var baseLayers = [];
    var defaultBaseLayer = null;

    var mapHeight = mapElement.height();
    var mapWidth = mapElement.width();

    var defaultControls = [
             new OpenLayers.Control.PanZoom(),
             new OpenLayers.Control.Navigation({zoomWheelEnabled: false, handleRightClicks:true, zoomBoxKeyMask: OpenLayers.Handler.MOD_CTRL})
           ];

    var dataProj = new OpenLayers.Projection("EPSG:4326");

    var dataLayerOptions = {
        maxExtent: window.CdmOpenLayers.mapExtends.epsg_900913,
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
    this.init = function(){ // public function

      createLayers(options.baseLayerNames, options.defaultBaseLayerName);

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

        mapServiceRequest = mapserverBaseUrl + mapServicePath + '/' + mapserverVersion + '/areas.php?' + distributionQuery;

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
//        if(typeof legendPosition == 'number'){
//          occurrenceQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + options.legendPosition);
//        }

        occurrenceQuery = mergeQueryStrings(occurrenceQuery, 'callback=?');
//        var legendFormatQuery = mapElement.attr('legendFormatQuery');
//        if(legendFormatQuery !== undefined){
//          legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
//        }

        mapServiceRequest = mapserverBaseUrl + mapServicePath + '/' + mapserverVersion + '/points.php?' + occurrenceQuery;

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
     * public function
     */
    this.registerEvents = function(events){
        for (var key in events) {
            if (events.hasOwnProperty(key)) {
                map.events.register(key, map , events[key]);
            }
        }
    };

    /**
     * public function
     */
    this.getMap = function(){
        return map;
    };

    /**
     * Prints info on the current map into the jQuery element
     * as set in the options (options.infoElement)
     * public function
     *
     * @param jQuery $element
     */
    this.printInfo = function(){

        var mapExtendDegree = map.getExtent().clone();
        mapExtendDegree.transform(map.baseLayer.projection, CdmOpenLayers.projections.epsg_4326);

        var info = "<dl>";
        info += "<dt>zoom:<dt><dd>" + map.getZoom() + "</dd>";
        info += "<dt>resolution:<dt><dd>" + map.getResolution() + "</dd>";
        info += "<dt>max resolution:<dt><dd>" + map.getMaxResolution() + "</dd>";
        info += "<dt>scale:<dt><dd>" + map.getScale() + "</dd>";
        info += "<dt>extend bbox:<dt><dd>" + map.getExtent().toBBOX() + " (" + mapExtendDegree.toBBOX() + ")</dd>";
        info += "<dt>maxExtend bbox:<dt><dd>" + map.getMaxExtent().toBBOX() + "</dd>";
        info += "<dt>baselayer projection:<dt><dd>" + map.baseLayer.projection.getCode() + "</dd>";
        info += "</dl>";

        if(infoElement == null){
            infoElement = jQuery('<div class="map_info"></div>');
            mapElement.parent().after(infoElement);
        }
        infoElement.html(info);
    };

    /**
     * Initialize the Openlayers viewer with the base layer
     */
    var initOpenLayers = function(){


      if(options.showLayerSwitcher === true){
          defaultControls.push(new OpenLayers.Control.LayerSwitcher({'ascending':false}));
      }

//      var maxExtendByAspectRatio = cropBoundsToAspectRatio(defaultBaseLayer.maxExtent, mapWidth/mapHeight);
      var maxResolution = null;
      if(defaultBaseLayer.maxExtent != null){
          // gmaps has no maxExtend at this point
          maxResolution = defaultBaseLayer.maxExtent.getWidth()/ mapWidth;
      }

      console.log("maxResolution: " + maxResolution);
//      console.log("restrictedExtent: " + maxExtendByAspectRatio.toBBOX());

      map = new OpenLayers.Map(
          "openlayers_map",
          {
              // defines the map ui elements and interaction features
              controls: defaultControls,

              // maxResolution determines the lowest zoom level and thus places the map
              // in its maximum extend into the available view port so that no additinal
              // gutter is visible and no parts of the map are hidden
              // see http://trac.osgeo.org/openlayers/wiki/SettingZoomLevels
              maxResolution: maxResolution,

              // setting restrictedExtent the the maxExtend prevents from panning the
              // map out of its bounds
              restrictedExtent: defaultBaseLayer.maxExtent,
//              maxExtent: defaultBaseLayer.maxExtent,

             // Setting the map.fractionalZoom property to true allows zooming to an arbitrary level
             // (between the min and max resolutions).
             // fractional tiles are not supported by XYZ layers like OSM so this option would
             // however break the tile retrieval for OSM (e.g.: tile for frational zoom level
             // 1.2933333333333332 = http://b.tile.openstreetmap.org/1.2933333333333332/1/0.png)
             // fractionalZoom: 1,

              eventListeners: options.eventListeners

          }
      );

      //add the base layers
      map.addLayers(baseLayers);
      map.setBaseLayer(defaultBaseLayer);

      zoomToBounds = zoomToBoundsFor(options.boundingBox, defaultBaseLayer);

      zoomToBounds = cropBoundsToAspectRatio(zoomToBounds, map.getSize().w / map.getSize().h);


      console.log("zoomToBounds: " + zoomToBounds);

      // zoom to the extend of the bbox
      map.zoomToExtent(zoomToBounds, true);

      // readjust if the zoom level is out side of the min max
      if(map.getZoom() > options.maxZoom){
        map.zoomTo(options.maxZoom);
      } else if(map.getZoom() < options.minZoom){
        map.zoomTo(options.minZoom);
      }

    };

    /**
     * add a distribution layer
     */
    var addDataLayer = function(mapResponseObj){

      var layer;
      // add additional layers, get them from the mapResponseObj
      if(mapResponseObj !== undefined){
        if(mapResponseObj.points_sld !== undefined){
          // it is a response from the points.php

        var geoserverUri;
        if(mapResponseObj.geoserver) {
          geoserverUri = mapResponseObj.geoserver;
        } else {
          // it is an old servive which is not providing the corresponding geoserver URI, so we guess it
          geoserverUri = mapserverBaseUrl + "/geoserver/wms";
        }

        //TODO points_sld should be renamed to sld in response + fill path to sld should be given
          layer = new OpenLayers.Layer.WMS.Untiled(
              'points',
              geoserverUri,
              {layers: 'topp:rest_points' ,transparent:"true", format:"image/png"},
              dataLayerOptions );

          var sld = mapResponseObj.points_sld;
          if(sld.indexOf("http://") !== 0){
            // it is an old servive which is not providing the full sdl URI, so we guess it
            //  http://edit.africamuseum.be/synthesys/www/v1/sld/
            //  http://edit.br.fgov.be/synthesys/www/v1/sld/
            sld =  mapserverBaseUrl + "/synthesys/www/v1/sld/" + sld;

          }

          layer.params.SLD = sld;
          map.addLayers([layer]);

        } else {
          // it is a response from the areas.php
          for ( var i in mapResponseObj.layers) {
          var layerData = mapResponseObj.layers[i];

            layer = new OpenLayers.Layer.WMS(
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

//        createLegendLayer();
//        // 2. create the Legend Layer
        //TODO createLegendLayer as inner fiinction seems like an error
//        var createLegendLayer = function(){
  //
  //
//          var legendLayerOptions={
//              maxResolution: '.$maxRes.',
//              maxExtent: new OpenLayers.Bounds(0, 0, w, h)
//          };
  //
//          var legendLayer = new OpenLayers.Layer.Image(
//              'Legend',
//              legendSrcUrl,
//              new OpenLayers.Bounds(0, 0, w, h),
//              new OpenLayers.Size(w, h),
//              imageLayerOptions);
//        };
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
       for(var i = 0; i <  baseLayerNames.length; i++) {
         baseLayers[i] = window.CdmOpenLayers.getLayerByName(baseLayerNames[i]);
         if(baseLayerNames[i] == defaultBaseLayerName){
           defaultBaseLayer = baseLayers[i];
         }
       }
     };

     /**
      * returns the intersction of the bounds b1 and b2.
      * The b1 and b2 do not intersect b1 will be returned.
      *
      * @param OpenLayers.Bounds b1
      * @param OpenLayers.Bounds b2
      *
      * @return the bounds of the intersection between both rectangles
      */
     var intersectionOfBounds = function (b1, b2){

         if(b1.intersectsBounds(b2)){

             var left = Math.max(b1.left, b2.left);
             var bottom = Math.max(b1.bottom, b2.bottom);
             var right = Math.min(b1.right, b2.right);
             var top = Math.min(b1.top, b2.top);

             return new OpenLayers.Bounds(left, bottom, right, top);

         } else {
             return b1;
         }
     };

     /**
      *
      * @param OpenLayers.Bounds b
      * @param float aspectRatio width/height
      *
      * @return the bounds croped to the given aspectRatio
      */
     var cropBoundsToAspectRatio = function (b, aspectRatio){

         var cropedB = b.clone();

         if(aspectRatio == 1){
             return cropedB;
         }

         /*
          * LonLat:
          *   lon {Float} The x-axis coodinate in map units
          *   lat {Float} The y-axis coordinate in map units
          */
         var center = cropedB.getCenterLonLat();
         if(aspectRatio < 1){
             var dist = (b.getHeight() / 2) * aspectRatio;
             cropedB.top = center.lat + dist;
             cropedB.cropedBottom = center.lat - dist;
         } else if(aspectRatio > 1){
             var dist = (b.getWidth() / 2) / aspectRatio;
             cropedB.left = center.lon - dist;
             cropedB.right = center.lon + dist;
         }
         return cropedB;
     };

     /**
      * returns the zoom to bounds.
      *
      * @param bboxString
      *     a string representation of the bounds in degree
      * @param layer
      *     the Openlayers.Layer
      *
      * @return the bboxstring projected onto the layer and intersected with the maximum extend of the layer
      */
     var zoomToBoundsFor = function(bboxString, layer){
         var zoomToBounds;
         if(typeof bboxString == 'string' && bboxString.length > 6) {
             zoomToBounds = OpenLayers.Bounds.fromString(bboxString);
         } else {
             zoomToBounds = new OpenLayers.Bounds(-180, -90, 180, 90);
         }
         // transform bounding box given in degree values to the projection of the base layer
         zoomToBounds.transform(CdmOpenLayers.projections.epsg_4326, layer.projection);

         zoomToBounds = intersectionOfBounds(layer.maxExtent, zoomToBounds);

         return zoomToBounds;
     };



     /**
      * returns the version number contained in the version string:
      *   v1.1 --> 1.1
      *   v1.2_dev --> 1.2
      */
     var mapServerVersionNumber = function() {
       var pattern = /v([\d\.]+).*$/;
       var result;
       if (result = mapserverVersion.match(pattern) !== null) {
         return result[0];
       } else {
         return null;
       }
     };

  }; // end of CdmOpenLayers.Map
})();






