/******************************************************************************************************************************
 *  Sensor Data on local OLED Display
 *  Local Command via buttons 
 *  OLED Display is off as default. Press Sensor Button to update and display data
 *  Introduced the function "waitButtonPress (int waitTime)", to break initial loop
 *  Automatic Local Control
 *  Display automatic set-up parameters at Start-up
 *  Sensor data sent to Blynk app
 *  Control commands received from Blynk app      
 *  Automatic and Remote Control irrigation system - Logic LOW -  Developed by Bhuvnesh Singh 23 Feb 2019
 ********************************************************************************************************************************/
#define Developer "Er. BHUVNESH SINGH" 
#include "stationDefines.h"       // Project definitions
#include "stationCredentials.h"

/* ESP & Blynk */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Ethernet.h>
#include <ESP8266WebServer.h>

WidgetLED PUMPs(V0);  // Echo signal to Sensors Tab at Blynk App
WidgetLED PUMPa(V5); // Echo signal to Actuators Tab at Blynk App
WidgetLED LAMPs(V1);  // Echo signal to Sensors Tab at Blynk App
WidgetLED LAMPa(V6); // Echo signal to Actuators Tab at Blynk App

/* TIMER */
#include <SimpleTimer.h>
SimpleTimer timer;
HTTPClient http;

#include <SPI.h>
#include <Wire.h>

 BLYNK_WRITE(V5) {
  int pumpStatus = param.asInt();
    aplyCmd();  
 }

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Irrigation System By Er. Bhuvnesh Singh");
  Serial.println(".... Starting Setup");
  Serial.println(" ");

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);
  pinMode(PUMP_ON_BUTTON, INPUT_PULLUP);
  pinMode(LAMP_ON_BUTTON, INPUT_PULLUP);
  pinMode(SENSORS_READ_BUTTON, INPUT_PULLUP);

  WiFi.begin(ssid, pass);
 
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  
  Blynk.begin(auth, ssid, pass);
 
  PUMPs.off();
  LAMPs.off();
  PUMPa.off();
  LAMPa.off();
  digitalWrite(PUMP_PIN, LOW); // To be used with Relay module (inverted logic: normally HIGH)
  digitalWrite(LAMP_PIN, LOW); // To be used with Relay module (inverted logic: normally HIGH)
  
  waitButtonPress (SHOW_SET_UP); // Wait for Sensor Button to be pressed 
  //timer.setInterval(1000, startTimers);
  startTimers(); 
}

void loop()
{
  timer.run(); // Initiates SimpleTimer
  Blynk.run();
}

/****************************************************************
* Read remote commands 
****************************************************************/
BLYNK_WRITE(3) // Pump remote control
{
  int i=param.asInt();
  if (i==1) 
  {
    pumpStatus = !pumpStatus;
    aplyCmd();
  }
}

BLYNK_WRITE(4) // Lamp remote control
{
  int i=param.asInt();
  if (i==1) 
  {
    lampStatus = !lampStatus;
    aplyCmd();
  }
}

/****************************************************************
* Read local commands (Pump and Lamp buttons are normally "HIGH"):
****************************************************************/
void readLocalCmd() 
{  
  boolean digiValue = debounce(PUMP_ON_BUTTON);
  if (!digiValue) 
  {
    pumpStatus = !pumpStatus;
    aplyCmd();
  }

  digiValue = debounce(LAMP_ON_BUTTON);
  if (!digiValue) 
  {
    lampStatus = !lampStatus;
    aplyCmd();
  }
}

/***************************************************
* Receive Commands and act on RELAYs - Logic LOW
****************************************************/
void aplyCmd()
{
  if (pumpStatus == 1) 
  {
    Blynk.notify("Bhuvnesh Singh: Warning ==>> Pump ON"); 
    digitalWrite(PUMP_PIN, HIGH); // To be used with Relay module (inverted logic: activate with LOW)
    PUMPa.on();
  }
  else
      {
        digitalWrite(PUMP_PIN, LOW); // To be used with Relay module (inverted logic: normally HIGH)
        PUMPa.off();
      }
  
  if (lampStatus == 1) 
  {
    Blynk.notify("Bhuvnesh Singh: Warning ==>> Lamp ON");
    digitalWrite(LAMP_PIN, HIGH); // To be used with Relay module (inverted logic: activate with LOW)
    LAMPs.on();
    LAMPa.on();
  }
  else
      {
        digitalWrite(LAMP_PIN, LOW); // To be used with Relay module (inverted logic: normally HIGH)
        LAMPs.off();
        LAMPa.off();
      }
}
