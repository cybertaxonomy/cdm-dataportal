// $Id$

(function($){
 /**
  * 
  */
  $.fn.cdm_taxontree_widget = function(options) {
		
		var opts = $.extend({},$.fn.cdm_taxontree_widget.defaults, options);
		
		return this.each(function() {
		  
		  // init taxontree
		  $(this).find('ul.cdm_taxontree').cdm_taxontree(true, options);

		  var widget = $(this);
		  var optionList = $('.cdm_taxontree_widget select');
		  	  	  
		  bind_select_click(optionList, $(this).find('ul.cdm_taxontree'), opts.multiselect);
		  	  	  
			/*$(this).find('ul.cdm_taxontree li .widget_select').click(function(event){
		    event.stopPropagation();
		    var value = $(this).attr('title');
        if(select.children('[value='+value+']').length > 0){
          // remove from select
          select.children('[value='+value+']').remove();
        } else {
         // add to select
         if(!opts.multiselect){
           // remove all from select
           select.children().remove();
         }
         select.append('<option value="'+value+'" selected="selected">'+value+'</option>')
        }
			 }); // END click()
			 */
		}); // END each(		
	}; // END cdm_taxontree_widget() 
		 
	function bind_select_click(optionList, treelist, isMultiselect){
	 
	   treelist.find('li .widget_select').click(function(event){
        event.stopPropagation();
        var value = $(this).attr('title');
        if(optionList.children('[value='+value+']').length > 0){
          // remove from select
          optionList.children('[value='+value+']').remove();
        } else {
         // add to select
         if(!isMultiselect){
           // remove all from select
           optionList.children().remove();
         }
         optionList.append('<option value="'+value+'" selected="selected">'+value+'</option>')
        }
       });
	 
	} // END bind_select_click()
	 
})(jQuery);

$.fn.cdm_taxontree_widget.defaults = {  // set up default options
	element_name:           'widgetval',  // 
	multiselect:            false,        //
};

//$(document).ready(function() {$('.cdm_taxontree_widget').cdm_taxontree_widget();});
