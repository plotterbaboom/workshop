var plotly = require('plotly')('username', 'api_key');
var five = require("johnny-five");
var board;
var sensor;

board = new five.Board();

var data = [{x:[], y:[], stream:{token:'your_streamtoken', maxpoints:200}}];
var layout = {fileopt : "extend", filename : "nodenodenode"};

board.on("ready", function() {
  sensor = new five.Sensor({  pin: 0, freq: 250 });
  var stream = plotly.stream('your_streamtoken', function (res) {
    console.log(res);
  });

  sensor.on("change", function(err, reading) {
    var voltage = reading * .004882814;
    // For Fahrenheit
    var temperature = (((voltage - .5) * 100)*1.8) + 32;
    // For Celsius
    // var temperature = ((voltage - .5) * 100);
    console.log( temperature );
		var streamdata = JSON.stringify({ x : Date(), y : temperature });
		stream1.write(streamdata+'\n');
  });
	
});
