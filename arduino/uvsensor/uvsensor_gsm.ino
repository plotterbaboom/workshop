#include <GSM.h>
#include "plotly_streaming_gsm.h"

// Sign up to plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 3
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"25tm9197rz", "unbi52ww8a", "ibsfyg7qd8"};
// arguments: username, api key, streaming token, filename
plotly graph = plotly("workshop", "v6w5xlbx9j", tokens, "your_filename", nTraces);

// Setup UV Sensor (ML8511) Pins
int UVOUT = A0;
int REF_3V3 = A1;

void gsm_connect(){
  // ...
}

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Initialize pinMode for UV Sensor
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);

  gsm_connect();

  graph.fileopt="overwrite"; // See the "Usage" section in https://github.com/plotly/arduino-api for details
  bool success;
  success = graph.init();
  if(!success){while(true){}}
  graph.openStream();
}

unsigned long x;
int y;

void loop() {

  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);

  Serial.print("MP8511 output: ");
  Serial.print(uvLevel);
  graph.plot(millis(), uvLevel, tokens[0]);

  Serial.print("MP8511 voltage: ");
  Serial.print(outputVoltage);
  graph.plot(millis(), outputVoltage, tokens[1]);

  Serial.print("UV Intensity (mW/cm^2): ");
  Serial.print(uvIntensity);
  graph.plot(millis(), uvIntensity, tokens[2]);


}

//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);
}

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
