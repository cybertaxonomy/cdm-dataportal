  jQuery(document).ready(function() {
	  
	  function getFootnoteClassName(object){
		  return '.'+jQuery(object).attr('href').substr(1);
	  }
	  
	  function getFootnoteKeyClassName(object){
		  return '.'+jQuery(object).attr('href').substr(1).replace(/-/gi, '-key-') + ' a';
	  }
	  
	  jQuery('span.footnote-key a').mouseover(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  var fnKeyClassName = getFootnoteKeyClassName(this); 
		  jQuery('.footnote').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
		  jQuery('span.footnote-key a').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
		  jQuery(fnClassName).css('background-color', 'yellow');
		  jQuery(fnKeyClassName).css('background-color', 'yellow');
	  	}
	  ).mouseout(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  var fnKeyClassName = getFootnoteKeyClassName(this); 
		  jQuery(fnClassName).not('.active').css('background-color', 'transparent');
		  jQuery(fnKeyClassName).not('.active').css('background-color', 'transparent');
	  	}
	  ).click(function(e){
		  var fnClassName = getFootnoteClassName(this);
		  var fnKeyClassName = getFootnoteKeyClassName(this); 
		  jQuery('.footnote').css('background-color', 'transparent').removeClass('active'); 
		  jQuery('span.footnote-key a').css('background-color', 'transparent').removeClass('active');
		  jQuery(fnClassName).css('background-color', 'yellow').addClass('active');
		  jQuery(fnKeyClassName).css('background-color', 'yellow').addClass('active');
	  });
	  
  });
