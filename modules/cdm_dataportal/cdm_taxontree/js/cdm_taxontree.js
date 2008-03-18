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
    
    $('div.cdm_taxontree_container').hover(
    
      function() {
		    var container = $(this);
	      var container_w = parseFloat(container.width())
	        + parseFloat(container.css('padding-left'))
	        + parseFloat(container.css('padding-right')) 
	        + parseFloat(container.css('border-left-width'))
	        + parseFloat(container.css('border-right-width'))
	        + parseFloat(container.css('margin-left')) 
	        + parseFloat(container.css('margin-right'));
	      
	      var h = parseFloat($(this).parent('.cdm_taxontree_scroller_x').height());
	      var scroll_y = $(this).parent('.cdm_taxontree_scroller_x').scrollTop();
	      $(this).parent('.cdm_taxontree_scroller_x').find('.cdm_taxontree_scroller_y').css('overflow-y', 'auto').css('border-bottom-style', 'solid').scrollTop(scroll_y).end().width(Math.ceil(container_w)).css('overflow-y', 'visible').height(h);
      },
    
      function() {
        var scroll_y = $(this).parent('.cdm_taxontree_scroller_x').find('.cdm_taxontree_scroller_y').scrollTop();
	      $(this).parent('.cdm_taxontree_scroller_x').find('.cdm_taxontree_scroller_y').css('overflow-y', 'visible').css('border-bottom-style', 'none').end().css('overflow-y', 'auto').width('auto').scrollTop(scroll_y);  
	    }
	   );
	   
  });
    
}