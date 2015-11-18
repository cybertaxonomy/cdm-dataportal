/**
 * The dynabox supports
 * Expected dom structure, the element tags are variable:
 *
     <li id="dynabox_${dynabox_id}">
      <div class="dynabox_label"><a class="label" href="${url-to-content}">Label Text</span>
      <ul id="dynabox_${dynabox_id}_content" ><li> ...... </li></ul>
    </li>
 */

// see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

// the semi-colon before function invocation is a safety net against concatenated
// scripts and/or other plugins which may not be closed properly.
;(function($) {

  // Default options for the plugin as a simple object
  var defaults = {
    open_callback: function(){},
    close_callback: function(){},
    content_container_selector: null // optional selector for a container into which the dynabox content should be placed
  };

  this.dynabox = function(dynabox_id, options) {

    // Merge the options given by the user with the defaults
    this.options = $.extend({}, defaults, options);

    // get hold of the dom elements and attributed needed later on
    var dynabox_container =  $('#dynabox-' + dynabox_id);
    var dynabox_trigger = dynabox_container.children('.label');
    var dynabox_content = $('#dynabox-' + dynabox_id + '-content');

    var url = dynabox_trigger.attr('href');

    if(options.content_container_selector != null) {
      // move the content into the element specified by the
      // optional 'content_container_selector'
      dynabox_content.detach().appendTo(options.content_container_selector);
    }

    // register events
    dynabox_trigger.click(function(event) {
      loadContent(event);
    }).bind("contextmenu",function(e){
        e.preventDefault(); // disable context menu to avoid opening in new tab or window
    });

    dynabox_content.click(function(event){event.stopPropagation();});

    // ----- private functions ------ //

    var loadContent = function(event) {
      event.preventDefault(); //Cancel the default action (navigation) of the click.

      if(dynabox_content.find('.content').length > 0) {
        // content has already been loaded
        dynabox_content.slideToggle("fast", function(){
            toggleState(dynabox_content);
          }
        );
      } else {
        // no content so far, so load it
        if(url !== undefined && url.length > 1){
          dynabox_content.removeAttr('title').find('.loading').slideDown('fast',
            function(){
              toggleState(dynabox_content);
            }
          );
          $.get(url, function(html){
            dynabox_content.find('.loading').remove();
            dynabox_content.find('.dynabox-content-inner').html('<div class="content">' + html + '</div>').triggerElementsAdded();
          });
        }
      }


    };

    /**
     * toggles the closed/open state of the dynabox
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

