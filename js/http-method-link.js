/**
 * Java script to allow sending HTTP POST request from normal anchor elements.
 *
 * expected html:
 *
 * <a class="http-POST-link" data-cdm-http-post=""
 */
jQuery(document).ready(function($) {

    $('.http-POST-link').click(function(eventObject){
        eventObject.preventDefault();
        var target = $(eventObject.target);

        var postData = target.attr('data-cdm-http-post');
        var url = target.attr('href');
        var target = target.attr('target');

        ////////////////
        /*
        var contentType = target.attr('type');
        if(!contentType) {
            contentType = 'application/x-www-form-urlencoded';
        }

        $.ajax({
            type: 'POST',
            dataType : 'json',
            dataFilter : function(data, type) {
                altert(type);
                return data;
            },
            url: url,
//            headers: {
//                "Content-Type": contentType
//            },
            contentType: contentType,
            data: postData,
            complete: function(jqXHR, textStatus){
                alert(textStatus);
                var data = jqXHR.responseText;
                var win = window.open();
                win.document.write(data);
            },

        });
        */
        ////////////////////
        var postForm = $('<form id="http-method-link-form" action="' + url + '" method="POST" target="' + target + '"></form>');

        // extract request parameters from post_data
        var tokens = postData.split("&");
        for(var i=0; i < tokens.length; i++){
            pair = tokens[i].split('=');
            postForm.append('<input type="hidden" name="' + pair[0] + '" value="' + urldecode(pair[1]) + '" />');
        }

        $('body').append(postForm);
        postForm.submit();
        $('#http-method-link-form').remove();
        postForm = null;

    });

    function urldecode (str) {
        return decodeURIComponent((str + '').replace(/\+/g, '%20'));
    }
});
