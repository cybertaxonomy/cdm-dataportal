/**
 * Expected dom structure:
 *  '<div class="ahah-content" rel="'.$cdm_proxy_url.'"><span class="loading">Loading ....</span></div>';
 */

Drupal.ahahContentAutoAttach = (function ($) {
    
    $(".ahah-content").each(function(i){
      var ahahContent = $(this);
      url = ahahContent.attr("rel");
      ahahContent.removeAttr('rel').find('.loading').css('display', 'block');
      
      $.get(url, function(html){
        ahahContent.find('.loading').remove();
        ahahContent.append(html);
        });
      });
    
      // register with lightbox etc ...
      $('body').bind('overflow', function(event){
        
        var ahah_content_set = $(event.target).find('.ahah-content');
        if(ahah_content_set != undefined){
          $(ahah_content_set).each(function(i){
            var ahah_content = $(this);
            var url = ahah_content.attr('rel');
            if(url != undefined){
              ahah_content.removeAttr('rel').find('.loading').css('display', 'block');
              $.get(url, function(html){
                ahah_content.find('.loading').remove().end().append(html);
                });
            }
          })
          
        }
      });
      
    }(jQuery))
  


  jQuery(document).ready(Drupal.ahahContentAutoAttach);
