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

    $.fn.search_area_filter = function(filter_text_selector) {

        // firebug console stub (avoids errors if firebug is not active)
        if(typeof console === "undefined") {
            console = { log: function() { } };
        }

        form_items_container = this;
        var timer;

      // Merge the options given by the user with the defaults
      //this.options = $.extend({}, defaults, options);

      $(filter_text_selector).keyup(function(){
          clearTimeout(timer);
          timer = setTimeout(filter_elements($(this).val()), 1000);
      });

      var filter_elements = function(entered_text){

          console.log('entered text: ' + entered_text);

          form_items = form_items_container.find('.form-checkboxes').find('.form-item');

          if(entered_text.length > 0){
              form_items.hide();
              matching = form_items.filter(function(){
                      return $(this).text().toLowerCase().contains(entered_text.toLowerCase());
              });
              matching.show();
          } else {
              form_items.show();
          }

      };
  }

})(jQuery, document, window);

