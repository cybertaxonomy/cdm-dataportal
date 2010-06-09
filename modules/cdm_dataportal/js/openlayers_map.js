function CdmOpenlayersMap(mapElement, mapserverBaseUrl, options){
	
	var legendImgSrc = null;
	
	var map = null;
	
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
		
		// get and prepare the Urls
		var distributionQuery = mapElement.attr('distributionQuery');
		if(typeof legendPosition == 'number'){
			distributionQuery = mergeQueryStrings(distributionQuery, 'legend=1&mlp=' + options.legendPosition);				
		}
		// jsonp 
		distributionQuery = mergeQueryStrings(distributionQuery, 'foo=foo');
		
		var legendFormatQuery = mapElement.attr('legendFormatQuery');
		if(legendFormatQuery !== undefined){
			legendImgSrc = mergeQueryStrings('/GetLegendGraphic?SERVICE=WMS&VERSION=1.1.1', legendFormatQuery);
		}
		
		var mapServiceRequest = mapserverBaseUrl + '/areas.php?' + distributionQuery;
		$.ajax({
			  url: mapServiceRequest,
			  dataType: "jsonp",
			  success: function(data){
					initOpenLayers(data);
				},
			  jsonp: "foo"
			});
	};
	
	/**
	 * 
	 */
	var initOpenLayers = function(mapResponseObj){
			
		// instatiate the openlayers viewer
		map = new OpenLayers.Map('openlayers_map', mapOptions);
		
		//add the base layer
		map.addLayers([baseLayer]);
		
		// add additional layers, get them from 
		// the mapResponseObj
		if(mapResponseObj !== undefined){
			for ( var i in mapResponseObj.layers) {
				
				var layerData = mapResponseObj.layers[i];
				
				var layer = new OpenLayers.Layer.WMS.Untiled( 
						layerData.tdwg, 
						mapResponseObj.geoserver + "/wms",
					    {layers: layerByNameMap[layerData.tdwg] ,transparent:"true", format:"image/png"},
					    defaultLayerOptions );
				
				layer.params.SLD = layerData.sld;
				map.addLayers([layer]);
				
			}
			
			// zoom to the required area
			var boundsStr = (options.boundingBox !== undefined ? options.boundingBox : (mapResponseObj.bbox ? mapResponseObj.bbox : '-180, -90, 180, 90') );
			var zoomToBounds = OpenLayers.Bounds.fromString( boundsStr );
			map.zoomToExtent(zoomToBounds, false);
						
			// HACK:
			var hack = "/var/www/";
			mapResponseObj.legend = "http://edit.br.fgov.be/" + mapResponseObj.legend.substr(hack.length - 1);
			// END OF HACK
			if(options.legendPosition !== undefined){
				addLegend(mapResponseObj.geoserver + legendImgSrc + mapResponseObj.legend);
			}
		}
		
		
	};
	
	var addLegend= function(legendSrcUrl){
		mapElement.after('<div id="openlayers_legend"><img id="legend" src="' + legendSrcUrl + '"></div>');
		
		//TODO position legend according to options.legendPosition
		mapElement.next('#openlayers_legend')
			.css('top', -mapElement.height())
			.css('left', mapElement.width()- 140); //FIXME use correct image width
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
		boundingBox: null
};

