var profile1 = ['froms', 'arrival', 'date', 'type']
//°¡°¡
//style="display: none;"

var seat_cnt = 0;
var station_cnt = 0;
function set_clock() {
    $('.clockpicker').clockpicker()
        .find('input').change(function () {
            console.log(this.value);
        });
}
function result_clear() {
    $('#noresult').hide();
    $('#hasone').hide();
    $('#infolist').hide();
    $('#seatlist').hide();
    $('#buttonlist').hide();
    $('#table').hide();
    $('#onsale').hide();

}
function setseat() {
    //alert(seat_cnt);
    for (var i = 0; i < seat_cnt; i++) {
        var ii = i.toString();
        var ids = "#seattype" + ii;
        $(ids).attr("disabled", false);
        for (var j = 0; j < station_cnt; j++) {
            var jj = j.toString();
            ids = "#price" + jj + "s" + ii;
            $(ids).attr("disabled", false);
        }
    }
    for (var i = seat_cnt; i < 5; i++) {
        var ii = i.toString();
        var ids = "#seattype" + ii;
        $(ids).attr("disabled", true);
        for (var j = 0; j < station_cnt; j++) {
            var jj = j.toString();
            ids = "#price" + jj + "s" + ii;
            //alert(ids);
            $(ids).attr("disabled", true);
        }
    }
}
function addline() {
    var cnt = station_cnt.toString();
    station_cnt++;
    var scnt = station_cnt.toString();
    var finalout = "<tr id='stationline" + cnt + "'> <th scope='row'>" + scnt + "</th> <td> <input type='text' class='form-control' id='station" + cnt + "' onkeyup=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" onpaste=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" oncontextmenu=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\"/> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time" + cnt + "s0' value='00:00'> </div> </td> <td> <div class='input-group clockpicker'> <input type='text' class='form-control' id='time" + cnt + "s1' value='xx:xx' disabled> </div> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time" + cnt + "s2' value='00:00'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price" + cnt + "s0' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price" + cnt + "s1' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price" + cnt + "s2' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price" + cnt + "s3' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price" + cnt + "s4' value='0.0'> </div> </td> </tr>"
    $('#stationlist').append(finalout);
    cnt = (station_cnt - 2).toString();
    var ids = "#time" + cnt + "s1";
    $(ids).attr("disabled", false);
    $(ids).attr("type", "time");
    $(ids).val("00:00");

    set_clock();
}
function delline() {
    station_cnt--;
    var cnt = station_cnt.toString();
    $('#stationline' + cnt).remove();
    cnt = (station_cnt - 1).toString();
    var ids = "#time" + cnt + "s1";

    $(ids).attr("type", "text");
    $(ids).val("xx:xx");
    $(ids).attr("disabled", true);
}
function result_init(data) {
    $('#noresult').hide();
    $('#hasone').hide();
    $('#onsale').hide();

    $('#infolist').show();
    $('#buttonlist').show();
    $('#seatlist').show();
    $('#table').show();
    $('#stationlist').empty();

    $("input[name='train_id']").val(data["train_id"]);
    if (data["case"] == 1) {
        $("input[name='train_name']").val("");
        $("input[name='train_type']").val("");

        $('#sale').hide();
        $('#modify').hide();
        $('#delete').hide();
        $('#create').show();

        //seat_cnt = 1;
        station_cnt = 2;
        var finalout = "<tr id='stationline0'> <th scope='row'>1</th> <td> <input type='text' class='form-control' id='station0' onkeyup=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" onpaste=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" oncontextmenu=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\"/> </td> <td> <div class='input-group clockpicker'> <input type='text' class='form-control' id='time0s0' value='xx:xx' disabled> </div> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time0s1' value='00:00'> </div> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time0s2' value='00:00'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s0' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s1' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s2' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s3' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s4' value='0.0'> </div> </td> </tr>"
        $('#stationlist').append(finalout);
        finalout = "<tr id='stationline1'> <th scope='row'>2</th> <td> <input type='text' class='form-control' id='station1' onkeyup=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" onpaste=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" oncontextmenu=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\"/> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time1s0' value='00:00'> </div> </td> <td> <div class='input-group clockpicker'> <input type='text' class='form-control' id='time1s1' value='xx:xx' disabled> </div> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time1s2' value='00:00'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s0' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s1' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s2' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s3' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s4' value='0.0'> </div> </td> </tr>"
        $('#stationlist').append(finalout);

        $("#seatoption0").click();
    }
    else {
        $("input[name='train_name']").val(data["train_name"]);
        $("input[name='train_type']").val(data["train_type"]);

        if (data["onsale"] == 1) {
            $('#onsale').show();
            $('#buttonlist').hide();
        }
        $('#sale').show();
        $('#modify').show();
        $('#delete').show();
        $('#create').hide();


        //!!!
        station_cnt = 2;
        var finalout = "<tr id='stationline0'> <th scope='row'>1</th> <td> <input type='text' class='form-control' id='station0' onkeyup=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" onpaste=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" oncontextmenu=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\"/> </td> <td> <div class='input-group clockpicker'> <input type='text' class='form-control' id='time0s0' value='xx:xx' disabled> </div> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time0s1' value='00:00'> </div> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time0s2' value='00:00'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s0' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s1' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s2' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s3' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price0s4' value='0.0'> </div> </td> </tr>"
        $('#stationlist').append(finalout);
        finalout = "<tr id='stationline1'> <th scope='row'>2</th> <td> <input type='text' class='form-control' id='station1' onkeyup=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" onpaste=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\" oncontextmenu=\"value=value.replace(/[^\\a-\\z\\A-\\Z0-9\\u4E00-\\u9FA5]/g,'')\"/> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time1s0' value='00:00'> </div> </td> <td> <div class='input-group clockpicker'> <input type='text' class='form-control' id='time1s1' value='xx:xx' disabled> </div> </td> <td> <div class='input-group clockpicker'> <input type='time' class='form-control' id='time1s2' value='00:00'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s0' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s1' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s2' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s3' value='0.0'> </div> </td> <td> <div class='input-group'> <div class='input-group-addon'>$</div> <input type='number' step='0.01' class='form-control' id='price1s4' value='0.0'> </div> </td> </tr>"
        $('#stationlist').append(finalout);

        var n = data["station_cnt"];
        seat_cnt = data["seat_cnt"];
        for (var i = 0; i < seat_cnt; i++) {
            var ii = i.toString();
            var ids = "#seattype" + ii;
            $(ids).val(data["seattype" + ii]);
        }
        for (var i = seat_cnt; i < 5; i++) {
            var ii = i.toString();
            var ids = "#seattype" + ii;
            $(ids).val("");
        }

        for (var i = 2; i < n; i++) {
            addline();
        }

        for (var i = 0; i < n; i++) {
            var ii = i.toString();
            var ids = "#station" + ii;
            $(ids).val(data["station" + ii]);
            for (var j = 0; j < 3; j++) {
                var jj = j.toString();
                ids = "#time" + ii + "s" + jj;
                $(ids).val(data["time" + ii + "s" + jj]);
            }
            for (var j = 0; j < seat_cnt; j++) {
                var jj = j.toString();
                ids = "#price" + ii + "s" + jj;
                $(ids).val(data["price" + ii + "s" + jj]);
            }
        }

        if (seat_cnt == 1) {
            $("#seatoption0").click();
        }
        else if (seat_cnt == 2) {
            $("#seatoption1").click();
        }
        else if (seat_cnt == 3) {
            $("#seatoption2").click();
        }
        else if (seat_cnt == 4) {
            $("#seatoption3").click();
        }
        else if (seat_cnt == 5) {
            $("#seatoption4").click();
        }
    }

    set_clock();
}
$(document).ready(function () {
    $("#search").click(function () {
        $.post("manage", {
            _xsrf: $("input[name='_xsrf']").val(),
            case: 0,
            search_id: $("input[name='search_id']").val()
        },
            function (data, state) {
                if (data["errors"] == "Nothing") {
                    $("span[name='search_id']").text('');
                    result_clear();
                    $('#hasone').hide();
                    $('#onsale').hide();
                    $('#noresult').show();
                }
                else if (data["errors"] != "Failed") {
                    $("span[name='search_id']").text('');
                    result_init(data);
                }
                else {
                    $("span[name='search_id']").text('* ' + data["search_id"]);
                }
                return true;
            });
    });
    $("#addtrain").click(function () {
        $.post("manage", {
            _xsrf: $("input[name='_xsrf']").val(),
            case: 1,
            search_id: $("input[name='search_id']").val()
        },
            function (data, state) {
                if (data["errors"] == "Hasone") {
                    $("span[name='search_id']").text('');
                    result_clear();
                    $('#noresult').hide();
                    $('#onsale').hide();
                    $('#hasone').show();
                }
                else if (data["errors"] != "Failed") {
                    $("span[name='search_id']").text('');
                    result_init(data);
                }
                else {
                    $("span[name='search_id']").text('* ' + data["search_id"]);
                }
                return true;
            });
    });
    $("#sale").click(function () {
        $.post("manage", {
            _xsrf: $("input[name='_xsrf']").val(),
            case: 2,
            train_id: $("input[name='train_id']").val()
        },
            function (data, state) {
                if (data["errors"] != "Failed") {
                    alert("Success! This train is on sale now.");
                    $("#search").click();
                }
                else {
                    alert("Failed! Please check again.")
                }
                return true;
            });
    });
    $("#modify").click(function () {
        var is_empty = 0;
        var seat_type = $("#seattype0").val();
        for (var i = 1; i < seat_cnt; i++) {
            ii = i.toString();
            seat_type += " ";
            seat_type += $("#seattype" + ii).val();
            if ($("#seattype" + ii).val() == "") is_empty = 1;
        }
        var station_list = "";
        for (var i = 0; i < station_cnt; i++) {
            ii = i.toString();
            station_list += " ";
            station_list += $("#station" + ii).val();
            if ($("#station" + ii).val() == "") is_empty = 1;
            for (var j = 0; j < 3; j++) {
                var jj = j.toString();
                station_list += " ";
                station_list += $("#time" + ii + "s" + jj).val();
                if ($("#time" + ii + "s" + jj).val() == "") is_empty = 1;
            }
            for (var j = 0; j < seat_cnt; j++) {
                var jj = j.toString();
                station_list += " ";
                station_list += $("#price" + ii + "s" + jj).val();
                if ($("#price" + ii + "s" + jj).val() == "") is_empty = 1;
            }
        }
        $.post("manage", {
            _xsrf: $("input[name='_xsrf']").val(),
            case: 3,
            isempty: is_empty,
            train_id: $("input[name='train_id']").val(),
            train_name: $("input[name='train_name']").val(),
            train_type: $("input[name='train_type']").val(),
            stationcnt: station_cnt,
            seatcnt: seat_cnt,
            seattype: seat_type,
            stationlist: station_list
        },
            function (data, state) {
                if (data["errors"] == "Empty") {
                    alert("Warning! Containing illegal blocks.");
                }
                else if (data["errors"] != "Failed") {
                    alert("Success! This train has been modified.");
                    $("#search").click();
                }
                else {
                    alert("Failed! Please check again.")
                }
                return true;
            });
    });
    $("#delete").click(function () {
        $.post("manage", {
            _xsrf: $("input[name='_xsrf']").val(),
            case: 4,
            train_id: $("input[name='train_id']").val()
        },
            function (data, state) {
                if (data["errors"] != "Failed") {
                    alert("Success! This train has been deleted.");
                    $("#search").click();
                }
                else {
                    alert("Failed! Please check again.")
                }
                return true;
            });
    });
    $("#create").click(function () {
        var is_empty = 0;
        var seat_type = $("#seattype0").val();
        for (var i = 1; i < seat_cnt; i++) {
            ii = i.toString();
            seat_type += " ";
            seat_type += $("#seattype" + ii).val();
            if ($("#seattype" + ii).val() == "") is_empty = 1;
        }
        var station_list = "";
        for (var i = 0; i < station_cnt; i++) {
            ii = i.toString();
            station_list += " ";
            station_list += $("#station" + ii).val();
            if ($("#station" + ii).val() == "") is_empty = 1;
            for (var j = 0; j < 3; j++) {
                var jj = j.toString();
                station_list += " ";
                station_list += $("#time" + ii + "s" + jj).val();
                if ($("#time" + ii + "s" + jj).val() == "") is_empty = 1;
            }
            for (var j = 0; j < seat_cnt; j++) {
                var jj = j.toString();
                station_list += " ";
                station_list += $("#price" + ii + "s" + jj).val();
                if ($("#price" + ii + "s" + jj).val() == "") is_empty = 1;
            }
        }
        $.post("manage", {
            _xsrf: $("input[name='_xsrf']").val(),
            case: 5,
            isempty: is_empty,
            train_id: $("input[name='train_id']").val(),
            train_name: $("input[name='train_name']").val(),
            train_type: $("input[name='train_type']").val(),
            stationcnt: station_cnt,
            seatcnt: seat_cnt,
            seattype: seat_type,
            stationlist: station_list
        },
            function (data, state) {
                if (data["errors"] == "Empty") {
                    alert("Warning! Containing illegal blocks.");
                }
                else if (data["errors"] != "Failed") {
                    alert("Success! This train has been created.");
                    $("#search").click();
                }
                else {
                    alert("Failed! Please check again.")
                }
                return true;
            });
    });


    $("#seatoption0").click(function () {
        seat_cnt = 1;
        setseat();
    });
    $("#seatoption1").click(function () {
        seat_cnt = 2;
        setseat();
    });
    $("#seatoption2").click(function () {
        seat_cnt = 3;
        setseat();
    });
    $("#seatoption3").click(function () {
        seat_cnt = 4;
        setseat();
    });
    $("#seatoption4").click(function () {
        seat_cnt = 5;
        setseat();
    });

    $("#addline").click(function () {
        addline();
        setseat();
    });
    $("#delline").click(function () {
        if (station_cnt > 2) {
            delline();
            setseat();
        }
    });
});