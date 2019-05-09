var profile = ['froms', 'arrival', 'date']

$(document).ready(function () {
    $("#search").click(function () {
        alert($("input[name='froms']").val() + '\n' + $("input[name='arrival']").val() + '\n' + $("input[name='date']").val() + '\n' + $("input[name='type']").val() + '\n' + $('#available').is(':checked') + '\n' + $('#change').is(':checked') ),
            $.post("trains", {
            _xsrf: $("input[name='_xsrf']").val(),
            froms: $("input[name='froms']").val(),
            arrival: $("input[name='arrival']").val(),
            date: $("input[name='date']").val(),
            type: $("input[name='type']").val(),
            available: $('#available').is(':checked'),
            change: $('#change').is(':checked')
            
        },
            function (data, state) {
                if (data["errors"] != "Failed") {
                    for (var id in profile) {
                        key = profile[id];
                        $("span[name='" + key + "']").text('');
                    }
                    //alert("FUCK1");
                    //ÁÐ³ö»ð³µ
                    
                } else {
                    //alert("FUCK2");
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