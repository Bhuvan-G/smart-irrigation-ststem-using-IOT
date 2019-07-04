const mongoose = require('mongoose');
const client = require('prom-client');
const express = require('express');
const app = express();
var http = require('http')

const Logger = require('./../utils/logger');
const logger = new Logger( {moduleName: 'Module', color: 'bgBlue'} );

global.ECLogLevel = 2;

const Temp = require('./../api/models/temprature');

mongoose.connect('mongodb://localhost/ec_development_db');

const myModel = mongoose.model('EC_dht11', Temp);


const gauge = new client.Gauge({
	name: 'irrigationData', 
	help: 'irrigation_server_data',
	labelNames: ['value']
});

const server = http.createServer(app);
server.listen(2113, () => {
    logger.info(`Prometheus Server started on port ${server.address().port} :)`);
});

app.get('/metrics', (req, res) => {
	myModel.find({}, function (err, promData) {
		promData.forEach(function(resp){
			logger.info("Humidity="+resp.humidity+ ", Temprature=" + resp.temprature + ", Moisture=" + resp.moisture +" soil_temp="+resp.soil_temp)
			gauge.labels("moisture").set(parseInt(resp.moisture),parseInt(resp.timestamp)); // 2nd version, Same as above
			gauge.labels("humidity").set(parseInt(resp.humidity),parseInt(resp.timestamp));
			gauge.labels("temprature").set(parseInt(resp.temprature),parseInt(resp.timestamp));
			gauge.labels("soil_temp").set(parseInt(resp.soil_temp),parseInt(resp.timestamp));
		})
	});

  	res.set('Content-Type', client.register.contentType)
  	res.end(client.register.metrics())
})
