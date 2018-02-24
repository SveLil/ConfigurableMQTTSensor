var $$ = function (selector) { return document.querySelectorAll(selector); };
var $n = function (selector) { return document.getElementsByName(selector); };
var $ = function (selector) { return $$(selector)[0]};
var btnN = $('#getNetworks');
var debug = false;
var sensors = {};
function openTab(tab) {
    var i, tabcontent, tablinks;
    tabcontent = $("#tabs").childNodes;
    for (i = 0; i < tabcontent.length; i++) {
        if (tabcontent[i].nodeType == 1) {
            tabcontent[i].style.display = "none";
        }
    }
    tablinks = $$(".tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    $('#'+tab).style.display = "block";
    var tab = $('#'+tab+"Tab");
    if (tab) {
        tab.className += " active";
    }
}
function getXhr() {
    if (window.XMLHttpRequest) {
        return new XMLHttpRequest();
    } else {
        return new ActiveXObject("Microsoft.XMLHTTP");
    }
}
function postForm(name, callback) {
    var form = $("#"+name+"Form");
    var elements = form.elements;
    var params = [];
    for (var i = 0; i < elements.length; i++) {
        var el = elements[i];
        if (!el.name || el.disabled) {
            continue;
        }
        var param = el.name + "=";
        var value = el.value;
        if (el.tagName == "SELECT") {
            value =  el.options[el.selectedIndex].value;
        } else if (el.tagName == "INPUT") {
            if (el.type == "checkbox") {
                value = el.checked;
            } else {
                if (!value) {
                    continue;
                }
            }
        }
        param += encodeURIComponent(value);
        params.push(param);
    }
    var xhr = getXhr();
    xhr.onload = callback.bind(undefined, xhr);
    xhr.open("POST","save"+name);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhr.setRequestHeader('Accept', 'application/json');
    if (debug) {
        console.log(params.join('&'));
        xhr.onload();
    }
    xhr.send(params.join('&'));
}
function submitWiFi() {
    postForm('WiFi', function(xhr) {
        openTab('MQTT');
    });
}
function submitMQTT() {
    postForm('MQTT', function(xhr) {
        openTab('Sensor');
    });
}
function hide(s) {
    $(s).style.display = 'none';
}
function show(s) {
    $(s).style.display = 'block';
}
function scanNetworks() {
    var xhr = getXhr();
    btnN.innerText = 'Scanning...';
    xhr.onload = function() {
        btnN.innerText = 'Configure manually';
        btnN.onclick = toggleManual;
        show('#apcombo');
        hide('#ssid');
        var select = $('#aplist');
        while (select.length > 0) {
        select.remove(0);
        }
        var networks = JSON.parse(xhr.responseText);
        if (networks.sort) {
            networks.sort(function (a,b) {
                return a.ssid.localeCompare(b.ssid);
            });
        }
        for (var i=0; i<networks.length; i++) {
            var ssid = networks[i].ssid;
            var option = addOption(ssid + " (" + networks[i].type +")", ssid);
            select.add(option);
        }
    };
    xhr.open("GET","scan");
    xhr.setRequestHeader('Accept', 'application/json');
    xhr.send();
}
function toggleManual() {
    btnN.innerText = 'Scan for networks';
    btnN.onclick = scanNetworks;
    hide('#apcombo');
    show('#ssid');
}
function addOption(text, value) {
    var option = document.createElement('option');
    option.text = text;
    option.value = value;
    return option;
}
function createElement(type, text) {
    var div = document.createElement(type);
    div.innerText = text;
    return div;
}
function cancelSensor() {
    hide('#SensorForm');
    var addSensorBtn=$('#addCancelSensor');
    addSensorBtn.innerText ="Add sensor";
    addSensorBtn.onclick = addSensor;
}
function addSensor() {
    show('#SensorForm');
    var addSensor=$('#addCancelSensor');
    addSensor.innerText ="Cancel";
    addSensor.onclick = cancelSensor;
}
function deleteSensor() {
    var xhr = getXhr();
    var cNodes = this.childNodes;
    var parent = this.parentElement;
    var sensorId;
    for (var i=0; i<cNodes.length; i++) {
        if (cNodes[i].name=="sensorId") {
            sensorId = cNodes[i].value;
            break;
        }
    }
    var next = this.nextSibling
    parent.removeChild(this);
    xhr.onload = function() {
        while (next) {
            var cNodes = next.childNodes;
            for (var i=0; i<cNodes.length; i++) {
                if (cNodes[i].name=="sensorId") {
                    cNodes[i].value = cNodes[i].value - 1;
                    break;
                }
            }
            next = next.nextSibling;
        }
        if (parent.childNodes.length == 1) { // There is always text node left
            show('#SensorForm');
        }
    }
    xhr.open("POST","deleteSensor");
    xhr.setRequestHeader('Accept', 'application/json');
    xhr.send(sensorId);
    return false;
}
function disableForm(form) {
    var els = form.elements;
    for (var i=0; i<els.length; i++) {
        els[i].disabled = els[i].tagName == "INPUT";
    }
}
function editSensor(form) {
    var els = form.elements;
    var editBtn;
    var delBtn;
    for (var i=0; i<els.length; i++) {
        els[i].disabled = false;
        if (els[i].name == "editBtn") {
            editBtn = els[i];
            editBtn.textContent = "Save sensor";
        } else if (els[i].name == "delBtn") {
            delBtn = els[i];
            delBtn.textContent = "Cancel";
        }
    }
    var delFn = delBtn.onclick;
    var editFn = editBtn.onclick;
    delBtn.onclick = function() {
        disableForm(form);
        delBtn.textContent = "Delete sensor";
        delBtn.onclick = delFn;
        editBtn.textContent = "Edit sensor";
        editBtn.onclick = editFn;
        return false;
    }
    editBtn.onclick = function () {
        disableForm(form);
        delBtn.textContent = "Delete sensor";
        delBtn.onclick = delFn;
        editBtn.textContent = "Edit sensor";
        editBtn.onclick = editFn;
        postForm('Sensor', function(xhr) {
        });
        return false;
    }
    return false;
}

function createInputForConfigType(sDiv, configType, editable) {
    input =  document.createElement("input");
    input.name = configType.name;
    input.disabled = !editable;
    switch (configType.type) {
        case "BOOLEAN":
            input.type = "checkbox";
            break;
        case "INTEGER":
            input.type = "number";
            input.step = "1"
            break;
        case "FLOAT":
            input.type = "number";
            input.step = "any"
            break;
        case "BOOLEAN":
            input.type = "text";
            break;
        default:
            console.log("Unknown type: " + configType.type);
    }
    var label;
    if (editable) {
        label = sDiv;
        input.placeholder = configType.name;
    } else {
        label = createElement("label", configType.name);   
        sDiv.append(label);
    }
    label.append(input);
    return input;
}

function createConfigForm(sDiv, sensorType, config, editable) {
    var thisSensorConfigType = sensors[sensorType];
    if (!thisSensorConfigType) {
        console.log("Sensor type '"+sensorType+"' not found");
        return;
    }
    thisSensorConfigType.forEach(function(configType) {
        var input = createInputForConfigType(sDiv, configType, editable);
        input.value =  config[configType.name];
    });
}

function createSensorForm(sensorId, sensorType, config, sensorName) {
    var sList = $('#sensorList');
    var sDiv = document.createElement('form');
    sDiv.className = "sensor";
    var labelType = createElement("label", "Type");
    sDiv.append(labelType);
    var input =  document.createElement("input");
    input.value = sensorType;
    input.name = "sensorType";
    input.disabled = true;
    labelType.append(input);

    createConfigForm(sDiv, sensorType, config);

    var labelSensorName = createElement("label", "Name");
    sDiv.append(labelSensorName);
    input =  document.createElement("input");
    input.value =  sensorName;
    input.disabled = true;
    input.name = "sensorName";
    labelSensorName.append(input);

    input =  document.createElement("input");
    input.type = "hidden";
    input.name = "sensorId";
    input.value = sensorId;
    sDiv.append(input);

    var editBtn = createElement("button", "Edit sensor");
    editBtn.className = "smallBtn";
    editBtn.style = "float: left;";
    editBtn.name = "editBtn";
    editBtn.onclick = editSensor.bind(undefined, sDiv);
    var btnDiv = document.createElement('div');
    var delBtn = createElement("button", "Delete sensor");
    delBtn.className = "smallBtn";
    delBtn.style = "float: right;";
    delBtn.name = "delBtn";
    delBtn.onclick = deleteSensor.bind(sDiv);
    btnDiv.append(editBtn);
    btnDiv.append(delBtn);
    btnDiv.style = "height: 40px; border: 0px;";
    sDiv.append(btnDiv);
    sList.append(sDiv);
    var addSensorBtn =$('#addCancelSensor');
    addSensorBtn.innerText ="Add Sensor";
    addSensorBtn.disabled = false;
    addSensorBtn.onclick = addSensor;

    hide('#SensorForm');
}
function saveSensor(form) {
    if (!form) {
        form = $('#SensorForm');
    }
    postForm('Sensor', function(xhr) {
        //xhr contains sensorId;
        if (debug) {
            xhr = {};
            xhr.responseText = "{\"success\": true, \"sensorId\": 2}";
        }
        var response = JSON.parse(xhr.responseText);
        if (!response.success) {
            //display error;
        }
        createSensorForm(response.sensorId, $('#sensorType').value, $('#pin').value, $('#sensorName').value);
    });
}

window.onload = function() {
    openTab('WiFi');
    $('#submitWiFi').onclick = submitWiFi;
    $('#submitMQTT').onclick = submitMQTT;
    $('#addCancelSensor').onclick = addSensor;
    $('#saveSensor').onclick = saveSensor;
    btnN.onclick = scanNetworks;
    debug = document.location.protocol == "file:";
}
function setIfDefined(id, value) {
    if (value !== undefined) {
        $('#'+id).value = value;
    }
}
function loadData() {
    var sensorXhr = getXhr();
    sensorXhr.onload = function() {
        sensors = JSON.parse(sensorXhr.responseText);
        var dataXhr = getXhr();
        dataXhr.onload = function() {
            var config = JSON.parse(dataXhr.responseText);
            setIfDefined('ssid', config.ssid);
            setIfDefined('wifi_password', config.wpassword);
            setIfDefined('server', config.server);
            setIfDefined('port', config.port);
            setIfDefined('user', config.user);
            setIfDefined('password', config.mpassword);
            setIfDefined('board_name', config.board_name);
            setIfDefined('interval', config.interval);
            setIfDefined('baseTopic', config.baseTopic);
            setIfDefined('enableAP', config.enableAP);
            setIfDefined('enableDeepSleep', config.enableDeepSleep);
            if (config.sensors && config.sensors.length > 0) {
                config.sensors.forEach(function(sensor) {
                    createSensorForm(sensor.sensorId, sensor.sensorType,sensor.config, sensor.sensorName);
                });
            }
        }
        dataXhr.open("GET","load");
        dataXhr.setRequestHeader('Accept', 'application/json');
        dataXhr.send();    
    };
    sensorXhr.open("GET", "sensorTypeInfo");
    sensorXhr.setRequestHeader('Accept', 'application/json');
    sensorXhr.send();
}
function onSensorSelect() {
    var tlSelect = $('#tlSelect');
    $('#sensorType').value = tlSelect.options[tlSelect.selectedIndex].value;
    var activeSensorForm =  $('#activeSensorForm');
    // Empty form
    while (activeSensorForm.firstChild) {
        activeSensorForm.removeChild(activeSensorForm.firstChild);
    }
    // Reinit form
    createConfigForm(activeSensorForm, tlSelect.options[tlSelect.selectedIndex].value, [], true);
}