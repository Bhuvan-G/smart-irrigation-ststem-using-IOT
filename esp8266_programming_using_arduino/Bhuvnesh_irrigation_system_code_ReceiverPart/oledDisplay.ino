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
  oled.setTextXY(6,0); oled.putString("Dr. Dushyant Singh ");
  //oled.setTextXY(6,13); oled.putString("  Session  ");
  oled.setTextXY(7,0); oled.putString("   2015-2019 ");
}

/***************************************************
 * Display data at Serial Monitora & OLED Display
 **************************************************/
 
void displayData(void)
{ 
  String pumpStatusStr;
  String lampStatusStr;
  if (pumpStatus == 1) pumpStatusStr = "on ";
  if (pumpStatus == 0) pumpStatusStr = "off";
  if (lampStatus == 1) lampStatusStr = "on ";
  if (lampStatus == 0) lampStatusStr = "off";
  
  oled.setTextXY(1,0); oled.putString("Irrigation System");
  oled.setTextXY(2,0); oled.putString("                ");
  
  oled.setTextXY(3,0); oled.putString("AirTmp [C]  ");
  oled.setTextXY(3,11); oled.putString("      ");
  oled.setTextXY(3,11); oled.putString(String(temp));
 
  oled.setTextXY(4,0); oled.putString("AirHum [%]  ");
  oled.setTextXY(4,11); oled.putString("      ");
  oled.setTextXY(4,11); oled.putString(String(hum));
 
  oled.setTextXY(5,0); oled.putString("SoilTemp[C]  ");
  oled.setTextXY(5,11); oled.putString("      ");
  oled.setTextXY(5,11); oled.putString(String(soilTemp));
 
  oled.setTextXY(6,0); oled.putString("SoilHumd[%]  ");
  oled.setTextXY(6,11); oled.putString("      ");
  oled.setTextXY(6,11); oled.putString(String(soilMoister));

  oled.setTextXY(7,0); oled.putString(" PUMP ");
  oled.setTextXY(7,4); oled.putString(pumpStatusStr);

  oled.setTextXY(7,7); oled.putString("  LAMP ");
  oled.setTextXY(7,13); oled.putString(lampStatusStr);
}

/***************************************************
 * Clear OLED Display
 **************************************************/
void clearOledDisplay()
{
  oled.setFont(font8x8);
  oled.setTextXY(0,0); oled.putString("                ");
  oled.setTextXY(1,0); oled.putString("                ");
  oled.setTextXY(2,0); oled.putString("                ");
  oled.setTextXY(3,0); oled.putString("                ");
  oled.setTextXY(4,0); oled.putString("                ");
  oled.setTextXY(5,0); oled.putString("                ");
  oled.setTextXY(6,0); oled.putString("                ");
  oled.setTextXY(7,0); oled.putString("                ");
  oled.setTextXY(0,0); oled.putString("                ");              
}
