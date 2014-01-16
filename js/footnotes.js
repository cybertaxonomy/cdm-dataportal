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
;(function($, document, window, undefined) {

    $.fn.footnotes = function(eventSource) {

        var element;

        // firebug console stub (avoids errors if firebug is not active)
        if(typeof console === "undefined") {
            console = { log: function() { } };
        }

        if( eventSource !== undefined ){
            // if ahahContent() has been called as event handler use the eventSource
            element = $(eventSource);
            console.log("ahahContent() as domEventHandler for " + element);
        } else {
            // otherwise use this jQuery object
            element = $(this);
            console.log("ahahContent() as jQuery function for " + element);
        }


        element.find('span.footnote-key a').mouseover(function(e){
            var fnClassName = getFootnoteClassName(this);
            var fnKeyClassName = getFootnoteKeyClassName(this);
            $('.footnote').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
            $('span.footnote-key a').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
            $(fnClassName).css('background-color', 'yellow');
            $(fnKeyClassName).css('background-color', 'yellow');
        }
        ).mouseout(function(e){
            var fnClassName = getFootnoteClassName(this);
            var fnKeyClassName = getFootnoteKeyClassName(this);
            $(fnClassName).not('.active').css('background-color', 'transparent');
            $(fnKeyClassName).not('.active').css('background-color', 'transparent');
        }
        ).click(function(e){
            var fnClassName = getFootnoteClassName(this);
            var fnKeyClassName = getFootnoteKeyClassName(this);
            $('.footnote').css('background-color', 'transparent').removeClass('active');
            $('span.footnote-key a').css('background-color', 'transparent').removeClass('active');
            $(fnClassName).css('background-color', 'yellow').addClass('active');
            $(fnKeyClassName).css('background-color', 'yellow').addClass('active');
        });

        function getFootnoteClassName(object){
            return '.'+$(object).attr('href').substr(1);
        }

        function getFootnoteKeyClassName(object){
            return '.'+$(object).attr('href').substr(1).replace(/-/gi, '-key-') + ' a';
        }

    };

})(jQuery, document, window);

jQuery(document).ready(
    function() {
        //
        jQuery('body').footnotes();

        // register the ahahContent as domEventHandler
        // see domEvents.js
        //
        // the for example dynabox will execute the callback after loading
        // the content into the dynabox. By this it is for possible
        // to have ahahContent content inside of dynaboxes
        document.domEventHandlers.push(jQuery().footnotes);
    }
);
