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

    $.fn.table_body_collapse = function() {

        var $element;
        var $table_header;
        var $table_body;
        var closed_state_icon;
        var opened_state_icon;

        // firebug console stub (avoids errors if firebug is not active)
        if(typeof console === "undefined") {
            console = { log: function() { } };
        }

        $element = $(this);
        console.log("table_body_collapse() as jQuery function for " + $element);

        $table_header = $($element.children('thead').children('tr'));
        $table_body = $($element.children('tbody'));

        closed_state_icon = $table_header.find('.icon_cell').attr('data-state-closed-icon');
        opened_state_icon = $table_header.find('.icon_cell').attr('data-state-opened-icon');

        $table_body.hide();

        $table_header.css('cursor', 'pointer')
        .click(function(e){
            if($table_body.css('display') == 'none'){
                $table_body.show();
                $table_header.find('.icon_cell').html(opened_state_icon);
            } else {
                $table_body.hide();
                $table_header.find('.icon_cell').html(closed_state_icon);
            }
        });

    };

})(jQuery, document, window);

jQuery(document).ready(
    function() {
        // auto attach to all tables having the class attribute table-body-collapse
        jQuery('table.table-body-collapse').each(function(){
            jQuery(this).table_body_collapse();
        });
    }
);
