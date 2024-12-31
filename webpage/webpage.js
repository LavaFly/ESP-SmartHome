window.onload = function () {
    var temperature_data = [];
    var humidity_data = [];
    var co2_data = []
    var pathToServer = "http://localhost:8080/mock_sensor.json";
    //var pathToMe = "http://anna.local/json"
    var roomChart = new CanvasJS.Chart("roomChart", {
        //exportEnabled: true,
        animationEnabled: true,
        backgroundColor: "",
        title: {
            text: "Temperature & Humidity",
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
            yValueFormatString: "###.##",
            axisYIndex: 0,
            showInLegend: true,
            color: "#CD7045",
            dataPoints: temperature_data
        },
        {
            type: "spline",
            name: "Humidity",
            connectNullData: true,
            yValueFormatString: "###.##",
            axisYType: "secondary",
            showInLegend: true,
            color: "#00BFFF",
            dataPoints: humidity_data
        }
        ],
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
            dataPoints: co2_data
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
        // clear previous data
        while (temperature_data.length > 0) {
            temperature_data.pop();
            humidity_data.pop();
            co2_data.pop();
        }
        for (var i = 0; i < data.length; i++) {
            data[i].time = data[i].time * 1000;
            temperature_data.push({
                x: new Date(data[i].time),
                y: data[i].temperature
            });
            humidity_data.push({
                x: new Date(data[i].time),
                y: data[i].humidity
            });
            co2_data.push({
                x: new Date(data[i].time),
                y: data[i].co2
            });
        }
        renderCharts();
    }
    function setupCharts() {
        $.getJSON(pathToServer, addBaseData);
        setInterval($.getJSON(pathToServer, addBaseData), 60000);
    }
    function renderCharts() {
        roomChart.render();
        airqualityChart.render();
    }
    setupCharts();
}
