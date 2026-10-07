/**
 * Shows a progress bar for a long running CDM web service operation
 * (e.g. reindex, purge) by polling the progress monitor of the operation.
 *
 * The monitor is polled via a url which is passed in, usually the drupal
 * cdm_api/proxy, so that no CORS headers are required at the CDM server.
 *
 * USAGE:
 *   jQuery('#index-progress').cdm_ws_progress_monitor(monitorUrl);
 */
(function($){

  $.fn.cdm_ws_progress_monitor = function(monitorUrl, options) {

    var opts = $.extend({}, $.fn.cdm_ws_progress_monitor.defaults, options);

    return this.each(function() {

      var $progress_bar_value = $('<div class="progress_bar_value">0%</div>');
      var $progress_bar_indicator = $('<div class="progress_bar_indicator"></div>');
      var $progress_bar = $('<div class="progress_bar"></div>').append($progress_bar_indicator).append($progress_bar_value);
      var $progress_titel = $('<h4 class="progress_title">CDM REST service progress</h4>');
      var $progress_status = $('<div class="progress_status">waiting ...</div>');
      var $ws_progress_outer = $('<div class="cdm_ws_progress"></div>').append($progress_titel).append($progress_bar).append($progress_status);

      $progress_bar.css('width', opts.width).css('background-color', opts.background_color).css('height', opts.bar_height);
      $progress_bar_indicator.css('background-color', opts.indicator_color).css('height', opts.bar_height).css('width', '0%');
      $progress_bar_value.css('text-align', 'center').css('vertical-align', 'middle').css('margin-top', '-' + opts.bar_height);
      $ws_progress_outer.css('border', opts.border).css('padding', opts.padding);

      $(this).append($ws_progress_outer);

      var showProgress = function(monitor) {
        $progress_titel.text(monitor.taskName);
        var percentTwoDecimalDigits = Math.round(monitor.percentage * 100) / 100;
        $progress_bar_value.text(percentTwoDecimalDigits + "%");
        $progress_bar_indicator.css('width', percentTwoDecimalDigits + "%");
        if (monitor.failed) {
          $progress_status.html('<span class="error">An error occurred</span>');
        } else if (monitor.done) {
          $progress_status.text("Done");
        } else {
          $progress_status.text(monitor.subTask + " [work ticks: " + (Math.round(monitor.workDone * 100) / 100) + "/" + monitor.totalWork + "]");
          window.setTimeout(poll, opts.poll_interval_ms);
        }
      };

      var poll = function() {
        $.ajax({
          url: monitorUrl,
          dataType: 'json',
          cache: false,
          success: showProgress,
          error: function(xhr, text, error) {
            $progress_status.html('<span class="error">Error while retrieving the progress: ' + text + '</span>');
          }
        });
      };

      poll();
    });
  };

  $.fn.cdm_ws_progress_monitor.defaults = {
    background_color: "#F3F3F3",
    indicator_color:  "#D9EAF5",
    width:            "100%",
    bar_height:       "1.5em",
    border:           "1px solid #D9EAF5",
    padding:          "1em",
    poll_interval_ms: 2000
  };

})(jQuery);
