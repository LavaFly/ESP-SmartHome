function checkWebsiteStatus(url, statusElement) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.timeout = 3000;
    xhr.onreadystatechange = function () {
        if (xhr.readyState === 4) {
            if (xhr.status >= 200 && xhr.status < 300) {
                statusElement.classList.add('online');
            } else {
                statusElement.classList.add('offline');
            }
        }
    };
    xhr.onerror = function () {
        statusElement.classList.add('offline');
    };
    xhr.send();
}
function sendGetRequest(module, endpoint) {
    var baseUrl = 'http://' + module + '.local/';
    var url = baseUrl + endpoint;

    console.log(url);

    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.send();
}
window.onload = function () {
    var baseData = { temperature: [], humidity: [], co2: [], brightness: [] }
    var tentData = { temperature: [], humidity: [], co2: [], brightness: [], resistance: [] }
    var pcData = { temperature: [], humidity: [] }

    var pathToBase = "http://localhost:8080/website/mock_sensor.json" // http://baseModule.local/json
    var pathToTent = "http://localhost:8080/website/mock_sensor.json" // http://tentModule.local/json
    var pathToPc = "http://localhost:8080/website/mock_sensor.json" // http://pcModule.local/json

    var baseChart = new CanvasJS.Chart("baseChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "Room Stats",
            fontColor: "#999999"
        },
        axisX: {
            lableAngle: -50,
            valueFormatString: "DD HH:mm",
            labelFontColor: "#fffff0"
        },
        axisY: {
            title: "Temperature",
            suffix: "°C",
            lineColor: "#CD7045",
            tickColor: "#CD7045",
            labelFontColor: "#CD7045",
            titleFontColor: "#999999"
        }
        ,
        axisY2: {
            title: "Humidity",
            suffix: "%",
            lineColor: "#00BFFF",
            tickColor: "#00BFFF",
            labelFontColor: "#00BFFF",
            titleFontColor: "#999999"
        }
        ,
        toolTip: {
            shared: true
        },
        legend: {
            cursor: "pointer",
            itemclick: toggleDataSeries,
            fontColor: "#999999"
        },
        data: [{
            type: "spline",
            name: "Temperature",
            connectNullData: true,
            xValueType: "dateTime",
            xValueFormatString: "HH:mm DD/MMMM",
            yValueFormatString: "#,###",
            axisYIndex: 0,
            showInLegend: true,
            color: "#CD7045",
            dataPoints: baseData.temperature
        },
        {
            type: "spline",
            name: "Humidity",
            connectNullData: true,
            axisYIndex: 1,
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: baseData.humidity
        },
        {
            type: "spline",
            name: "CO2",
            connectNullData: true,
            axisYType: "secondary",
            showInLegend: true,
            color: "#A9A9A9",
            dataPoints: baseData.co2
        },
        {
            type: "spline",
            name: "Brightness",
            connectNullData: true,
            axisYType: "secondary",
            showInLegend: true,
            color: "yellow",
            dataPoints: baseData.brightness
        }],
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
    var tentChart = new CanvasJS.Chart("tentChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "Base Module",
            fontColor: "#999999"
        },
        axisX: {
            lableAngle: -50,
            valueFormatString: "DD HH:mm",
            labelFontColor: "#fffff0"
        },
        axisY: [{
            title: "Temperature",
            suffix: "°C",
            lineColor: "#CD7045",
            tickColor: "#CD7045",
            labelFontColor: "#CD7045",
            titleFontColor: "#999999"
        },
        {
            title: "Humidity",
            suffix: "%",
            lineColor: "#00BFFF",
            tickColor: "#00BFFF",
            labelFontColor: "#00BFFF",
            titleFontColor: "#999999"
        }],
        axisY2: [{
            title: "Quality",
            lineColor: "#A9A9A9",
            //tickColor: "#A9A9A9",
            labelFormatter: function(e){
				return  "";
			},
            labelFontColor: "#A9A9A9",
            titleFontColor: "#999999"
        },
        {
            title: "Brightness",
            lineColor: "yellow",
            //tickColor: "#FF8000",
            labelFormatter: function(e){
				return  "";
			},
            labelFontColor: "#FF8000",
            titleFontColor: "#999999",
        }
        ],
        toolTip: {
            shared: true
        },
        legend: {
            cursor: "pointer",
            itemclick: toggleDataSeries,
            fontColor: "#999999"
        },
        data: [{
            type: "spline",
            name: "Temperature",
            connectNullData: true,
            //nullDataLineDashType: "solid",
            xValueType: "dateTime",
            xValueFormatString: "HH:mm DD/MMMM",
            yValueFormatString: "#,###",
            axisYIndex: 0,
            showInLegend: true,
            color: "#CD7045",
            dataPoints: tentData.temperature
        },
        {
            type: "spline",
            name: "Humidity",
            connectNullData: true,
            axisYIndex: 1,
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: tentData.humidity
        },
        {
            type: "spline",
            name: "Quality",
            connectNullData: true,
            axisYType: "secondary",
            showInLegend: true,
            color: "#A9A9A9",
            dataPoints: tentData.resistance
        },
        {
            type: "spline",
            name: "Brightness",
            connectNullData: true,
            axisYType: "secondary",
            showInLegend: true,
            color: "yellow",
            dataPoints: tentData.brightness
        }],
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
    var pcChart = new CanvasJS.Chart("pcChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "PC Module",
            fontColor: "#999999"
        },
        axisX: {
            lableAngle: -50,
            valueFormatString: "DD HH:mm",
            labelFontColor: "#fffff0"
        },
        axisY: [{
            title: "Temperature",
            suffix: "°C",
            lineColor: "#993300",
            tickColor: "#993300",
            labelFontColor: "#993300",
            titleFontColor: "#999999"
        }
        ],
        axisY2: [{
            title: "Humidity",
            suffix: "°C",
            lineColor: "#00BFFF",
            tickColor: "#00BFFF",
            labelFontColor: "#00BFFF",
            titleFontColor: "#999999"
        }],
        toolTip: {
            shared: true
        },
        legend: {
            cursor: "pointer",
            itemclick: toggleDataSeries,
            fontColor: "#999999"
        },
        data: [{
            type: "spline",
            name: "BME680",
            connectNullData: true,
            //nullDataLineDashType: "solid",
            xValueType: "dateTime",
            xValueFormatString: "HH:mm DD/MMMM",
            yValueFormatString: "#,###",
            axisYIndex: 0,
            showInLegend: true,
            color: "#993300",
            dataPoints: pcData.temperature
        },
        {
            type: "spline",
            name: "DHT22",
            connectNullData: true,
            axisYIndex: 1,
            axisYType: "secondary",
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: pcData.humidity
        }],
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
    function toggleDataSeries(e) {
        if (typeof (e.dataSeries.visible) === "undefined" || e.dataSeries.visible) {
            e.dataSeries.visible = false;
        } else {
            e.dataSeries.visible = true;
        }
        renderCharts();
    }
    function addBaseData(data) {
        // clear previous
        baseData = { temperature: [], humidity: [], co2: [], brightness: [] }
        for (var i = 0; i < data.length; i++) {
            // canvasjs wants timestamps with milliseconds
            data[i].time = data[i].time * 1000;
            baseData.temperature.push({
                x: new Date(data[i].time),
                y: data[i].temperature
            });
            baseData.humidity.push({
                x: new Date(data[i].time),
                y: data[i].humidity
            });
            baseData.co2.push({
                x: new Date(data[i].time),
                y: data[i].co2
            });
            baseData.brightness.push({
                x: new Date(data[i].time),
                y: data[i].brightness
            });
        }
    }
    function addPcData(data) {
        pcData = { temperature: [], humidity: [] }
        for (var i = 0; i < data.length; i++) {
            data[i].time = data[i].time * 1000;
            pcData.temperature.push({
                x: new Date(data[i].time),
                y: data[i].temperature
            });
            pcData.humidity.push({
                x: new Date(data[i].time),
                y: data[i].humidity
            });
        }
    }
    function addTentData(data) {
        tentData = { temperature: [], humidity: [], co2: [], brightness: [], resistance: [] }
        for (var i = 0; i < data.length; i++) {
            data[i].time = data[i].time * 1000;
            tentData.temperature.push({
                x: new Date(data[i].time),
                y: data[i].temperature
            });
            tentData.humidity.push({
                x: new Date(data[i].time),
                y: data[i].humidity
            });
            tentData.co2.push({
                x: new Date(data[i].time),
                y: data[i].co2
            });
            tentData.brightness.push({
                x: new Date(data[i].time),
                y: data[i].brightness
            });
            tentData.resistance.push({
                x: new Date(data[i].time),
                y: data[i].resistance
            });
        }

    }
    function setupCharts() {
        //$.getJSON(pathToServer, addBaseData);
        $.getJSON(pathToBase, addBaseData);
        $.getJSON(pathToPc, addPcData);
        $.getJSON(pathToTent, addTentData);
        //setInterval($.getJSON(pathToServer, addData), 60000);

        renderCharts();
    }
    function renderCharts() {
        baseChart.render();
        tentChart.render();
        pcChart.render();
    }
    checkWebsiteStatus('http://baseModule.local/isLive', document.getElementById('baseStatus'));
    checkWebsiteStatus('http://ledModule.local/isLive', document.getElementById('ledStatus'));
    checkWebsiteStatus('http://lightingModule.local/isLive', document.getElementById('lightingStatus'));
    checkWebsiteStatus('http://pcModule.local/isLive', document.getElementById('pcStatus'));
    checkWebsiteStatus('http://tentModule.local/isLive', document.getElementById('tentStatus'));
    checkWebsiteStatus('http://camModule.local/isLive', document.getElementById('camStatus'));
    setupCharts();
    initWebSocket();
    initButton();
    /**
    document.addEventListener('DOMContentLoaded', function () {
        var boxes = document.querySelectorAll('.gradientColor');

        boxes.forEach(function (box) {
            var rect = box.getBoundingClientRect();
            var offset = (rect.left / window.innerWidth) * 100; // Calculate offset as percentage
            box.style.setProperty('background-position', offset + '%'); // Set gradient start
        });
    });**/
}
var wsHost = 'ws://ledModule.local/ws';
var websocket;
function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(wsHost);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    console.log('some message');
}
function initButton(){
    document.getElementById('submitButton').addEventListener('click', onSubmit);
}
function onSubmit(){
    var textFieldContent = document.getElementById('fancyInput').value;
    console.log('sending message ' + textFieldContent);
    document.getElementById('fancyInput').value = '';
    websocket.send(textFieldContent);
}
