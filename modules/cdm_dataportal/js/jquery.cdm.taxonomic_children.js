/*********************************************************
 *             taxonomic_children plugin
 *********************************************************
 *
 *  Expected dom structure:
 *  <span data-cdm-taxon-uuid="{taxon-uuid}"> ... </span>
 *
 * based on https://github.com/johndugan/jquery-plugin-boilerplate
 */

/*
 The semi-colon before the function invocation is a safety net against
 concatenated scripts and/or other plugins which may not be closed properly.

 "undefined" is used because the undefined global variable in ECMAScript 3
 is mutable (ie. it can be changed by someone else). Because we don't pass a
 value to undefined when the anonymyous function is invoked, we ensure that
 undefined is truly undefined. Note, in ECMAScript 5 undefined can no
 longer be modified.

 "window" and "document" are passed as local variables rather than global.
 This (slightly) quickens the resolution process.
 */

;(function ( $, window, document, undefined ) {

  /*
   Store the name of the plugin in the "pluginName" variable. This
   variable is used in the "Plugin" constructor below, as well as the
   plugin wrapper to construct the key for the "$.data" method.

   More: http://api.jquery.com/jquery.data/
   */
  var pluginName = 'taxonomic_children';

  /*
   The "Plugin" constructor, builds a new instance of the plugin for the
   DOM node(s) that the plugin is called on. For example,
   "$('h1').pluginName();" creates a new instance of pluginName for
   all h1's.
   */
  // Create the plugin constructor
  function Plugin ( element, options ) {
    /*
     Provide local access to the DOM node(s) that called the plugin,
     as well local access to the plugin name and default options.
     */
    this.element = element;
    this._name = pluginName;
    this._defaults = $.fn[pluginName].defaults;
    /*
     The "$.extend" method merges the contents of two or more objects,
     and stores the result in the first object. The first object is
     empty so that we don't alter the default options for future
     instances of the plugin.

     More: http://api.jquery.com/jquery.extend/
     */
    this.options = $.extend( {}, this._defaults, options );

    // firebug console stub (avoids errors if firebug is not active)
    if (typeof console === "undefined") {
      console = {
        log: function () {
        }
      };
    }

    /*
     The "init" method is the starting point for all plugin logic.
     Calling the init method here in the "Plugin" constructor function
     allows us to store all methods (including the init method) in the
     plugin's prototype. Storing methods required by the plugin in its
     prototype lowers the memory footprint, as each instance of the
     plugin does not need to duplicate all of the same methods. Rather,
     each instance can inherit the methods from the constructor
     function's prototype.
     */
    this.init();
  }

  // Avoid Plugin.prototype conflicts
  $.extend(Plugin.prototype, {

    // Initialization logic
    init: function () {
      /*
       Create additional methods below and call them via
       "this.myFunction(arg1, arg2)", ie: "this.buildCache();".

       Note, you can access the DOM node(s), plugin name, default
       plugin options and custom plugin options for a each instance
       of the plugin by using the variables "this.element",
       "this._name", "this._defaults" and "this.options" created in
       the "Plugin" constructor function (as shown in the buildCache
       method below).
       */
      this.isDataLoaded = false;
      this.buildCache();
      this.bindEvents();
    },

    // Remove plugin instance completely
    destroy: function() {
      /*
       The destroy method unbinds all events for the specific instance
       of the plugin, then removes all plugin data that was stored in
       the plugin instance using jQuery's .removeData method.

       Since we store data for each instance of the plugin in its
       instantiating element using the $.data method (as explained
       in the plugin wrapper below), we can call methods directly on
       the instance outside of the plugin initalization, ie:
       $('selector').data('plugin_myPluginName').someOtherFunction();

       Consequently, the destroy method can be called using:
       $('selector').data('plugin_myPluginName').destroy();
       */
      this.unbindEvents();
      this.$element.removeData();
    },

    // Cache DOM nodes for performance
    buildCache: function () {
      /*
       Create variable(s) that can be accessed by other plugin
       functions. For example, "this.$element = $(this.element);"
       will cache a jQuery reference to the elementthat initialized
       the plugin. Cached variables can then be used in other methods.
       */

      this.$element = $(this.element);

      this.taxonUuid = this.$element.attr('data-cdm-taxon-uuid');
      this.rankLimitUuid = this.$element.attr('data-rank-limit-uuid');
      if(this.rankLimitUuid == '0'){
        // '0' is used in the cdm_dataportal settings as value for 'no rank limit'
        this.rankLimitUuid = undefined;
      }
      if(this.$element.hasClass('classification-chooser')){
        this.classificationChooser = true;
      }
      this.destinationUri = this.$element.attr('data-destination-uri');

      this.classificationMode =  this.$element.attr('data-cdm-classification-mode');

      if (this.$element.attr('data-cdm-align-with') == 'prev') {
        var prev = this.$element.prev();
        this.alignOffset = {
          'padding': prev.width(),
          'left' : prev.width()
        }
      } else {
        this.alignOffset = {
          'padding': this.$element.width(),
          'left' : '0'
        }
      }

      // Create new elements
      this.container = $('<div class="' + this._name + ' box-shadow-b-5-1"></div>')
        .css('background-color', 'rgba(255,255,255,0.7)')
        .css('position', 'absolute')
        .css('overflow', 'auto');
      this.children = $('<div class="children"></div>');

      this.loading_class_attr = 'fa fa-spinner fa-pulse';
      // used to preserve the class attributes of the icon
      this.icon_class_attr = null;

      this.container.append(this.children);


    },

    // Bind events that trigger methods
    bindEvents: function() {
      var plugin = this;

      /*
       Bind event(s) to handlers that trigger other functions, ie:
       "plugin.$element.on('click', function() {});". Note the use of
       the cached variable we created in the buildCache method.

       All events are namespaced, ie:
       ".on('click'+'.'+this._name', function() {});".
       This allows us to unbind plugin-specific events using the
       unbindEvents method below.

       this works at earliest with v1.7, with 1.4.4 we need to use bind:
       */
      plugin.$element.bind('mouseenter', function() { // 'mouseenter' or 'click' are appropriate candidates
        plugin.showChildren.call(plugin);
       });

      plugin.$element.bind('click', function (event){
        if(event.target == this){
          // prevents eg from executing clicks if the
          // trigger element is an <a href=""> element
          event.preventDefault();
        }
        event.stopPropagation();
        plugin.showChildren.call(plugin);
      });

      plugin.container.mouseleave(function (){
        plugin.hideChildren.call(plugin);
      });

      $(document).click(function (){
        plugin.hideChildren.call(plugin);
      });

      /*
      plugin.$element.children('i.fa').hover(
        function(){
          this.addClass(this.options.hoverClass);
        },
        function(){
          this.removeClass(this.options.hoverClass);
        }
      );
      */
    },

    // Unbind events that trigger methods
    unbindEvents: function() {
      /*
       Unbind all events in our plugin's namespace that are attached
       to "this.$element".

       this works at earliest with v1.7, with 1.4.4 we need to unbind without
       namespace specificity
       */
      this.$element.unbind('click');
      // TODO complete this ...
    },

    log: function (msg) {
      console.log('[' + this._name + '] ' + msg);
    },

    showChildren: function(){

      var plugin = this;

      var trigger_position =  this.$element.position();

      this.log('trigger_position: ' + trigger_position.top + ', ' + trigger_position.left);

      // Unused; TODO when re-enabling this needs to be fixed
      //         when using rotate, in IE and edge the child element are also rotated, need to reset child elements.
      // this.$element.addClass(this.options.activeClass);

      this.container.hide();
      if(!this.container.parent() || this.container.parent().length == 0){
        // first time this container is used
        this.$element.append(this.container);
      }

      this.baseHeight = this.$element.parent().height();
      this.lineHeight = this.$element.parent().css('line-height').replace('px', ''); // TODO use regex fur replace

      this.log('baseHeight: ' + this.baseHeight);
      this.log('lineHeight: ' + this.lineHeight);

      this.offset_container_top = this.lineHeight - trigger_position.top  + 1;

      this.container
        .css('top', - this.offset_container_top + 'px')
        .css('left', (trigger_position.left - this.alignOffset.left) + 'px')
        .css('padding-left', this.alignOffset.padding + 'px')
        .css('padding-right', this.alignOffset.padding + 'px')
        .css('z-index', 10)
        .show();

      if(!this.isDataLoaded){
        this.icon_class_attr = this.$element.prev('i').attr('class'),
        this.$element.prev('i').attr('class', this.loading_class_attr);
        $.get(this.requestURI(undefined, undefined), function(html){
          plugin.handleDataLoaded(html);
        });
      } else {
        if(this.container.find('ul').length > 0) {
          this.container.show();
          this.adjustHeightAndMaxWidth();
          this.scrollToSelected();
        }
      }
    },

    hideChildren: function(){
      //return; // uncomment for debugging
      this.container.slideUp();
      //this.container.detach();
    },

    handleDataLoaded: function(html){

      this.isDataLoaded = true;
      var listContainer = $(html);
      if(listContainer[0].tagName != 'UL') {
        // unwrap from potential enclosing div, this is
        // necessary in case of compose_classification_selector
        listContainer = listContainer.children('ul');
      }

      this.container.hide();

      if(listContainer.children().length > 0) {
        this.children.append(listContainer);
        this.itemsCount = listContainer.children().length;

        this.container.show();
        this.adjustHeightAndMaxWidth();
        this.scrollToSelected();

        // data loading may take quite long
        // need to check asynchronously if the
        // mouse still is hovering
        var this_plugin = this;
        setTimeout(function() {
          this_plugin.checkMouseOver();
        },
        300);

      }

      this.$element.prev('i').attr('class', this.icon_class_attr);
    },

    calculateViewPortRows: function() {

      var max;
      if(this.options.viewPortRows.max) {
        max = this.options.viewPortRows.max;
      } else {
        // no absolute maximum defined: calculate the current max based on the window viewport
        max = Math.floor( ($(window).height() - this.element.getBoundingClientRect().top) / this.lineHeight) - 2;
        this.log('max: ' + max);
      }
      var rows = Math.max(this.itemsCount, this.options.viewPortRows.min);
      rows = Math.min(rows, max);
      this.log('rows: ' + max);
      return rows;
    },

    adjustHeightAndMaxWidth: function(){

      // adjustHeightAndMaxWidth
      var viewPortRows = this.calculateViewPortRows(this.itemsCount); //(itemsCount > this.options.viewPortRows.min ? this.options.viewPortRows.max : this.options.viewPortRows.min);
      this.log('itemsCount: ' + this.itemsCount + ' => viewPortRows: ' + viewPortRows);

      this.container.css('height', viewPortRows * this.lineHeight + 'px');
      this.children
        .css('padding-top', this.lineHeight + 'px') // one row above current
        .css('padding-bottom', (viewPortRows - 2) * this.lineHeight + 'px'); // subtract 2 lines (current + one above)

      // adjust width to avoid the container hang out of the viewport
      max_width = Math.floor($(window).width() - this.element.getBoundingClientRect().left - 40);
      this.log('max_width: ' + max_width);
      this.container.css('max-width', max_width + 'px');
    },

    scrollToSelected: function () {

      // first reset the scroll position to 0 so that all calculation are using the same reference position
      this.container.scrollTop(0);
      var scrollTarget = this.children.find(".focused");
      if(scrollTarget && scrollTarget.length > 0){
        var position = scrollTarget.position();
        if(position == undefined){
          // fix for IE >= 9 and Edge
          position = scrollTarget.offset();
        }
        var scroll_target_offset_top = position.top;
        this.log("scroll_target_offset_top: " + scroll_target_offset_top + ", offset_container_top: " + this.offset_container_top);
        this.container.scrollTop(scroll_target_offset_top - this.lineHeight + 1); // +1 yields a better result
      }
    },

    requestURI: function(pageIndex, pageSize){

      var contentRequest;
      var renderFunction;
      var proxyRequestQuery= '';

      // pageIndex, pageSize are not yet used, prepared for future though
      if(!pageIndex){
        pageIndex = 0;
      }
      if(!pageSize) {
        pageSize = 100;
      }

      if(this.classificationChooser){
        renderFunction = this.options.renderFunction.classifications + '?destination=' + this.destinationUri;
        contentRequest = 'NULL'; // using the plain compose function which does not require any data to be passes as parameter

      } else {
        renderFunction = this.options.renderFunction.taxonNodes;
        proxyRequestQuery = '?currentTaxon=' + this.taxonUuid;
        if(this.taxonUuid) {
          if(this.classificationMode == 'siblings') {
            contentRequest =
              this.options.cdmWebappBaseUri
              + this.options.cdmWebappRequests.taxonSiblings
                .replace('{classificationUuid}', this.options.classificationUuid)
                .replace('{taxonUuid}', this.taxonUuid);
          } else {
            // default mode is 'children'
            contentRequest =
              this.options.cdmWebappBaseUri
              + this.options.cdmWebappRequests.taxonChildren
                .replace('{classificationUuid}', this.options.classificationUuid)
                .replace('{taxonUuid}', this.taxonUuid);
          }
        } else if(this.rankLimitUuid){
          contentRequest =
            this.options.cdmWebappBaseUri
            + this.options.cdmWebappRequests.childNodesAt
              .replace('{classificationUuid}', this.options.classificationUuid)
              .replace('{rankUuid}', this.rankLimitUuid);
        } else {
          contentRequest =
            this.options.cdmWebappBaseUri
            + this.options.cdmWebappRequests.classificationRoot
              .replace('{classificationUuid}', this.options.classificationUuid);
        }
      }

      this.log("contentRequest: " + contentRequest);

      var proxyRequest = this.options.proxyRequest
        .replace('{contentRequest}', encodeURIComponent(encodeURIComponent(contentRequest)))
        .replace('{renderFunction}', renderFunction);

      var request = this.options.proxyBaseUri + '/' + proxyRequest + proxyRequestQuery;
      this.log("finalRequest: " + request);

      return request;
    },

    checkMouseOver: function(){
      // see http://stackoverflow.com/questions/6035137/jquery-check-hover-status-before-start-trigger/6035278#6035278
      //
      // this.container.find(':hover').length == 0
      // is(':hover')
      //this.log('>>>> hover: ' + this.container.find(':hover').length + ' | ' +  this.container.is(':hover') );
      if(!this.container.is(':hover')){
        this.hideChildren();
      }
    }

  });

  /*
   Create a lightweight plugin wrapper around the "Plugin" constructor,
   preventing against multiple instantiations.

   More: http://learn.jquery.com/plugins/basic-plugin-creation/
   */
  $.fn[pluginName] = function ( options ) {
    this.each(function() {
      if ( !$.data( this, "plugin_" + pluginName ) ) {
        /*
         Use "$.data" to save each instance of the plugin in case
         the user wants to modify it. Using "$.data" in this way
         ensures the data is removed when the DOM element(s) are
         removed via jQuery methods, as well as when the userleaves
         the page. It's a smart way to prevent memory leaks.

         More: http://api.jquery.com/jquery.data/
         */
        $.data( this, "plugin_" + pluginName, new Plugin( this, options ) );
      }
    });
    /*
     "return this;" returns the original jQuery object. This allows
     additional jQuery methods to be chained.
     */
    return this;
  };


  $.fn[pluginName].defaults = {
    hoverClass: undefined, // unused
    activeClass: undefined, // unused
    /**
     * uuid of the current classification - required
     */
    classificationUuid: undefined,
    /**
     * uuid of the current taxon - required
     */
    taxonUuid: undefined,
    cdmWebappBaseUri: undefined,
    proxyBaseUri: undefined,
    cdmWebappRequests: {
      taxonChildren: "portal/classification/{classificationUuid}/childNodesOf/{taxonUuid}",
      taxonSiblings: "portal/classification/{classificationUuid}/siblingsOf/{taxonUuid}",
      childNodesAt: "portal/classification/{classificationUuid}/childNodesAt/{rankUuid}.json",
      classificationRoot: "portal/classification/{classificationUuid}/childNodes.json"
    },
    proxyRequest: "cdm_api/proxy/{contentRequest}/{renderFunction}",
    renderFunction: {
      taxonNodes: "cdm_taxontree",
      classifications: "classification_selector"
    },
    // viewPortRows: if max is 'undefined' the height will be adapted to the window viewport
    viewPortRows: {min: 3, max: undefined}
  };

})( jQuery, window, document );
