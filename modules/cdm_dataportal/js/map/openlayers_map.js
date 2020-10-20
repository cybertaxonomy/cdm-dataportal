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
          this.cdmOpenlayersMap.create();
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
    baseLayerNames: ["open_topomap"],
    defaultBaseLayerName: 'open_topomap',
    maxZoom: 15,
    minZoom: 0,
    // hide the map when the data layer has no features
    hideEmptyMap: true,
    debug: true,
    layerLoadingTimeout: 1000, // ms
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
        proj4js_def: null,
        max_extent: null,
        units: null,
        untiled: null
    },
    wmsOverlayLayerData: {
      name: null,
      url: null,
      params: null,
      untiled: null
    },
    /**
     * when true the map is made resizable by adding the jQueryUI widget resizable
     * to the map container. This feature requires that the jQueryUI is loaded
     */
    resizable: false,
    wfsRootUrl: 'http://edit.africamuseum.be/geoserver/topp/ows',
    specimenPageBaseUrl: '/cdm_dataportal/occurrence/',
      specimenLinkText: 'Open unit'
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
        Proj4js.defs["EPSG:7777777"] = '+proj=lcc +lat_1=42 +lat_2=56 +lat_0=35 +lon_0=24 +x_0=3000000 +y_0=100000 +ellps=intl +towgs84=-87,-98,-121,0,0,0,0 +units=m +no_defs';

        var projections = {
                epsg_4326: new OpenLayers.Projection("EPSG:4326"),
                epsg_900913: new OpenLayers.Projection("EPSG:900913"),
                epsg_3857:  new OpenLayers.Projection("EPSG:3857"),
                epsg_7777777:  new OpenLayers.Projection("EPSG:7777777")
        };
        var mapExtends = {
                epsg_4326: new OpenLayers.Bounds(-180, -90, 180, 90),
                //  Spherical Mercator epsg_900913 is not supporting the whole marble
                epsg_900913: new OpenLayers.Bounds(-179, -85, 179, 85),
                //  Spherical Mercator
                epsg_3857: new OpenLayers.Bounds(-179, -85, 179, 85)
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
       * The top most layer which will be places above all data layers
       *
       * @type {null}
       */
      var wmsOverlay = null;

        /**
         * The Control element to handle clicks on features in KML layers
         * @type {null}
         */
      var kmlSelectControl = null;

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

        /**
         * This as been introduced for #5683 with value TRUE,
         * but can cause feature to tbe hidden in KML layers.
         * Therefore this is now set to false.
         *
         * @type {boolean}
         */
      var zoomToClosestLevel = false;

      var LAYER_DATA_CNT = 0;

      /* this is usually the <div id="openlayers"> element */
      var mapContainerElement = mapElement.parent();

      var errorMessageCtl;
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

      /**
       * Known projections by layer name. This map helps avoid requesting the server for the
       * projection. See readProjection()
       */
      var layerProjections = {
        'topp:tdwg_level_1': CdmOpenLayers.projections.epsg_4326,
        'topp:tdwg_level_2': CdmOpenLayers.projections.epsg_4326,
        'topp:tdwg_level_3': CdmOpenLayers.projections.epsg_4326,
        'topp:tdwg_level_4': CdmOpenLayers.projections.epsg_4326,
        'topp:phytogeographical_regions_of_greece': CdmOpenLayers.projections.epsg_4326,
        'topp:euromed_2013': CdmOpenLayers.projections.epsg_7777777,
        'topp:flora_cuba_2016': CdmOpenLayers.projections.epsg_4326
      };

      if(opts.resizable === true) {
        // resizable requires jQueryUI to  be loaded!!!
        mapContainerElement.resizable({
          resize: function( event, ui ) {
            map.updateSize();
            //   this.printInfo();
          }
        });
      }

        /**
         * Removes the map and the parent container from the
         * DOM and destroys the OpenLayers map object
         */
        function removeMap() {
            //  if you are using an application which removes a container
            //  of the map from the DOM, you need to ensure that you destroy the map before this happens;
            map.destroy;
            jQuery(map.div).parent().remove();
        }

        function reportAjaxError(textStatus, requestUrl, errorThrown) {
            if (!textStatus) {
                textStatus = "error";
            }
            var errorMessage = errorThrown != undefined ? errorThrown : 'unspecified error';
            log(textStatus + " requesting  " + requestUrl + " failed due to " + errorMessage, true);
            errorMessageCtl.show();
            errorMessageCtl.add(textStatus + ":" + errorThrown);
        }

        /**
         *
         */
        this.create = function(){ // public function

          // set the height of the container element
          adjustHeight();

          // register for resize events to be able to adjust the map aspect ratio and legend position
          jQuery( window ).resize(function() {
            adjustHeight();
            adjustLegendAsElementPosition();
          });

          createBaseLayers(opts.baseLayerNames, opts.defaultBaseLayerName, opts.customWMSBaseLayerData);

          initMap();

          var boundingBoxEPSG4326 = null;
          if(opts.boundingBox){
            boundingBox = OpenLayers.Bounds.fromString(opts.boundingBox);
            boundingBoxEPSG4326 = boundingBox.transform(wmsBaseLayer.projection, projections.epsg_4326);
          }

          // -- Distribution Layer --
          var mapServiceRequest;
          var distributionQuery = mapElement.attr('data-distributionQuery');

          if(distributionQuery !== undefined){
            distributionQuery = mergeQueryStrings(distributionQuery, '&recalculate=false');
            if(typeof legendPosition === 'number'){
              distributionQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + opts.legendPosition);
            }
            if(boundingBoxEPSG4326){
              distributionQuery = mergeQueryStrings(distributionQuery, 'bbox=' + boundingBoxEPSG4326);
            }

            // distributionQuery = mergeQueryStrings(distributionQuery, 'callback=?');
            var legendFormatQuery = mapElement.attr('data-legendFormatQuery');
            if(legendFormatQuery !== undefined){
              legendImgSrc = mergeQueryStrings('/wms?SERVICE=WMS&VERSION=1.1.1&REQUEST=GetLegendGraphic ', legendFormatQuery);
            }

            mapServiceRequest = mapserverBaseUrl + mapServicePath + '/' + mapserverVersion + '/rest_gen.php?' + distributionQuery;

            LAYER_DATA_CNT++;
            jQuery.ajax({
              url: mapServiceRequest,
              dataType: "json",
                // timeout: layerLoadingTimeout,
              success: function(data){
                  var layers = createDataLayer(data, "AREA");
                  addLayers(layers);
                // layerDataLoaded(); will be called after reading the projection from the WFS
                // for the data layer, see readProjection()
              },
              error: function(jqXHR, textStatus, errorThrown){
                  reportAjaxError("Distribution Layer: " +textStatus, mapServiceRequest, errorThrown);
              }
            });
          }

          // -- Occurrence Layer --
          var occurrenceQuery = mapElement.attr('data-occurrenceQuery');
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
            if(boundingBoxEPSG4326){
              occurrenceQuery = mergeQueryStrings(occurrenceQuery, 'bbox=' + boundingBoxEPSG4326);
            }

            mapServiceRequest = mapserverBaseUrl + mapServicePath + '/' + mapserverVersion + '/rest_gen.php?' + occurrenceQuery;

            LAYER_DATA_CNT++;
            jQuery.ajax({
              url: mapServiceRequest,
              dataType: "json",
              success: function(data){
                  var layers = createDataLayer(data, "POINT");
                  addLayers(layers);
                  // layerDataLoaded(); will be called after reading the projection from the WFS for the data layer
              },
                error: function(jqXHR, textStatus, errorThrown){
                    reportAjaxError("Occurrence Layer: " + textStatus, mapServiceRequest, errorThrown);
                }
            });
          }

            // -- KML Layer --
            var kmlRequestUrl = mapElement.attr('data-kml-request-url');
            if(kmlRequestUrl !== undefined){

                LAYER_DATA_CNT++;
                var kmlLayer = new OpenLayers.Layer.Vector("KML", {
                    strategies: [new OpenLayers.Strategy.Fixed()],
                    protocol: new OpenLayers.Protocol.HTTP({
                        url: kmlRequestUrl,
                        format: new OpenLayers.Format.KML({
                            extractStyles: true,
                            extractAttributes: true
                            // maxDepth: 2
                        })
                    })
                });
                map.addLayer(kmlLayer);
                // create select control
                kmlSelectControl = new OpenLayers.Control.SelectFeature(kmlLayer);
                kmlLayer.events.on({
                    "featureselected": onKmlFeatureSelect,
                    "featureunselected": onKmlFeatureUnselect,
                    "reportError": true,
                    'loadend': function(event) {
                        if(opts.hideEmptyMap && kmlLayer.features.length == 0){
                            log("No feature in KML layer, removing map ...")
                            removeMap();
                        } else {
                            applyLayerZoomBounds(event);
                            disablePolygonFeatureClick(event);
                        }
                    }
                });
                map.addControl(kmlSelectControl);
                kmlSelectControl.activate();

                initPostDataLoaded();
            }

          if(LAYER_DATA_CNT === 0) {
            // a map only with base layer
            initPostDataLoaded();
          }

          // -- Overlay Layer --
          if(opts.wmsOverlayLayerData.params){
            overlay_layer_params = opts.wmsOverlayLayerData.params;
            overlay_layer_params.transparent=true;
            wmsOverlay = createWMSLayer(
              opts.wmsOverlayLayerData.name,
              opts.wmsOverlayLayerData.url,
              overlay_layer_params,
              null,
              null,
              null,
              null,
              opts.wmsOverlayLayerData.untiled
            );

            if(map.addLayer(wmsOverlay)){
              wmsOverlay.setVisibility(true);
              map.setLayerIndex(wmsOverlay, 100);
              log("Overlay wms added");
            } else {
              log("ERROR adding overlay wms layer")
            }
          }

          log("Map viewer creation complete.");

        };

      /**
       * Provides the layer name which can be used in WMS/WFS requests.
       * The layerData.tdwg field contains invalid layer names in case of
       * the tdwg layers. This function handles with this bug.
       *
       * @param layerData
       * @returns String
       *    the correct layer name
       */
        var fixLayerName = function(layerData){
         var wmsLayerName = layerByNameMap[layerData.tdwg];
         if(!wmsLayerName){
           wmsLayerName = "topp:" + layerData.tdwg;
         }
         return wmsLayerName;
        };

        var layerDataLoaded = function() {
          LAYER_DATA_CNT--;
          if(LAYER_DATA_CNT === 0){
            initPostDataLoaded();
          }
        };

        var initPostDataLoaded = function () {
          // all layers prepared, make them visible
          map.layers.forEach(function(layer){
            layer.setVisibility(true);
          });

          // zoom to the zoomToBounds
          log(" > starting zoomToExtend: " + zoomToBounds + ", zoomToClosestLevel: " + zoomToClosestLevel, true);
          map.zoomToExtent(zoomToBounds, zoomToClosestLevel);


          if(map.getZoom() > opts.maxZoom){
            map.zoomTo(opts.maxZoom);
          } else if(map.getZoom() < opts.minZoom){
            map.zoomTo(opts.minZoom);
          }

          // make sure the wmsOverlay is still on top
          if(wmsOverlay){
            map.setLayerIndex(wmsOverlay, 100);
          }

          log(" > zoomToExtend done", true);
        };

      /**
       * Returns  the projection of the defaultBaseLayer which is the
       * the projection to which all other layers and locations must be transformed.
       */
      var referenceProjection = function() {
        if(defaultBaseLayer){
          if(defaultBaseLayer.projection){
            return defaultBaseLayer.projection;
          } else if(defaultBaseLayer.sphericalMercator === true){
            return CdmOpenLayers.projections.epsg_900913;
          } else {
            log("Error - referenceProjection() defaultBaseLayer " + defaultBaseLayer.name + " misses projection information");
          }

        } else {
          log("Error - referenceProjection() defaultBaseLayer not set");
          return null;
        }
      };

      /**
       * Returns the maxExtent of the defaultBaseLayer.
       */
      var referenceMaxExtent = function() {
        if(defaultBaseLayer){
          return defaultBaseLayer.maxExtent;
        } else {
          log("Error - referenceMaxExtent() defaultBaseLayer not set");
          return null;
        }
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
                  info += "<dt>map extent bbox:<dt><dd class=\"map-extent-bbox\"><span class=\"layer-value\">" + map.getExtent().toBBOX() + "</span>, (in degree:<span class=\"degree-value\">" + mapExtendDegree.toBBOX() + "</span>)</dd>";
                  info += "<dt>map maxExtent bbox:<dt><dd>" + map.getMaxExtent().toBBOX() + "</dd>";
                  info += "<dt>baselayer extent bbox:<dt><dd class=\"baselayer-extent-bbox\"><span class=\"layer-value\">" +  map.baseLayer.getExtent().toBBOX() + "</span>, (in degree: <span class=\"degree-value\">"
                    + map.baseLayer.getExtent().clone().transform(map.baseLayer.projection, CdmOpenLayers.projections.epsg_4326) + "</span>)</dd>";
                  info += "<dt>baselayer projection:<dt><dd>" + map.baseLayer.projection.getCode() + "</dd>";
                }
            } else {
                info += "<dt>bbox:<dt><dd>" + (mapExtendDegree !== null ? mapExtendDegree.toBBOX() : 'NULL') + "</dd>";
            }
            info += "</dl>";

            if(infoElement === null){
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
          errorMessageCtl = errorMessageControl();
          errorMessageCtl.deactivate(); // initially inactive
          defaultControls.push(errorMessageCtl);

          defaultControls.unshift(layerLoadingControl()); // as first control, needs to be below all others!


//          var maxExtentByAspectRatio = cropBoundsToAspectRatio(defaultBaseLayer.maxExtent, getWidth/getHeight);
          var maxResolution = null;
          // gmaps has no maxExtent at this point, need to check for null
          if(referenceMaxExtent() !== null){
              maxResolution = Math[(opts.displayOutsideMaxExtent ? 'max' : 'min')](
                referenceMaxExtent().getWidth() / getWidth(),
                referenceMaxExtent().getHeight() / getHeight()
              );
          }
          console.log("mapOptions.maxResolution: " + maxResolution);
          console.log("mapOptions.restrictedExtent: " + referenceMaxExtent());

          map = new OpenLayers.Map(
            mapElement.attr('id'),
            {
              // defines the map ui elements and interaction features
              controls: defaultControls,

              // maxResolution determines the lowest zoom level and thus places the map
              // in its maximum extent into the available view port so that no additional
              // gutter is visible and no parts of the map are hidden
              // see http://trac.osgeo.org/openlayers/wiki/SettingZoomLevels
              // IMPORTANT!!!
              // the maxResulution set here will be overwritten if the baselayers maxResolution
              // it is set
              maxResolution: maxResolution,

              // setting restrictedExtent the the maxExtent prevents from panning the
              // map out of its bounds
              restrictedExtent: referenceMaxExtent(),
//                      maxExtent: referenceMaxExtent(),

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
          zoomToBounds = calculateZoomToBounds(opts.boundingBox ? opts.boundingBox : defaultBaseLayerBoundingBox);
          // zoomToBounds = cropBoundsToAspectRatio(zoomToBounds, map.getSize().w / map.getSize().h);
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
         * @param mapResponseArray
         *   The map service returns the mapResponseObj in an array with one element.
         * @param dataType
         *   either "AREA" or "POINT"
         */
        var createDataLayer = function(mapResponseArray, dataType){

          console.log("createDataLayer() : creating data layer of type " + dataType);

          var dataLayerOptions = makeWMSLayerOptions();
          dataLayerOptions.displayOutsideMaxExtent = true; // move into makeWMSLayerOptions?

          var layers = [];
          // add additional layers, get them from the mapResponseObj
          if(mapResponseArray !== undefined){
             var mapResponseObj = mapResponseArray[0];
            if(dataType === "POINT" && mapResponseObj.points_sld !== undefined){
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
              console.log("createDataLayer() : start with adding distribution layers :");
              for ( var i in mapResponseObj.layers) {
                var layerData = mapResponseObj.layers[i];

                var layerName = fixLayerName(layerData);
                console.log(" " + i +" -> " +layerName);
                var layer = new OpenLayers.Layer.WMS(
                  layerName,
                  mapResponseObj.geoserver + "/wms",
                  {
                      layers: fixLayerName(layerData),
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
                var newBounds =  OpenLayers.Bounds.fromString( mapResponseObj.bbox );
                var projection;
                if(dataType === "POINT"){
                  projection = CdmOpenLayers.projections.epsg_4326;
                  // mapResponseObj.bbox are bounds  are always returned in EPSG:4326 since the point service does not know about the projection
                  // console.log("createDataLayer() POINT: referenceProjection()=" + referenceProjection() + ", map.getProjectionObject()=" + map.getProjectionObject() );
                  processDataBounds(projection, newBounds, dataType, layerDataLoaded);
                } else {
                  // Type == AREA
                  // the bounds are in the projection of the data layer
                  // here we expect that all data layers are in the same projection and will use the first data layer as reference
                  // the edit map service is most probably working the same and is not expected to be able to handle multiple data layers
                  // with different projections
                  readProjection(layers[0], function(projection) {
                    processDataBounds(projection, newBounds, dataType, layerDataLoaded);
                  })
                }

                console.log("createDataLayer() " + dataType + ": transforming newBounds " + newBounds + " from projection=" +  projection + " to referenceProjection()=" + referenceProjection());
                newBounds.transform(projection, referenceProjection());
                if(dataBounds !== null){
                  dataBounds.extend(newBounds);
                } else if(newBounds !== undefined){
                  dataBounds = newBounds;
                }

                zoomToBounds = dataBounds;
                console.log("createDataLayer() : viewport zoomToBounds are now: " + zoomToBounds);
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
       * transforms the newBounds from the projection to the referenceProjection() and finally calculates the
       * zoomBounds for the viewport.
       *
       * @param projection
       * @param newBounds
       * @param layerDataTypeString
       *    Only used for logging, (either "AREA" or "POINT")
       * @param callback
       */
        var processDataBounds = function(projection, newBounds, layerDataTypeString, callback){

          console.log("createDataLayer() " + layerDataTypeString + ": transforming newBounds " + newBounds + " from projection=" +  projection + " to referenceProjection()=" + referenceProjection());
          newBounds.transform(projection, referenceProjection());
          if(dataBounds !== null){
            dataBounds.extend(newBounds);
          } else if(newBounds !== undefined){
            dataBounds = newBounds;
          }

          zoomToBounds = dataBounds;
          console.log("createDataLayer() : viewport zoomToBounds are now: " + zoomToBounds);
          zoomToClosestLevel = false;
          callback();
        };

      /**
       * Get the crs data from the WFS and read the projection name from it. Finally the supplied callback will
       * be called with the matching projection object as parameter.
       * @param layer
       * @param callback
       *   Function(Projection projection)
       */
        var readProjection = function(layer, callback){

          var projection = layer.projection;

          if(!projection) {
            projection = layerProjections[layer.name];
          }

          if(projection) {
            callback(projection);
          } else {
            // asking the edit map server would be the best:
            //    > http://edit.africamuseum.be/geoserver/topp/ows?service=WFS&version=1.0.0&request=GetFeature&typeName=topp:euromed_2013&maxFeatures=1&outputFormat=application/json
            // or
            //    > http://edit.africamuseum.be/geoserver/topp/ows?service=WFS&request=getCapabilities'
            // but the latter returns only XML
            var parameters = {
              service: 'WFS',
              version: '1.0.0',
              request: 'GetFeature',
              typeName: layer.name,
              maxFeatures: 1, // only one feature
              outputFormat: 'text/javascript',
              format_options: 'callback:getJson'
            };

            jQuery.ajax({
              url: opts.wfsRootUrl + "?" + jQuery.param(parameters),
              dataType: 'json',
              success: function(data, textStatus, jqXHR){
                if(data.crs && data.crs.type && data.crs.properties.code){
                  var projectionName = data.crs.type + "_" + data.crs.properties.code;
                  log("projection name found in WFS response:" + projectionName);
                  projection = CdmOpenLayers.projections[projectionName.toLowerCase()];
                  callback(projection);
                }
              },
              error : function(jqXHR, textStatus, errorThrown) {
                log("projection name not found in WFS response, due to error: " + textStatus);
                projection = CdmOpenLayers.projections.epsg_4326;
                callback(projection);
              }

            });
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
                if (baseLayerNames[i] === "custom_wms_base_layer_1"){
                    wmsBaseLayer =createWMSLayer(
                            customWMSBaseLayerData.name,
                            customWMSBaseLayerData.url,
                            customWMSBaseLayerData.params,
                            customWMSBaseLayerData.projection,
                            customWMSBaseLayerData.proj4js_def,
                            customWMSBaseLayerData.units,
                            customWMSBaseLayerData.max_extent,
                            customWMSBaseLayerData.untiled
                    );
                  wmsBaseLayer.setIsBaseLayer(true);
                  baseLayers[i] = wmsBaseLayer;
                } else {
                    baseLayers[i] = window.CdmOpenLayers.getLayerByName(baseLayerNames[i]);
                }
                // set default baselayer
                if(baseLayerNames[i] === defaultBaseLayerName){
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
         * @param b OpenLayers.Bounds to crop
         * @param aspectRatio as fraction of width/height as float value
         *
         * @return the bounds cropped to the given aspectRatio
         */
        var cropBoundsToAspectRatio = function (b, aspectRatio){

            var cropedB = b.clone();

            if(aspectRatio === 1){
                return cropedB;
            }

            /*
             * LonLat:
             *   lon {Float} The x-axis coordinate in map units
             *   lat {Float} The y-axis coordinate in map units
             */
            var center = cropedB.getCenterLonLat();
            var dist;
            if(aspectRatio < 1){
                dist = (b.getHeight() / 2) * aspectRatio;
                cropedB.top = center.lat + dist;
                cropedB.cropedBottom = center.lat - dist;
            } else if(aspectRatio > 1){
                dist = (b.getWidth() / 2) / aspectRatio;
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
       * @param bboxString
       *     a string representation of the bounds in degree for epsg_4326
       *
       * @return the bboxstring projected onto the layer and intersected with the maximum extent of the layer
       */
      var calculateZoomToBounds = function(bboxString){
        var zoomToBounds;
        if(bboxString) {
          zoomToBounds = OpenLayers.Bounds.fromString(bboxString);
          if(referenceProjection().proj.projName){
            // SpericalMercator is not supporting the full extent -180,-90,180,90
            // crop if need to -179, -85, 179, 85
            if(zoomToBounds.left < -179){
              zoomToBounds.left =  -179;
            }
            if(zoomToBounds.bottom < -85){
              zoomToBounds.bottom =  -85;
            }
            if(zoomToBounds.right > 179){
              zoomToBounds.right =  179;
            }
            if(zoomToBounds.top > 85){
              zoomToBounds.top = 85;
            }
          }
          // transform bounding box given in degree values to the projection of the base layer
          zoomToBounds.transform(CdmOpenLayers.projections.epsg_4326, referenceProjection());
        } else if(referenceMaxExtent()) {
          zoomToBounds = referenceMaxExtent();
          // no need to transform since the bounds are obtained from the layer
        } else {
          // use the more narrow bbox of the SphericalMercator to avoid reprojection problems
          // SpericalMercator is not supporting the full extent!
          zoomToBounds = CdmOpenLayers.mapExtends.epsg_900913;
          // transform bounding box given in degree values to the projection of the base layer
          zoomToBounds.transform(CdmOpenLayers.projections.epsg_4326, referenceProjection());
        }

        zoomToBounds = intersectionOfBounds(referenceMaxExtent(), zoomToBounds);

        log("zoomBounds calculated: " + zoomToBounds.toString());

        return zoomToBounds;
      };

      var log = function(message, addTimeStamp){
        var timestamp = '';
        if(addTimeStamp === true){
          var time = new Date();
          timestamp = '[' + time.getSeconds() + '.' + time.getMilliseconds() + ' s] ';
        }
        console.log(timestamp + message);
      };

      var makeWMSLayerOptions = function(projection, proj4js_def, maxExtent, units, untiled) {
        var wmsOptions = {
          isBaseLayer: false,
          displayInLayerSwitcher: true
        };

        if (projection) {
          if (proj4js_def) {
            // in case projection has been defined for the layer and if there is also
            // a Proj4js.defs, add it!
            Proj4js.defs[projection] = proj4js_def;
          }
          wmsOptions.projection = projection;
          if (maxExtent === null) {
            maxExtent = CdmOpenLayers.mapExtends.epsg_4326.clone();
            maxExtent.transform(CdmOpenLayers.projections.epsg_4326, projection);
          }
        } else {
          // use the projection and maxextent of the base layer
          maxExtent = referenceMaxExtent();
        }

        if (maxExtent) {
          wmsOptions.maxExtent = maxExtent;
        }

        if (units) {
          wmsOptions.units = units;
        }

        if (true || untiled) {
          wmsOptions.singleTile = true;
          wmsOptions.ratio = opts.aspectRatio;
        }

        return wmsOptions;
      };

      var applyLayerZoomBounds = function(event){
            var layer = event.object;
            zoomToBounds = layer.getDataExtent();
            log("data bounds of layer as zoom bounds: " + zoomToBounds.toString());
            layerDataLoaded();
      };

      var disablePolygonFeatureClick = function(event){
          var layer = event.object;
          var kmlLayerElement = jQuery('#' + layer.id);
          //log("KML Layer DOM element: " + kmlLayerElement);
          kmlLayerElement.find('path').css('pointer-events', 'none');
      }

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
      var createWMSLayer= function(name, url, params, projection, proj4js_def, units, maxExtent, untiled){

        console.log("creating WMS Layer " + name);

        var wmsOptions = makeWMSLayerOptions(projection, proj4js_def, maxExtent, units, untiled);

        var wmsLayer = new OpenLayers.Layer.WMS(
            name,
            url,
            params,
            wmsOptions
          );

          if(wmsLayer === null){
            console.log("Error creating WMS Layer");
          }

          return  wmsLayer;
        };

      var onKmlFeatureSelect = function(event) {
        var feature = event.feature;
        // Since KML is user-generated, do naive protection against
        // Javascript.
        var content = "";
        if(feature.attributes.name){
            content += "<h3>" + feature.attributes.name + "</h3>";
        }
        if(feature.attributes.description) {
            // ${specimen-base-url}
            var description = feature.attributes.description;
            description = description.replace('${occurrence-link-base-url}', opts.specimenPageBaseUrl);
            //description = description.replace('${specimen-link-text}', opts.specimenLinkText); // no longer used
            content += "<p>" + description + "</p>";
        }
        if (content.search("<script") != -1) {
            content = "Content contained Javascript! Escaped content below.<br>" + content.replace(/</g, "&lt;");
        }
        if(content.length > 0){
            popup = new OpenLayers.Popup.FramedCloud("balloon",
                feature.geometry.getBounds().getCenterLonLat(),
                new OpenLayers.Size(250, 150),
                content,
                null, true, onKmlPopupClose);
                popup.autoSize = false;
                // popup.imageSize = new OpenLayers.Size(1276, 736);
                // popup.fixedRelativePosition = true;
                // popup.maxSize = new OpenLayers.Size(50, 50);
            feature.popup = popup;
            map.addPopup(popup);
        }
      };
      var onKmlFeatureUnselect =   function(event) {
            var feature = event.feature;
            if(feature.popup) {
                map.removePopup(feature.popup);
                feature.popup.destroy();
                delete feature.popup;
            }
        };
      var onKmlPopupClose = function(evt) {
          kmlSelectControl.unselectAll();
        };

    var layerLoadingControl = function() {

      var control = new OpenLayers.Control();

      OpenLayers.Util.extend(control, {

        LAYERS_LOADING: 0,

        type: 'LayerLoading',
        title: 'Layer loading',

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
          this.div.style.backgroundColor= 'rgba(255, 255, 255, 0.3)';
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
          fa_class.value = "fa fa-refresh fa-spin fa-sync-alt fa-5x";
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

    var errorMessageControl = function() {

            var control = new OpenLayers.Control();

            OpenLayers.Util.extend(control, {

                messageText: "The map is currently broken due to problems with the map server.",
                id: 'OpenLayers_Control_ErrorMessages',
                type: 'ErrorMessages',
                title: 'Error messages',

                errorDetails: null,

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
                },

                addErrorMessage: function(errorText){
                    var li1 = document.createElement("li");
                    li1.appendChild(document.createTextNode(errorText));
                    this.errorDetails.appendChild(li1);
                },
                hide: function(){
                    this.div.style.display = 'none';
                },
                show: function(){
                    this.div.style.display = 'flex';
                },

                draw: function () {

                    // call the default draw function to initialize this.div
                    OpenLayers.Control.prototype.draw.apply(this, arguments);

                    this.map.events.register('updatesize', this, function(e){
                            this.updateSize();
                        }
                    );

                    // using flexbox here!
                    //see this.show();
                    this.div.style.justifyContent = "center";
                    this.div.style.alignItems = "center";

                    this.errorDetails = document.createElement("ul");
                    this.errorDetails.setAttribute('style', 'font-size:80%');

                    var contentDiv = document.createElement("div");
                    contentDiv.appendChild(document.createTextNode(this.messageText));

                    contentDiv.appendChild(this.errorDetails);
                    this.div.setAttribute('style', 'background-color: rgba(200, 200, 200, 0.3);');
                    this.div.appendChild(contentDiv);

                    this.updateSize();
                    this.hide();
                    return this.div;
                },
            });
            return control;
        }

    }; // end of CdmOpenLayers.Map
})();









