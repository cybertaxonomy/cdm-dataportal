

(function($){

	var $progress_container; // and other dom elements?
	$.fn.cdm_ws_progress = function(progress_container_selector, options) {

		var opts = $.extend({},$.fn.cdm_ws_progress.defaults, options);

		var $progress_bar_value, $progress_bar_indicator, $progress_status, $progress_titel;

		var monitorUrl;

		var isRunning = false;

		var showProgress = function(monitor){
			$progress_titel.text(monitor.taskName);
			$progress_bar_value.text(monitor.percentage + "%");
			$progress_bar_indicator.css('width', monitor.percentage + "%");
			if(monitor.failed){
				$progress_status.text("An error occurred");
			} else if (monitor.done) {
				$progress_status.text("Done");
				isRunning = false;
			} else {
				$progress_status.text(monitor.subTask + " [chunk " + monitor.workDone + "/" + monitor.totalWork + "]");
			}
			monitorProgess();
		}

		var monitorProgess = function(jsonpRedirect){
			if(jsonpRedirect != undefined){
				monitorUrl = jsonpRedirect.redirectURL;
			}
			$.ajax({
				url: monitorUrl,
				dataType: "jsonp",
				success: function(data){
					showProgress(data);
				},
			});
		}

		$progress_container = $(progress_container_selector);

		return this.each(function(index) {
			var $this = $(this);

			// creating progressbar and other display lements
			$progress_bar_value = $('<div class="progress_bar_value">0%</div>');
			$progress_bar_indicator = $('<div class="progress_bar_indicator"></div>')
			$progress_bar = $('<div class="progress_bar"></div>').append($progress_bar_indicator).append($progress_bar_value);
			$progress_status = $('<div class="progress_status">initializing ...</div>');
			$progress_titel = $('<h4 class="progress_title">CDM REST serivce - progress</h4>');
			$ws_progress_outer = $('<div class="cdm_ws_progress" id="cdm_ws_progress_' + index + '"></div>').append($progress_titel).append($progress_bar).append($progress_status);

			// styling element
			$progress_bar.css('with', opts.width).css('background-color', opts.background_color).css('height', opts.bar_height);
			$progress_bar_indicator.css('background-color', opts.indicator_color).css('height', opts.bar_height);
			$progress_bar_value.css('text-align', 'center').css('vertical-align', 'middle').css('margin-top', '-'+opts.bar_height);
			$ws_progress_outer.css('border', opts.border).css('padding', opts.padding);
			// >>> DEBUG
			$progress_bar_indicator.css('width', '0%');
			$ws_progress_outer.css('display', 'none');
			// <<<<

			//finally append the progress widget to the container
			$progress_container.append($ws_progress_outer);

			// register onClick for each of the elements
			$this.click(function(event){

				//Cancel the default action (navigation) of the click.
				event.preventDefault();

				// prevent from starting again
				if(!isRunning){

					isRunning = true;

					var url = $this.attr('href');
					$.ajax({
						url: url + '.json',
						dataType: "jsonp",
						success: function(data){
							monitorProgess(data);
						},
					});
						// show progress indicator
						$ws_progress_outer.css('display', 'block');
				}  // END !isRunning
			}); // END click()

		});

	};

	$.fn.cdm_ws_progress.defaults = {// set up default options
			background_color:	"#F3F3F3",
			indicator_color:	"#D9EAF5",
			width: 				"100%",
			bar_height: 		"1.5em",
			border:				"1px solid #D9EAF5",
			padding:			"1em"
	};

})(jQuery);
