/******************************************************************************************************************************
 *  Sensor Data on local OLED Display
 *  Local Command via buttons 
 *  OLED Display is off as default. Press Sensor Button to update and display data
 *  Introduced the function "waitButtonPress (int waitTime)", to break initial loop
 *  Automatic Local Control
 *  Display automatic set-up parameters at Start-up
 *  Sensor data sent to Blynk app
 *  Control commands received from Blynk app
 *       
 *  Automatic and Remote Control irrigation system - Logic LOW -  Developed by Bhuvnesh Singh 23 Feb 2019
 ********************************************************************************************************************************/
#define Developer "Er. BHUVNESH SINGH" 
#include "stationDefines.h"       // Project definitions
#include "stationCredentials.h"

/* ESP & Blynk */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
WidgetLED PUMPs(V0);  // Echo signal to Sensors Tab at Blynk App
WidgetLED PUMPa(V5); // Echo signal to Actuators Tab at Blynk App
WidgetLED LAMPs(V1);  // Echo signal to Sensors Tab at Blynk App
WidgetLED LAMPa(V6); // Echo signal to Actuators Tab at Blynk App

/* TIMER */
#include <SimpleTimer.h>
SimpleTimer timer;

/* OLED */
#include <ACROBOTIC_SSD1306.h> // library for OLED: SCL ==> D1; SDA ==> D2
#include <SPI.h>
#include <Wire.h>

int airHum = 0;
int airTemp = 0;

/* DS18B20 Temperature Sensor */
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

   int t;
   int h;
   int temp;
   int hum;

  BLYNK_WRITE(V10) // V10 is the number of Virtual Pin  
  {
  int t = param.asInt(); 
  }
  
  BLYNK_WRITE(V11) // V11 is the number of Virtual Pin  
  {
  int h = param.asInt(); 
  }
  
  BLYNK_WRITE(V13) // V13 is the number of Virtual Pin  
  {
  int temp = param.asInt();
  }

  BLYNK_WRITE(V12) // V12 is the number of Virtual Pin  
  {
  int hum = param.asInt();
  }

void getDhtData(void)
{
  int airTemp = t;
  int airHum = h;
}

void getSoilMoisterData()
{
  int soilMoister = hum;
}

void getSoilTempData()
{
  int soilTemp = temp;
}

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println("Irrigation System By Er. Bhuvnesh Singh");
  Serial.println(".... Starting Setup");
  Serial.println(" ");

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);
  pinMode(PUMP_ON_BUTTON, INPUT_PULLUP);
  pinMode(LAMP_ON_BUTTON, INPUT_PULLUP);
  pinMode(SENSORS_READ_BUTTON, INPUT_PULLUP);

  Blynk.begin(auth, ssid, pass);
  oledStart();
 
  PUMPs.off();
  LAMPs.off();
  PUMPa.off();
  LAMPa.off();
  digitalWrite(PUMP_PIN, HIGH); // To be used with Relay module (inverted logic: normally HIGH)
  digitalWrite(LAMP_PIN, HIGH); // To be used with Relay module (inverted logic: normally HIGH)

  timer.setInterval(1000L, getDhtData);
  timer.setInterval(1000L, getSoilMoisterData);
  timer.setInterval(1000L, getSoilTempData);
  
  waitButtonPress (SHOW_SET_UP); // Wait for Sensor Button to be pressed 
  oled.clearDisplay();
  startTimers();
  displayData();
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

  digiValue = debounce(SENSORS_READ_BUTTON);
  if (!digiValue) 
  {
    turnOffOLED = !turnOffOLED;
    if (!turnOffOLED)
    {
      oled.setTextXY(0,0); oled.putString("UPDATING SENSORS");
      getDhtData();
      getSoilMoisterData();
      getSoilTempData();
      oledStart();
      displayData();
    }else oled.clearDisplay();
  }
}

/***************************************************
* Receive Commands and act on RELAYs - Logic LOW
****************************************************/
void aplyCmd()
{
  if (pumpStatus == 1) 
  {
    Blynk.notify("irrigation: Warning ==>> Pump ON"); 
    digitalWrite(PUMP_PIN, LOW); // To be used with Relay module (inverted logic: activate with LOW)
    if (!turnOffOLED) displayData();
    PUMPs.on();
    PUMPa.on();
  }
  else
      {
        digitalWrite(PUMP_PIN, HIGH); // To be used with Relay module (inverted logic: normally HIGH)
        if (!turnOffOLED) displayData();
        PUMPs.off();
        PUMPa.off();
      }
  
  if (lampStatus == 1) 
  {
    Blynk.notify("irrigation: Warning ==>> Lamp ON");
    digitalWrite(LAMP_PIN, LOW); // To be used with Relay module (inverted logic: activate with LOW)
    if (!turnOffOLED) displayData();
    LAMPs.on();
    LAMPa.on();
  }
  else
      {
        digitalWrite(LAMP_PIN, HIGH); // To be used with Relay module (inverted logic: normally HIGH)
        if (!turnOffOLED) displayData();
        LAMPs.off();
        LAMPa.off();
      }
}

/***************************************************
* Automatically Control the Plantation based on sensors reading
****************************************************/
void autoControlPlantation(void)
{ 
  if (soilMoister < DRY_SOIL) 
  {
    turnPumpOn();
  }

  if (airTemp < COLD_TEMP) 
  {
    turnLampOn();
  }
}

/***************************************************
* Turn Pump On for a certain amount of time
****************************************************/
void turnPumpOn()
{
  pumpStatus = 1;
  aplyCmd();
  delay (TIME_PUMP_ON*1000);
  pumpStatus = 0;
  aplyCmd();
}

/***************************************************
* Turn Lamp On for a certain amount of time 
****************************************************/
void turnLampOn()
{
  lampStatus = 1;
  aplyCmd();
  delay (TIME_LAMP_ON*1000);
  lampStatus = 0;
  aplyCmd();
}
