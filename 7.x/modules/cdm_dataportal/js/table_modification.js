/**
 * Adds the functionality to toggle (hide/show) every second row by clicking on the first.
 * All first rows must belong to the class "summary_row"
 * and the second rows to the class "detail_row".
 *
 * @param selector the CSS selector for the table on which this functionality should be applied
 */
function addRowToggle(selector) {

    //hide all detail rows
    //jQuery(selector)[0].getElementsByClassName("detail_row").forEach(hide());
    //var detailRows = jQuery(selector)[0].getElementsByClassName("detail_row");
    //for(var i=0; i < detailRows.length; i++){
    //    detailRows[i].hide();
    //}

    jQuery(selector).click(function(event){
        jQuery("#derivate_details1").toggle(1000);
        //event.currentTarget.getElementsByClassName("detail_row").toggle(1000);
    })
}