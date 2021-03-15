/**
 * Expected dom structure like:
 *   <div class="derived-unit-tree">
 *      <div class="derived-unit-item derived-unit-tree-root ">
 *          <div class="unit-label">(B SP-99999).</div>
 *          <div class="unit-content">
 *          <ul class="specimens derivate_tree">
 *                  <li class="derived-unit-item">
 *                      <div class="unit-label">(B B-923845).</div>
 *                      <div class="unit-content">
 *                  </li>
                    <li class="derived-unit-item">
                        <div class="unit-label">(B DNA-9098080).</div>
                        <div class="unit-content"></div>
                    </li>
             </ul>
        </div>
 *   </div>
 * The plugin function should be bound to outer div with '.derived-unit-tree'
 */
;(function ($, document, window, undefined) {

    $.fn.derivationTree = function () {
        // firebug console stub (avoids errors if firebug is not active)
        if (typeof console === "undefined") {
            console = {
                log: function () {
                }
            };
        }

        var $element = $(this);
        $element.find(".derived-unit-item").each(function () {
            var $listItem = $(this);
            var unitLabel = $listItem.children('.unit-header');
            var unitContent = $listItem.children('.unit-content')
            if(unitContent.length == 0){
                // must be the root unit, we gonna dig one level deeper
                unitContent = $listItem.children('.unit-content-wrapper').children('.unit-content')
            }
            unitContent.hide();
            unitLabel.click(function(){
                unitContent.toggle();
            });
        });

    };
})(jQuery, document, window);
