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

    var pathToBase = "http://base.local/json" //  http://localhost:8080/website/mock_sensor.json
    var pathToTent = "http://tent.local/json" //  http://localhost:8080/website/mock_sensor.json
    var pathToPc = "http://pc.local/json" //  http://localhost:8080/website/mock_sensor.json

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
        },
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
            xValueType: "dateTime",
            xValueFormatString: "HH:mm DD/MMMM",
            yValueFormatString: '###.##',
            axisYIndex: 0,
            showInLegend: true,
            color: "#58c95a", // #A9A9A9
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
            text: "Tent Module",
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
            name: "Temperature",
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
            name: "Humidity",
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
    var airqualityChart = new CanvasJS.Chart("airqualityChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "CO2-Concentration",
            fontColor: "#999999"
        },
        axisX: {
            lableAngle: -50,
            valueFormatString: "DD HH:mm",
            labelFontColor: "#fffff0"
        },
        axisY: {
            title: "Concentration",
            suffix: "ppm",
            minimum: 0,
            maximum: 2000,
            lineColor: "#58c95a",
            tickColor: "#58c95a",
            labelFontColor: "#58c95a",
            titleFontColor: "#999999",
            // <400 Greatest
            // 400 - 600 Very Good
            // 600 - 1000 Good
            // 1000 - 1500 Recommended to ventilate
            // >1500 Required to ventilate
            stripLines: [
                {
                    startValue: 0,
                    endValue: 400,
                    color: "#008000",
                    opacity: .2,
                    label: "Outdoor",
                    labelBackgroundColor: "transparent"
                },
                {
                    startValue: 401,
                    endValue: 600,
                    color: "#00FF00",
                    opacity: .2,
                    label: "Very Good",
                    labelBackgroundColor: "transparent"
                },
                {
                    startValue: 601,
                    endValue: 1000,
                    color: "#A2FF00",
                    opacity: .2,
                    label: "Okay",
                    labelBackgroundColor: "transparent"
                },
                {
                    startValue: 1001,
                    endValue: 1500,
                    color: "#ffff00",
                    opacity: .2,
                    label: "Ventilate",
                    labelBackgroundColor: "transparent"
                },
                {
                    startValue: 1501,
                    endValue: 2000,
                    color: "#e69138",
                    opacity: .2,
                    label: "Ventilate!!",
                    labelBackgroundColor: "transparent"
                }
            ],

        },
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
            name: "CO2",
            connectNullData: true,
            xValueType: "dateTime",
            xValueFormatString: "HH:mm DD/MMMM",
            yValueFormatString: "###.##",
            axisYIndex: 0,
            showInLegend: true,
            color: "#58c95a",
            dataPoints: baseData.co2
        },
        {
            type: "spline",
            name: "CO2",
            connectNullData: true,
            xValueType: "dateTime",
            xValueFormatString: "HH:mm DD/MMMM",
            yValueFormatString: "###.##",
            axisYIndex: 0,
            showInLegend: true,
            color: "#227f5f",
            dataPoints: tentData.co2
        }
        ],
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
        //baseData = { temperature: [], humidity: [], co2: [], brightness: [] }
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
        //pcData = { temperature: [], humidity: [] }
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
        renderCharts();
    }
    function addTentData(data) {
        //tentData = { temperature: [], humidity: [], co2: [], brightness: [], resistance: [] }
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
                y: data[i].quality
            });
        }

    }
    function setupCharts() {
        //$.getJSON(pathToServer, addBaseData);
        $.getJSON(pathToBase, addBaseData);
        $.getJSON(pathToTent, addTentData);
        $.getJSON(pathToPc, addPcData);
        //setInterval($.getJSON(pathToServer, addData), 60000);
        renderCharts();
    }
    function renderCharts() {
        baseChart.render();
        tentChart.render();
        pcChart.render();
        airqualityChart.render();
    }
    checkWebsiteStatus('http://base.local/isLive', document.getElementById('baseStatus'));
    checkWebsiteStatus('http://led.local/isLive', document.getElementById('ledStatus'));
    checkWebsiteStatus('http://lighting.local/isLive', document.getElementById('lightingStatus'));
    checkWebsiteStatus('http://pc.local/isLive', document.getElementById('pcStatus'));
    checkWebsiteStatus('http://tent.local/isLive', document.getElementById('tentStatus'));
    checkWebsiteStatus('http://cam.local/isLive', document.getElementById('camStatus'));
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
var wsHost = 'ws://led.local/ws';
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
