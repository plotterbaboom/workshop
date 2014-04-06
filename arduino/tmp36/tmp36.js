var plotly = require('plotly')('username', 'api_key');
var five = require("johnny-five");
var board = new five.Board();

var data = [{x:[], y:[], stream:{token:'streamtoken', maxpoints:200}}];
var layout = {fileopt : "extend", filename : "tmp36 nodey arduino!"};

board.on("ready", function() {

  // create a new tmp36 sensor object
  var tmp36 = new five.Sensor({
    pin: "A0",
    freq: 1000, // get reading every 1000ms
    thresh: 0.5
  });
  // initialize the plotly graph
  plotly.plot(data,layout,function (err, res) {
    if (err) console.log(err);
    console.log(res);
    //once it's initialized, create a plotly stream
    //to pipe your data!
    var stream = plotly.stream('streamtoken', function (err, res) {
      if (err) console.log(err);
      console.log(res);
    });
    // this gets called each time there is a new sensor reading
    tmp36.on("data", function() {
      var data = {
        x : getDateString(),
        y : convertTemperature(this.value)
      };
      console.log(data);
      // write the data to the plotly stream
      stream.write(JSON.stringify(data)+'\n');
    });
  });
});

// helper function to convert sensor value to temp
function convertTemperature (value) {
  var voltage = value * 0.004882814;
  var celsius = (voltage - 0.5) * 100;
  var fahrenheit = celsius * (9 / 5) + 32;
  return celsius;
}

// little helper function to get a nicely formatted date string
function getDateString () {
  var time = new Date();
  // 14400000 is (GMT-4 Montreal)
  // for your timezone just multiply +/-GMT by 3600000
  var datestr = new Date(time - 14400000).toISOString().replace(/T/, ' ').replace(/\..+/, '');
  return datestr;
}
