#include <GSM.h>
#include "plotly_streaming_gsm.h"
#include "DHT.h"

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 2
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"streamtoken1", "streamtoken2"};
// arguments: username, api key, streaming token, filename
plotly graph("streaming-demos", "3yyglqsi85", tokens, "filename", nTraces);

// DHT Sensor Setup
#define DHTPIN 2 // We have connected the DHT to Digital Pin 2
#define DHTTYPE DHT22 // This is the type of DHT Sensor (Change it to DHT11 if you're using that model)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT object

void gsm_connect(){
  // ...
}

void setup() {

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
  dht.begin();
}

unsigned long x;
int y;

void loop() {
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      graph.plot(millis(), t, tokens[0]);
      graph.plot(millis(), h, tokens[1]);
}
