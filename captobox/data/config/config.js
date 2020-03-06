var number_items_capt1 = 0;
var number_items_capt2 = 0;
var current_draged_capt1 = "";
var current_draged_capt2 = "";
var frequency = 2000;
var xh_serveur = new XMLHttpRequest();
var config = "/lecture_config";
var current_draged_choose = "";
var request = "";
var ssid = "";
var client = "";
var password = "";
var host = "";
var stream = "";
var version = "";

function get_config() {
    xh_serveur.open("GET", config, true);

    xh_serveur.onreadystatechange = function () {
        if (xh_serveur.readyState == 4) {
            if (xh_serveur.status == 200) {
                var res = JSON.parse(xh_serveur.responseText);
                client = res.client;
                ssid = res.ssid_client;
                password = res.password_client;
                request = res.request;
                frequency = res.frequency;
                host = res.host;
                stream = res.stream;
                version = res.vers;

                //current_draged_capt1=res.capt1;
                //current_draged_capt2=res.capt2;
                if (client == "true") {
                    document.getElementById('client').checked = true;
                }
                if (stream == "true") {
                    document.getElementById('stream').checked = true;
                }
                document.getElementById('ssid').value = ssid;
                document.getElementById('password').value = password;
                document.getElementById('request').value = request;
                document.getElementById('frequency').value = frequency;
                document.getElementById('host').value = host;
                document.getElementById('p1').innerHTML = version;

                //document.getElementById('capt1').name = current_draged_capt1;
                //document.getElementById('capt2').value = current_draged_capt2;
                //console.log(current_draged_capt1);
                console.log(version);
                console.log("configuration chargée");

            }
        }
    }

    xh_serveur.send(null);
}

function upload_config(e) {
    frequency = document.getElementById('frequency').value;
    client = document.getElementById('client').checked;
    stream = document.getElementById('stream').checked;
    ssid = document.getElementById('ssid').value;
    password = document.getElementById('password').value;
    host = document.getElementById('host').value;
    request = document.getElementById('request').value;

    console.log("capteur 1 : " + liste1.options[liste1.selectedIndex].value);
    console.log("capteur 2 : " + liste2.options[liste2.selectedIndex].value);
    var envoi_config = "write?capt1=" + liste1.options[liste1.selectedIndex].value + "&capt2=" + liste2.options[liste2.selectedIndex].value + "&frequency=" + frequency + "&client=" + client + "&ssid=" + ssid + "&password=" + password + "&request=" + request + "&host=" + host + "&stream=" + stream;
    console.log(envoi_config);
    httpGet(envoi_config);

    setTimeout(goback(), 6000);
}

function httpGet(theUrl) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", theUrl, false); // false for synchronous request
    xmlHttp.send(null);
    return xmlHttp.responseText;
}

function goback() {
    console.log("redirection !");
    window.location = '../index.html';
}
