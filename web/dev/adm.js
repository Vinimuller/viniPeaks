// const URL = "http://192.168.0.12";
const URL = "";

$(document).ready(function () {        
    var logStatus = false;
    var installMode = false;

    function loadCentralInfo(){
        $.getJSON(URL + "/centralConfig.json", /*{value: freq},*/ function(centralConfig) {
            
            $("#ssid").html(centralConfig.network.ssid);
            $("#centralId").html(centralConfig.general.centralId);
            $("#centralIdInput").val(centralConfig.general.centralId);
            $("#pkgTimeoutInput").val(centralConfig.general.pkgTimeout);

        }).fail(function(){
            alert("error - get central config")
        });
    };

    function getLog(){
        var date = new Date();
        $.getJSON(URL + "/getLog", /*{value: freq},*/ function(data) {
            $("#logStatus").html(date.toLocaleString());
            $("#log").append(data.log);

            if(logStatus == true){
                setTimeout(getLog, 5000);
            }
        }).fail(function(){
            alert("error - get Log")
        });
    }
    //jquery initial configs
    $.ajaxSetup({
        // Disable caching of AJAX responses
        cache: false
    });

    $("#crudSensorStatus").hide();
    $("#centralConfigStatus").hide();
    
    loadCentralInfo();

    //btn Events
    $("#btnLog").click(function() {
        logStatus = !logStatus;
        getLog();
    });
    $("#btnInstallMode").click(function() {
        installMode = installMode?0:1;
        $.ajax({
            type: "POST",
            url: URL + "/installMode",
            data:{
                installMode: installMode
            },
            success: function(response)
            {
                console.log(installMode);
                if(installMode == true)
                {
                    $("#installModeStatus").html("On");
                }else{
                    $("#installModeStatus").html("Off");
                }        
            },
            error: function(){         
                alert("error - enter install mode");
            }
        });

    });
    
    //forms
    $("#formCrudSensor").submit(function(e) 
    {
        e.preventDefault();
        var data = {
            uid: $("#uid").val(),
            dbid: $("#dbid").val(),
            loraAddr: $("#loraAddr").val(),
            sensorOk: $("#sensorOk").val(),
            interval: $("#interval").val(),
            var: $("#var").val(),
            allData: $("#allData").val()
        }
        var dataStr = JSON.stringify(data);
        var action = "/central/" + $("#action").children("option:selected").val();
        $.ajax({
            type: "POST",
            url: URL + "/crudSensor",
            data: {
                action: action,
                data: dataStr
            },
            success: function(response){
                $("#crudSensorStatus").slideDown(300).delay(2000).slideUp(1000);
                loadCentralInfo();
            },
            error: function(){
                alert("error - send data");
            }
        });
    });
    $("#formCentralConfig").submit(function(e) 
    {
        e.preventDefault();
        var data = {
            centralId: $("#centralIdInput").val(),
            pkgTimeout: $("#pkgTimeoutInput").val()
        }

        $.ajax({
            type: "POST",
            url: URL + "/centralConfig",
            data: data,
            success: function(response){
                $("#centralConfigStatus").slideDown(300).delay(2000).slideUp(1000);
                loadCentralInfo();
            },
            error: function(){
                alert("error - config Central");
            }
        });
    });
});