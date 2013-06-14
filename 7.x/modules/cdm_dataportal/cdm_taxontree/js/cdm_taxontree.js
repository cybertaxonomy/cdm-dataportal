// see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

/**
 * Expected DOM structure:
 *
 * <div id="cdm_taxontree_parent">
 *      <div class="cdm_taxontree_scroller_xy">
 *              <ul class="cdm_taxontree">
 *                  <li class="leaf filter_included">
 *                      <a href="/d7/test/cdm_dataportal/taxon/996dc2b4-e73d-4d40-99f9-fac18b503d1c"></a>
 *                  </li>
 *                  ...
 *              </ul>
 *      </div>
 * </div>
 */
;(function($, document, window, undefined) {

    $.fn.cdm_taxontree = function(options) {

    var opts = $.extend({}, $.fn.cdm_taxontree.defaults, options);

    var vertical_scroller_selector = 'cdm_taxontree_scroller_xy';

    var cdm_taxontree_parent = $(this);
    var cdm_taxontree_list = cdm_taxontree_parent.find('ul.cdm_taxontree');

      /* ----------- magicbox ---------- */
      if (opts.magicbox) {
        cdm_taxontree_parent.cdm_taxontree_magicbox();
        vertical_sroller_selector =  'cdm_taxontree_scroller_x';
      }

      /* ----------- tree browser ---------- */
      cdm_taxontree_list.delegate("li:not(.invisible)", "click", function(event) {
              handle_taxon_node_click(event);
          }
      );
      // Stop event propagation for links (unclear why this is nessecary,
      // was this needed for the filter buttons?)
      cdm_taxontree_list.delegate("li a", "click", function(event) {
              event.stopPropagation();
          }
      );

      /* ----------- widget ------------------- */
      if (opts.widget) {
        var widget = cdm_taxontree_parent.find('.cdm_taxontree_widget');
        var optionList = widget.find('select');

        // Keep all options unselected.
        optionList.change(function() {
            cdm_taxontree_list.children("[@selected]").remove();
            cdm_taxontree_list.children().removeAttr('selected');
        });
        optionList.children("[@selected]").click(function() {
            cdm_taxontree_list.remove();
        });
        // Select all options onsubmit.
        optionList.parents('form').submit(function() {
          optionList.children().attr('selected', 'selected');
        });

        bind_select_click(optionList, cdm_taxontree_list, opts.multiselect);
      };

      // finally scroll to the focused element
      scrollToFocused();

    /**
     * handler function for clicks on the li elelements which
     * represent CDM TaxonNodes. The click will load the
     * nodes children via an AHAH call to the CdmdataPortal
     * and expands the clicked node.
     *
     * @param event
     *   The javascript event object of the click event
     */
    function handle_taxon_node_click(event){

        event.stopPropagation();
        var li = $(event.target);
        if (li.hasClass('collapsed')) {
          var bindChildren = (li.find('ul').length == 0);
          if (bindChildren) {
            var url = li.attr('data-cdm-ahahurl');
            if (url != undefined) {
              li.removeAttr('data-cdm-ahahurl');
              var parent_li = li;
              li.set_background_image('loading_subtree.gif');

              // Load DOM subtree via AHAH and append it.
              $.get(url, function(html) {
                parent_li.set_background_image('minus.png');
                if (opts.magicbox) {
                  // Preserve scroll positions.
                  var tmp_scroller_y_left = parent_li.parents('div.cdm_taxontree_container').children().scrollTop();

                  parent_li.append(html);

                  // Resize parent container.
                  cdm_taxontree_container_resize(tree_container);

                  // Restore scroll positions.
                  tree_container.children().scrollTop(tmp_scroller_y_left);
                } else {
                  parent_li.append(html);
                }
              });
            }
          } else {
            li.set_background_image('minus.png');
          }
          li.removeClass('collapsed').addClass('expanded').children(
              'ul').css('display', 'block');
        } else if (li.hasClass('expanded')) {
          li.removeClass('expanded').addClass('collapsed').children(
              'ul').css('display', 'none');
          li.set_background_image('plus.png');
        }
    };

    /**
     *
     */
    function bind_select_click(optionList, treelist, isMultiselect) {
      treelist.find('li .widget_select').click(
          function(event) {
            event.stopPropagation();
            var li_widget_select = $(event.target);
            var value = li_widget_select.attr('alt');
            if (optionList.children('[value=' + value + ']').length > 0) {
              // Remove from select.
              optionList.children('[value=' + value + ']').remove();
            } else {
              // Add to select.
              if (!isMultiselect) {
                // Remove all from select
                optionList.children().remove();
              }
              optionList.append('<option value="' + value + '">'
                  + li_widget_select.attr('title') + '</option>');

              // Fix bug in IE.
              if (jQuery.browser['msie']) {
                if (jQuery.browser['version'].charAt(0) <= '6') {
                  return;
                }
              }
              // optionList.children().removeAttr('selected'); // Yields a bug
              // in IE6, @see
              // http://gimp4you.eu.org/sandbox/js/test/removeAttr.html
              optionList.children("[@selected]").attr('selected', '');
            }
          });
    } // END bind_select_click()

    /**
     *
     */
    function scrollToFocused() {
        var focusedElement = cdm_taxontree_parent.find('.focused');
        var lineHeight = focusedElement.css('line-height');
        lineHeight = lineHeight.replace('px', '');
        lineHeight = lineHeight.length == 0 ? 18 : lineHeight;
        cdm_taxontree_parent.find('div.' + vertical_scroller_selector).scrollTo(focusedElement, {duration: 400, axis:'y', offset:-2 * lineHeight});

    }

  }; // END cdm_taxontree()

})(jQuery, document, window);

/**
 * helper function to set the background image as jQuery extension
 */
jQuery.fn.set_background_image = function(imageFile) {
  var bg_image_tmp = jQuery(this).css('background-image');
  var bg_image_new = bg_image_tmp.replace(/^(.*)(\/.*)(\))$/, '$1/' + imageFile + '$3');
  if (jQuery.browser.mozilla) {
    // Special bug handling for mozilla: strip of last closing bracket.
    bg_image_new = bg_image_new.substr(0, bg_image_new.length - 1);
  }
  jQuery(this).css('background-image', bg_image_new);
};

jQuery.fn.cdm_taxontree.defaults = { // Set up default options.
        widget : false, // True = enable widget mode.
        magicbox : false, // True = enable quirky magicbox.
        element_name : 'widgetval',
        multiselect : false // True = enable selection of multiple.
};


// ====================================================================================== //

/**
 * jQuery function which implements the Magicbox behavior for the taxontree.
 * A Magicbox the container of the taxon tree will extend on mouse over
 * events in order to show the entrys without truncation through cropping.
 */
jQuery.fn.cdm_taxontree_magicbox = function() {
  // Exclude IE6 and lower versions.
  if (!(jQuery.browser['msie'] && jQuery.browser['version'].charAt(0) < '7')) {

    var container = $(this).parent().parent('div.cdm_taxontree_container');
    if (container[0] != undefined) {
        container.hover(
            function(event) {
                handle_mouseOver(event);
            },
            function(event) {
                handle_mouseOut(event);
            }
        );
    }
  } // END exclude IE6

  /**
   * expands the box on mouse over events the
   */
  function handle_mouseOver(event){
      var taxontree_container = $(event.target);
      var scroller_x = taxontree_container.parent();
      var scroller_y = taxontree_container.children('.cdm_taxontree_scroller_y');
      var container = scroller_x.parent();

      var h = parseFloat(scroller_x.height());
      var scroll_top = scroller_x.scrollTop();
      var border_color = scroller_x.css('border-top-color');

      // Store scroll_left of scroller_x so that it can be restored on mouseOut.
      scroller_x.append('<div class="_scrollLeft" style="display: none;" title="'
              + scroller_x.scrollLeft() + '"></div>');

      var newWidth = cdm_taxontree_container_resize(taxontree_container);

      var shift_left = '0';
      if (scroller_x.hasClass('expand-left')) {
        shift_left = container.outerWidth({margin : true}) - newWidth;
      }

      scroller_y.css('overflow-y', 'auto')
          .css('border-color', border_color)
          .scrollTop(scroll_top);
      scroller_x.css('overflow-y', 'visible')
          .css('overflow-x', 'visible')
          .css('margin-left', shift_left)
          .css('border-color', 'transparent')
          .height(h);
  };

  /**
   * Restores the original appearance on mouse out events
   */
  function handle_mouseOut(event){

      var taxontree_container = $(event.target);
      var scroller_x = taxontree_container.parent('.cdm_taxontree_scroller_x');
      var scroller_y = taxontree_container.children('.cdm_taxontree_scroller_y');
      var border_color = scroller_y.css('border-top-color');

      var scroll_top = scroller_y.scrollTop();
      scroller_y.css('overflow-y', 'visible')
          .css('border-color', 'transparent');
      scroller_x.css('overflow-y', 'auto').css('margin-left', '0').css(
          'border-color', border_color).width('auto').scrollTop(scroll_top);

      // Restore scroll_left of scroller_x.
      var scrollLeft = scroller_x.children('._scrollLeft').attr('title');
      scroller_x.scrollLeft(scrollLeft);
      scroller_x.children('._scrollLeft').remove();
  };

  /**
   * Resizes the taxontree_container and returns the new outerWidth
   */
  function taxontree_container_resize(taxontree_container) {
    var current_w = taxontree_container.parent().width();

    // Determine max horizontal extent of any children.
    var tree_list = taxontree_container.find('.cdm_taxontree_scroller_y > ul');
    var w = tree_list.css('position', 'absolute').outerWidth({
      margin : true
    });
    tree_list.css('position', 'static');

    // Other Browsers than Firefox.
    if (jQuery.browser['msie']) {
      if (jQuery.browser['version'].charAt(0) == '7') {
        w = w + 17;
      }
      if (jQuery.browser['version'].charAt(0) <= '6') {
        return;
      }
    }

    if (current_w < w) {
        taxontree_container.parent().width(w);
        taxontree_container.children().width(w);
    }
    return taxontree_container.children().outerWidth();
  };

  /**
   * Debug function, currently unused
   */
  function cdm_taxontree_container_debug_size(taxontree_container, msg) {
      var out = msg + '<br />    scoll_x: ' + taxontree_container.parent().width()
          + '<br />    container: ' + taxontree_container.width() + '<br />    scoll_y: '
          + taxontree_container.children().width() + '<br />    ul: '
          + taxontree_container.find('.cdm_taxontree_scroller_y > ul').width() + '<br />';
      jQuery('#DEBUG_JS').append(out);
  };
};

//====================================================================================== //