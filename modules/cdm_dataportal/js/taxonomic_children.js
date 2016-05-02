/**
 * Expected dom structure:
 * <span data-cdm-taxon-uuid="{taxon-uuid}"> ... </span>
 */

// see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

// the semi-colon before function invocation is a safety net against concatenated
// scripts and/or other plugins which may not be closed properly.
;(function($, document, window, undefined) {


  // Name the plugin so it's only in one place
  var pluginName = 'taxonomic_children';

  // Default options for the plugin as a simple object
  var defaults = {
  };

  function Plugin(element, options) {

    this.element = element;

    // firebug console stub (avoids errors if firebug is not active)
    if (typeof console === "undefined") {
      console = {
        log: function () {
        }
      };
    }

    // Merge the options given by the user with the defaults
    this.options = $.extend({}, defaults, options);

    // Attach data to the element
    this.$el      = $(element);
    this.$el.data(name, this);

    this._defaults = defaults;

    this.container;
    this.children;
    this.loading;

    var meta = this.$el.data(name + '-opts');
    this.opts = $.extend(this._defaults, options, meta);

    // Initialization code to get the ball rolling
    // If your plugin is simple, this may not be necessary and
    // you could place your implementation here
    this.init();
  }

  Plugin.prototype = {
    // Public functions accessible to users
    // Prototype methods are shared across all elements
    // You have access to this.options and this.element
    // If your plugin is complex, you can split functionality into more
    // methods like this one

    init: function () {
      // Plugin initializer - prepare your plugin
      this.$el.click(this.createContainer());
    },

    createContainer: function () {
      var trigger_position =  trigger_element.position();
      this.log('trigger-position: ' + trigger_position.top + ', ' + trigger_position.left);

      container = $('<div class="' + pluginName + ' clearfix"></div>')
        .css('background-color', 'red')
        .css('position', 'absolute')
        .css('top', trigger_position.top)
        .css('left', trigger_position.left);
      children = $('<div>CHILDREN</div><ul class="children"></ul>');
      loading = $('<i class="fa-spinner fa-2x" />').hide();

      container.append(children).append(loading);
      trigger_element.append(container);
      
    },

    log: function (msg) {
      console.log('[' + pluginName + '] ' + msg);
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

})(jQuery, document, window);