 /*****************************************************************************************************************************
 *  Smart Irrigation System using NodeMCU ESP-12
 *  DHT connected to NodeMCU pin D3 (Ambient Temperature and Relative Humidity)
 *  Soil Moister Sensor connected to A0
 *  Sensor data sent to Blynk app     
 *  Developed by Bhuvnesh Singh 23 Feb 2019
 ********************************************************************************************************************************/
 
#include "stationDefines.h"       // Project definitions
#include "stationCredentials.h"

/* ESP & Blynk */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>


WidgetLED PUMPs(V0);  // Echo signal to Sensors Tab at Blynk App
WidgetLED PUMPa(V5); // Echo signal to Actuators Tab at Blynk App
WidgetLED LAMPs(V1);  // Echo signal to Sensors Tab at Blynk App
WidgetLED LAMPa(V6); // Echo signal to Actuators Tab at Blynk App

/* TIMER */
#include <SimpleTimer.h>
SimpleTimer timer;
ESP8266WebServer server;

/* DHT22*/
#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);
float airHum = 0;
float airTemp = 0;

const char* host = "http://192.168.43.217";
const char port = 4111;
const int   watchdog = 60000; //  Frequency of sending data to Domoticz
unsigned long previousMillis = millis(); 
HTTPClient http;

/* DS18B20 Temperature Sensor */
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

/***************************************************
 * setup started
 **************************************************/
void setup()
{
  Serial.begin(115200);
  delay(1000);
  pinMode(DHTPIN, INPUT);
  pinMode(soilMoisterPin, INPUT);
  pinMode(ONE_WIRE_BUS, INPUT);
  
  Serial.println("Irrigation System By Er. Bhuvnesh Singh");
  Serial.println(".... Starting Setup");
  Serial.println(" ");

  Serial.println("Connecting Wifi...");

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP()); 

  Blynk.begin(auth, ssid, pass);
  dht.begin();
}

void loop()
{
  timer.run(); // Initiates SimpleTimer
  Blynk.run();

/***************************************************
 * Get DHT data
 **************************************************/
  float tempIni = airTemp;
  float humIni = airHum;
  airTemp = dht.readTemperature();
  airHum = dht.readHumidity();        
  Serial.print("air Temperature(in Percentage) = ");
  Serial.print(airTemp);
  Serial.println("%");
  delay(1000);
  Serial.print("air humidity(in Percentage) = ");
  Serial.print(airHum);
  Serial.println("%");
  delay(1000);

  if (isnan(airHum) || isnan(airTemp))   // Check if any reads failed and exit early (to try again).
  {
    Serial.println("Failed to read from DHT sensor!");
    airTemp = tempIni;
    airHum = humIni;
    return;
  }
  
/***************************************************
 * Get Soil Moister Sensor data
 **************************************************/
 
  soilMoister = 0;
  digitalWrite (soilMoisterVcc, HIGH);
  delay (500);

  soilMoister = ( 100.00 - ( (analogRead(soilMoisterPin)/1023.00) * 100.00 ) );
  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(soilMoister);
  Serial.println("%");
  delay(1000);
  
/***************************************************
 * Get SoilTemp sensor data
 **************************************************/
 
  DS18B20.requestTemperatures(); 
  soilTemp = DS18B20.getTempCByIndex(0);

  int newData = ((soilTemp + 0.05) * 10); //fix soilTemp value to 1 decimal place.
  soilTemp = (newData / 10.0);           
  Serial.print("Soil Temperature(in Percentage) = ");
  Serial.print(soilTemp);
  Serial.println("%");      
  Serial.print("connecting to ");
  Serial.println(host);
  Serial.print("Requesting URL: ");
  String url = "http://192.168.43.217:4111/api/temp?";
         url += "temp=";
         url += String(airTemp);
         url += "&hum=";
         url += String(airHum);
         url += "&moist=";
         url += String(soilMoister);
         url += "&soil_temp=";
         url += String(soilTemp);
  Serial.println(url);
  delay(500);
  
  if((WiFi.status() == WL_CONNECTED)) {
    delay(100);
  http.begin(url);
  
  int httpCode = http.GET();
  
    if (httpCode > 0) {       
        String payload = http.getString();
        Serial.println(httpCode); 
        Serial.println(payload);
    }
    else {
      Serial.print("Error on HTTP Request.......... ");
    }
  Serial.println("closing connection");
  http.end();
  }
  
  delay(1000);
  
  Blynk.virtualWrite(V12, soilMoister); // virtual pin V12
  Blynk.virtualWrite(V10, airTemp); // virtual pin V10
  Blynk.virtualWrite(V11, airHum); // virtual pin V11
  Blynk.virtualWrite(V13, soilTemp); //virtual pin V13  
}
