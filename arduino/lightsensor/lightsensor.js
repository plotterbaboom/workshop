var plotly = require('plotly')('demos','tj6mr52zgp');
var five = require("johnny-five"),
  board, photoresistor;

board = new five.Board();

var data = [{x:[], y:[], stream:{token:'lu1xzzrt70', maxpoints:200}}];
var layout = {fileopt : "overwrite", filename : "photoresistor nodey arduino!"};

board.on("ready", function() {

  // Create a new `photoresistor` hardware instance.
  photoresistor = new five.Sensor({
    pin: "A0",
    freq: 1000
  });

  plotly.plot(data,layout,function (err, res) {
    console.log(res);
    var stream = plotly.stream('lu1xzzrt70', function (res) {
      console.log(res);
    });
    // "data" get the current reading from the photoresistor
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
      stream.write(JSON.stringify(data)+'\n');
    });
  });
});

function getDateString () {
  var time = new Date();
  var datestr = new Date(time - 14400000).toISOString().replace(/T/, ' ').replace(/\..+/, '');
  return datestr;
}
