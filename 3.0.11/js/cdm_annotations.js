

Drupal.cdm_annotationsAutoAttach = function(){

	$('span.annotation_toggle').click( function(){
	
		var spanElement = $(this);
		var annotation_box = $(this).parent().find('.annotation_box');;
			
	
		function getAnnotations (){
			var url = spanElement.attr('rel');
			
			if(url != undefined){
				$.get(url, displayAnnotations);
			}
		}
		
		function displayAnnotations(html){
			annotation_box.empty().append(html);
			var form = annotation_box.find('.annotation_create').find('form');
			
			form.submit(function(){
			
				var options = {'success' : getAnnotations};
			
				$(this).ajaxSubmit(options);
				//alert ($(this).formSerialize());
				return false;
			});
			annotation_box.show();//.slideToggle("fast");
		}
		
		getAnnotations();
		
	});
		
}


if (Drupal.jsEnabled) {
  $(document).ready(Drupal.cdm_annotationsAutoAttach);
}