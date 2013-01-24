/**
 * Expected dom structure:
 *  <li class="dynabox">
      <div class="dynabox_label"><span class="label">Label Text</span>
      <ul class="dynabox_content" title="{url-to-content}"><li> ...... </li></ul>
    </li>
 */

// the semi-colon before function invocation is a safety net against concatenated
// scripts and/or other plugins which may not be closed properly.
;(function($) {
  this.footnotes = function() {

      function getFootnoteClassName(object){
        return '.'+$(object).attr('href').substr(1);
      }

      function getFootnoteKeyClassName(object){
        return '.'+$(object).attr('href').substr(1).replace(/-/gi, '-key-') + ' a';
      }

      $('span.footnote-key a').mouseover(function(e){
		var fnClassName = getFootnoteClassName(this);
		var fnKeyClassName = getFootnoteKeyClassName(this);
		$('.footnote').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
		$('span.footnote-key a').css('background-color', 'transparent').css('background-color', 'transparent').removeClass('active');
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
        $('span.footnote-key a').css('background-color', 'transparent').removeClass('active');
        $(fnClassName).css('background-color', 'yellow').addClass('active');
        $(fnKeyClassName).css('background-color', 'yellow').addClass('active');
      });
  }
})(jQuery);

jQuery(document).ready(
    function() {
      footnotes();
    }
);
