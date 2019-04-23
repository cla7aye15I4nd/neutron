$(document).ready(function () {
    $("#login").click(function () {
        $.post("login", {
            _xsrf    : $("input[name='_xsrf']").val(),
            username : $("input[name='username']").val(),
            password : $("input[name='password']").val(),
            remember : $("input[name='remember']").val()
        },
        function(data) {
            if (Object.keys(data).length == 2) {
                var prevUrl = document.referrer;
                if (prevUrl == '' || /.*\/login.*/.test(prevUrl) || /.*\/register.*/.test(prevUrl) || /.*\/reset-password.*/.test(prevUrl)) {
                    prevUrl = '/';
                };
                window.location.href = prevUrl;
            }
            $("#loginhint").text(data);            
            return true;
        });
    }); 
});