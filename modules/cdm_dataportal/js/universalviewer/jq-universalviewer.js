/**
 * jq-universalviewer
 * Version: 1.0
 * URL: ${URL}
 * Description: jQuery wrapper for Universalviewer
 * Requires: universalviewer@^3.0.36 (https://github.com/UniversalViewer/universalviewer)
 * Author: Andreas Kohlbecker
 * Copyright: Copyright 2020
 * License: Mozilla Public License Version 1.1
 */

// Plugin closure wrapper
// Uses dollar, but calls jQuery to prevent conflicts with other libraries
// Semicolon to prevent breakage with concatenation
// Pass in window as local variable for efficiency (could do same for document)
// Pass in undefined to prevent mutation in ES3
;(function($, document, window, undefined) {
    "use strict";

    // Name the plugin so it's only in one place
    var pluginName = 'jqUniversalviewer';

    // Default options for the plugin
    var defaults = {
        root: undefined,
        configUri: undefined,
        manifestUri: 'http://wellcomelibrary.org/iiif/b18035723/manifest'
    };

    var urlDataProvider;

    // Plugin constructor
    // This is the boilerplate to set up the plugin to keep our actual logic in one place
    function Plugin(element, options) {
        this.element = element;

        // Merge the options given by the user with the defaults
        this.options = $.extend({}, defaults, options);

        // Attach data to the element
        this.$el      = $(element);
        this.$el.data(name, this);

        this._defaults = defaults;

        var meta      = this.$el.data(name + '-opts');
        this.opts     = $.extend(this._defaults, options, meta);

        this.uv = null;

        // Initialization code to get the ball rolling
        this.init();
    }

    // firebug console stub (avoids errors if firebug is not active)
    if (typeof console === "undefined") {
         console = {
             log: function () {
             }
         };
     }

    Plugin.prototype = {
        // Public functions accessible to users
        // Prototype methods are shared across all elements
        // You have access to this.options and this.element
        // If your plugin is complex, you can split functionality into more
        // methods like this one
        init: function() {
            var plugin = this;
               setupUV(plugin, this.element);
        }
    };

    $.fn[pluginName] = function(options) {
        // Iterate through each DOM element and return it
        return this.each(function() {
            // prevent multiple instantiations
            if (!$.data(this, 'plugin_' + pluginName)) {
                $.data(this, 'plugin_' + pluginName, new Plugin(this, options));
            }
        });
    };

    console.log('jq-universalviewer.js > $.fn.' + pluginName + ' bound to ' + $.fn.jquery)

    // Private function that is only called by the plugin
    var setupUV = function(plugin, element) {

        urlDataProvider = new UV.URLDataProvider();
        var collectionIndex = urlDataProvider.get('c');

        console.log("setupUV() with " + plugin.options.manifestUri);
        var uvElement = $(element);
        var parentDiv = $(element).parent('div');
        adaptSizeTo(uvElement, parentDiv);
        $(window).resize(function (event) {
            adaptSizeTo(uvElement, parentDiv);
        });

        var uvdata = {
            root: plugin.options.root,
            configUri: plugin.options.configUri,
            iiifResourceUri: plugin.options.manifestUri,
            collectionIndex: (collectionIndex !== undefined) ? Number(collectionIndex) : undefined,
            manifestIndex: Number(urlDataProvider.get('m', 0)),
            sequenceIndex: Number(urlDataProvider.get('s', 0)),
            canvasIndex: Number(urlDataProvider.get('cv', 0)),
            rotation: Number(urlDataProvider.get('r', 0)),
            rangeId: urlDataProvider.get('rid', ''),
            xywh: urlDataProvider.get('xywh', ''),
        };

        plugin.uv = createUV(element, uvdata, urlDataProvider);
        console.log('createUV done:' + plugin.uv);

        plugin.uv.on('created', function() {
            // console.log('created');
            Utils.Urls.setHashParameter('manifest', plugin.options.manifestUri);
        });


        // plugin.uv.on('openseadragonExtension.open', function() {
        //     console.log('osd opened');
        // });
    };


    var adaptSizeTo = function(uvElement, jqElement) {
        var parentWidth = jqElement.width();
        console.log("adjusting size from parent div :" + parentWidth);
        uvElement.css('width', parentWidth);
    }


})(jQueryUV, document, window);
console.log('jq-universalviewer.js jQueryUV: ' + jQueryUV.fn.jquery)
