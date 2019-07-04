
/* Automatic Control Parameters Definition */
#define DRY_SOIL      35
#define WET_SOIL      65
#define COLD_TEMP     12
#define HOT_TEMP      22
#define TIME_PUMP_ON  15
#define TIME_LAMP_ON  15

/* TIMER */
#define READ_BUTTONS_TM   1L  // definitions in seconds
#define READ_SOIL_TEMP_TM 2L
#define READ_SOIL_HUM_TM  10L
#define READ_AIR_DATA_TM  2L
#define SEND_UP_DATA_TM   10L
#define AUTO_CTRL_TM      60L     

/* DHT22*/
#define DHTPIN D3  
#define DHTTYPE DHT11 

/* OLED */
boolean turnOffOLED = 1;
#define SHOW_SET_UP   30

/* Soil Moister */
#define soilMoisterPin A0
#define soilMoisterVcc D8
int soilMoister = 0;

/* DS18B20 Temperature Sensor */
#define ONE_WIRE_BUS 14 // DS18B20 on NodeMCU pin D5 corresponds to GPIO 014 on Arduino
float soilTemp;
