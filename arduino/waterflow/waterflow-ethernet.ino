#include <SPI.h>
#include <Ethernet.h>
#include "plotly_streaming_ethernet.h"

#define flow_sensor_pin 2
// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);

  if (x == lastflowpinstate) {
    lastflowratetimer++;
    return; // nothing changed!
  }

  if (x == HIGH) {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  flowrate = 1000.0;
  flowrate /= lastflowratetimer;  // in hertz
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

// Sign up for plotly here: https://plot.ly
// View your API key and streamtokens here: https://plot.ly/settings
#define nTraces 1
// View your tokens here: https://plot.ly/settings
// Supply as many tokens as data traces
// e.g. if you want to ploty A0 and A1 vs time, supply two tokens
char *tokens[nTraces] = {"stream_token"};
// arguments: username, api key, streaming token, filename, # of traces
plotly graph("username", "api_key", tokens, "filename", nTraces);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte my_ip[] = { 199, 168, 222, 18 }; // google will tell you: "public ip address"
void startEthernet(){
    Serial.println("... Initializing ethernet");
    if(Ethernet.begin(mac) == 0){
        Serial.println("... Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, my_ip);
    }
    Serial.println("... Done initializing ethernet");
    delay(1000);
}


void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(flow_sensor_pin, INPUT);
  digitalWrite(flow_sensor_pin, HIGH);
  lastflowpinstate = digitalRead(flow_sensor_pin);
  useInterrupt(true);
  startEthernet();
  graph.init();
  graph.openStream();
}

unsigned long x;
int y;

void loop() {

  Serial.print("Freq: "); Serial.println(flowrate);
  Serial.print("Pulses: "); Serial.println(pulses, DEC);
  float liters = pulses;
  liters /= 7.5;
  liters /= 60.0;
/*
  // if a brass sensor use the following calculation
  float liters = pulses;
  liters /= 8.1;
  liters -= 6;
  liters /= 60.0;
*/
  Serial.print(liters); Serial.println(" Liters");
  graph.plot(millis(), liters, tokens[0]);
  delay(100);
}
