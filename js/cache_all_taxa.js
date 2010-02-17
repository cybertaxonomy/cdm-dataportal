function CacheBot(searchTaxaUrl, taxonPageUrl, pageSize, progressCallback, readyCallback, doneCallback){
		
	var taxonPageUrl = taxonPageUrl;
	var searchTaxaUrl = searchTaxaUrl;
	var pageSize;
	
	var progress = 0;
	var i = 0;
	var doRun = false;
	
	var dataPager = null;
	
	this.elapsedMillies = 0;
	this.estimatedMillies = 0;
	
	/**
	 * 
	 */
	var init = function(data, statusText){
	 	dataPager = data;
	 	if(dataPager != undefined){
	 		progressCallback(progress, null, null);
	 	}
	 	if(readyCallback != undefined){
	 		readyCallback();
	 	}
	 	elapsedMillies = 0;
	};
	
	/**
	 * 
	 */
	var requestNextDataPage = function(callback){
		var uri = searchTaxaUrl + escape("&pageSize=" + pageSize);
		if(dataPager != null){
			uri += escape("&page="+ dataPager.nextIndex);
		}
		$.getJSON(uri, callback);
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
		if( i > dataPager.lastRecord - 1 && dataPager.nextIndex != undefined){
			requestNextDataPage(function(data, statusText){
				dataPager = data;
				parent.run(nowMillies);
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
				$.get(taxonPageUrl + taxon.uuid + '/all', function(data, textStatus, XMLHttpRequest){
					if(textStatus == 'success'){
						parent.run(nowMillies);
					} else {
						parent.stop();
						readyCallback();
						
					}
				});	
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
	requestNextDataPage(function(data, statusText){
		init(data, statusText);
	});
}


$(document).ready(function() {

	var searchTaxaUrl = $('#cache_site [name=searchTaxaUrl]').val();
	var taxonPageUrl = $('#cache_site [name=taxonPageUrl]').val();
	
	$('#cache_site [name=start]').attr('disabled', 'disabled');
	$('#cache_site [name=stop]').attr('disabled', 'disabled');
	$('#cache_site #progress').css({background: '#012456', color: '#349AAF', fontSize: '100%', fontFamily: 'monospace', padding: '10px'})

	var formatTime = function(date){
		var h = parseInt(date.getTime() / 1000 / 60 / 60);
		var m = parseInt( (date.getTime() / 1000/ 60) % 60);
		var s = parseInt( (date.getTime() / 1000 ) % 60);
		return '' + h +'h '+ m +'m '+ s +'s ';
	}

	
	var progressCallback = function(progress, elapsedTime, estimatedTime){
		var percent = Math.floor(progress * 10000) / 100;
		var html = '<span style="font-size:300%">' + percent + '%</span>';
		if(elapsedTime != null){
			html += '<br />elapsed time: ' + formatTime(elapsedTime) 
			+ '<br />estimated time: ' + formatTime(estimatedTime);
		}
		$('#cache_site #progress').html(html);
	}
	
	var readyCallback = function(progress){
		$('#cache_site [name=start]').removeAttr('disabled');
	}
	
	var doneCallback = function(progress){
		var percent = Math.floor(progress * 10000) / 100;
		$('#cache_site #progress').text('DONE');
		$('#cache_site [name=stop]').removeAttr('disabled');
	}
	
	var cacheBot = new CacheBot(searchTaxaUrl, taxonPageUrl, 3, progressCallback, readyCallback, doneCallback);

	$('#cache_site [name=start]').click(function(){
		cacheBot.start();
		$('#cache_site [name=start]').attr('disabled', 'disabled');
		$('#cache_site [name=stop]').removeAttr('disabled');
	}).attr('disabled', 'disabled');
	$('#cache_site [name=stop]').click(function(){
		cacheBot.stop();
		$('#cache_site [name=stop]').attr('disabled', 'disabled');
	}).attr('disabled', 'disabled');
});