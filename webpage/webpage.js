window.onload = function () {
    var temperatureBase_data = [];
    var temperaturePC_data = [];
    var humidityBase_data = [];
    var humidityPC_data = [];
    var co2_data = []
    var resistance_data = [];
    var brightness_data = [];
    var pathToServer = "http://localhost:8080/mock_sensor.json";
    var pathToBase = "http://baseModule.local/allData"
    var pathToPc = "http://pcModule.local/allData"
    var bigChart = new CanvasJS.Chart("bigChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "BME680 Measurements"
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
            labelFontColor: "#CD7045"
        },
        {
            title: "Humidity",
            suffix: "%",
            lineColor: "#00BFFF",
            tickColor: "#00BFFF",
            labelFontColor: "#00BFFF"
        }],
        axisY2: [{
            title: "Quality",
            suffix: "%",
            lineColor: "#A9A9A9",
            tickColor: "#A9A9A9",
            labelFontColor: "#A9A9A9"
        },
        {
            title: "Brightness",
            suffix: "%",
            lineColor: "#FF8000",
            tickColor: "#FF8000",
            labelFontColor: "#FF8000"
        }
        ],
        toolTip: {
            shared: true
        },
        legend: {
            cursor: "pointer",
            itemclick: toggleDataSeries
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
            color: "#FF8000",
            dataPoints: brightness_data
        }]
    });
    var baseChart = new CanvasJS.Chart("baseChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "BME680 Measurements"
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
            labelFontColor: "#CD7045"
        },
        {
            title: "Humidity",
            suffix: "%",
            lineColor: "#00BFFF",
            tickColor: "#00BFFF",
            labelFontColor: "#00BFFF"
        }],
        axisY2: [{
            title: "Quality",
            suffix: "%",
            lineColor: "#A9A9A9",
            tickColor: "#A9A9A9",
            labelFontColor: "#A9A9A9"
        },
        {
            title: "Brightness",
            suffix: "%",
            lineColor: "#FF8000",
            tickColor: "#FF8000",
            labelFontColor: "#FF8000"
        }
        ],
        toolTip: {
            shared: true
        },
        legend: {
            cursor: "pointer",
            itemclick: toggleDataSeries
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
            color: "#FF8000",
            dataPoints: brightness_data
        }]
    });
    var temperatureChart = new CanvasJS.Chart("temperatureChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "Temperature"
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
            labelFontColor: "#CD7045"
        },
        {
            title: "PC Temperature",
            suffix: "°C",
            lineColor: "#00BFFF",
            tickColor: "#00BFFF",
            labelFontColor: "#00BFFF"
        }],
        toolTip: {
            shared: true
        },
        legend: {
            cursor: "pointer",
            itemclick: toggleDataSeries
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
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: temperaturePC_data
        }]
    });
    var airqualityChart = new CanvasJS.Chart("airqualityChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "Air Quality"
        },
        axisX: {
            lableAngle: -50,
            valueFormatString: "DD HH:mm",
            labelFontColor: "#fffff0"
        },
        axisY: [{
            title: "CO2 Concentration",
            suffix: "ppm",
            lineColor: "#CD7045",
            tickColor: "#CD7045",
            labelFontColor: "#CD7045"
        },
        {
            title: "Air Resistance",
            suffix: "",
            lineColor: "#00BFFF",
            tickColor: "#00BFFF",
            labelFontColor: "#00BFFF"
        }],
        toolTip: {
            shared: true
        },
        legend: {
            cursor: "pointer",
            itemclick: toggleDataSeries
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
            color: "#CD7045",
            dataPoints: co2_data
        },
        {
            type: "spline",
            name: "Resistance",
            connectNullData: true,
            axisYIndex: 1,
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: resistance_data
        }]
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

            // temp solution
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
        $.getJSON(pathToServer, addBaseData);
        //$.getJSON(pathToBase, addBaseData);
        //$.getJSON(pathToPc, addPcData);
        //setInterval($.getJSON(pathToServer, addData), 60000);
    }
    function renderCharts() {
        bigChart.render();
        baseChart.render();
        temperatureChart.render();
        airqualityChart.render();
    }
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
    checkWebsiteStatus('http://baseModule.local/isLive', document.getElementById('baseStatus'));
    checkWebsiteStatus('http://ledModule.local/isLive', document.getElementById('ledStatus'));
    checkWebsiteStatus('http://lighingModule.local/isLive', document.getElementById('lightingStatus'));
    //checkWebsiteStatus('http://pcModule.local/isLive', document.getElementById('pcStatus'));
    checkWebsiteStatus('http://displayModule.local/isLive', document.getElementById('displayStatus'));
    checkWebsiteStatus('http://camModule.local/isLive', document.getElementById('camStatus'));
    setupCharts();
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