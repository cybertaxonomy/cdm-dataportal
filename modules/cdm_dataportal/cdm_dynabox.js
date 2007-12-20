// $Id$

/**
 * Expected dom structure:
 *  <li class="dynabox">
      <div class="dynabox_label"><span class="dynabox_toggler">+</span>Lable Text</div>
      <div class="dynabox_content"> ...... </div>
    </li>
 */


Drupal.cdm_dynaboxAutoAttach = function () {

  $('li.dynabox').find('div.dynabox_content').hide();
  $('li.dynabox').find('div.dynabox_label> span.dynabox_toggler').click(
    function () {
     var dynabox_content = jQuery($(this).parents('.dynabox')[0]).find('div.dynabox_content');
     var url = dynabox_content.attr('title');
     dynabox_content.removeAttr('title');
     dynabox_content.slideToggle();
     dynabox_content.find('.loading').css( 'display', 'block');
     $.getJSON(url, function(jsonObj){
        var taxonSTO = jsonObj.root;
        dynabox_content.find('.loading').css( 'display', 'none');
        dynabox_content.append(taxonSTO.name.fullname);
     
     });
    });
}


if (Drupal.jsEnabled) {
  $(document).ready(Drupal.cdm_dynaboxAutoAttach);
}