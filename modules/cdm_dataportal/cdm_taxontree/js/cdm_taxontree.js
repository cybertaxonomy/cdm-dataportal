// $Id$

/** 
 * Copyright (C) 2007 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
 
(function($){

 /**
  * 
  */
  $.fn.cdm_taxontree = function(options) {
  
    var opts = $.extend({},$.fn.cdm_taxontree.defaults, options);
    
		return this.each(function() {
		  
	   /* ----------- magicbox ---------- */
      $(this).cdm_taxontree_magicbox();
		  
		  /* ----------- tree browser ---------- */
			$(this).children('li').not('.invisible').click(function(event) {
				event.stopPropagation();
				if($(this).hasClass('collapsed')){
					var bindChildren = ($(this).find('ul').length == 0);
					if(bindChildren){
						var url = $(this).attr('title');
						if(url != undefined){
							$(this).removeAttr('title');
							var parent_li = $(this);
							var bg_image_tmp = parent_li.css('background-image');
							var bg_image_loading = bg_image_tmp.replace(/^(.*)(\/.*)(\))$/, '$1/loading_subtree.gif$3')
							parent_li.css('background-image', bg_image_loading);
							
							// load DOM subtree and append it
							$.get(url, function(html){
							  var tree_container = parent_li.parents('div.cdm_taxontree_container');
						    parent_li.css('background-image', bg_image_tmp);
								// preserve scroll positions
								var tmp_scroller_y_left = tree_container.children().scrollTop();
								parent_li.append(html).find('ul').cdm_taxontree(options);
							  // resize parent container
							  tree_container.cdm_taxontree_container_resize();
							  // restore scroll positions
                tree_container.children().scrollTop(tmp_scroller_y_left);
							});
							
						}
					} 
					$(this).removeClass('collapsed').addClass('expanded').children('ul').css('display', 'block');
				} else if($(this).hasClass('expanded')){
				  $(this).removeClass('expanded').addClass('collapsed').children('ul').css('display', 'none');
				}
			}); // END click()
			
			$(this).children('li').children('a').click(function(event) {
			 event.stopPropagation();
			});
			
			/* ----------- widget ------------------- */
			if(opts.widget){
        var widget = $(this).parents('.cdm_taxontree_widget');
        var optionList = widget.find('select');
        
        // keep all options unselected
        optionList.change(function(){
          $(this).children().removeAttr('selected');
        });
        // select all options onsubmit
        optionList.parents('form').submit(function(){
          optionList.children().attr('selected', 'selected');
        });
        //
        bind_select_click(optionList, $(this), opts.multiselect);
      }
		});
		
		function bind_select_click(optionList, treelist, isMultiselect){
     treelist.find('li .widget_select').click(function(event){
        event.stopPropagation();
        var value = $(this).attr('alt');
        if(optionList.children('[value='+value+']').length > 0){
          // remove from select
          optionList.children('[value='+value+']').remove();
        } else {
         // add to select
         if(!isMultiselect){
           // remove all from select
           optionList.children().remove();
         }
         optionList.append('<option value="'+value+'">'+$(this).attr('title')+'</option>');
         optionList.children().removeAttr('selected');
        }
       });
  } // END bind_select_click()
  
  
	}; // END cdm_taxontree()
	 
})(jQuery);


/**
 *
 */
$.fn.cdm_taxontree_container_resize = function() {

    var current_w = $(this).parent().width();
    
    // determine max horizontal extent of any children 
    var tree_list = $(this).find('.cdm_taxontree_scroller_y > ul');
    var w = tree_list.css('position', 'absolute').outerWidth({ margin: true });
    tree_list.css('position', 'static');

    // Other Browsers than Firefox
    if( jQuery.browser['msie']) {
      if(jQuery.browser['version'].charAt(0) == '7'){
        w = w + 17;
      }
      if(jQuery.browser['version'].charAt(0) <= '6'){
        return;
      }
    }
    
    if(current_w < w){
      $(this).parent().width(w);
      $(this).children().width(w);
    }
}

/**
 *
 */
$.fn.cdm_taxontree_container_debug_size = function(msg) {

  var out = msg 
     + '<br />    scoll_x: ' + $(this).parent().width()
     + '<br />        container: ' + $(this).width()
     + '<br />            scoll_y: ' + $(this).children().width()
     + '<br />                ul: ' + $(this).find('.cdm_taxontree_scroller_y > ul').width()
     + '<br />';
  $('#DEBUG_JS').append(out);

}

/**
 *
 */
$.fn.cdm_taxontree_magicbox = function() {
  
  // exclude IE6 and lower versions
  if(!(jQuery.browser['msie'] && jQuery.browser['version'].charAt(0) < '7')){

		var container = $(this).parent().parent('div.cdm_taxontree_container');
		if(container[0] == undefined){
		  return;
		}
	
	  container.hover(
		  // --- mouseOver ---- //
			function() {
			  var scroller_x = $(this).parent();
			  var scroller_y = $(this).children('.cdm_taxontree_scroller_y'); 
			    
			  var h = parseFloat(scroller_x.height());
			  var scroll_top = scroller_x.scrollTop();
			  var border_color = scroller_x.css('border-top-color');

			  // store scroll_left of scroller_x so that it can be restored on mouseOut
			  scroller_x.append('<div class="_scrollLeft" style="display: none;" title="'+scroller_x.scrollLeft()+'"></div>');
			  
			  $(this).cdm_taxontree_container_resize();
			  scroller_y.css('overflow-y', 'auto').css('border-color', border_color).scrollTop(scroll_top);
			  scroller_x.css('overflow-y', 'visible').css('overflow-x', 'visible').css('border-color', 'transparent').height(h);
			}
			// ----------------- //
			,    
			// --- mouseOut ---- //
			function() {
				//return; 
				var container = $(this);
				var scroller_x = $(this).parent('.cdm_taxontree_scroller_x');
				var scroller_y = container.children('.cdm_taxontree_scroller_y');
				var border_color = scroller_y.css('border-top-color');
				
				var scroll_top = scroller_y.scrollTop();
				scroller_y.css('overflow-y', 'visible').css('border-color', 'transparent');
				scroller_x.css('overflow-y', 'auto').css('border-color', border_color).width('auto').scrollTop(scroll_top);
				
				// restore scroll_left of scroller_x 
				var scrollLeft = scroller_x.children('._scrollLeft').attr('title');
				scroller_x.scrollLeft(scrollLeft)
				scroller_x.children('._scrollLeft').remove();
		   }
		   // ------------------ //
		);
	}
	// END exclude IE6
	
    
}

$.fn.cdm_taxontree.defaults = {  // set up default options
  widget:                 false,         // true = enable widget mode
  element_name:           'widgetval',  // 
  multiselect:            false,        // true = enable selection of multiple 
};

/* ========================== auto activate ========================== 

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('ul.cdm_taxontree').cdm_taxontree();
  });
}
*/ 