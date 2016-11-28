//see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

/**
 * Expected dom structure:
 *  '<div class="ahah-content" rel="'.$cdm_proxy_url.'"><span class="loading">Loading ....</span></div>';
 */
(function($, document, window, undefined) {

    $.fn.cdm_openlayers_map = function(mapserverBaseUrl, mapserverVersion, options) {

      var opts = $.extend({},$.fn.cdm_openlayers_map.defaults, options);

      // sanitize invalid opts.boundingBox
      if(opts.boundingBox &&  !( typeof opts.boundingBox  == 'string' && opts.boundingBox .length > 6)) {
        opts.boundingBox = null;
      }

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
    // These are bounds in the epsg_4326 projection in degree
    boundingBox: null,
    aspectRatio: 2, // w/h
    showLayerSwitcher: false,
    baseLayerNames: ["mapproxy_vmap0"],
    defaultBaseLayerName: 'mapproxy_vmap0',
    maxZoom: 4,
    minZoom: 0,
    debug: true,
    /**
     * allows the map to display parts of the layers which are outside
     * the maxExtent if the aspect ratio of the map and of the baselayer
     * are not equal
     */
    displayOutsideMaxExtent: false,
    //  customWMSBaseLayerData: {
    //  name: "Euro+Med",
    //  url: "http://edit.africamuseum.be/geoserver/topp/wms",
    //  params: {layers: "topp:em_tiny_jan2003", format:"image/png", tiled: true},
    //  projection: "EPSG:7777777",
    //  maxExtent: "-1600072.75, -1800000, 5600000, 5850093",
    //  units: 'm'
    //  }
    customWMSBaseLayerData: {
        name: null,
        url: null,
        params: null,
        projection: null,
        max_extent: null,
        units: null
    },
    /**
     * when true the map is made resizable by adding the jQueryUI widget resizable
     * to the map container. This feature requires that the jQueryUI is loaded
     */
    resizable: false
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

        // EPSG:3857 from http://spatialreference.org/ref/sr-org/6864/proj4/
        // OpenStreetMap etc
        Proj4js.defs["EPSG:3857"] = '+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +a=6378137 +b=6378137 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs';

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
            getMap: function (){},
            getLayerByName: function(layerName){} // initially empty function, will be populated by openlayers_layers.js
        };

    })(); // end of namespace definition for CdmOpenLayers

    /**
     * The CdmOpenLayers.Map constructor
     * @param mapElement
     * @param mapserverBaseUrl
     * @param mapserverVersion
     * @param opts
     * @returns
     */
    window.CdmOpenLayers.Map = function(mapElement, mapserverBaseUrl, mapserverVersion, opts){

      var mapServicePath = '/edit_wp5';

      // firebug console stub (avoids errors if firebug is not active)
      if(typeof console === "undefined") {
          console = { log: function() { } };
      }

      // sanitize given options
      try {
          opts.customWMSBaseLayerData.max_extent = OpenLayers.Bounds.fromString(opts.customWMSBaseLayerData.max_extent);
      } catch(e){
          opts.customWMSBaseLayerData.max_extent = null;
      }


      var legendImgSrc = null;

      var map = null;

      var infoElement = null;

      var baseLayers = [];

      var defaultBaseLayer = null;

      /**
       * Default bounding box for map viewport in the projection of the base layer.
       * as defined by the user, can be null.
       *
       * These are bounds in the epsg_4326 projection, and will be transformed to the baselayer projection.
       *
       * @type string
       */
      var defaultBaseLayerBoundingBox = "-180,-90,180,90";

      /**
       * bounding box for map viewport as defined by the user, can be null.
       *
       * These are bounds in the projection of the base layer.
       *
       * @type string
       */
      var boundingBox = null;

      /**
       * Bounds for the view port calculated from the data layer responses.
       * These are either calculated by the minimum bounding box which
       * encloses the data in the data layers, or it is equal to the
       * boundingBox as defined by the user.
       *
       * These are bounds in the projection of the base layer.
       *
       * @see boundingBox
       *
       * @type OpenLayers.Bounds
       */
      var dataBounds = null;

      /**
       * Final value for the view port, calculated from the other bounds.
       *
       * These are bounds in the projection of the base layer.
       *
       * @type OpenLayers.Bounds
       */
      var zoomToBounds = null;

      var zoomToClosestLevel = true;

      var LAYER_DATA_CNT = 0;

      /* this is usually the <div id="openlayers"> element */
      var mapContainerElement = mapElement.parent();

      var defaultControls = [
         new OpenLayers.Control.PanZoom(),
         new OpenLayers.Control.Navigation(
           {
             zoomWheelEnabled: false,
             handleRightClicks:true,
             zoomBoxKeyMask: OpenLayers.Handler.MOD_CTRL
           }
         )
      ];


      var layerByNameMap = {
              tdwg1: 'topp:tdwg_level_1',
              tdwg2: 'topp:tdwg_level_2',
              tdwg3: 'topp:tdwg_level_3',
              tdwg4: 'topp:tdwg_level_4'
      };

      if(opts.resizable == true) {
        // resizable requires jQueryUI to  be loaded!!!
        mapContainerElement.resizable({
          resize: function( event, ui ) {
            map.updateSize();
            //   this.printInfo();
          }
        });
      }

        /**
         *
         */
        this.init = function(){ // public function

          // set the height of the container element
          adjustHeight();

          // register for resize events to be able to adjust the map aspect ratio and legend position
          jQuery( window ).resize(function() {
            adjustHeight();
            adjustLegendAsElementPosition();
          });

          createBaseLayers(opts.baseLayerNames, opts.defaultBaseLayerName, opts.customWMSBaseLayerData);

          initMap();

          // now it is
          if(opts.boundingBox){
            boundingBox = OpenLayers.Bounds.fromString(opts.boundingBox);
            boundingBox.transform(CdmOpenLayers.projections.epsg_4326, map.getProjectionObject());
          }

          // -- Distribution Layer --
          var mapServiceRequest;
          var distributionQuery = mapElement.attr('distributionQuery');

          if(distributionQuery !== undefined){
            distributionQuery = mergeQueryStrings(distributionQuery, '&recalculate=false');
            if(typeof legendPosition == 'number'){
              distributionQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + opts.legendPosition);
            }
            if(opts.boundingBox){
              distributionQuery = mergeQueryStrings(distributionQuery, 'bbox=' + boundingBox);
            }

            distributionQuery = mergeQueryStrings(distributionQuery, 'callback=?');
            var legendFormatQuery = mapElement.attr('legendFormatQuery');
            if(legendFormatQuery !== undefined){
              legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
            }

            mapServiceRequest = mapserverBaseUrl + mapServicePath + '/' + mapserverVersion + '/rest_gen.php?' + distributionQuery;

            LAYER_DATA_CNT++;
            jQuery.ajax({
              url: mapServiceRequest,
              dataType: "jsonp",
              success: function(data){
                  var layers = createDataLayer(data, "AREA");
                  addLayers(layers);
                  layerDataLoaded();
              }
            });
          }

          // -- Occurrence Layer --
          var occurrenceQuery = mapElement.attr('occurrenceQuery');
          if(occurrenceQuery !== undefined){
            occurrenceQuery = mergeQueryStrings(occurrenceQuery, '&recalculate=false');
//              if(typeof legendPosition == 'number'){
//              occurrenceQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + opts.legendPosition);
//              }


            occurrenceQuery = mergeQueryStrings(occurrenceQuery, 'callback=?');
//              var legendFormatQuery = mapElement.attr('legendFormatQuery');
//              if(legendFormatQuery !== undefined){
//              legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
//              }
            if(opts.boundingBox){
              occurrenceQuery = mergeQueryStrings(occurrenceQuery, 'bbox=' + boundingBox);
            }

            mapServiceRequest = mapserverBaseUrl + mapServicePath + '/' + mapserverVersion + '/rest_gen.php?' + occurrenceQuery;

            LAYER_DATA_CNT++;
            jQuery.ajax({
              url: mapServiceRequest,
              dataType: "jsonp",
              success: function(data){
                  var layers = createDataLayer(data, "POINT");
                  addLayers(layers);
                  layerDataLoaded();
              }
            });
          }

            if(LAYER_DATA_CNT == 0) {
              // a map only with base layer
              initPostDataLoaded();
            }

        };

        var layerDataLoaded = function() {
          LAYER_DATA_CNT--;
          if(LAYER_DATA_CNT == 0){
            initPostDataLoaded();
          }
        };

        var initPostDataLoaded = function () {
          // all layers prepared, make the visible
          map.layers.forEach(function(layer){

            // hack for cuba
            if(layer.name == "flora_cuba_2016_regions"){
              map.setLayerZIndex(layer, 5);
            }
            if(layer.name == "flora_cuba_2016_provinces"){
              map.setLayerZIndex(layer, 6);
            }
            if(layer.name == "flora_cuba_2016_world"){
              map.setLayerZIndex(layer, 4);
            }

            layer.setVisibility(true);
          });

          // zoom to the zoomToBounds
          log(" > starting zoomToExtend " + zoomToBounds, true);
          map.zoomToExtent(zoomToBounds, zoomToClosestLevel);

          if(map.getZoom() > opts.maxZoom){
            map.zoomTo(opts.maxZoom);
          } else if(map.getZoom() < opts.minZoom){
            map.zoomTo(opts.minZoom);
          }

          log(" > zoomToExtend done", true);
        };

        var getHeight = function(){
          return mapContainerElement.width() / opts.aspectRatio;
        };

        var getWidth = function(){
          return mapContainerElement.width();
        };

        var adjustHeight = function() {
          mapContainerElement.css("height", getHeight());
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
         * as set in the options (opts.infoElement)
         * public function
         *
         */
        this.printInfo = function(){


            var mapExtendDegree = null;
            if(map.getExtent() != null){
              // If the baselayer is not yet set, getExtent() returns null.
              mapExtendDegree = map.getExtent().clone();
              mapExtendDegree.transform(map.baseLayer.projection, CdmOpenLayers.projections.epsg_4326);
            }

            var info = "<dl>";
            info += "<dt>zoom:<dt><dd>" + map.getZoom() + "</dd>";
            if(opts.debug){
                info += "<dt>map resolution:<dt><dd>" + map.getResolution() + "</dd>";
                info += "<dt>map max resolution:<dt><dd>" + map.getMaxResolution() + "</dd>";
                info += "<dt>map scale:<dt><dd>" + map.getScale() + "</dd>";
                info += "<dt>map width, height:<dt><dd>" + mapContainerElement.width() +  ", " + mapContainerElement.height() + "</dd>";
                info += "<dt>map aspect ratio:<dt><dd>" + mapContainerElement.width() / mapContainerElement.height() + "</dd>";
                if(map.getExtent() != null){
                  info += "<dt>map extent bbox:<dt><dd class=\"map-extent-bbox\">" + map.getExtent().toBBOX() + ", <strong>degree:</strong> <span class=\"degree-value\">" + mapExtendDegree.toBBOX() + "</span></dd>";
                  info += "<dt>map maxExtent bbox:<dt><dd>" + map.getMaxExtent().toBBOX() + "</dd>";
                  info += "<dt>baselayer extent bbox:<dt><dd class=\"baselayer-extent-bbox\">" +  map.baseLayer.getExtent().toBBOX() + ", <strong>degree:</strong> <span class=\"degree-value\">"
                    + map.baseLayer.getExtent().clone().transform(map.baseLayer.projection, CdmOpenLayers.projections.epsg_4326) + "</span></dd>"
                  info += "<dt>baselayer projection:<dt><dd>" + map.baseLayer.projection.getCode() + "</dd>";
                }
            } else {
                info += "<dt>bbox:<dt><dd>" + (mapExtendDegree != null ? mapExtendDegree.toBBOX() : 'NULL') + "</dd>";
            }
            info += "</dl>";

            if(infoElement == null){
                infoElement = jQuery('<div class="map_info"></div>');
                mapElement.parent().after(infoElement);
            }
            infoElement.html(info);
        };

        /**
         * Initialize the Openlayers Map with the base layer
         */
        var initMap = function(){

          if(opts.showLayerSwitcher === true){
              defaultControls.push(new OpenLayers.Control.LayerSwitcher({'ascending':false}));
          }

          // defaultControls.unshift(layerLoadingControl()); // as first control, needs to be below all others!

//          var maxExtentByAspectRatio = cropBoundsToAspectRatio(defaultBaseLayer.maxExtent, getWidth/getHeight);
          var maxResolution = null;
          // gmaps has no maxExtent at this point, need to check for null
          if(defaultBaseLayer.maxExtent != null){
              maxResolution = Math[(opts.displayOutsideMaxExtent ? 'max' : 'min')](
                      defaultBaseLayer.maxExtent.getWidth() / getWidth(),
                      defaultBaseLayer.maxExtent.getHeight() / getHeight()
              );
          }
          console.log("mapOptions.maxResolution: " + maxResolution);
          console.log("mapOptions.restrictedExtent: " + defaultBaseLayer.maxExtent);

          map = new OpenLayers.Map(
            mapElement.attr('id'),
            {
              // defines the map ui elements and interaction features
              controls: defaultControls,

              // maxResolution determines the lowest zoom level and thus places the map
              // in its maximum extent into the available view port so that no additinal
              // gutter is visible and no parts of the map are hidden
              // see http://trac.osgeo.org/openlayers/wiki/SettingZoomLevels
              // IMPORTANT!!!
              // the maxResulution set here will be overwritten if the baselayers maxResolution
              // it is set
              maxResolution: maxResolution,

              // setting restrictedExtent the the maxExtent prevents from panning the
              // map out of its bounds
              restrictedExtent: defaultBaseLayer.maxExtent,
//                      maxExtent: defaultBaseLayer.maxExtent,

              // Setting the map.fractionalZoom property to true allows zooming to an arbitrary level
              // (between the min and max resolutions).
              // fractional tiles are not supported by XYZ layers like OSM so this option would
              // break the tile retrieval for OSM (e.g.: tile for fractional zoom level
              // 1.2933333333333332 = http://b.tile.openstreetmap.org/1.2933333333333332/1/0.png)
              fractionalZoom: defaultBaseLayer.CLASS_NAME != "OpenLayers.Layer.OSM" && defaultBaseLayer.CLASS_NAME != "OpenLayers.Layer.XYZ",

              eventListeners: opts.eventListeners,
              // creating the map with a null theme, since we include the stylesheet directly in the page
              theme: null

            }
          );

          //add the base layers

          addLayers(baseLayers);
          map.setBaseLayer(defaultBaseLayer);

          // calculate the bounds to zoom to
          zoomToBounds = zoomToBoundsFor(opts.boundingBox ? opts.boundingBox : defaultBaseLayerBoundingBox, defaultBaseLayer);
          zoomToBounds = cropBoundsToAspectRatio(zoomToBounds, map.getSize().w / map.getSize().h);
          console.log("baselayer zoomToBounds: " + zoomToBounds);

        };

        var addLayers = function(layers){

          layers.forEach(function(layer){
            // layer.setVisibility(false);
          });

          map.addLayers(layers);
        };

        /**
         * add a distribution or occurrence layer
         *
         * @param mapResponseObj
         *   The reponse object returned by the edit map service
         * @param dataType
         *   either "AREA" or "POINT"
         */
        var createDataLayer = function(mapResponseObj, dataType){

            console.log("creating data layer of type " + dataType);

            var dataLayerOptions = {
                    isBaseLayer: false,
                    displayInLayerSwitcher: true,
                    maxExtent: map.maxExtent.clone().transform(new OpenLayers.Projection("EPSG:4326"), map.baseLayer.projection),
                    displayOutsideMaxExtent: true
            };

            var layers = [];
            // add additional layers, get them from the mapResponseObj
            if(mapResponseObj !== undefined){
                if(dataType == "POINT" && mapResponseObj.points_sld !== undefined){
                  var pointLayer;
                  // it is a response for an point map
                  var geoserverUri;
                  if(mapResponseObj.geoserver) {
                      geoserverUri = mapResponseObj.geoserver;
                  } else {
                      // it is an old service which is not providing the corresponding geoserver URI, so we guess it
                      geoserverUri = mapserverBaseUrl + "/geoserver/wms";
                  }

                  //TODO points_sld should be renamed to sld in response + fill path to sld should be given
                  pointLayer = new OpenLayers.Layer.WMS(
                          'points',
                          geoserverUri,
                          {
                              layers: 'topp:rest_points',
                              transparent:"true",
                              format:"image/png"
                          },
                          dataLayerOptions
                  );

                  var sld = mapResponseObj.points_sld;
                  if(sld.indexOf("http://") !== 0){
                      // it is an old servive which is not providing the full sdl URI, so we guess it
                      //  http://edit.africamuseum.be/synthesys/www/v1/sld/
                      //  http://edit.br.fgov.be/synthesys/www/v1/sld/
                      sld =  mapserverBaseUrl + "/synthesys/www/v1/sld/" + sld;
                  }
                  pointLayer.params.SLD = sld;

                  layers.push(pointLayer);
                } else {
                    // it is a response from for a distribution map
                    console.log("start with adding distribution layers :");
                    for ( var i in mapResponseObj.layers) {
                        var layerData = mapResponseObj.layers[i];

                        console.log(" " + i +" -> " + layerData.tdwg);
                        var layer = new OpenLayers.Layer.WMS(
                                layerData.tdwg,
                                mapResponseObj.geoserver + "/wms",
                                {
                                    layers: layerByNameMap[layerData.tdwg],
                                    transparent:"true",
                                    format:"image/png"
                                },
                                dataLayerOptions
                                );
                        layer.params.SLD = layerData.sld;
                        layer.setOpacity(opts.distributionOpacity);

                        layers.push(layer);
                    }
                }

                if(layers.length > 0) {
                  // calculate zoomBounds using the first layer
                  if(mapResponseObj.bbox !== undefined){
                    // mapResponseObj.bbox are bounds for the projection of the specific layer
                    var newBounds =  OpenLayers.Bounds.fromString( mapResponseObj.bbox );
                    newBounds.transform(layers[0].projection, map.getProjectionObject());
                    if(dataBounds !== null){
                      dataBounds.extend(newBounds);
                    } else if(newBounds !== undefined){
                      dataBounds = newBounds;
                    }

                    zoomToBounds = dataBounds;
                    console.log("data layer zoomToBounds: " + zoomToBounds);
                    zoomToClosestLevel = false;
                  }
                }



                if(legendImgSrc != null && opts.legendPosition !== undefined && mapResponseObj.legend !== undefined){
                    var legendSrcUrl = mapResponseObj.geoserver + legendImgSrc + mapResponseObj.legend;
                    addLegendAsElement(legendSrcUrl);
                    //addLegendAsLayer(legendSrcUrl, map);
                }

                return layers;
            }

        };

        /**
         *
         */
        var addLegendAsElement= function(legendSrcUrl){

            var legendElement = jQuery('<div class="openlayers_legend"></div>');
            var legendImage = jQuery('<img src="' + legendSrcUrl + '"/>');
            legendElement
                .css('opacity', opts.legendOpacity)
                .css('position', 'relative')
                .css('z-index', '1002')
                .css('top', -mapElement.height());
            legendImage.load(function () {
                jQuery(this).parent()
                    .css('left', getWidth() - jQuery(this).width())
                    .width(jQuery(this).width());
                // reset height to original value
                adjustHeight();
            });
            legendElement.html(legendImage);
            mapElement.after(legendElement);
        };

         var adjustLegendAsElementPosition = function (){
           var legendContainer = mapContainerElement.children('.openlayers_legend');
           var legendImage = legendContainer.children('img');
           legendContainer.css('top', -mapElement.height())
             .css('left', getWidth() - legendImage.width());
         };


        var addLegendAsLayer= function(legendSrcUrl, map){
            var w, h;

            // 1. download image to find height and width
            mapElement.after('<div class="openlayers_legend"><img src="' + legendSrcUrl + '"></div>');
            mapElement.next('.openlayers_legend').css('display', 'none').css('opacity', opts.legendOpacity).find('img').load(function () {

                w = mapElement.next('.openlayers_legend').find('img').width();
                h = mapElement.next('.openlayers_legend').find('img').height();
                mapElement.next('.openlayers_legend').remove();

//              createLegendLayer();
//              // 2. create the Legend Layer
                //TODO createLegendLayer as inner function seems like an error
//              var createLegendLayer = function(){
                //
                //
//              var legendLayerOptions={
//              maxResolution: '.$maxRes.',
//              maxExtent: new OpenLayers.Bounds(0, 0, w, h)
//              };
                //
//              var legendLayer = new OpenLayers.Layer.Image(
//              'Legend',
//              legendSrcUrl,
//              new OpenLayers.Bounds(0, 0, w, h),
//              new OpenLayers.Size(w, h),
//              imageLayerOptions);
//              };
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
        var createBaseLayers = function( baseLayerNames, defaultBaseLayerName, customWMSBaseLayerData){

            for(var i = 0; i <  baseLayerNames.length; i++) {
                // create the layer
                if (baseLayerNames[i] == "custom_wms_base_layer_1"){
                    baseLayers[i] = createWMSBaseLayer(
                            customWMSBaseLayerData.name,
                            customWMSBaseLayerData.url,
                            customWMSBaseLayerData.params,
                            customWMSBaseLayerData.projection,
                            customWMSBaseLayerData.proj4js_def,
                            customWMSBaseLayerData.units,
                            customWMSBaseLayerData.max_extent
                    );
                } else {
                    baseLayers[i] = window.CdmOpenLayers.getLayerByName(baseLayerNames[i]);
                }
                // set default baselayer
                if(baseLayerNames[i] == defaultBaseLayerName){
                    defaultBaseLayer = baseLayers[i];
                }

            }
        };

        /**
         * returns the intersection of the bounds b1 and b2.
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
         * @return the bounds cropped to the given aspectRatio
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



      /**
       * returns the zoom to bounds.
       *
       * NOTE: only used for the base layer
       *
       * @param bboxString
       *     a string representation of the bounds in degree for epsg_4326
       * @param layer
       *     the Openlayers.Layer
       *
       * @return the bboxstring projected onto the layer and intersected with the maximum extent of the layer
       */
      var zoomToBoundsFor = function(bboxString, layer){
        var zoomToBounds;
        if(bboxString) {
          zoomToBounds = OpenLayers.Bounds.fromString(bboxString);
          // transform bounding box given in degree values to the projection of the base layer
          zoomToBounds.transform(CdmOpenLayers.projections.epsg_4326, layer.projection);
        } else if(layer.maxExtent) {
          zoomToBounds = layer.maxExtent;
          // no need to transform since the bounds are obtained from the layer
        } else {
          zoomToBounds = new OpenLayers.Bounds(-180, -90, 180, 90);
          // transform bounding box given in degree values to the projection of the base layer
          zoomToBounds.transform(CdmOpenLayers.projections.epsg_4326, layer.projection);
        }

        zoomToBounds = intersectionOfBounds(layer.maxExtent, zoomToBounds);

        return zoomToBounds;
      };

        var log = function(message, addTimeStamp){
          var timestamp = '';
          if(addTimeStamp == true){
            var time = new Date();
            timestamp = time.getSeconds() + '.' + time.getMilliseconds() + 's';
          }
          console.log(timestamp + message);
        };

        /**
         * Creates a WMS Base layer
         * @param String name
         *     A name for the layer
         * @param String url
         *     Base url for the WMS (e.g.  http://wms.jpl.nasa.gov/wms.cgi)
         * @param Object params
         *     An object with key/value pairs representing the GetMap query string parameters and parameter values.
         * @param Object projection
         *    A OpenLayers.Projection object
         */
        var createWMSBaseLayer= function(name, url, params, projection, proj4js_def, units, maxExtent){

            console.log("creating WMSBaseLayer");

            if(projection && proj4js_def){
                // in case projection has been defined for the layer and if there is also
                // a Proj4js.defs, add it!
                Proj4js.defs[projection] = proj4js_def;
            }

            if(maxExtent == null){
                maxExtent = CdmOpenLayers.mapExtends.epsg_4326.clone();
                maxExtent.transform(CdmOpenLayers.projections.epsg_4326, projection);
            }

          wmsLayer = new OpenLayers.Layer.WMS(
            name,
            url,
            params,
            {
              maxExtent: maxExtent,
              projection: projection,
              units: units,
              isBaseLayer: true,
              displayInLayerSwitcher: true
            }
          );

          if(wmsLayer == null){
            console.log("Error creating WMSBaseLayer");
          }

          return  wmsLayer;
        };

        var layerLoadingControl = function() {

          var control = new OpenLayers.Control();

          OpenLayers.Util.extend(control, {

            LAYERS_LOADING: 0,

            updateState: function () {
              if(this.div != null){
                if (this.LAYERS_LOADING > 0) {
                  this.div.style.display = "block";
                } else {
                  this.div.style.display = "none";
                }
              }
            },

            updateSize: function () {
              this.div.style.width = this.map.size.w + "px";
              this.div.style.height = this.map.size.h  + "px";
              this.div.style.textAlign = "center";
              this.div.style.lineHeight = this.map.size.h  + "px";
            },

            counterIncrease: function (layer) {
              this.control.LAYERS_LOADING++;
              log(' > loading start : ' + this.layer.name + ' ' + this.control.LAYERS_LOADING, true);
              this.control.updateState();
            },

            counterDecrease: function (layer) {
              this.control.LAYERS_LOADING--;
              log(' > loading end : ' + this.layer.name + ' ' + this.control.LAYERS_LOADING, true);
              this.control.updateState();
            },

            draw: function () {

              // call the default draw function to initialize this.div
              OpenLayers.Control.prototype.draw.apply(this, arguments);

              this.map.events.register('updatesize', this, function(e){
                  this.updateSize();
                }
              );

              var loadingIcon = document.createElement("i");
              var fa_class = document.createAttribute("class");
              // fa-circle-o-notch fa-spin
              // fa-spinner fa-pulse
              // fa-refresh
              fa_class.value = "fa fa-refresh fa-spin fa-5x";
              loadingIcon.attributes.setNamedItem(fa_class);

              this.updateSize();

              this.div.appendChild(loadingIcon);

              this.registerEvents();

              return this.div;
            },

            registerEvents: function() {

              this.map.events.register('preaddlayer', this, function(e){
                console.log(" > preaddlayer " + e.layer.name);
                e.layer.events.register('loadstart', {control: this, layer: e.layer}, this.counterIncrease);
                e.layer.events.register('loadend', {control: this, layer: e.layer}, this.counterDecrease);
              });
            }

          });

          return control;
        }

    }; // end of CdmOpenLayers.Map
})();









