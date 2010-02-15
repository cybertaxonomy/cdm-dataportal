// $Id $
// Author: Obslogic (Mike Smith aka Lionfish)

$(document).ready(function()
{
  var fieldid;
  $('span.expertsdb_addnode_form').hide();

  //the 'select from select box' has been selected
  $('a.expertsdb_addnode_select_link').click(function()
  {
    fieldid=this.id;
    $('select.expertsdb_addnode_select').filter('[@id='+fieldid+']').attr("disabled", false);
    $('input.expertsdb_addnode_source').filter('[@name=expertsdb_addnode_'+fieldid+']').val('');//clear to show not creating a node
    $('span.expertsdb_addnode_form').filter('.'+fieldid).hide();

  });

  //click on the general 'create new' links
  $('span.expertsdb_addnode_links').click(function()
  {
    fieldid=this.id;
    $('select.expertsdb_addnode_select').filter('[@id='+fieldid+']').attr("disabled", true);
    $('select.expertsdb_addnode_select').filter('[@id='+fieldid+']').selectNone();
  });

  //click on particular form type
  $('a.expertsdb_addnode_item').click(function()
  {
    var typeid=this.id;
    $('span.expertsdb_addnode_form').filter('[@id='+typeid+']').filter('.'+fieldid).siblings().hide();
    $('span.expertsdb_addnode_form').filter('[@id='+typeid+']').filter('.'+fieldid).show();
    $('input.expertsdb_addnode_source').filter('[@name=expertsdb_addnode_'+fieldid+']').val(typeid);
  });
});

//unselects every item in 'this'
jQuery.fn.selectNone = function()
{
  this.each(function()
  {
    for (var i=0;i<this.options.length;i++)
    {
      option = this.options[i];
      option.selected = false;
    }
  });
}
