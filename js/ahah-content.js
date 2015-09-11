// see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

/**
 * Expected dom structure:
 *  '<div class="ahah-content" rel="'.$cdm_proxy_url.'"><span class="loading">Loading ....</span></div>';
 */
;(function($, document, window, undefined) {


    $.fn.ahahContent = function (eventSource) {

    // firebug console stub (avoids errors if firebug is not active)
    if(typeof console === "undefined") {
        console = { log: function() { } };
    }

    var element;
    if( eventSource !== undefined ){
        // if ahahContent() has been called as event handler use the eventSource
        element = $(eventSource);
        console.log("ahahContent() as domEventHandler for " + element);
    } else {
        // otherwise use this jQuery object
        element = $(this);
        console.log("ahahContent() as jQuery function for " + element);
    }


      // register with all elements matching the css class selector '.ahah-content'
      element.find(".ahah-content").each(function(index, element) {

          var ahahContent = $(element);

          var url = ahahContent.attr("data-cdm-ahah-url");
          ahahContent.attr("data-cdm-ahah-url", '');
          ahahContent.attr("data-cdm-ahah-url-loaded", url);

          console.log("ahahContent() url:" + url);
          if(url !== undefined && url.length > 1){
              ahahContent.find('.loading').css('display', 'block');
              $.get(url, function(html){
                  ahahContent.find('.loading').remove().end().append(html);
              });
          }
      });

    };
})(jQuery, document, window);



jQuery(document).ready(
    function() {
        //
        jQuery("body").ahahContent();

        // register events to detect lightbox opening
        // for FireFox, Webkit and Opera
        jQuery("body").bind('overflow', function(event){
               jQuery('#jquery-lightbox').ahahContent();
        });
        // for Chrome and >=IE9(???)
        //   - Chrome: this only works for the first image in a lighbox gallery FIXME (need lightbox with callback!)
        jQuery("body").bind('overflowchanged', function(event){
                window.setTimeout(
                        function() {
                            jQuery('#jquery-lightbox').ahahContent();
                        },
                        2000 // milli seconds
                );
        });

        // register the ahahContent as domEventHandler
        // see domEvents.js
        //
        // the for example dynabox will execute the callback after loading
        // the content into the dynabox. By this it is for possible
        // to have ahahContent content inside of dynaboxes
        document.domEventHandlers.push(jQuery().ahahContent);
    }
);
