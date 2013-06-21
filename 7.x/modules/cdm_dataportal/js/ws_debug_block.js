/**
 * Java script to setup and anable the cdm webservice debug block.
 * The cdm webservice debug block is implemented in the cdm_api submodule
 */
jQuery(document).ready(function($) {

    $('#block-cdm-api-cdm-ws-debug .content').hide(); // hide parent element

    $('#cdm-ws-debug-table').dataTable({
        "bPaginate" : false,
        "bFilter" : true,
        "bSort" : true,
        "bInfo" : true,
        "bProcessing" : true,
        "bStateSave " : true,
        "bRegex" : true, // the search string will be treated as a regular expression
        "aaSorting": [[ 1, "asc" ]], // initially sort by time
        "fnFooterCallback": function( nFoot, aData, iStart, iEnd, aiDisplay ) {

            var colums_to_sum_up = {0: "Total:", 2: "doSum", 3: "doSum", 4: "doSum"};

            $(nFoot).find('th').each(function(colIndex){
                var newHtml = '';
                if(colums_to_sum_up[colIndex] == "doSum"){
                    sum = 0;
                    for(var rowIndex = iStart; rowIndex < iEnd; rowIndex++){
                        sum += Number(aData[rowIndex][colIndex]);
                    }
                    newHtml = sum;
                } else if(colums_to_sum_up[colIndex]) {
                    newHtml = colums_to_sum_up[colIndex];
                }
                $(this).html(newHtml);
            });
          },
        "oLanguage": {
              "sSearch": "Filter table rows (search string will be treated as a regular expression):"
        }
    });

    $('#block-cdm-api-cdm-ws-debug .title').css('cursor', 'pointer').colorbox({
        "href" : '#cdm-ws-debug-table_wrapper', // wrapper created by dataTable()
        "inline" : true,
        "width" : '90%',
        "height" : '90%'
    });
});