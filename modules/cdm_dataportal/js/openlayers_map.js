function CdmOpenlayersMap(mapElement, mapserverBaseUrl, options){
	
	var legendImgSrc = null;
	
	var map = null;
	
	var dataBounds = null;
	
	
	 
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
	 * NOTE: labs.metacarta.com is currently unavailable
	 * 
	 * Available Projections:
	 * 		EPSG:900913
	 * 		EPSG:4326
	 */
	var metacarta_vmap0 = new OpenLayers.Layer.WMS( 
	    "Metacarta Vmap0",
	    "http://labs.metacarta.com/wms/vmap0",
	    {layers: 'basic', format:"png"},
	     {
			maxExtent: mapExtend_4326,
			isBaseLayer: true,
			displayInLayerSwitcher: true
		}
	 );
	
	/**
	 * Available Projections:
	 * 		EPSG:4326
	 */
    var osgeo_vmap0 = new OpenLayers.Layer.WMS(
        "OpenLayers World",
        "http://vmap0.tiles.osgeo.org/wms/vmap0",
        {layers: 'basic', format:"png"},
        {
			maxExtent: mapExtend_4326,
			isBaseLayer: true,
			displayInLayerSwitcher: true
		}
    );

	 
	// create Google Mercator layers
    var gmap = new OpenLayers.Layer.Google(
        "Google Streets",
        {'sphericalMercator': true}
    );
    var gsat = new OpenLayers.Layer.Google(
        "Google Satellite",
        {type: G_SATELLITE_MAP, 'sphericalMercator': true, numZoomLevels: 22}
    );
    var ghyb = new OpenLayers.Layer.Google(
        "Google Hybrid",
        {type: G_HYBRID_MAP, 'sphericalMercator': true}
    );

    // create Virtual Earth layers
    var veroad = new OpenLayers.Layer.VirtualEarth(
        "Virtual Earth Roads",
        {'type': VEMapStyle.Road, 'sphericalMercator': true}
    );
    var veaer = new OpenLayers.Layer.VirtualEarth(
        "Virtual Earth Aerial",
        {'type': VEMapStyle.Aerial, 'sphericalMercator': true}
    );
    var vehyb = new OpenLayers.Layer.VirtualEarth(
        "Virtual Earth Hybrid",
        {'type': VEMapStyle.Hybrid, 'sphericalMercator': true}
    );

    // create Yahoo layer
//    var yahoo = new OpenLayers.Layer.Yahoo(
//        "Yahoo Street",
//        {'sphericalMercator': true}
//    );
//    var yahoosat = new OpenLayers.Layer.Yahoo(
//        "Yahoo Satellite",
//        {'type': YAHOO_MAP_SAT, 'sphericalMercator': true}
//    );
//    var yahoohyb = new OpenLayers.Layer.Yahoo(
//        "Yahoo Hybrid",
//        {'type': YAHOO_MAP_HYB, 'sphericalMercator': true}
//    );

    // create OSM layer
    var mapnik = new OpenLayers.Layer.OSM();
    // create OAM layer
    var oam = new OpenLayers.Layer.XYZ(
        "OpenAerialMap",
        "http://tile.openaerialmap.org/tiles/1.0.0/openaerialmap-900913/${z}/${x}/${y}.png",
        {
            sphericalMercator: true
        }
    );

    // create OSM layer
    var osmarender = new OpenLayers.Layer.OSM(
        "OpenStreetMap (Tiles@Home)",
        "http://tah.openstreetmap.org/Tiles/tile/${z}/${x}/${y}.png"
    );
	
	/**
	 * 
	 */
	this.init = function(){
		
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
			
			$.ajax({
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
//			if(typeof legendPosition == 'number'){
//				occurrenceQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + options.legendPosition);				
//			}
	
			occurrenceQuery = mergeQueryStrings(occurrenceQuery, 'callback=?');
//			var legendFormatQuery = mapElement.attr('legendFormatQuery');
//			if(legendFormatQuery !== undefined){
//				legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
//			}
			
			mapServiceRequest = mapserverBaseUrl + '/points.php?' + occurrenceQuery;
			
			$.ajax({
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
			
		// instatiate the openlayers viewer
		map = new OpenLayers.Map('openlayers_map', mapOptions.EPSG900913);
		
		//add the base layer
		//map.addLayers([mapnik ,gmap]);
		//map.addLayers([veroad ,gmap, metacartaVmap0]);
        map.addLayers([
                       //osgeo_vmap0, 
                       gmap, gsat, //ghyb, 
                       veroad, veaer, //vehyb,
                       oam, mapnik, osmarender
                       ]);

		
		if(options.showLayerSwitcher == true){
			map.addControl(new OpenLayers.Control.LayerSwitcher({'ascending':false}));
		}
		
		// zoom to the required area
		var boundsStr = (typeof options.boundingBox == 'string' && options.boundingBox.length > 6 ? options.boundingBox : '-180, -90, 180, 90');
		var zoomToBounds = OpenLayers.Bounds.fromString( boundsStr );
		map.zoomToExtent(zoomToBounds.transform(dataProj, map.getProjectionObject()), false);
		
		// adjust height of openlayers container div
		$('#openlayers').css('height', $('#openlayers #openlayers_map').height());
		
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
			$(this).parent()
				.css('position', 'relative')
				.css('z-index', '1002')
				.css('top', -mapElement.height())
				.css('left', mapElement.width()- $(this).width())
				.width($(this).width());
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
			
			createLegendLayer();
		});
		
		// 2. create the Legend Layer
		var createLegendLayer = function(){

			
			var legendLayerOptions={
					maxResolution: '.$maxRes.',
					maxExtent: new OpenLayers.Bounds(0, 0, w, h)
			};
			
			var legendLayer = new OpenLayers.Layer.Image(
					'Legend',
					legendSrcUrl,
					new OpenLayers.Bounds(0, 0, w, h),
					new OpenLayers.Size(w, h),
					imageLayerOptions);
		};

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


$.fn.cdm_openlayers_map.defaults = {  // set up default options
		legendPosition:  null,			// 1,2,3,4,5,6 = display a legend in the corner specified by the number
		displayWidth: 400,
		distributionOpacity: 0.75,
		legendOpacity: 0.75,
		boundingBox: null,
		showLayerSwitcher: false,
		maxZoom: 4,
		minZoom: 1
};

