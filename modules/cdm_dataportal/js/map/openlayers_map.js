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
            aspectRatio: 2, // w/h
            showLayerSwitcher: false,
            baseLayerNames: ["osgeo_vmap0"],
            defaultBaseLayerName: 'osgeo_vmap0',
            maxZoom: 4,
            minZoom: 0,
            debug: true,
            /**
             * allows the map to display parts of the layers which are outside
             * the maxExtent if the aspect ratio of the map and of the baselayer
             * are not equal
             */
            displayOutsideMaxExtent: false,
            customWMSBaseLayerData: {
                name: null,
                url: null,
                params: null,
                projection: null,
                max_extent: null,
                units: null
            }
//  customWMSBaseLayerData: {
//  name: "Euro+Med",
//  url: "http://edit.africamuseum.be/geoserver/topp/wms",
//  params: {layers: "topp:em_tiny_jan2003", format:"image/png", tiled: true},
//  projection: "EPSG:7777777",
//  maxExtent: "-1600072.75, -1800000, 5600000, 5850093",
//  units: 'm'
//  }
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
            getLayerByName: function(layerName){} // initially empty fuction, will be populated by openlayers_layers.js
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

        var dataBounds = null;

        var baseLayers = [];
        var defaultBaseLayer = null;

        /* this is usually the <div id="openlayers"> element */
        var mapContainerElement = mapElement.parent();

        var defaultControls = [
                               new OpenLayers.Control.PanZoom(),
                               new OpenLayers.Control.Navigation({zoomWheelEnabled: false, handleRightClicks:true, zoomBoxKeyMask: OpenLayers.Handler.MOD_CTRL})
                               ];


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

            // set the height of the container element
            adjustHeight();

            // register for resize events to be able to adjust the map aspect ratio and legend position
            jQuery( window ).resize(function() {
              adjustHeight();
              adjustLegendAsElementPosition();
            });



            createLayers(opts.baseLayerNames, opts.defaultBaseLayerName, opts.customWMSBaseLayerData);

            initMap();

            // -- Distribution Layer --
            var mapServiceRequest;
            var distributionQuery = mapElement.attr('distributionQuery');

            if(distributionQuery !== undefined){
                distributionQuery = mergeQueryStrings(distributionQuery, '&recalculate=false');
                if(typeof legendPosition == 'number'){
                    distributionQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + opts.legendPosition);
                }

                distributionQuery = mergeQueryStrings(distributionQuery, 'callback=?');
                var legendFormatQuery = mapElement.attr('legendFormatQuery');
                if(legendFormatQuery !== undefined){
                    legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
                }

                mapServiceRequest = mapserverBaseUrl + mapServicePath + '/' + mapserverVersion + '/rest_gen.php?' + distributionQuery;

                jQuery.ajax({
                    url: mapServiceRequest,
                    dataType: "jsonp",
                    success: function(data){
                        addDataLayer(data, "AREA");
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

                mapServiceRequest = mapserverBaseUrl + mapServicePath + '/' + mapserverVersion + '/rest_gen.php?' + occurrenceQuery;

                jQuery.ajax({
                    url: mapServiceRequest,
                    dataType: "jsonp",
                    success: function(data){
                        addDataLayer(data, "POINT");
                    }
                });
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
         * @param jQuery $element
         */
        this.printInfo = function(){

            var mapExtendDegree = map.getExtent().clone();
            mapExtendDegree.transform(map.baseLayer.projection, CdmOpenLayers.projections.epsg_4326);

            var info = "<dl>";
            info += "<dt>zoom:<dt><dd>" + map.getZoom() + "</dd>";
            if(opts.debug){
                info += "<dt>map resolution:<dt><dd>" + map.getResolution() + "</dd>";
                info += "<dt>map max resolution:<dt><dd>" + map.getMaxResolution() + "</dd>";
                info += "<dt>map scale:<dt><dd>" + map.getScale() + "</dd>";
                info += "<dt>map extent bbox:<dt><dd>" + map.getExtent().toBBOX() + ", <strong>degree:</strong> " + mapExtendDegree.toBBOX() + "</dd>";
                info += "<dt>map maxExtent bbox:<dt><dd>" + map.getMaxExtent().toBBOX() + "</dd>";
                info += "<dt>baselayer extent bbox:<dt><dd>" + map.baseLayer.getExtent().toBBOX() + ", <strong>degree:</strong> <span style=color:green;'>" + map.baseLayer.getExtent().clone().transform(map.baseLayer.projection, CdmOpenLayers.projections.epsg_4326) + "</span></dd>"
                info += "<dt>baselayer projection:<dt><dd>" + map.baseLayer.projection.getCode() + "</dd>";
            } else {
                info += "<dt>bbox:<dt><dd>" + mapExtendDegree.toBBOX() + "</dd>";
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
                    "openlayers_map",
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
            map.addLayers(baseLayers);
            map.setBaseLayer(defaultBaseLayer);

            // calculate the bounds to zoom to
            zoomToBounds = zoomToBoundsFor(opts.boundingBox, defaultBaseLayer);
            zoomToBounds = cropBoundsToAspectRatio(zoomToBounds, map.getSize().w / map.getSize().h);
            console.log("zoomToBounds: " + zoomToBounds);

            // zoom to the extent of the bbox
            map.zoomToExtent(zoomToBounds, true);

            // readjust if the zoom level is out side of the min max
//          if(map.getZoom() > opts.maxZoom){
//          map.zoomTo(opts.maxZoom);
//          } else if(map.getZoom() < opts.minZoom){
//          map.zoomTo(opts.minZoom);
//          }

        };

        /**
         * add a distribution or occurrence layer
         *
         * @param mapResponseObj
         *   The reponse object returned by the edit map service
         * @param dataType
         *   either "AREA" or "POINT"
         */
        var addDataLayer = function(mapResponseObj, dataType){

            console.log("creating data layer of type " + dataType);

            var dataLayerOptions = {
                    isBaseLayer: false,
                    displayInLayerSwitcher: true,
                    maxExtent: map.maxExtent.clone().transform(new OpenLayers.Projection("EPSG:4326"), map.baseLayer.projection),
                    displayOutsideMaxExtent: true
            };

            var layer;
            // add additional layers, get them from the mapResponseObj
            if(mapResponseObj !== undefined){
                if(dataType == "POINT" && mapResponseObj.points_sld !== undefined){

                    // it is a response for an point map
                    var geoserverUri;
                    if(mapResponseObj.geoserver) {
                        geoserverUri = mapResponseObj.geoserver;
                    } else {
                        // it is an old servive which is not providing the corresponding geoserver URI, so we guess it
                        geoserverUri = mapserverBaseUrl + "/geoserver/wms";
                    }

                    //TODO points_sld should be renamed to sld in response + fill path to sld should be given
                    layer = new OpenLayers.Layer.WMS(
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

                    layer.params.SLD = sld;
                    map.addLayers([layer]);

                } else {
                    // it is a response from for a distribution map
                    for ( var i in mapResponseObj.layers) {
                        var layerData = mapResponseObj.layers[i];

                        layer = new OpenLayers.Layer.WMS(
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
                        map.addLayers([layer]);

                    }

                }

                // zoom to the required area
                if(mapResponseObj.bbox !== undefined){
                    var newBounds =  OpenLayers.Bounds.fromString( mapResponseObj.bbox );
                    newBounds.transform(layer.projection, map.getProjectionObject());
                    if(dataBounds !== null){
                        dataBounds.extend(newBounds);
                    } else if(newBounds !== undefined){
                        dataBounds = newBounds;
                    }
                    map.zoomToExtent(dataBounds, false);

                    if(map.getZoom() > opts.maxZoom){
                        map.zoomTo(opts.maxZoom);
                    } else if(map.getZoom() < opts.minZoom){
                        map.zoomTo(opts.minZoom);
                    }
                }


                if(legendImgSrc != null && opts.legendPosition !== undefined && mapResponseObj.legend !== undefined){
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
        var createLayers = function( baseLayerNames, defaultBaseLayerName, customWMSBaseLayerData){

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
         * returns the zoom to bounds.
         *
         * @param bboxString
         *     a string representation of the bounds in degree
         * @param layer
         *     the Openlayers.Layer
         *
         * @return the bboxstring projected onto the layer and intersected with the maximum extent of the layer
         */
        var zoomToBoundsFor = function(bboxString, layer){
            var zoomToBounds;
            if(typeof bboxString == 'string' && bboxString.length > 6) {
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

            return  new OpenLayers.Layer.WMS(
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
        };

    }; // end of CdmOpenLayers.Map
})();






