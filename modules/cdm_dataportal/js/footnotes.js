if (Drupal.jsEnabled) {
  $(document).ready(function() {
	  
	  function getFootnoteClassName(object){
		  return '.'+$(object).attr('href').substr(1);
	  }
	  
	  function getFootnoteKeyClassName(object){
		  return '.'+$(object).attr('href').substr(1).replace(/-/gi, '-key-');
	  }
	  
	  $('a.footnote-key').mouseover(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  var fnKeyClassName = getFootnoteKeyClassName(this); 
		  $('.footnote').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
		  $('a.footnote-key').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
		  $(fnClassName).css('background-color', 'yellow');
		  $(fnKeyClassName).css('background-color', 'yellow');
	  	}
	  ).mouseout(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  var fnKeyClassName = getFootnoteKeyClassName(this); 
		  $(fnClassName).not('.active').css('background-color', 'transparent');
		  $(fnKeyClassName).not('.active').css('background-color', 'transparent');
	  	}
	  ).click(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  var fnKeyClassName = getFootnoteKeyClassName(this); 
		  $('.footnote').css('background-color', 'transparent').removeClass('active'); 
		  $('a.footnote-key').css('background-color', 'transparent').removeClass('active');
		  $(fnClassName).css('background-color', 'yellow').addClass('active');
		  $(fnKeyClassName).css('background-color', 'yellow').addClass('active');
	  });
	  
  }));
}