var profile = ['username', 'password', 'email', 'confirm', 'phone']

$(document).ready(function () {
    $("#register").click(function () {
        $.post("register", {
            _xsrf: $("input[name='_xsrf']").val(),
            username: $("input[name='username']").val(),
            password: $("input[name='password']").val(),
            email: $("input[name='email']").val(),
            phone: $("input[name='phone']").val(),
            confirm: $("input[name='confirm']").val()
        },
            function (data, state) {
                if (data["errors"] == "Success") {
                    window.location.href = '/login';
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
            });
    });
});
function keyRegister() {
    if (event.keyCode == 13)
        document.getElementById("register").click();
}