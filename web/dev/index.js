// const URL = "http://192.168.0.12";
const URL = "";

$(document).ready(function () {

    function loadSensors(sensor)
    {
        //Conversion from adc to volumetric moisture
        var umidadeVM = Math.round((0.0004676884*sensor.umidade - 0.0723441469) * 100);

        var component = '<div class="container-1 contact" id="' + sensor.uid + '">' +
        '<h2> Sensor: ' + sensor.uid + '</h2>' +
        '<label> Umidade: </label>' +
        '<label class="data">'+ umidadeVM + '%</label><br>' +
        '<label> Temperatura do Ar: </label>' +
        '<label class="data">'+ sensor.t_ar + 'C</label><br>' +
        '<label> Bateria: </label>' +
        '<label class="data">'+ sensor.v_bat + '%</label><br>' +
        '<label> Último dado: </label>' +
        '<label class="data">'+ sensor.lastData + '</label><br>' +
        '</div>'; 

        $("#sensors").append(component);
    }

    function loadSensorInfo(){
        $.getJSON(URL + "/sensorConfig.json", /*{value: freq},*/ function(sensorConfig) {
            $.each( sensorConfig.sensorData, function( key, value ) {
                loadSensors(value);
              });
        }).fail(function(){
            alert("error - get sensor config")
        });
    };

    function loadCentralInfo(){
        $.getJSON(URL + "/centralConfig.json", /*{value: freq},*/ function(centralConfig) {
            
            $("#ssid").html(centralConfig.network.ssid);
            $("#centralId").html(centralConfig.general.centralId);
        }).fail(function(){
            alert("error - get central config")
        });
    };

    //jquery initial configs
    $.ajaxSetup({
        // Disable caching of AJAX responses
        cache: false
    });

    $("#configWifiStatus").hide();
    loadCentralInfo();
    loadSensorInfo();
    
    $("#formConfigWifi").submit(function(e) 
    {
        e.preventDefault();
        var data = {
            ssid: $("#ssidInput").val(),
            password: $("#senha").val()
        }
        $.ajax({
            type: "POST",
            url: URL + "/configWifi",
            data: data,
            success: function(response){
                $("#configWifiStatus").slideDown(300).delay(2000).slideUp(1000);
                loadCentralInfo();
            },
            error: function(){
                alert("error - config Wifi");
            }
        });
    });
});