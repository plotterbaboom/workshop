var plotly = require('plotly')('username', 'api_key');
var five = require("johnny-five");
var board;
var sensor;

board = new five.Board();

var data = [{x:[], y:[], stream:{token:'streamtoken', maxpoints:200}}];
var layout = {fileopt : "extend", filename : "tmp36 nodey arduino!"};

board.on("ready", function() {

  // create a new tmp36 sensor object
  tmp36 = new five.Sensor({
    pin: "A1",
    freq: 1000,
    thresh: 0.5
  });

  plotly.plot(data,layout,function (err, res) {
    console.log(res);
    var stream = plotly.stream('streamtoken', function (res) {
      console.log(res);
    });
    // 'freq' sets data event pulses. this gets called each time.
    tmp36.on("data", function() {
      var date = getDateString();
      data = {
        x : date,
        y : getTemp(this.value)
      }
      console.log(data);
      // write the payload to the plotly stream
      stream.write(JSON.stringify(data)+'\n');
    });
  });
});

// helper function to convert sensor value to temp
function getTemp (value) {
  var voltage = value * 0.004882814;
  var celsius = (voltage - 0.5) * 100;
  var fahrenheit = celsius * (9 / 5) + 32;
  return celsius;
}

// helper function to return date string!
function getDateString () {
  var time = new Date();
  var datestr = new Date(time - 14400000).toISOString().replace(/T/, ' ').replace(/\..+/, '');
  return datestr;
}
