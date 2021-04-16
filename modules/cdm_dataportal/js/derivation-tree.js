/**
 * Expected dom structure like:
 *   <div class="item-tree">
 *      <div class="item-list">
 *          <ul>
 *              <li class="derived-unit-item derived-unit-tree-root ">
 *                  <div class="unit-label">(B SP-99999).</div>
 *                  <div class="unit-content">
 *                  <ul class="specimens derivate_tree">
 *                      <li class="derived-unit-item">
 *                          <div class="unit-label">(B B-923845).</div>
 *                          <div class="unit-content">
 *                      </li>
                        <li class="derived-unit-item">
                            <div class="unit-label">(B DNA-9098080).</div>
                            <div class="unit-content"></div>
                        </li>
                    </ul>
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

        let $element = $(this);
        $element.find(".derived-unit-item").each(function () {
            let $listItem = $(this);
            let $itemWrapper = $listItem.children('.item-wrapper');
            let $unitHeader = $itemWrapper.children('.unit-header-wrapper').children('.unit-header');
            let $collapsibleSymbol = $unitHeader.find('.unit-label > .tree-node-symbol-collapsible');

            $unitHeader.find('.page-link').click(function(event){
                event.stopPropagation();
            });
            let $unitContent = $itemWrapper.children('.unit-content');
            $unitContent.hide();
            $unitHeader.click(function(){
                $unitContent.toggle();
            });
            $collapsibleSymbol.click(function(event){
                $listItem.toggleClass('collapsed').children('.item-list').children('ul.derived-unit-item').toggleClass('collapsed');
                    $(this).find('.fa').toggleClass('fa-rotate-90');
                    event.stopPropagation();
                }
            ).hover(function(event){
                event.stopsPropagation(); // allows the css hover effect to be handled but stops from bubbling up to the label
            });

        });

    };
})(jQuery, document, window);
