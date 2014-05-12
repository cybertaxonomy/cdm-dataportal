/**
 * Expected dom structure:
 *  <li class="dynabox">
      <div class="dynabox_label"><span class="label">Label Text</span>
      <ul class="dynabox_content" title="{url-to-content}"><li> ...... </li></ul>
    </li>
 */

// see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

// the semi-colon before function invocation is a safety net against concatenated
// scripts and/or other plugins which may not be closed properly.
;(function($) {

  // Default options for the plugin as a simple object
  var defaults = {
      open_callback: function(){},
      close_callback: function(){}
  };

  this.dynabox = function(dynabox_id, options) {

    // Merge the options given by the user with the defaults
      this.options = $.extend({}, defaults, options);

    $('.dynabox-' + dynabox_id + ' .label').click(function(event) {
      loadContent(event);
    });

    $('.dynabox-' + dynabox_id).find('.dynabox-' + dynabox_id + '-content').click(function(event){event.stopPropagation();});

    //$('li.dynabox> span').click(function(event){event.stopPropagation();});

    var loadContent = function(event) {
      event.preventDefault(); //Cancel the default action (navigation) of the click.
      var dynabox_content = $(event.target).parent('.dynabox-' + dynabox_id).find('.dynabox-' + dynabox_id + '-content');

      if(dynabox_content.find('.content').length > 0) {
        // content has already been loaded
        dynabox_content.slideToggle("fast", function(){
            toggleState(dynabox_content);
          }
        );
      } else {
        // no content so far, so load it
        var url = dynabox_content.attr('title');
        if(url !== undefined && url.length > 1){
          dynabox_content.removeAttr('title').find('.loading').slideDown('fast',
            function(){
              toggleState(dynabox_content);
            }
          );
          $.get(url, function(html){
            dynabox_content.find('.loading').remove().end().append('<div class="content">' + html + '</div>').triggerElementsAdded();
          });
        }
      }


    };

    /**
     * toggles the closed/open state of the supplied dynabox
     */
    var toggleState = function(dynabox_content) {
      if (dynabox_content.css('display') == 'none'){
        options.close_callback();
      } else {
        options.open_callback();
      }
    };
  };


})(jQuery);

