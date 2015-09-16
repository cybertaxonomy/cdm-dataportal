/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - http://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {

	$(document).ready(function() {

		// ======== functions ========== //
		/**
		 *
		 */
		rgbToHsl = function (rgb) {
			var min, max, delta, h, s, l;
		    var r = rgb[0], g = rgb[1], b = rgb[2];
		    min = Math.min(r, Math.min(g, b));
		    max = Math.max(r, Math.max(g, b));
		    delta = max - min;
		    l = (min + max) / 2;
		    s = 0;
		    if (l > 0 && l < 1) {
		      s = delta / (l < 0.5 ? (2 * l) : (2 - 2 * l));
		    }
		    h = 0;
		    if (delta > 0) {
		      if (max == r && max != g) h += (g - b) / delta;
		      if (max == g && max != b) h += (2 + (b - r) / delta);
		      if (max == b && max != r) h += (4 + (r - g) / delta);
		      h /= 6;
		    }
		    return [h, s, l];
		  };

		/**
		 *
		 */
		hexToRgb = function (color) {
		    if (color.length == 7) {
		      return [parseInt('0x' + color.substring(1, 3)) / 255,
				parseInt('0x' + color.substring(3, 5)) / 255,
				parseInt('0x' + color.substring(5, 7)) / 255];
			  }
		else if (color.length == 4) {
			return [parseInt('0x' + color.substring(1, 2)) / 15,
				parseInt('0x' + color.substring(2, 3)) / 15,
				parseInt('0x' + color.substring(3, 4)) / 15];
			}
		};

		/**
		 *
		 */
		colorize = function(element, color){
			element.css('background-color', color);
			rgb = hexToRgb(color);
			hsl = rgbToHsl(rgb);
			element.css({
		        backgroundColor: color,
		        color: hsl[2] > 0.5 ? '#000' : '#fff'
		      });
		};

		// ======== initialize colorpicker ========== //

		var color_input = $('.color-picker');

		color_input.ColorPicker({
			onSubmit: function(hsb, hex, rgb, el) {
				$(el).val('#' + hex);
				colorize($(el), '#' + hex);
				$(el).ColorPickerHide();
			},
			onBeforeShow: function () {
				$(this).ColorPickerSetColor(this.value);
			},
		})
		.bind('keyup', function(){
			$(this).ColorPickerSetColor(this.value);
		});

		// set initial color for each of the inputs
		color_input.each(function(index, element) {
			element = $(element);
			if(element.val()) {
				colorize(element, element.val());
			}
		});
	});

})(jQuery, Drupal, this, this.document);
