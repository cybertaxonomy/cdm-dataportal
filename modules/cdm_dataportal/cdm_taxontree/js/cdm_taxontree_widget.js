// $Id$



(function($){
 /**
  * 
  */
  $.fn.cdm_taxontree_widget = function() {
		
		return this.each(function() {
		
		  var widget = $(this);
		  var listing = $(this).children('ul.listing');
		  
		  $(this).find('ul.cdm_taxontree li .widget_select').each(function() {		  
				// is it in listing of selected?
				var value = $(this).attr('title');
			  if(listing.children('input .value_'+value)){
			   $(this).addClass('selected');
			  }
		  });
		  
			$(this).find('ul.cdm_taxontree li .widget_select').click(function(event) {
				event.stopPropagation();
			  var value = $(this).attr('title');
			  if(listing.find('input .value_'+value).length > 0){
			    // remove from listing
			    $(this).removeClass('selected');
			    listing.children('input .value_'+value).parent().remove();
			   
        } else {
          // add to listing
         $(this).addClass('selected');
         listing.children().append('<li><input type="text" value="'+value+'"/></li>')
        }
        
			}); // END click()
		}); // END each(
	}; // END cdm_taxontree_widget()
	 
})(jQuery);


$(document).ready(function() {$('.cdm_taxontree_widget').cdm_taxontree_widget();});
