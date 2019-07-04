'use strict';

const express = require('express');
const app = express();
const router = express.Router();
const bodyParser = require('body-parser');
const fs = require('fs');
const jsonminify = require('jsonminify');
const mongoose = require('mongoose');
const device = require('express-device');
const http = require('http');
const WebSocket = require('ws');
const uuid = require("uuid");

//Logger
const Logger = require('./utils/logger');
const logger = new Logger( {moduleName: 'Module', color: 'bgBlue'} );

//Configuration
const config = JSON.parse(jsonminify(fs.readFileSync('./ec-config.json', 'utf8')));
const port = process.env.MONGO_URI ? config.prodPort : config.devPort;
const mongoServerURI = process.env.MONGO_URI || config.MONGO_URI;
global.ECLogLevel = config.logLevel || 2;

// parse requests of content-type - application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ limit: '50mb',extended: true }));
// parse requests of content-type - application/json
app.use(bodyParser.json({limit: '50mb'}));
app.use(device.capture());

//Created models load here
const Temp = require('./api/models/temprature');

// Initate mongoose instance
logger.info('Connecting to mongo server at ' + mongoServerURI);
mongoose.Promise = global.Promise;
mongoose.connect(mongoServerURI);
mongoose.connection.on('error', function(error) {
    logger.error('MongoDB connection error' + error);
});


try {
    app.use(require('./api/routes'));
} catch(err) {
    logger.error('Could not load routes ' + JSON.stringify(err));
    console.log(err);
    process.exit(1);
}
   
const server = http.createServer(app);
// const wss = new WebSocket.Server({ server });
// wss.on('connection', function connection(ws,req) {

//   ws.id = uuid.v4();
//   ws.send(ws.id+"");
//   ws.on('message', function incoming(data) {
//     wss.clients.forEach(function each(client) {
//       //client.send(data);
//       let obj = JSON.parse(data);
//       if(client.id==obj.id){
//         client.send(obj.message);
//       }
//     });
//   });
// });



server.listen(port, () => {
    logger.info(`Server started on port ${server.address().port} :)`);
});


logger.info('EC RESTful API server started on: ' + port);
