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

