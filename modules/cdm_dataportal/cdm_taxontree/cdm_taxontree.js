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
  $(document).ready(function() {$('ul.cdm_taxontree').cdm_taxontree();});
}