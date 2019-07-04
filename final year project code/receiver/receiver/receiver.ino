 /* ESP & Blynk */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

/* TIMER */
#include <SimpleTimer.h>
SimpleTimer timer;
 
const int   watchdog = 60000; //  Frequency of sending data to Domoticz
unsigned long previousMillis = millis(); 
HTTPClient http;

// WiFi Parameters
const char* ssid = "Redmi 5";
const char* pass = "87654321";

int temp;
int hum;
int moist;
int soil_temp;
ESP8266WebServer server(80);

void handleSentVar() {
    if (server.hasArg("temp")) { // this is the variable sent from the client
    int airTemp = server.arg("temp").toInt();
    Serial.print(airTemp); //THIS CHANGES EVEN IT'S SUPER SLOW
    /*Serial.print(",");
    int airHum = server.arg("hum").toInt();
    Serial.print(airHum); //THIS CHANGES EVEN IT'S SUPER SLOW
    Serial.print(",");
    int soilMoister = server.arg("moist").toInt();
    Serial.print(soilMoister); //THIS CHANGES EVEN IT'S SUPER SLOW
    Serial.print(",");
    int soilTemp = server.arg("soil_temp").toInt();
    Serial.print(soilTemp); //THIS CHANGES EVEN IT'S SUPER SLOW
    Serial.println(" ");*/
    
    server.send(200, "text/html", "Data received");

    delay(1000);
  }
}
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  WiFi.softAP(ssid, pass);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/data/", HTTP_GET, handleSentVar); 
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  handleSentVar();
}
