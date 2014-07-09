#include <WiFi.h>
#include "plotly_streaming_wifi.h"

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 2
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"25tm9197rz", "unbi52ww8a"};
// arguments: username, api key, streaming token, filename
plotly graph = plotly("workshop", "v6w5xlbx9j", tokens, "your_filename", nTraces);


// Setup Analog Light Sensor Pin
int sensorPin = A0;    // select the input pin for the potentiometer
float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux

int status = WL_IDLE_STATUS;     // the Wifi radio's status
char ssid[] = "wifi_network_name"; //  your network SSID (name)
char pass[] = "wifi_network_password"; // // your network password

void wifi_connect(){
    // attempt to connect using WPA2 encryption:
    Serial.println("... Attempting to connect to WPA network...");
    status = WiFi.begin(ssid, pass);
    // if you're not connected, stop here:
    if ( status != WL_CONNECTED) {
      Serial.println("... Couldn't get a WiFi connection, trying again");
      wifi_connect();
    }
    // if you are connected, print out info about the connection:
    else {
      Serial.println("... Connected to network");
    }
}

void setup() {
  analogReference(EXTERNAL);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  wifi_connect();

  graph.fileopt="overwrite"; // See the "Usage" section in https://github.com/plotly/arduino-api for details
  bool success;
  success = graph.init();
  if(!success){while(true){}}
  graph.openStream();
}

void loop() {
  int rawValue = analogRead(sensorPin);

  Serial.print("Raw = ");
  Serial.print(rawValue);
  graph.plot(millis(), rawValue, tokens[0]);

  Serial.print(" - Lux = ");
  Serial.println(RawToLux(rawValue));
  graph.plot(millis(), RawToLux(rawValue), tokens[1]);

  delay(200);

}

float RawToLux(int raw)
{
  float logLux = raw * logRange / rawRange;
  return pow(10, logLux);
}
