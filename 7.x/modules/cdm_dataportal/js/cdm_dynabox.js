/**
 * Expected dom structure:
 *  <li class="dynabox">
      <div class="dynabox_label"><span class="label">Label Text</span>
      <ul class="dynabox_content" title="{url-to-content}"><li> ...... </li></ul>
    </li>
 */


// the semi-colon before function invocation is a safety net against concatenated
// scripts and/or other plugins which may not be closed properly.
;(function($){
	this.dynabox = function() {


		$('.dynabox .label').click(function(event) {
			loadContent(event);
		});

		$('.dynabox').find('.dynabox_content').click(function(event){event.stopPropagation();});

		//$('li.dynabox> span').click(function(event){event.stopPropagation();});

		var loadContent = function(event) {
			event.preventDefault(); //Cancel the default action (navigation) of the click.
			var dynabox_content = $(event.target).parent('.dynabox').find('.dynabox_content');

			if(dynabox_content.find('.content').length > 0) {
				dynabox_content.slideToggle("fast");
			} else {
				var url = dynabox_content.attr('title');
				if(url !== undefined && url.length > 1){
					dynabox_content.removeAttr('title').find('.loading').show();
					$.get(url, function(html){
						dynabox_content.find('.loading').remove().end().append('<div class="content">' + html + '</div>');
					});
				}
			}

		};
	}
})(jQuery);



jQuery(document).ready(
		function() {
			dynabox();
		}
);
