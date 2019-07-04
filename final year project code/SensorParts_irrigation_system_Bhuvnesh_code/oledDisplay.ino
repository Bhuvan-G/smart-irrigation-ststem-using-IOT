/***************************************************
 * Start OLED
 **************************************************/
void oledStart(void)
{
  Wire.begin();  
  oled.init();                      // Initialze SSD1306 OLED display
  clearOledDisplay();
  //oled.setFont(font5x7);            // Set font type (default 8x8)
  oled.clearDisplay();              // Clear screen
  
  oled.setTextXY(0,0); oled.putString("  Developed by ");
  oled.setTextXY(1,0); oled.putString(String(Developer));  
  oled.setTextXY(2,0); oled.putString("Er. Amit Kumar");
  oled.setTextXY(3,0); oled.putString("Er. Dharmendra Kumar");
  oled.setTextXY(4,0); oled.putString("Er. Ajit Singh");
  
  oled.setTextXY(5,0); oled.putString("   Guided By  ");
  oled.setTextXY(6,0); oled.putString("Dr. Dushyant Sir ");
  //oled.setTextXY(6,13); oled.putString("  Session  ");
  oled.setTextXY(7,0); oled.putString("   2015-2019 ");
  delay(10000);
}

/***************************************************
 * Display data at Serial Monitora & OLED Display
 **************************************************/
 
void displayData(void)
{  
  oled.setTextXY(1,0); oled.putString("Irrigation System");
  oled.setTextXY(2,0); oled.putString("                ");
  
  oled.setTextXY(3,0); oled.putString("AirTmp [%]  ");
  oled.setTextXY(3,11); oled.putString("      ");
  oled.setTextXY(3,11); oled.putString(String(airTemp));
 
  oled.setTextXY(4,0); oled.putString("AirHum [%]  ");
  oled.setTextXY(4,11); oled.putString("      ");
  oled.setTextXY(4,11); oled.putString(String(airHum));
 
  oled.setTextXY(5,0); oled.putString("SoilTmp[%]  ");
  oled.setTextXY(5,11); oled.putString("      ");
  oled.setTextXY(5,11); oled.putString(String(soilTemp));
 
  oled.setTextXY(6,0); oled.putString("SoilHum[%]  ");
  oled.setTextXY(6,11); oled.putString("      ");
  oled.setTextXY(6,11); oled.putString(String(soilMoister));
}

/***************************************************
 * Clear OLED Display
 **************************************************/
void clearOledDisplay()
{
  oled.setFont(font8x8);
  oled.setTextXY(0,0); oled.putString("                ");
  oled.setTextXY(1,0); oled.putString(" WiFi Connected ");
  oled.setTextXY(2,0); oled.putString("                ");
  oled.setTextXY(3,0); oled.putString("                ");
  oled.setTextXY(4,0); oled.putString("                ");
  oled.setTextXY(5,0); oled.putString("                ");
  oled.setTextXY(6,0); oled.putString("                ");
  oled.setTextXY(7,0); oled.putString("                ");
  oled.setTextXY(0,0); oled.putString("                "); 
  delay(1000);             
}
