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
    var temperatureBase_data = [];
    var temperaturePC_data = [];
    var humidityBase_data = [];
    var humidityPC_data = [];
    var co2_data = []
    var resistance_data = [];
    var brightness_data = [];
    //var pathToServer = "http://localhost:8080/mock_sensor.json";
    var pathToBase = "http://baseModule.local/json"
    var pathToPc = "http://pcModule.local/allData"
    var bigChart = new CanvasJS.Chart("bigChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "Overview of the System",
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
            dataPoints: temperatureBase_data
        },
        {
            type: "spline",
            name: "Humidity",
            connectNullData: true,
            axisYIndex: 1,
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: humidityBase_data
        },
        {
            type: "spline",
            name: "CO2",
            connectNullData: true,
            axisYType: "secondary",
            showInLegend: true,
            color: "#A9A9A9",
            dataPoints: co2_data
        },
        {
            type: "spline",
            name: "Brightness",
            connectNullData: true,
            axisYType: "secondary",
            showInLegend: true,
            color: "yellow",
            dataPoints: brightness_data
        }],
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
    var baseChart = new CanvasJS.Chart("baseChart", {
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
            dataPoints: temperatureBase_data
        },
        {
            type: "spline",
            name: "Humidity",
            connectNullData: true,
            axisYIndex: 1,
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: humidityBase_data
        },
        {
            type: "spline",
            name: "Quality",
            connectNullData: true,
            axisYType: "secondary",
            showInLegend: true,
            color: "#A9A9A9",
            dataPoints: resistance_data
        },
        {
            type: "spline",
            name: "Brightness",
            connectNullData: true,
            axisYType: "secondary",
            showInLegend: true,
            color: "yellow",
            dataPoints: brightness_data
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
            dataPoints: temperaturePC_data
        },
        {
            type: "spline",
            name: "DHT22",
            connectNullData: true,
            axisYIndex: 1,
            axisYType: "secondary",
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: humidityPC_data
        }],
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });

    var sensorChart = new CanvasJS.Chart("sensorChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "Temperature",
            fontColor: "#999999"
        },
        axisX: {
            lableAngle: -50,
            valueFormatString: "DD HH:mm",
            labelFontColor: "#fffff0"
        },
        axisY: [{
            title: "Base Temperature",
            suffix: "°C",
            lineColor: "#CD7045",
            tickColor: "#CD7045",
            labelFontColor: "#CD7045",
            titleFontColor: "#999999"
        }
        ],
        axisY2: [{
            title: "PC Temperature",
            suffix: "°C",
            lineColor: "#993300",
            tickColor: "#993300",
            labelFontColor: "#993300",
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
            color: "#CD7045",
            dataPoints: temperatureBase_data
        },
        {
            type: "spline",
            name: "DHT22",
            connectNullData: true,
            axisYIndex: 1,
            axisYType: "secondary",
            showInLegend: true,
            color: "#993300",
            dataPoints: temperaturePC_data
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
            text: "Air Quality",
            fontColor: "#999999"
        },
        axisX: {
            lableAngle: -50,
            valueFormatString: "DD HH:mm",
            labelFontColor: "#fffff0"
        },
        axisY: [{
            title: "CO2 Concentration",
            suffix: "ppm",
            lineColor: "#595959",
            tickColor: "#595959",
            labelFontColor: "#595959",
            titleFontColor: "#999999"
        }
        ],
        axisY2: [{
            title: "Air Resistance",
            suffix: "",
            lineColor: "#737373",
            tickColor: "#737373",
            labelFontColor: "#737373",
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
            name: "CO2",
            connectNullData: true,
            xValueType: "dateTime",
            xValueFormatString: "HH:mm DD/MMMM",
            yValueFormatString: "#,###",
            axisYIndex: 0,
            showInLegend: true,
            color: "#595959",
            dataPoints: co2_data
        },
        {
            type: "spline",
            name: "Resistance",
            connectNullData: true,
            axisYType: "secondary",
            axisYIndex: 1,
            showInLegend: true,
            color: "#737373",
            dataPoints: resistance_data
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
        // clear previous data
        while (temperatureBase_data.length > 0) {
            temperatureBase_data.pop();
            humidityBase_data.pop();
            co2_data.pop();
            resistance_data.pop();
            brightness_data.pop();

            // temp solution
            temperaturePC_data.pop();
            humidityPC_data.pop();
        }
        for (var i = 0; i < data.length; i++) {
            data[i].time = data[i].time * 1000;
            temperatureBase_data.push({
                x: new Date(data[i].time),
                y: data[i].temperature
            });
            humidityBase_data.push({
                x: new Date(data[i].time),
                y: data[i].humidity
            });
            co2_data.push({
                x: new Date(data[i].time),
                y: data[i].co2
            });
            resistance_data.push({
                x: new Date(data[i].time),
                y: data[i].quality
            });
            brightness_data.push({
                x: new Date(data[i].time),
                y: data[i].brightness
            });
        }
        renderCharts();
    }
    function addPcData(data) {
        // clear previous data
        while (temperaturePC_data.length > 0) {
            temperaturePC_data.pop();
            humidityPC_data.pop();
        }
        for (var i = 0; i < data.length; i++) {
            data[i].time = data[i].time * 1000;
            temperaturePC_data.push({
                x: new Date(data[i].time),
                y: (Math.random() * (30 - 10) + 10)
            });
            humidityPC_data.push({
                x: new Date(data[i].time),
                y: (Math.random() * (100 - 40) + 40)
            });
        }
        renderCharts();
    }
    function setupCharts() {
        //$.getJSON(pathToServer, addBaseData);
        $.getJSON(pathToBase, addBaseData);
        $.getJSON(pathToPc, addPcData);
        setInterval($.getJSON(pathToServer, addBaseData), 60000);
    }
    function renderCharts() {
        bigChart.render();
        baseChart.render();
        sensorChart.render();
        airqualityChart.render();
        pcChart.render();
    }
    checkWebsiteStatus('http://baseModule.local/isLive', document.getElementById('baseStatus'));
    checkWebsiteStatus('http://ledModule.local/isLive', document.getElementById('ledStatus'));
    checkWebsiteStatus('http://lightingModule.local/isLive', document.getElementById('lightingStatus'));
    checkWebsiteStatus('http://pcModule.local/isLive', document.getElementById('pcStatus'));
    checkWebsiteStatus('http://displayModule.local/isLive', document.getElementById('displayStatus'));
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
