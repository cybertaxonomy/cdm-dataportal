var fileDownloadCheckTimer;
/**
 * function to block the UI as 
 * long as the download have not started yet,
 * in order to not start several requests at
 * the same time.
 * 
 */
function blockUIForDownload() {
	
  var token = new Date().getTime()/1000|0;//'1234'; //use the current timestamp as the token value
  jQuery('#downloadTokenValueId').val(token);
  jQuery.blockUI( { message:'</br><h1><img src="../css/jquery-ui/images/ajax-loader.png">Please wait until the file is ready to download...</h1><br/>'});
  fileDownloadCheckTimer = window.setInterval(function () {
    var cookieValue = jQuery.cookie('fileDownloadToken');
    if (cookieValue == token)
     finishDownload();
  }, 1000);
}

function finishDownload() {
  window.clearInterval(fileDownloadCheckTimer);
  jQuery.cookie('fileDownloadToken', null); //clears this cookie value
  jQuery.unblockUI();
}

/**
 * 
 * function validates combobox 
 * and if selected item is empty,
 * it jumps back to default value. 
 * 
 */
function validateForm(){
	var select = document.getElementById("edit-combobox");
	if(select.value==null || select.value == -1|| select.value==""){
		//alert('Please select a valid classification');
		//jQuery('#edit-combobox').addClass("error");
		select.selectedIndex = 0;
	}
}


/**
 *  function to select & deselect all
 *  checkboxes in one fieldset.
 */
jQuery(function () {
    jQuery("#checkall").click(function () {
        jQuery(this).parents("fieldset:eq(0)").find(":checkbox").attr("checked", this.checked);
    });
});

/**
 * function to sort all checkboxes
 * in a alpanumerical way
 */

//var elements = jQuery('input');
//var sorted = jQuery(jQuery(elements).toArray().sort(function(a, b) {
//  return a.value > b.value;
//}));
//
//jQuery(elements).each(function(i) {
//	jQuery(this).after(jQuery(sorted).eq(i));
//});