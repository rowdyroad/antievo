
var SerialPort = require("serialport").SerialPort
var serialPort = new SerialPort("/dev/cu.usbmodem1421", {
  baudrate: 115200
});

var connection = null;

var fs = require('fs');

function onLineReceived(line) {
    try {
       console.log(line);
        var dump_line = Math.floor(Date.now() / 1000).toString() + '\t' + line + '\n';
        fs.appendFile('dump.txt', dump_line, function (err) {
            if (err) throw err;
        });
        if (connection) {
            connection.send(JSON.stringify({type:'measure', data: line}));
        }
    } catch (err) {}
}

var data = '';
serialPort.on('data', function(buffer) {
    data += buffer;
    var pos = data.indexOf("\r\n");
    if (pos != -1) {
	   var line = data.substr(0, pos);
	   data = data.substr(pos + 2);
	   onLineReceived(line);
    }
});


var WebSocketServer = require('ws').Server, wss = new WebSocketServer({ port: 8080 });

wss.on('connection', function(ws) {
   connection = ws;
});