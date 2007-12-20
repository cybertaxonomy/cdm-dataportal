// $Id$

/**
 * Expected dom structure:
 *  <li class="dynabox">
      <div class="dynabox_label"><span class="label">Lable Text</span>
      <ul class="dynabox_content"><li> ...... </li></ul>
    </li>
 */


Drupal.cdm_dynaboxAutoAttach = function () {

  $('li.dynabox').find('.dynabox_content').hide();
  $('li.dynabox').click(
    function () {
      var dynabox_content = $(this).toggleClass("dynabox_expanded").find('.dynabox_content').slideToggle("fast");

      var url = dynabox_content.attr('title');
      
      if(url != undefined){
        dynabox_content.removeAttr('title').find('.loading').css( 'display', 'block');
	      $.get(url, function(html){
          dynabox_content.find('.loading').remove().end().append(html);
        });
      }
      
    });
    $('li.dynabox> span').click(function(){return false;});
}


if (Drupal.jsEnabled) {
  $(document).ready(Drupal.cdm_dynaboxAutoAttach);
}