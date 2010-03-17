if (Drupal.jsEnabled) {
  $(document).ready($(document).ready(function() {
	  
	  function getFootnoteClassName(object){
		  return '.'+$(object).attr('href').substr(1);
	  }
	  
	  $('a.footnote-key').mouseover(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  $('.footnote').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
		  $(fnClassName).css('background-color', 'yellow');
	  	}
	  ).mouseout(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  $(fnClassName).not('.active').css('background-color', 'transparent');
	  	}
	  ).click(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  $('.footnote').css('background-color', 'transparent').removeClass('active');
		  $(fnClassName).css('background-color', 'yellow').addClass('active');
	  });
	  
  }));
}