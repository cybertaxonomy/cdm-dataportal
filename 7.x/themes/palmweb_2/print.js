(function ($) {
    $(document).ready(function () { 
        $('#print-button').append('<a href="#print"><img title="Print this page" alt="Print this page " src="/files/cdm_dataportal/print_icon.gif "> Print this page</a>');
        $('#print-button').click(function () {
            window.print();
        });
    });
})(jQuery);
