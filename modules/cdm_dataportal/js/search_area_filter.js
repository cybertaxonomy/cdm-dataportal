/**
 * Expected dom structure:
 *  <div class="form-item form-type-checkbox form-item-search-areas-area-0-4f854b84-2d93-4b0d-a40d-9e05be7cbae1">
        <input
            id="edit-search-areas-area-0-4f854b84-2d93-4b0d-a40d-9e05be7cbae1"
            name="search[areas][area][0][4f854b84-2d93-4b0d-a40d-9e05be7cbae1]"
            value="4f854b84-2d93-4b0d-a40d-9e05be7cbae1"
            class="form-checkbox" type="checkbox">
        <label class="option" for="edit-search-areas-area-0-4f854b84-2d93-4b0d-a40d-9e05be7cbae1">
            <span class="parents">Pacific (6) &gt; Northwestern Pacific (62) &gt; </span>Marianas (MRN)
        </label>
    </div>
 */

// see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

// the semi-colon before function invocation is a safety net against concatenated
// scripts and/or other plugins which may not be closed properly.
;(function($, document, window, undefined) {


    // Name the plugin so it's only in one place
    var pluginName = 'search_area_filter';

    // Default options for the plugin as a simple object
    var defaults = {
    };

    function Plugin(element, filter_text_selector, options) {

        this.element = element;
        this.text_field = $(filter_text_selector);
        this.form_items;
        this.checked_items_container;

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

            var timer;

            // init
            var form_item_container = this.$el;

            form_items = form_item_container.find('.form-checkboxes').find('.form-item');
            // --- add container for labels of checked items
            checked_items_container = $('<div class="checked-items clearfix"></div>');
            this.text_field.before(checked_items_container);

            form_items.find('input:checked').each(function(){
                handle_item_checked($(this));
            });


            this.text_field.keyup(function () {
                clearTimeout(timer);
                timer = setTimeout(filter_elements($(this).val()), 1000);
            });

            form_items.children('input').change(function (e) {
                handle_item_checked($(this));
            });
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


    var handle_item_checked = function(item){

        var label = item.siblings('label');

        console.log('input of ' + label.text() + ' changed to ' + (item.is(':checked') ? '1':'0'));

        if (item.is(':checked')) {
            checked_items_container.append('<div class="selected-item-label" data-cdm-checked="' + item.val() + '">' + label.text() + '</div>');
        } else {
            // i was trying to use  an attribute selector 'div[data-cdm-checked="' + target.val() + '"' here,
            // but it did not work at all,
            // so looping over the children seems to be the better approach
            checked_items_container.children().each(function(index, element){
                var el = $(element);
                if(el.attr('data-cdm-checked') == item.val()){
                    console.log('   removing');
                    el.remove();
                }
            });
        }
    };

    var filter_elements = function(entered_text){

        console.log('entered text: ' + entered_text);

        // --- remove all highlighting and
        // --- hide all form items except checked items which always must be visible
        form_items.each(function(){
            var form_item_container = $(this);
            if(form_item_container.children(':checked').length == 0){
                form_item_container.hide();
            }
            var matching_label = form_item_container.find('.child-label');
            matching_label.html(matching_label.text());
        });


        if(entered_text.length > 0){

            var rexgexp = new RegExp(entered_text , 'i');

            var matching_items = form_items.filter(function(){
                return $(this).text().match(rexgexp);
            });

            // --- highlite the matching text
            matching_items.each(function(){
                var matching_label = $(this).find('.child-label');
                var matching_snippet = matching_label.text().match(rexgexp);
                if(matching_snippet && matching_snippet.length > 0){
                    // NOTE this will only highlite the first match in the string
                    matching_label.html(matching_label.text().replace(matching_snippet[0], '<span class="highlite">' + matching_snippet[0] + '</span>'));
                }
                var matching_label_abbrev = $(this).find('.child-label-abbreviated');
                matching_snippet = matching_label_abbrev.text().match(rexgexp);
                if(matching_snippet && matching_snippet.length > 0){
                  // NOTE this will only highlite the first match in the string
                  matching_label_abbrev.html(matching_label_abbrev.text().replace(matching_snippet[0], '<span class="highlite">' + matching_snippet[0] + '</span>'));
                }
            });
            matching_items.show();

            // --- also show the options of parent areas
            var parent_uuids = [];
            matching_items.find('span.parent').each(function(){
                var parent_uuid = $(this).attr('data-cdm-parent');
                if(parent_uuids.indexOf(parent_uuid) == -1){
                    parent_uuids.push(parent_uuid);
                }
            });

            parent_uuids.forEach(function(uuid, index, array) {
                // form-item-search-areas-area-0-bda3f9fb-30cf-43fd-8d17-5e4d69545ed5"
                // TODO make the class attribute prefix configurable!
                $('.form-item-search-areas-area-0-' + uuid).show();
            });
        } else {
            form_items.show();
        }

    };


})(jQuery, document, window);

