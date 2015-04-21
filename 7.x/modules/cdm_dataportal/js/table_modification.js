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
    //hide all collapse icons
    jQuery(selector + " .collapse_icon").hide();

    //register click on every summary row
    jQuery(selector + " .summary_row").click(
        function(event){
            jQuery(event.target).parent(".summary_row").next().toggle(500);//toggle detail row when clicking on corresponding summary row
            jQuery(event.target).parent(".summary_row").find(".expand_icon").toggle();//toggle collapse/expand icons
            jQuery(event.target).parent(".summary_row").find(".collapse_icon").toggle();
        })
    //register click on every summary row icon
    jQuery(selector + " .summary_row_icon").click(
        function(event){
            jQuery(event.target).parent().parent(".summary_row").next().toggle(500);//toggle detail row when clicking on corresponding summary row
            jQuery(event.target).parent().parent(".summary_row").find(".expand_icon").toggle();//toggle collapse/expand icons
            jQuery(event.target).parent().parent(".summary_row").find(".collapse_icon").toggle();
        })

        //color summary row when hovering over it
    jQuery(selector + " .summary_row").mouseenter(
        function(event){
            jQuery(event.target).parent(".summary_row").css("background","#FFCC00");
        })

    jQuery(selector + " .summary_row").mouseleave(
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