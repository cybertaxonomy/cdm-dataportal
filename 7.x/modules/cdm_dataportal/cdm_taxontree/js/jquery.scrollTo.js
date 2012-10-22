/**
 * jQuery.ScrollTo
 *
 * Copyright (c) 2008 Ariel Flesler - aflesler(at)gmail(dot)com |
 * http://flesler.blogspot.com Dual licensed under the MIT
 * (http://www.opensource.org/licenses/mit-license.php) and GPL
 * (http://www.opensource.org/licenses/gpl-license.php) licenses. Date:
 * 2/19/2008
 *
 * @projectDescription Easy element scrolling using jQuery. Tested with jQuery
 *                     1.2.1. On FF 2.0.0.11, IE 6, Opera 9.22 and Safari 3
 *                     beta. on Windows.
 *
 * @author Ariel Flesler
 * @version 1.3.3
 *
 * @id jQuery.scrollTo
 * @id jQuery.fn.scrollTo
 * @param {String,
 *          Number, DOMElement, jQuery, Object} target Where to scroll the
 *          matched elements. The different options for target are: - A number
 *          position (will be applied to all axes). - A string position ('44',
 *          '100px', '+=90', etc ) will be applied to all axes - A jQuery/DOM
 *          element ( logically, child of the element to scroll ) - A string
 *          selector, that will be relative to the element to scroll (
 *          'li:eq(2)', etc ) - A hash { top:x, left:y }, x and y can be any
 *          kind of number/string like above.
 * @param {Number}
 *          duration The OVERALL length of the animation, this argument can be
 *          the settings object instead.
 * @param {Object}
 *          settings Hash of settings, optional.
 * @option {String} axis Which axis must be scrolled, use 'x', 'y', 'xy' or
 *         'yx'.
 * @option {Number} duration The OVERALL length of the animation.
 * @option {String} easing The easing method for the animation.
 * @option {Boolean} margin If true, the margin of the target element will be
 *         deducted from the final position.
 * @option {Object, Number} offset Add/deduct from the end position. One number
 *         for both axes or { top:x, left:y }.
 * @option {Object, Number} over Add/deduct the height/width multiplied by
 *         'over', can be { top:x, left:y } when using both axes.
 * @option {Boolean} queue If true, and both axis are given, the 2nd axis will
 *         only be animated after the first one ends.
 * @option {Function} onAfter Function to be called after the scrolling ends.
 * @option {Function} onAfterFirst If queuing is activated, this function will
 *         be called after the first scrolling ends.
 * @return {jQuery} Returns the same jQuery object, for chaining.
 *
 * @example $('div').scrollTo( 340 );
 *
 * @example $('div').scrollTo( '+=340px', { axis:'y' } );
 *
 * @example $('div').scrollTo( 'p.paragraph:eq(2)', 500, { easing:'swing',
 *          queue:true, axis:'xy' } );
 *
 * @example var second_child =
 *          document.getElementById('container').firstChild.nextSibling;
 *          $('#container').scrollTo( second_child, { duration:500, axis:'x',
 *          onAfter:function(){ alert('scrolled!!'); }});
 * 
 * @example $('div').scrollTo( { top: 300, left:'+=200' }, { offset:-20 } );
 *
 * Notes: - jQuery.scrollTo will make the whole window scroll, it accepts the
 * same arguments as jQuery.fn.scrollTo. - If you are interested in animated
 * anchor navigation, check http://jquery.com/plugins/project/LocalScroll. - The
 * options margin, offset and over are ignored, if the target is not a jQuery
 * object or a DOM element. - The option 'queue' won't be taken into account, if
 * only 1 axis is given.
 */

(function($) {

  var $scrollTo = $.scrollTo = function(target, duration, settings) {
    $scrollTo.window().scrollTo(target, duration, settings);
  };

  $scrollTo.defaults = {
    axis : 'y',
    duration : 1
  };

  // Returns the element that needs to be animated to scroll the window.
  $scrollTo.window = function() {
    return $($.browser.safari ? 'body' : 'html');
  };

  $.fn.scrollTo = function(target, duration, settings) {
    if (typeof duration == 'object') {
      settings = duration;
      duration = 0;
    }
    settings = $.extend({}, $scrollTo.defaults, settings);

    // Speed is still recognized for backwards compatibility.
    duration = duration || settings.speed || settings.duration;

    // Make sure the settings are given right.
    settings.queue = settings.queue && settings.axis.length > 1;

    if (settings.queue)
      // Let's keep the overall speed, the same.
      duration /= 2;
    settings.offset = both(settings.offset);
    settings.over = both(settings.over);

    return this.each(function() {
          var elem = this, $elem = $(elem), t = target, toff, attr = {}, win = $elem
              .is('html,body');
          switch (typeof t) {
            // Will pass the regex.
            case 'number':
            case 'string':
              if (/^([+-]=)?\d+(px)?$/.test(t)) {
                t = both(t);
                // We are done,
                break;
              }
              // Relative selector, no break!
              t = $(t, this);
            case 'object':
           // DOM/jQuery
              if (t.is || t.style)
                // Get the real position of the target.
                toff = (t = $(t)).offset();
          }
          $.each(
                  settings.axis.split(''),
                  function(i, axis) {
                    var Pos = axis == 'x' ? 'Left' : 'Top', pos = Pos
                        .toLowerCase(), key = 'scroll' + Pos, act = elem[key], Dim = axis == 'x' ? 'Width'
                        : 'Height', dim = Dim.toLowerCase();
                    // jQuery/DOM
                    if (toff) {
                      attr[key] = toff[pos]
                          + (win ? 0 : act - $elem.offset()[pos]);
                      // If it's a dom element, reduce the margin.
                      if (settings.margin) {
                        attr[key] -= parseInt(t.css('margin' + Pos)) || 0;
                        attr[key] -= parseInt(t.css('border' + Pos + 'Width')) || 0;
                      }
                      // Add/deduct the offset.
                      attr[key] += settings.offset[pos] || 0;

                      if (settings.over[pos])// Scroll to a fraction of its
                        // width/height.
                        attr[key] += t[dim]() * settings.over[pos];
                    } else
                      // Remove the unnecessary 'px'.
                      attr[key] = t[pos];
                    
                    // Number or 'number'.
                    if (/^\d+$/.test(attr[key]))
                      // Check the limits.
                      attr[key] = attr[key] <= 0 ? 0 : Math.min(attr[key],
                          max(Dim));
                    // Queueing each axis is required.
                    if (!i && settings.queue) {
                      // Don't waste time animating, if there's no need.
                      if (act != attr[key])
                        // Intermediate animation.
                        animate(settings.onAfterFirst);
                      // Don't animate this axis again in the next iteration.
                      delete attr[key];
                    }
                  });
          animate(settings.onAfter);

          function animate(callback) {
            $elem.animate(attr, duration, settings.easing, callback
                && function() {
                  callback.call(this, target);
                });
          }
          ;
          function max(Dim) {
            var el = win ? $.browser.opera ? document.body
                : document.documentElement : elem;
            return el['scroll' + Dim] - el['client' + Dim];
          }
          ;
        });
  };

  function both(val) {
    return typeof val == 'object' ? val : {
      top : val,
      left : val
    };
  }
  ;

})(jQuery);