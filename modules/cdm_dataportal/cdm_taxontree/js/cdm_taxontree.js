// $Id$



(function($){
 /**
  * 
  */
  $.fn.cdm_taxontree = function() {
		
		return this.each(function() {
		  
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
							$.get(url, function(html){
						    parent_li.css('background-image', bg_image_tmp);
								parent_li.append(html).find('ul').cdm_taxontree();
							  // fire resize for parent container
							  var tree_container = parent_li.parents('div.cdm_taxontree_container');
							  tree_container.trigger('resize');
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
		});
	}; // END cdm_taxontree()
	 
})(jQuery);

if (Drupal.jsEnabled) {
  $(document).ready(function() {
  
    $('ul.cdm_taxontree').cdm_taxontree();
    
    $('div.cdm_taxontree_container').resize(
      function(){
        var current_w = $(this).parent().width();
        // determine max horizontal extent of any children 
        var tree_list = $(this).find('.cdm_taxontree_scroller_y > div > ul');
        var w = tree_list.css('position', 'absolute').outerWidth({ margin: true });
        tree_list.css('position', 'static');
        
        if(current_w < w){
	        $(this).parent().width(w);
	        $(this).children().width(w);
	        console.log('resize to '+w);
        } else {
         console.log('kepping old value '+w);
        }
      }
    );
    
    $('div.cdm_taxontree_container').hover(
      
      // mouseOver
      function() {
      
		    var scroller_x = $(this).parent();
		    var scroller_y = $(this).children('.cdm_taxontree_scroller_y');
		    
	        
	      var h = parseFloat(scroller_x.height());
	      var w = $(this).outerWidth({ margin: true });
	      var scroll_top = scroller_x.scrollTop();
	        
	      scroller_y.css('overflow-y', 'auto').css('border-style', 'solid').scrollTop(scroll_top);
	      // store scroll_left of scroller_x so that it can be restored on mouseOut
	      scroller_x.append('<div class="_scrollLeft" style="display: none;" title="'+scroller_x.scrollLeft()+'"></div>');
	      scroller_x.width(w).css('overflow-y', 'visible').css('overflow-x', 'visible').css('border-style', 'none').height(h);
      },
    
      // mouseOut
      function() {
       //return; 
       var container = $(this);
       var scroller_x = $(this).parent('.cdm_taxontree_scroller_x');
       var scroller_y = container.children('.cdm_taxontree_scroller_y');
       
       var scroll_top = $(this).parent('.cdm_taxontree_scroller_x').find('.cdm_taxontree_scroller_y').scrollTop();
	     scroller_y.css('overflow-y', 'visible').css('border-style', 'none');
	     scroller_x.css('overflow-y', 'auto').css('border-style', 'solid').width('auto').scrollTop(scroll_top);  
	     // restore scroll_left of scroller_x 
	     var scrollLeft = scroller_x.children('._scrollLeft').attr('title');
       scroller_x.scrollLeft(scrollLeft).children('._scrollLeft').remove();
	    }
	   );
	   
  });
    
}