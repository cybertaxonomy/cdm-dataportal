// $Id$

(function($){
 /**
  * 
  */
 $.fn.cdm_taxontree = function() {
 
    return this.each(function() {
      
      $(this).find('li').click(function(event) {
       event.stopPropagation();
      
        if($(this).hasClass('collapsed')){
          
					var parent_li = $(this);
					var bindChildren = ($(this).find('ul').length == 0);
	        if(bindChildren){
						var url = parent_li.attr('title');
						if(url != undefined){
						  parent_li.children().css('color', '#ff0000');
						  parent_li.removeAttr('title');
						  $.get(url, function(html){
						    child_ul = parent_li.append(html).find('ul');
						    child_ul.cdm_taxontree();
						  });
						}
			   } 
			   $(this).removeClass('collapsed').addClass('expanded').children('ul').css('display', 'block');
			  } else if($(this).hasClass('expanded')){
				  $(this).removeClass('expanded').addClass('collapsed').children('ul').css('display', 'none');
			  } 
		  }); // END click()
   });
 }; // END cdm_taxontree()
 
 
})(jQuery);

if (Drupal.jsEnabled) {
  $(document).ready(function() {$('ul.cdm_taxontree').cdm_taxontree();});
}