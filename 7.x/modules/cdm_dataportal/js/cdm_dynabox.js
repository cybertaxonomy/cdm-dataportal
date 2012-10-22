/**
 * Expected dom structure:
 *  <li class="dynabox">
      <div class="dynabox_label"><span class="label">Lable Text</span>
      <ul class="dynabox_content"><li> ...... </li></ul>
    </li>
 */


Drupal.cdm_dynaboxAutoAttach = (function ($) {

  var loadDynaContent =  function(event) {
    event.preventDefault(); //Cancel the default action (navigation) of the click.
    var dynabox_content = $(this).toggleClass("dynabox_expanded").parent('.dynabox').find('.dynabox_content').slideToggle("fast");

    var url = dynabox_content.attr('title');

    if(url !== undefined && url.length > 1){
      dynabox_content.removeAttr('title').find('.loading').css( 'display', 'block');
      $.get(url, function(html){
        dynabox_content.find('.loading').remove().end().append(html);
      });
    }
  }

  $('.dynabox').find('.dynabox_content').hide().click(function(event){event.stopPropagation();});

  $('.dynabox a.label').dblclick(loadDynaContent);
  $('.dynabox a.label').click(loadDynaContent);

    //$('li.dynabox> span').click(function(event){event.stopPropagation();});
})(jQuery)



jQuery(document).ready(Drupal.cdm_dynaboxAutoAttach);
