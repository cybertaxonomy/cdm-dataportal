function CacheBot(searchTaxaUrl, taxonPageUrl, pageSize, progressCallback, readyCallback, doneCallback, errorCallback){

	var taxonPageUrl = taxonPageUrl;
	var searchTaxaUrl = searchTaxaUrl;
	var pageSize;

	var progress = 0;
	var i = 0;
	var doRun = false;

	var dataPager = null;

//	this.elapsedMillies = 0;
//	this.estimatedMillies = 0;

	/**
	 * Log to the Firebug Console if Firebug is available
	 * @param msg the massage to show in the firebug Console
	 * @return
	 */
	var log = function(msg){
		if (window.console && window.console.firebug){
			console.log(msg);
		}
	}

	/**
	 *
	 * @param data
	 * @param statusText
	 * @return
	 */
	var init = function(data, statusText){
	 	dataPager = data;
	 	if(dataPager != undefined){
	 		progressCallback(progress, null, null, "Ready!");
	 	}
	 	if(readyCallback != undefined){
	 		readyCallback();
	 	}
	 	elapsedMillies = 0;
	 	estimatedMillies = 0;

	};

	/**
	 *
	 * @param callback
	 * @return
	 */
	var requestNextDataPage = function(callback){
		var uri = searchTaxaUrl + escape("&pageSize=" + pageSize);
		if(dataPager != null){
			uri += escape("&pageNumber="+ dataPager.nextIndex);
		}
		log('page->'+uri);
		$.ajax({
			url: uri,
			dataType: 'json',
			cache: false, // browser will not cache
			success: callback,
			error: function(XMLHttpRequest, statusText ){
					stop();
					errorCallback(statusText, 'A network error occurred, please start again.', unescape(uri), true);
				}
		});
	}

	/**
	 *
	 */
	this.run = function(lastMillies){

		 //
		var now = new Date();
		var nowMillies = now.getTime();
		if(lastMillies != undefined){
			elapsedMillies += (nowMillies - lastMillies);
			estimatedMillies = (elapsedMillies / i) * dataPager.count;
		}

		var parent = this;
		// get next page of data
		if( i > dataPager.lastRecord - 1 && i < dataPager.count && dataPager.nextIndex != undefined){
			requestNextDataPage(function(data, statusText){
				if(statusText == 'success'){
					dataPager = data;
					parent.run(nowMillies);
				} else {
					parent.stop();
					readyCallback(statusText);
				}

			});
		} else {
			// update the progress callback
			var p = (i / dataPager.count);
			if(p != progress){
				progress = p;
				progressCallback(progress, new Date(elapsedMillies), new Date(estimatedMillies));
			}
			// get new taxon page
			if (doRun == true && i++ < dataPager.count){
				var ri = i - (dataPager.firstRecord);
				var taxon = dataPager.records[ri];
				var taxonUrl = taxonPageUrl + taxon.uuid + '/all';
				log('taxon->'+taxonUrl);
				$.ajax({
					url: taxonUrl,
					cache: false, // browser will not cache
					complete: function(xmlHttpRequest, statusText){
							if(xmlHttpRequest.status != 200){
								errorCallback(statusText, null, unescape(taxonUrl), false);
							}
							parent.run(nowMillies);
						}
				});
				taxon = null;
			} else if(i == dataPager.count) {
				// DONE!
				if(doneCallback != undefined){
					doneCallback();
			 	}
			} else if(doRun == false){
				readyCallback();
			}
		}
	};

	/**
	 *
	 */
	this.start =  function(){
			doRun = true;
			this.run();
		};

	/**
	 *
	 */
    this.stop = function(){
			doRun = false;
		};

	// get list of taxa and initialize the Bot
	progressCallback(0, null, null, "Initializing, please wait ...");
	requestNextDataPage(function(data, statusText){
		init(data, statusText);
	});
}

// -----------------------------------------------------------------------------

$(document).ready(function() {

	var searchTaxaUrl = $('#cdm-settings-cache [name=searchTaxaUrl]').val();
	var taxonPageUrl = $('#cdm-settings-cache [name=taxonPageUrl]').val();

	$('#cdm-settings-cache [name=start]').attr('disabled', 'disabled');
	$('#cdm-settings-cache [name=stop]').attr('disabled', 'disabled');
	$('#cdm-settings-cache #progress').css({background: '#012456', color: '#349AAF', fontSize: '100%', padding: '10px'})
    $('#cdm-settings-cache #progress').html(
    		'<div id="usermessage" style="font-size:95%; width: 50%; float:right; font-weight:light; padding:10px; border-left: 1px solid; height: 3.9em;"></div>'
    		+'<div id="counter" style="font-size:300%; padding:10px;"></div>'
    		+'<div id="time" style="clear:both; border-top:1px solid #349AAF;"></div>'
    		+'<div id="log" style="border-top:1px solid #349AAF; font-size:85%; overflow: auto; white-space:nowrap;"></div>');

	var formatTime = function(date){
		var h = parseInt(date.getTime() / 1000 / 60 / 60);
		var m = parseInt( (date.getTime() / 1000/ 60) % 60);
		var s = parseInt( (date.getTime() / 1000 ) % 60);
		return '' + h +'h '+ m +'m '+ s +'s ';
	}


	var progressCallback = function(progress, elapsedTime, estimatedTime, userMessage){
		var percent = Math.floor(progress * 10000) / 100;
		$('#counter').text(percent + '%');
		if(elapsedTime != null){
			var timehtml =
				'elapsed time: ' + formatTime(elapsedTime)
				+ '<br />estimated time: ' + formatTime(estimatedTime)
				+ '<br />remainig time: ' + formatTime(new Date(estimatedTime.getTime() - elapsedTime.getTime()));
			$('#time').html(timehtml);
		}
		if(userMessage == undefined || userMessage == null ){
			userMessage = '';
		}
		$('#usermessage').html(userMessage);

	}

	var readyCallback = function(message){
		$('#cdm-settings-cache [name=start]').removeAttr('disabled');
		if(message != undefined){
			$('#cdm-settings-cache').append('<div class="error">'+message+'</div>');

		}
	}

	var doneCallback = function(progress){
		var percent = Math.floor(progress * 10000) / 100;
		$('#cdm-settings-cache #progress').text('DONE');
		$('#cdm-settings-cache [name=stop]').removeAttr('disabled');
	}

	var errorCallback = function(errorMessage, userMessage, taxonUrl, doStop){
		var logentry = '<div>' + errorMessage + ' : ' + taxonUrl + '' + '</div>';
		if($('#log div').length == 0){
			$('#log').html(logentry);
		} else {
			$('#log div:last').append(logentry)
		}
		if(userMessage == undefined || userMessage == null ){
			userMessage = '';
		}
		$('#usermessage').html(userMessage);
		if(doStop){
			$('#cdm-settings-cache [name=stop]').attr('disabled', 'disabled');
			$('#cdm-settings-cache [name=start]').removeAttr('disabled');
		}
	}

	var cacheBot = new CacheBot(searchTaxaUrl, taxonPageUrl, 25, progressCallback, readyCallback, doneCallback, errorCallback);

	$('#cdm-settings-cache [name=start]').click(function(){
		cacheBot.start();
		$('#cdm-settings-cache [name=start]').attr('disabled', 'disabled');
		$('#cdm-settings-cache [name=stop]').removeAttr('disabled');
		$('#usermessage').html(' ');
	}).attr('disabled', 'disabled');
	$('#cdm-settings-cache [name=stop]').click(function(){
		cacheBot.stop();
		$('#cdm-settings-cache [name=stop]').attr('disabled', 'disabled');
	}).attr('disabled', 'disabled');
});