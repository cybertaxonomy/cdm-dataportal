/*
 * Layer definitions for CdmOpenLayers (openlayers_map.js)
 */

(function() {

    window.CdmOpenLayers.getLayerByName = function(layerName){

        switch(layerName){

          case 'metacarta_vmap0':
            /**
             * NOTE: labs.metacarta.com is currently unavailable
             *
             * Available Projections:
             *     EPSG:900913
             *     EPSG:4326
             */
            return  new OpenLayers.Layer.WMS(
                  "Metacarta Vmap0",
                  "http://labs.metacarta.com/wms/vmap0",
                  {layers: "basic", format:"png"},
                  {
                      maxExtent: window.CdmOpenLayers.mapExtends.epsg_4326,
                      projection: window.CdmOpenLayers.projections.epsg_4326,
                      isBaseLayer: true,
                      displayInLayerSwitcher: true
                  }
            );

          case 'osgeo_vmap0':
            /**
             * Available Projections:
             *    EPSG:4269
                  EPSG:4326
                  EPSG:900913
             */
            return  new OpenLayers.Layer.WMS(
                  "OSGEO Vmap0",
                  "http://vmap0.tiles.osgeo.org/wms/vmap0",
                  {layers: 'basic', format:"png"},
                  {
                    maxExtent: window.CdmOpenLayers.mapExtends.epsg_4326,
                    projection: new OpenLayers.Projection("EPSG:4326"),
                    isBaseLayer: true,
                    displayInLayerSwitcher: true
                  }
                );

          case 'mapproxy_vmap0':
            /**
             * Available Projections:
             *    EPSG:4269
             EPSG:4326
             EPSG:900913
             */
            return  new OpenLayers.Layer.WMS(
              "OSGEO Vmap0 via mapproxy",
              "http://geo.cybertaxonomy.org/mapproxy/service",
              {layers: 'vmap0', format:"image/png"},
              {
                maxExtent: window.CdmOpenLayers.mapExtends.epsg_4326,
                projection: new OpenLayers.Projection("EPSG:4326"),
                isBaseLayer: true,
                displayInLayerSwitcher: true
              }
            );

         /**
          * ETOPO1 Global Relief Model
          */
         case 'edit-etopo1':
            return  new OpenLayers.Layer.WMS(
                  "ETOPO1 Global Relief Model",
                  "http://edit.africamuseum.be/geoserver/topp/wms",
                  {layers: 'topp:color_etopo1_ice_full', format:"image/png"},
                  {
                    maxExtent: window.CdmOpenLayers.mapExtends.epsg_4326,
                    projection: new OpenLayers.Projection("EPSG:4326"),
                    isBaseLayer: true,
                    displayInLayerSwitcher: true
                  }
                );

        /**
         * ETOPO1 Global Relief Model
         */
          case 'mapproxy_etopo1':
            return  new OpenLayers.Layer.WMS(
              "ETOPO1 Global Relief Model via mapproxy",
              "http://cybertaxonomy.org:8082/mapproxy/service",
              {layers: 'etopo1', format:"image/png"},
              {
                maxExtent: window.CdmOpenLayers.mapExtends.epsg_4326,
                projection: new OpenLayers.Projection("EPSG:4326"),
                isBaseLayer: true,
                displayInLayerSwitcher: true
              }
            );

          /*
           * OSM Layers:
           *
           *   minZoomLevel is not supported for XYZ layers, of which OSM is a subclass.
           *   http://trac.osgeo.org/openlayers/ticket/2909
           *   http://trac.osgeo.org/openlayers/ticket/2189
           *   solution in OL 2.12
           *
           *   see also http://stackoverflow.com/questions/4240610/min-max-zoom-level-in-openlayers
           *   and http://trac.osgeo.org/openlayers/wiki/SettingZoomLevels
           *
           *   To allow OSM baselayers to zoom out to the full extend of the world the map width must be
           *   multiple of 256px since the osm tiles from tile.openstreetmap.org have a size of 256px x 256px
           *   and fractional tiles are not supported by XYZ layers like OSM
           */
          case 'mapnik':
              return  new OpenLayers.Layer.OSM();

          case 'mapquest_open':
              // see http://developer.mapquest.com/web/products/open/map
              return new OpenLayers.Layer.XYZ(
                      "MapQuest",
                      [
                          "http://otile1.mqcdn.com/tiles/1.0.0/map/${z}/${x}/${y}.png",
                          "http://otile2.mqcdn.com/tiles/1.0.0/map/${z}/${x}/${y}.png",
                          "http://otile3.mqcdn.com/tiles/1.0.0/map/${z}/${x}/${y}.png",
                          "http://otile4.mqcdn.com/tiles/1.0.0/map/${z}/${x}/${y}.png"
                      ],
                      {
                          // If using the MapQuest-OSM tiles, OpenStreetMap must be given credit for the data
                          attribution: "Data, imagery and map information provided by <a href='http://www.mapquest.com/'  target='_blank'>MapQuest</a>, <a href='http://www.openstreetmap.org/' target='_blank'>Open Street Map</a> and contributors, <a href='http://creativecommons.org/licenses/by-sa/2.0/' target='_blank'>CC-BY-SA</a>  <img src='http://developer.mapquest.com/content/osm/mq_logo.png' border='0'>",
                          transitionEffect: "resize"
                      }
                  );

          case 'mapquest_sat':
              // see http://developer.mapquest.com/web/products/open/map
              return new OpenLayers.Layer.XYZ(
                      "MapQuest Sattelite",
                      [
                          "http://otile1.mqcdn.com/tiles/1.0.0/sat/${z}/${x}/${y}.png",
                          "http://otile2.mqcdn.com/tiles/1.0.0/sat/${z}/${x}/${y}.png",
                          "http://otile3.mqcdn.com/tiles/1.0.0/sat/${z}/${x}/${y}.png",
                          "http://otile4.mqcdn.com/tiles/1.0.0/sat/${z}/${x}/${y}.png"
                      ],
                      {
                          // If using the MapQuest-OSM tiles, OpenStreetMap must be given credit for the data
                          attribution: "Data, imagery and map information provided by <a href='http://www.mapquest.com/'  target='_blank'>MapQuest</a>, <a href='http://www.openstreetmap.org/' target='_blank'>Open Street Map</a> and contributors, <a href='http://creativecommons.org/licenses/by-sa/2.0/' target='_blank'>CC-BY-SA</a>  <img src='http://developer.mapquest.com/content/osm/mq_logo.png' border='0'>",
                          transitionEffect: "resize"
                      }
                  );

          // create Google Mercator layers
          case 'gmroadmap':
            return  new OpenLayers.Layer.Google(
                      "Google Roadmap",
                      {
                          type: 'roadmap',
                          projection: window.CdmOpenLayers.projections.epsg_900913,
                          // Allow user to pan forever east/west
                          // Setting this to false only restricts panning if sphericalMercator is true.
                          wrapDateLine: false,
                          sphericalMercator: true,
                          numZoomLevels: 21
                      }
                  );

          case 'gsatellite':
              // FIMXE: layer distorted, use OpenLayers.Layer.Google.v3?
            return  new OpenLayers.Layer.Google(
                      "Google Satellite",
                      {
                        type: 'satellite',
                        projection: window.CdmOpenLayers.projections.epsg_900913,
                        // Allow user to pan forever east/west
                        // Setting this to false only restricts panning if sphericalMercator is true.
                        wrapDateLine: false,
                        numZoomLevels: 22
                      }
                  );
          case 'ghybrid':
            return  new OpenLayers.Layer.Google(
                      "Google Hybrid",
                      {
                          type: 'hybrid',
                          projection: window.CdmOpenLayers.projections.epsg_900913,
                          // Allow user to pan forever east/west
                          // Setting this to false only restricts panning if sphericalMercator is true.
                          wrapDateLine: false,
                          numZoomLevels: 22
                      }
                  );

          case 'gterrain':
            return  new OpenLayers.Layer.Google(
              "Google Terrain",
              {
                type: 'terrain',
                projection: window.CdmOpenLayers.projections.epsg_900913,
                // Allow user to pan forever east/west
                // Setting this to false only restricts panning if sphericalMercator is true.
                wrapDateLine: false,
                numZoomLevels: 22
              }
            );

          /*
           * FIXME: apiKey needs to be specified
          case 'veroad':
          case 'bing_road':
              return  new OpenLayers.Layer.Bing({
                  name: "Road",
                  key: apiKey,
                  type: "Road"
              });

          case 'vehyb':
          case 'bing_hybrid':
              return  new OpenLayers.Layer.Bing({
                  name: "Hybrid",
                  key: apiKey,
                  type: "AerialWithLabels"
              });

          case 'veaer':
          case 'bing_arial':
              return  new OpenLayers.Layer.Bing({
                  name: "Aerial",
                  key: apiKey,
                  type: "Aerial"
              });

          */

        }

        return null;
      };


})();

