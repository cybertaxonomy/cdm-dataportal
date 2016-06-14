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
      this.subTaxonName = undefined;
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


      var nextLiElement = this.$element.parent('li').next();
      if(nextLiElement != undefined){
        this.subTaxonName = nextLiElement.children('a').text();
      }

      // Create new elements
      this.container = $('<div class="' + this._name + ' box-shadow-b-5-1"></div>')
        .css('background-color', 'rgba(255,255,255,0.7)')
        .css('position', 'absolute')
        .css('overflow', 'auto');
      this.children = $('<div class="children"></div>');

      this.loading = $('<i class="fa-spinner fa-2x" />')
        .css('position', 'absolute')
        .hide();

      this.container.append(this.children).append(this.loading);
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
        /*
         Use the "call" method so that inside of the method being
         called, ie: "someOtherFunction", the "this" keyword refers
         to the plugin instance, not the event handler.

         More: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/call
         */
        plugin.showChildren.call(plugin); // TODO? why can't handleShowChildren(plugin) be used?
      });

      plugin.$element.children('i.fa').hover(
        function(){
          this.addClass(this.options.hoverClass);
        },
        function(){
          this.removeClass(this.options.hoverClass);
        }
      );
      
      plugin.container.mouseleave(function (){
        plugin.hideChildren.call(plugin);
      });

      $(document).click(function (){
        plugin.hideChildren.call(plugin);
      });

    },

    // Unbind events that trigger methods
    unbindEvents: function() {
      /*
       Unbind all events in our plugin's namespace that are attached
       to "this.$element".

       this works at earliest with v1.7, with 1.4.4 we need to unbind without
       namespace specifity
       */
      this.$element.unbind('click');
    },

    log: function (msg) {
      console.log('[' + this._name + '] ' + msg);
    },

    showChildren: function(){

      var plugin = this;
      var trigger_position =  this.$element.position();

      this.log('trigger_position: ' + trigger_position.top + ', ' + trigger_position.left);

      this.$element.addClass(this.options.activeClass);

      this.$element.append(this.container);

      this.baseHeight = this.$element.parent().height();
      this.lineHeight = this.$element.parent().css('line-height').replace('px', ''); // TODO use regex fur replace

      this.log('baseHeight: ' + this.baseHeight);
      this.log('lineHeight: ' + this.lineHeight);

      this.offset_container_top = this.lineHeight - trigger_position.top;

      this.container
        .css('top', - this.offset_container_top + 'px')
        .css('left', trigger_position.left + 'px')
        .css('padding-left', this.$element.width() + 'px')
        .css('padding-right', this.$element.width() + 'px')
        .css('z-index', 10)
        .show();

      if(!this.isDataLoaded){
        $.get(this.requestURI(undefined, undefined), function(html){
          plugin.handleDataLoaded(html);
        });
      } else {
        this.adjustHeight();
        this.scrollToSelected();
      }
    },

    hideChildren: function(){
      // return;
      this.container
        .detach();
    },

    handleDataLoaded: function(html){

      this.loading.hide();
      this.isDataLoaded = true;
      var listContainer = $(html);
      this.children.append(listContainer);
      this.itemsCount = listContainer.children().length;

      this.adjustHeight();
      this.scrollToSelected();
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
      return (this.itemsCount > this.options.viewPortRows.min ? max : this.options.viewPortRows.min);
    },

    adjustHeight: function(itemsCount){

      var viewPortRows = this.calculateViewPortRows(itemsCount); //(itemsCount > this.options.viewPortRows.min ? this.options.viewPortRows.max : this.options.viewPortRows.min);
      this.log('itemsCount: ' + itemsCount + ' => viewPortRows: ' + viewPortRows);

      this.container.css('height', viewPortRows * this.lineHeight + 'px');
      this.children
        .css('padding-top', this.lineHeight + 'px') // one row above current
        .css('padding-bottom', (viewPortRows - 2) * this.lineHeight + 'px'); // subtract 2 lines (current + one above)
    },

    scrollToSelected: function () {

      if(this.subTaxonName){
        var scrollTarget = this.children
          .find("a:contains('" + this.subTaxonName + "')")
          .css('font-weight', 'bold');
        var scroll_target_offset_top = scrollTarget.position().top;
        this.log("scroll_target_offset_top: " + scroll_target_offset_top + ", offset_container_top: " + this.offset_container_top);
        this.container.scrollTop(scroll_target_offset_top - this.lineHeight);
      }
    },

    requestURI: function(pageIndex, pageSize){

      // pageIndex, pageSize are not yet used, prepared for future though
      var contentRequest;

      if(!pageIndex){
        pageIndex = 0;
      }
      if(!pageSize) {
        pageSize = 100;
      }

      if(this.taxonUuid){
        contentRequest =
          this.options.cdmWebappBaseUri
          + this.options.cdmWebappTaxonChildrenRequest
            .replace('{classificationUuid}', this.options.classificationUuid)
            .replace('{taxonUuid}', this.taxonUuid);

      } else if(this.rankLimitUuid){
        contentRequest =
          this.options.cdmWebappBaseUri
          + this.options.cdmWebappClassificationChildnodesAtRequest
            .replace('{classificationUuid}', this.options.classificationUuid)
            .replace('{rankUuid}', this.rankLimitUuid);
      } else {
        contentRequest =
          this.options.cdmWebappBaseUri
          + this.options.cdmWebappClassificationRootRequest
            .replace('{classificationUuid}', this.options.classificationUuid);
      }

      this.log("contentRequest: " + contentRequest);

      var proxyRequest = this.options.proxyRequest
        .replace('{contentRequest}', encodeURIComponent(encodeURIComponent(contentRequest)))
        .replace('{renderFunction}', this.options.renderFunction);

      var request = this.options.proxyBaseUri + '/' + proxyRequest;
      this.log("finalRequest: " + request);

      return request;
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

  /*
   Attach the default plugin options directly to the plugin object. This
   allows users to override default plugin options globally, instead of
   passing the same option(s) every time the plugin is initialized.

   For example, the user could set the "property" value once for all
   instances of the plugin with
   "$.fn.pluginName.defaults.property = 'myValue';". Then, every time
   plugin is initialized, "property" will be set to "myValue".

   More: http://learn.jquery.com/plugins/advanced-plugin-concepts/
   */
  $.fn[pluginName].defaults = {
    hoverClass: undefined,
    activeClass: undefined,
    classificationUuid: undefined,
    cdmWebappBaseUri: undefined,
    proxyBaseUri: undefined,
    cdmWebappTaxonChildrenRequest: "portal/classification/{classificationUuid}/childNodesOf/{taxonUuid}",
    cdmWebappClassificationChildnodesAtRequest: "portal/classification/{classificationUuid}/childNodesAt/{rankUuid}.json",
    cdmWebappClassificationRootRequest: "portal/classification/{classificationUuid}/childNodes.json",
    proxyRequest: "cdm_api/proxy/{contentRequest}/{renderFunction}",
    renderFunction: "cdm_taxontree",
    // viewPortRows: if max is 'undefined' the height will be adapted to the window viewport
    viewPortRows: {min: 3, max: undefined}
  };

})( jQuery, window, document );
