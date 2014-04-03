var plotly = require('plotly')('username','api_key');
var five = require("johnny-five");
var board = new five.Board();

// plotly init data
var data = [{x:[], y:[], stream:{token:'streamtoken', maxpoints:200}}];
var layout = {fileopt : "overwrite", filename : "photoresistor nodey arduino!"};

// lets do this
board.on("ready", function() {
  // create a new `photoresistor` sensor object
  var photoresistor = new five.Sensor({
    pin: "A0",
    freq: 50 // send reading every 50ms
  });
  // initialize that plotly graph
  plotly.plot(data,layout,function (err, res) {
    console.log(res);
    //once it's initialized, create a plotly stream
    //to pipe your data!
    var stream = plotly.stream('streamtoken', function (res) {
      console.log(res);
    });
    // this gets called everytime photoresistor returns its value
    photoresistor.scale([ 0, 200 ]).on("data", function() {
      console.log(this.value);
      var date = getDateString();
      data = {
        x : date,
        y : 0,
        marker : {
          size : this.value,
          color : "orange"
        },
        mode: "markers"
      }
      // write the data to the plotly stream
      stream.write(JSON.stringify(data)+'\n');
    });
  });
});

// little helper function to get a nicely formatted date string
function getDateString () {
  var time = new Date();
  // 14400000 is (GMT-4 Montreal)
  // for your timezone just multiply +/-GMT by 3600000
  var datestr = new Date(time - 14400000).toISOString().replace(/T/, ' ').replace(/\..+/, '');
  return datestr;
}
