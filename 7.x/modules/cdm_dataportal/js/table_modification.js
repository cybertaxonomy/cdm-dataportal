/**
 * Adds the functionality to toggle (hide/show) every second row by clicking on the first.
 * All first rows must belong to the class "summary_row"
 * and the second rows to the class "detail_row".
 *
 * @param selector the CSS selector for the table on which this functionality should be applied
 */
function addRowToggle(selector) {

    //hide all detail rows
    jQuery(selector + " .detail_row").hide();

    jQuery(selector + " .summary_row").click(function(event){//register click on every summary row
        jQuery(event.target).parent(".summary_row").next().toggle(500);//toggle detail row when clicking on corresponding summary row
    }).hover(function(event){//register mouse over on every summary row
        jQuery(event.target).parent(".summary_row").css("background","#FFCC00");
    },function(event){//register mouse over on every summary row
        jQuery(event.target).parent(".summary_row").css("background","");
    }).css('cursor', 'hand').css('cursor', 'pointer');//show mouse cursor as a link

}