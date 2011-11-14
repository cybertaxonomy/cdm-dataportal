$(function(){
  // Hover emulation for IE 6.
  if ($.browser.msie) {
    $('.main-menu li').hover(function() {
      $(this).addClass('iehover');
    }, function() {
      $(this).removeClass('iehover');
    });
  }
});

// && parseInt(jQuery.browser.version) == 6