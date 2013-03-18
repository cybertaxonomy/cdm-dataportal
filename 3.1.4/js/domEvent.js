// see also https://github.com/geetarista/jquery-plugin-template/blob/master/jquery.plugin-template.js

// the semi-colon before function invocation is a safety net against concatenated
// scripts and/or other plugins which may not be closed properly.
;(function($, document, window, undefined) {

    if( document.domEventListeners === undefined) {
        document.domEventHandlers = [];
    }

    $.fn.triggerElementsAdded = function () {
        var element = $(this);
        document.domEventHandlers.forEach(
                function(handler){
                    handler(element);
                }
           );
    }

    /**
     * can be used in two forms:
     *
     * 1. $('#container').elementsAdded(): triggers an domElementsAdded' event
     *
     * 2. $('#container').elementsAdded(function(event){
     *        // handle the event ...
     *     })
     *     this binds the handler function for the 'domElementsAdded' event.
     */
//    $.fn.elementsAdded = function (handler) {
//
//        var eventType = 'domElementsAdded';
//
//        if(handler === undefined) {
//            $.event.trigger({
//                type: eventType
//            });
//        } else {
//            $(this).bind(eventType, handler)
//        }
//    }
})(jQuery, document, window);

