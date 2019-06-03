profile = ['code']
$(document).ready(function () {
    $("#check").click(function () {
        $.post("email", {
            _xsrf: $("input[name='_xsrf']").val(),
            code: $("input[name='code']").val(),
            case: 0
        },
            function (data, state) {
                if (data["errors"] != "Failed") {
                    alert("Success! Your email has been verified.");
                    window.location.href="/"
                    //$("#updhint").attr('class', 'alert alert-success')
                } else {
                    for (var id in profile) {
                        key = profile[id];
                        if (data[key] == undefined) {
                            $("span[name='" + key + "']").text('');
                        } else {
                            $("span[name='" + key + "']").text('* ' + data[key]);
                        }
                    }
                }
                return true;
            });
    });
    $("#resend").click(function () {
        $.post("email", {
            _xsrf: $("input[name='_xsrf']").val(),
            case: 1
        },
            function (data, state) {
                if (data["errors"] != "Failed") {
                    $("#updhint").attr('class', 'alert alert-success')
                } else {
                    alert("Sorry, but we have no money to buy an email service. So we don't have the ability to send emails too many times.");
                }
                return true;
            });
    });
});