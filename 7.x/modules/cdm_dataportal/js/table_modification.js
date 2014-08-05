/**
 * Adds the functionality to toggle (hide/show) every second row by clicking on the first.
 * All first rows must belong to the class "summary_row"
 * and the second rows to the class "detail_row".
 *
 * @param selector the CSS selector for the table on which this functionality should be applied
 */
function addRowToggle(selector) {

    //hide all detail rows and color them
    jQuery(selector + " .detail_row").hide().css("background","#F9F9F9");

    //register click on every summary row
    jQuery(selector + " .summary_row").click(
        function(event){
            jQuery(event.target).parent(".summary_row").next().toggle(500);//toggle detail row when clicking on corresponding summary row
        })
        //color summary row when hovering over it
        .mouseenter(
        function(event){
            jQuery(event.target).parent(".summary_row").css("background","#FFCC00");
        })
        .mouseleave(
        function(event){
            jQuery(event.target).parent(".summary_row").css("background","");
        })
        //show mouse cursor as a link
        .css('cursor', 'hand').css('cursor', 'pointer');
}
//.hover(//register mouse hover on every summary row
//    function(event){
//        jQuery(event.target).parent(".summary_row").css("background","#FFCC00");
//    }
//    ,function(event){
//        jQuery(event.target).parent(".summary_row").css("background","");
//    })