// $id$
function tinytaxalterroot(){
  var tinytaxReturn = function (data) {
    var returnHtml = Drupal.parseJson(data);
    $('#tinytaxroot-'+returnHtml['vid']).html(returnHtml['html']);
    $('a.tinytaxlink').click(tinytaxalterroot);
  }
  $.get(this.href, null, tinytaxReturn);
  return false
}

function tinytax_cdm_alterroot(){
  var tinytaxReturn = function (data) {
    var returnHtml = Drupal.parseJson(data);
    $('#tinytaxroot-cdm').html(returnHtml['html']);
    $('a.tinytax_cdm_link').click(tinytax_cdm_alterroot);
  }
  $.get(this.href, null, tinytaxReturn);
  return false
}

if (Drupal.jsEnabled) {
  $(document).ready(function(){
    $('a.tinytaxlink').click(tinytaxalterroot);
    $('a.tinytax_cdm_link').click(tinytax_cdm_alterroot);
  });
}