var profile = ['username', 'password', 'email', 'confirm', 'phone']
$(document).ready(function () {
    $("#update").click(function () {
        $.post("profile", {
            _xsrf: $("input[name='_xsrf']").val(),
            username: $("input[name='username']").val(),
            email: $("input[name='email']").val(),
            phone: $("input[name='phone']").val()
        },
            function (data, state) {
                if (data["errors"] == "Success") {
                    $("#updhint").attr('class', 'alert alert-success')
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
});