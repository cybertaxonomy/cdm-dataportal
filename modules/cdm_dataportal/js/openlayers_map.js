function CdmOpenlayersMap(mapElement, mapserverBaseUrl, options){
	
	var legendImgSrc = null;
	
	var map = null;
	
	var dataBounds = null;
	
	var layerByNameMap = {
			tdwg1: 'topp:tdwg_level_1',
			tdwg2: 'topp:tdwg_level_2',
			tdwg3: 'topp:tdwg_level_3',
			tdwg4: 'topp:tdwg_level_4'
	};
	 
	var defaultLayerOptions = {
		maxExtent: new OpenLayers.Bounds(-180, -90, 180, 90),
		isBaseLayer: false,
		displayInLayerSwitcher: false
	};
	
	var mapOptions={
	     controls: 
	       [ 
	         new OpenLayers.Control.PanZoom(),
	        // new OpenLayers.Control.LayerSwitcher(),
	         new OpenLayers.Control.Navigation({zoomWheelEnabled: false, handleRightClicks:true, zoomBoxKeyMask: OpenLayers.Handler.MOD_CTRL})
	       ],
	       maxExtent: new OpenLayers.Bounds(-180, -90, 180, 90),
	       maxResolution: (360 / options.displayWidth),
	       restrictedExtent: new OpenLayers.Bounds(-180, -90, 180, 90),
	       projection: new OpenLayers.Projection("EPSG:4326")
    };
	 
	
	 var baseLayer = new OpenLayers.Layer.WMS( 
	    "OpenLayers WMS",
	    "http://labs.metacarta.com/wms/vmap0",
	    {layers: 'basic'}, 
	    this.defaultLayerOptions );
	 
	 baseLayer.options.isBaseLayer = true;
	
	/**
	 * 
	 */
	this.init = function(){
		
		initOpenLayers();

		// -- Distribution Layer --
		var distributionQuery = mapElement.attr('distributionQuery');
		
		if(distributionQuery != undefined){
			if(typeof legendPosition == 'number'){
				distributionQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + options.legendPosition);				
			}
	
			distributionQuery = mergeQueryStrings(distributionQuery, 'callback=?');
			var legendFormatQuery = mapElement.attr('legendFormatQuery');
			if(legendFormatQuery !== undefined){
				legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
			}
			
			var mapServiceRequest = mapserverBaseUrl + '/areas.php?' + distributionQuery;
			
			$.ajax({
				  url: mapServiceRequest,
				  dataType: "jsonp",
				  success: function(data){
						addDistributionLayer(data);
					}
				});
		}
		
		// -- Occurrence Layer --
		var occurrenceQuery = mapElement.attr('occurrenceQuery');
		if(occurrenceQuery != undefined){
//			if(typeof legendPosition == 'number'){
//				occurrenceQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + options.legendPosition);				
//			}
	
			occurrenceQuery = mergeQueryStrings(occurrenceQuery, 'callback=?');
//			var legendFormatQuery = mapElement.attr('legendFormatQuery');
//			if(legendFormatQuery !== undefined){
//				legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
//			}
			
			var mapServiceRequest = mapserverBaseUrl + '/points.php?' + occurrenceQuery;
			
			$.ajax({
				  url: mapServiceRequest,
				  dataType: "jsonp",
				  success: function(data){
						addDistributionLayer(data);
					}
				});
		}
		
			
	};
	
	/**
	 * Initialize the Openlayers viewer with the base layer
	 */
	var initOpenLayers = function(){
			
		// instatiate the openlayers viewer
		map = new OpenLayers.Map('openlayers_map', mapOptions);
		
		//add the base layer
		map.addLayers([baseLayer]);
		
			
		//map.addControl(new OpenLayers.Control.LayerSwitcher({'ascending':false}));

		
		// zoom to the required area
		var boundsStr = (typeof options.boundingBox == 'string' && options.boundingBox.length > 6 ? options.boundingBox : '-180, -90, 180, 90');
		var zoomToBounds = OpenLayers.Bounds.fromString( boundsStr );
		map.zoomToExtent(zoomToBounds, false);
		
		// adjust height of openlayers container div
		$('#openlayers').css('height', $('#openlayers #openlayers_map').height());
		
	};
	
	/**
	 * add a distribution layer
	 */
	var addDistributionLayer = function(mapResponseObj){
			
		
		// add additional layers, get them from 
		// the mapResponseObj
		if(mapResponseObj !== undefined){
			if(mapResponseObj.points_sld !== undefined){
				// it is a response from the points.php
				//TODO points_sld should be renamed to sld in response + fill path to sld should be given				
				
				var layer = new OpenLayers.Layer.WMS.Untiled( 
						'points', 
						"http://193.190.116.6:8080/geoserver/wms/wms",
						{layers: 'topp:rest_points' ,transparent:"true", format:"image/png"},
						defaultLayerOptions );

				var sld = mapResponseObj.points_sld;
				if(sld.indexOf("http://") != 0){
					sld = "http://edit.br.fgov.be/synthesys/www/v1/sld/" + sld;
				}
				
				layer.params.SLD = sld;					
				map.addLayers([layer]);

			} else {
				// it is a response from the areas.php				
				for ( var i in mapResponseObj.layers) {
				var layerData = mapResponseObj.layers[i];
				
					var layer = new OpenLayers.Layer.WMS.Untiled( 
							layerData.tdwg, 
							mapResponseObj.geoserver + "/wms",
							{layers: layerByNameMap[layerData.tdwg] ,transparent:"true", format:"image/png"},
							defaultLayerOptions );
					layer.params.SLD = layerData.sld;
					layer.setOpacity(options.distributionOpacity);
					map.addLayers([layer]);
					
				}
				
			}

			// zoom to the required area
			if(mapResponseObj.bbox != undefined){
				var newBounds =  OpenLayers.Bounds.fromString( mapResponseObj.bbox );
				if(dataBounds !== null){
					dataBounds.extend(newBounds);
				} else if(newBounds !== undefined){
					dataBounds = newBounds;
				}
				map.zoomToExtent(dataBounds, false);
				if(map.getZoom() > options.maxZoom){
					map.zoomTo(options.maxZoom);
				}
			}
						

			if(options.legendPosition != undefined && mapResponseObj.legend != undefined){
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
				.css('z-index', '30000')
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
					imageLayerOptions
			);
		}

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
		maxZoom: 4
};

