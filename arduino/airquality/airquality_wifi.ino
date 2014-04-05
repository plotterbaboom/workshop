#include <WiFi.h>
#include "plotly_streaming_wifi.h"

#include "AirQuality.h"
#include "Arduino.h"


// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 1
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"8xdfnkq1nb"};
// arguments: username, api key, streaming token, filename
plotly graph("streaming-demos", "3yyglqsi85", tokens, "filename", nTraces);
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte my_ip[] = { 199, 168, 222, 18 }; // google will tell you: "public ip address"

AirQuality airqualitysensor;
int current_quality =-1;
int sensorpin = A0; //analog pin 0

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
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  wifi_connect();

  bool success;
  success = graph.init();
  if(!success){while(true){}}
  graph.openStream();
  airqualitysensor.init(14);
}

unsigned long x;
int y;

void loop() {
current_quality=airqualitysensor.slope();
  if (current_quality >= 0) { // if a valid data returned.
    if (current_quality==0)
    Serial.println("High pollution! Force signal active");
    else if (current_quality==1)
    Serial.println("High pollution!");
    else if (current_quality==2)
    Serial.println("Low pollution!");
    else if (current_quality ==3)
    Serial.println("Fresh air");
  }
    graph.plot(millis(), current_quality, tokens[0]);
}

ISR(TIMER2_OVF_vect) {
  if(airqualitysensor.counter==122) {//set 2 seconds as a detected duty
  airqualitysensor.last_vol=airqualitysensor.first_vol;
    airqualitysensor.first_vol=analogRead(sensorpin);
    airqualitysensor.counter=0;
    airqualitysensor.timer_index=1;
    PORTB=PORTB^0x20;
  } else {
    airqualitysensor.counter++;
  }
}
