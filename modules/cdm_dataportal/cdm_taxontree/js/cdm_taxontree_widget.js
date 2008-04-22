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
  $.fn.cdm_taxontree_widget = function(options) {
		
		var opts = $.extend({},$.fn.cdm_taxontree_widget.defaults, options);
		
		return this.each(function() {
		  
		  // init taxontree

		  $(this).find('ul.cdm_taxontree').cdm_taxontree(true, options);

		  
		  var widget = $(this);
		  var select = $('.cdm_taxontree_widget select');
		  	  
     // #### select.click(function(){
     // ####   $(this).children().attr('selected', 'selected');
     // #### });
     		 
		 // #### select.find('option').each(function() {		  
		 // ####	widget.find('ul.cdm_taxontree li .value_'+$(this).text()).addClass('selected');
		 // #### });
		  
			$(this).find('ul.cdm_taxontree li .widget_select').click(function(event) {
				event.stopPropagation();
			  var value = $(this).attr('title');
			  if(select.children('[value='+value+']').length > 0){
			    // remove from select
			    // #### widget.find('ul.cdm_taxontree li .value_'+value).removeClass('selected');
			    select.children('[value='+value+']').remove();
        } else {
         // add to select
         if(!opts.multiselect){
           // remove all from select
           // #### widget.find('ul.cdm_taxontree li .widget_select').removeClass('selected');
           select.children().remove();
         }
         // #### widget.find('ul.cdm_taxontree li .value_'+value).addClass('selected');
         select.append('<option value="'+value+'" selected="selected">'+value+'</option>')
        }
        
			}); // END click()
		}); // END each(
	}; // END cdm_taxontree_widget() 
	 
})(jQuery);

$.fn.cdm_taxontree_widget.defaults = {  // set up default options
	element_name:           'widgetval',  // 
	multiselect:            false,        //
};

//$(document).ready(function() {$('.cdm_taxontree_widget').cdm_taxontree_widget();});
