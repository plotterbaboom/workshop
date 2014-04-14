#include <GSM.h>
#include "plotly_streaming_gsm.h"

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 2
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"25tm9197rz", "unbi52ww8a"};
// arguments: username, api key, streaming token, filename
plotly graph("workshop", "v6w5xlbx9j", tokens, "filename", nTraces);


//Sensor Setup
#define airquality_sensor_pin 0;
#define gas_sensor_pin 1;

void gsm_connect(){
  // ...
}

void setup() {
  graph.maxpoints = 100;
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  gsm_connect();

  bool success;
  success = graph.init();
  if(!success){while(true){}}
  graph.openStream();
}

unsigned long x;
int y;
float volume;

void loop() {
    int airquality_value = analogRead(airquality_sensor_pin);
    int gas_value = analogRead(gas_sensor_pin);
    volume = (float)gas_value/1024*5.0*1000;
    graph.plot(millis(), airquality_value, tokens[0]);
    graph.plot(millis(), volume, tokens[1]);
    delay(50);
}
