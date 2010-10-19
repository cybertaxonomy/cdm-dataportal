// $Id$

/**
 * Expected dom structure:
 *  <li class="dynabox">
      <div class="dynabox_label"><span class="label">Lable Text</span>
      <ul class="dynabox_content"><li> ...... </li></ul>
    </li>
 */


Drupal.cdm_dynaboxAutoAttach = function () {

  $('.dynabox').find('.dynabox_content').hide().click(function(event){event.stopPropagation();});
  $('.dynabox span.label').click(
    function () {
      var dynabox_content = $(this).toggleClass("dynabox_expanded").parent('.dynabox').find('.dynabox_content').slideToggle("fast");

      var url = dynabox_content.attr('title');
      
      if(url != undefined){
        dynabox_content.removeAttr('title').find('.loading').css( 'display', 'block');
	      $.get(url, function(html){
            dynabox_content.find('.loading').remove().end().append(html);
          });
      }
      
    });
    //$('li.dynabox> span').click(function(event){event.stopPropagation();});
}


if (Drupal.jsEnabled) {
  $(document).ready(Drupal.cdm_dynaboxAutoAttach);
}