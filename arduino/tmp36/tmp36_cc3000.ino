#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <plotly_streaming_cc3000.h>

#define tmp36sensor_pin 0

#define WLAN_SSID       "wifi_network_name"
#define WLAN_PASS       "wifi_network_password"
#define WLAN_SECURITY   WLAN_SEC_WPA2

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 1
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"25tm9197rz"};
// arguments: username, api key, streaming token, filename
plotly graph("workshop", "v6w5xlbx9j", tokens, "filename", nTraces);



void wifi_connect(){
  /* Initialise the module */
  Serial.println(F("\n... Initializing..."));
  if (!graph.cc3000.begin())
  {
    Serial.println(F("... Couldn't begin()! Check your wiring?"));
    while(1);
  }

  // Optional SSID scan
  // listSSIDResults();

  if (!graph.cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }

  Serial.println(F("... Connected!"));

  /* Wait for DHCP to complete */
  Serial.println(F("... Request DHCP"));
  while (!graph.cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }
}


void setup() {
  graph.maxpoints = 100;
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
}

void loop() {
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(tmp36sensor_pin);
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  //converting from 10 mv per degree wit 500 mV offset
  //to degrees ((voltage - 500mV) times 100)
  float temperatureC = (voltage - 0.5) * 100;
  // now convert to Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  graph.plot(millis(), temperatureC, tokens[0]);
  delay(500);
}
