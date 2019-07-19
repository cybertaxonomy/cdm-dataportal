// see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

// the semi-colon before function invocation is a safety net against concatenated
// scripts and/or other plugins which may not be closed properly.
;(function($, document, window, undefined) {

  // Optional, but considered best practice by some
  "use strict";

  // Name the plugin so it's only in one place
  var pluginName = 'utis_client';

  // Default options for the plugin as a simple object
  var defaults = {
    providers: ['bgbm-phycobank', 'diatombase', 'worms'],
    // webserviceUrl : 'https://cybertaxonomy.eu/eu-bon/utis/1.3',
    webserviceUrl : 'http://test.e-taxonomy.eu/eubon-utis',
    pageSize: 20,
    spinnerIcon: null
  };

  // Plugin constructor
  // This is the boilerplate to set up the plugin to keep our actual logic in one place
  function Plugin(element, options) {
    this.element = element;
    this.formItemQueryText = null;
    this.formItemProviderCheckboxes = null;
    this.formItemProviderSeachMode = null;
    this.resultContainer = null;
    this.spinnerContainer = null;
    this.pageIndex = 0;
    this.autoLoadTolerance = 20; // start autoloading x pixels before the bottom of the result container

    // firebug console stub (avoids errors if firebug is not active)
    if (typeof console === "undefined") {
      console = {
        log: function () {
        }
      };
    }

    // Merge the options given by the user with the defaults
    this.options = $.extend({}, defaults, options);

    // Attach data to the elment
    this.$el      = $(element);
    this.$el.data(name, this);

    this._defaults = defaults;

    var meta      = this.$el.data(name + '-opts');
    this.opts     = $.extend(this._defaults, options, meta);

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

      var plugin = this;
      var form = $("<form/>",
        {
          style: 'vertical-align: top;'
        }
      );

      this.formItemQueryText = $("<input/>",
        {
          type: 'text',
          placeholder: 'scientific name query string',
          name: 'query',
          // value: "Navicula", // search term preset for testing
          style: 'width:100%'
        }
      ).keypress(function(event){
        if ( event.which == 13 ) { // 13 = ENTER
          event.preventDefault();
          form.submit();
        }
      });

      var checkBoxesDiv = $('<div/>',
        {
          'class': 'checkboxes',
           style: 'display: inline-block; width:30%; vertical-align: top;'
        });
      var checkboxesArray = [];
      $.each(this.opts.providers, function (index, value) {
        var checkbox_id = 'checkbox_' + value;
        var checkbox =   $("<input/>",
          {
            type: 'checkbox',
            name: 'provider',
            checked: index == 0 ? 'checked' : '',
            value: value,
            id:  checkbox_id
          }
        );
        var label = $('<label for="' + checkbox_id + '" style="display: inline-block; vertical-align: top;">&nbsp;' +  value + '</label><br/>');
        // label.append(checkbox);
        checkBoxesDiv.append(checkbox).append(label);

        checkboxesArray.push(checkbox);
      });

      this.formItemProviderCheckboxes = checkboxesArray;

      var submit = $("<input/>",
        {
          type: 'submit',
          value: 'Search',
          style: 'width:100%; margin-top: 1em;'
        }
      );

      this.resultContainer = $('<div/>',
        {
          'class': 'results',
          style: 'overflow-y: auto; height: 20em; border: inset 1px rgb(239, 240, 241); margin-top: 1em;'
        }
        ).hide().bind('scroll', function() {
          if($(this).scrollTop() + $(this).innerHeight() >= $(this)[0].scrollHeight - plugin.autoLoadTolerance) {
            plugin.pageIndex++;
            plugin.executeQuery(plugin);
          }
        });


      form.append($('<div />', {style: 'width: 30%; display: inline-block; vertical-align: top; padding-right: 1em;'}).append(this.formItemQueryText).append(submit));
      form.append(checkBoxesDiv);
      // form.append(submit);

      form.submit(function(event){
        event.stopPropagation();
        event.preventDefault();
        // reset the page index when the form was submitted
        plugin.pageIndex = 0;
        plugin.executeQuery(plugin);
        return false;

      });
      this.$el.append(form);
      this.$el.append(this.resultContainer);
      this.spinnerContainer = $('<div class="spinner" style="position: relative; bottom: 50px; width: 100%; height: 50px; margin: auto; text-align: center;"></div>');
      this.spinnerContainer.hide();
      this.resultContainer.append(this.spinnerContainer);
      if(this.options.spinnerIcon !== undefined){
        var icon = $(this.options.spinnerIcon);
        this.spinnerContainer.append(icon);
      }
    },

  /**
   *
   * @returns {string}
   */
  providerSelection: function(){
    var checkboxes = this.formItemProviderCheckboxes;
    return function(){
    var providerSelection = [];
      $.each(checkboxes, function (index, checkbox) {
        if(checkbox.is(':checked')){
          providerSelection.push(checkbox.attr('value'));
        }
      });
      return providerSelection.join(',');
    }
  },

  /**
   *
   * @returns {string}
   */
  queryString: function(){
    var textField = this.formItemQueryText;
    return function(){
     return textField.val();
    }
  },

  /**
   *
   * @param plugin
   */
  executeQuery: function (plugin) {

    plugin.spinnerContainer.show();
    $.getJSON(plugin.options.webserviceUrl + '/search',
      {
        providers: plugin.providerSelection(), // Closure!
        searchMode: 'scientificNameLike',
        query: plugin.queryString(), // Closure!
        pageSize: plugin.options.pageSize,
        pageIndex: plugin.pageIndex
      },
      function (tnrMsg, textStatus, jqXHR) {
        if(plugin.pageIndex === 0){
          plugin.resultContainer.empty();
          plugin.resultContainer.append(plugin.spinnerContainer);
        }
        if (tnrMsg.query[0].response.length === 0 && plugin.pageIndex === 0) {
          plugin.resultContainer.hide();
        } else {
          plugin.resultContainer.show()
        }
        // in UTIS there is always only one query
        var response = tnrMsg.query[0].response;
        $.each(response, function (index, record) {
          plugin.spinnerContainer.before(plugin.makeResultEntry(record));
        });
        plugin.spinnerContainer.hide();
      });
  },

  makeResultEntry: function (record) {

    var resultEntry = $('<div/>', {'class': 'result-entry'});
    resultEntry.append($('<div/>', {'class': 'matching-name', style: 'font-weight: bold;'}).text(record.matchingNameString));

    var relation = record.matchingNameType;
    var taxonName = record.taxon.taxonName.scientificName;
    if(relation !== undefined){
      relation = relation.toLowerCase();
      if(relation !== 'taxon') {
        resultEntry
          .append($('<span/>', {'class': 'relation', style: 'opacity: 0.5;'}).text(relation + ' for '))
          .append($('<span/>', {'class': 'taxon'}).text(taxonName));
      }
      resultEntry.append($('<div/>', {'class': 'checklist', style: 'opacity: 0.5; '}).text('Checklist: ') // text-align: right?
          .append($('<a/>', { href: record.checklistUrl, target: 'checklist'}).text(record.checklist)
          )
      );
    }

    return resultEntry;
  }

  }; // END of  Plugin.prototype

  $.fn[pluginName] = function(options) {
    // Iterate through each DOM element and return it
    return this.each(function() {
      // prevent multiple instantiations
      if (!$.data(this, 'plugin_' + pluginName)) {
        $.data(this, 'plugin_' + pluginName, new Plugin(this, options));
      }
    });
  };

  // ---------------------

    /*
    function getFootnoteClassName(object){
      return '.'+$(object).attr('href').substr(1);
    }

    function getFootnoteKeyClassName(object){
      return '.'+$(object).attr('href').substr(1).replace(/-/gi, '-key-') + ' a';
    }
    */


})(jQuery, document, window);