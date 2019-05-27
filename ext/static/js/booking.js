$(document).ready(function () {
    $.post("booking", {
        _xsrf: $("input[name='_xsrf']").val(),
    },
        function (data, state) {
            if (data["errors"] == "Nothing") {

                $('#trainlist').empty();
                $('#trainlist').append("<div class='no-result'><div class='center-brick'><i class='s - icon icon - warning'></i><h2 class='tit'>You don't have any bookings or we can't access your bookings at this time.</h2></div></div>");
            }
            else {

                //ÁĞ³ö»ğ³µ

                $('#trainlist').empty();

                var line = new Array();
                var n = data["cnt"];
                for (var i = 0; i < n; i++) {
                    var ii = i.toString();

                    line[0] = "<div class='train-result - list'><ul class='result-list'><li class='result-item  clearfix'>";
                    line[1] = "<h4 class='train-num'>" + data["code" + ii] + "</h4>";//!!!
                    line[2] = "<div class='train-route clearfix'><div class='train-from clearfix'><span class='line-start'><i class='c-icon icon-start-point'></i></span>";
                    line[3] = "<span class='train-time time-start'>" + data["start" + ii] + "</span><span class='train-station station-from'>" + data["startpos" + ii] + "</span>";//!!!
                    line[4] = "</div><div class='train-to clearfix'><span class='line-end'><i class='c-icon icon-end-point'></i></span>";
                    line[5] = "<span class='train-time time-end'>" + data["end" + ii] + "</span><span class='train-station station-to'>" + data["endpos" + ii] + "</span>";//!!!
                    line[6] = "</div></div><div class='train-duration'>" + data["time" + ii] + "</div><ul class='train-seat'>";//!!!

                    var m = data["seatcnt" + ii];
                    for (var j = 0; j < m; j++) {
                        var ln = 7 + (j * 4);
                        var jj = j.toString();
                        line[ln] = "<li class=' '><span class='class-type'>" + data["seattype" + ii + "|" + jj] + "</span><span class='c-price'>";//!!!
                        line[ln + 1] = "<em class='price-num'>" + data["seatdata" + ii + "|" + jj] + "</em>";//!!!
                        line[ln + 2] = "";//!!!
                        line[ln + 3] = "</span><button class='c-btn btn-key  btn-book-avalible' name='refund_ticket' href='javascript:;' value='" + "refund_ticket " + $("#userid").val() + " 1 " + data["code" + ii] + " " + data["startpos" + ii] + " " + data["endpos" + ii] + " " + data["seatdata" + ii + "|" + jj] + " " + data["seattype" + ii + "|" + jj] + "'>" + "REFUND" + "</button></li>";//!!!
                    }
                    var cur = 7 + (m * 4);
                    line[cur] = "</ul></li></ul></div>";
                    var finalout = line[0];
                    for (var k = 1; k <= cur; k++) {
                        finalout += line[k];
                    }
                    $('#trainlist').append(finalout);

                }
                $("[name='refund_ticket']").click(function () {
                    //alert("FUCK!!!!");
                    if ($("#userid").val() == 0) {
                        alert("Please login first!");
                    }
                    else {
                        $.post("buyticket", {
                            _xsrf: $("input[name='_xsrf']").val(),
                            command: $(this).val()
                        });
                        alert("Success! Ticket has been refunded.");
                        location.assign(location);
                    }
                });
            }
            return true;
        });
});