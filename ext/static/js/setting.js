profile = ['old', 'new', 'confirm']
$(document).ready(function () {
    $("#update").click(function () {
        $.post("setting", {
            _xsrf: $("input[name='_xsrf']").val(),
            old: $("input[name='oldpassword']").val(),
            new: $("input[name='newpassword']").val(),
            confirm: $("input[name='confirmpassword']").val()
        },
            function (data, state) {
                if (data["errors"] != "Failed") {
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