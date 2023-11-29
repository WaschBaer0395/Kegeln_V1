

#include "kegeln_access_point.h"
#include "kegeln_struct_for_json.h"
#include "kegeln_wifi_manager.h"
#include "arduino_access_point.h"

//Arduino Libraries:
//#include <DHT.h>
#include <SPI.h>
#include <WiFiNINA.h>

//_____ Internet Stuff ________
Wifi_Manager_Class          wifi_manager;

//_____ Misc __________________
Lcd_Display_Class           lcd_display;

//_____Pin definitons for sensors
const byte Sensor_Round = 21; 
const byte Sensor_1 = 14;
const byte Sensor_2 = 2;
const byte Sensor_3 = 3;
const byte Sensor_4 = 4;
const byte Sensor_5 = 5;
const byte Sensor_6 = 6;
const byte Sensor_7 = 7;
const byte Sensor_8 = 8;
const byte Sensor_9 = 9;

//____Round counters
int current_round = 1;
int total_round = 0;
int total_pins_fallen = 0;
int duds = 0;



//----- Arduino Setup Function -------------------------------------------------------------------------
void setup() {

  //_____ Arduino functions ________________________________
  Serial.begin(9600);

  //____Check if WiFi Available______

  //INITIALIZE_WIFI_COMMUNICATION(&lcd_display, &wifi_manager);
  wifi_manager.SETUP_WIFI_CONNECTION(&lcd_display);
  delay(1000);
  wifi_manager.SETUP_MQTT_CONNECTION(&lcd_display);
  
  //_____Defining Pinmodes for connected Sensors
  pinMode(Sensor_Round, INPUT);
  pinMode(Sensor_1, INPUT_PULLUP);
  pinMode(Sensor_2, INPUT_PULLUP);
  pinMode(Sensor_3, INPUT_PULLUP);
  pinMode(Sensor_4, INPUT_PULLUP);
  pinMode(Sensor_5, INPUT_PULLUP);
  pinMode(Sensor_6, INPUT_PULLUP);
  pinMode(Sensor_7, INPUT_PULLUP);
  pinMode(Sensor_8, INPUT_PULLUP);
  pinMode(Sensor_9, INPUT_PULLUP);
  lcd_display.Clear();
}
//------------------------------------------------------------------------------------------------------

//----- Arduino Loop Function --------------------------------------------------------------------------
void loop() {
  int down = 0;
  int up = 0;
  int pins[] = {
    digitalRead(Sensor_1),
    digitalRead(Sensor_2),
    digitalRead(Sensor_3),
    digitalRead(Sensor_4),
    digitalRead(Sensor_5),
    digitalRead(Sensor_6),
    digitalRead(Sensor_7),
    digitalRead(Sensor_8),
    digitalRead(Sensor_9),
  };

    // counting the amount of downed pins
  for (int i=0; i<sizeof(pins)/sizeof(int); i++) {
    if(pins[i] == 1){
      up++;
    }
    else{
      down++;
    }
  }


  
  //Status row
  lcd_display.CenterPrint("Do :" + String(down) + " Up :" + String(up) + " Ro :" + (bool)digitalRead(Sensor_Round),0);
  // Row 5 Consisting of Pin 9 
  lcd_display.CenterPrint(String(convertStateToChar(pins[8])),15);
  // Row 4 Consisting of Pin 7 and 8
  lcd_display.CenterPrint(String(convertStateToChar(pins[6])) + "   " + String(convertStateToChar(pins[7])),24);
  // Row 3 Consisting of Pin 4 , 5 and 6
  lcd_display.CenterPrint(String(convertStateToChar(pins[3])) + "   " + String(convertStateToChar(pins[4])) + "   " + String(convertStateToChar(pins[5])),33);
  // Row 2 Consisting of Pin 2 and 3
  lcd_display.CenterPrint(String(convertStateToChar(pins[1])) + "   " + String(convertStateToChar(pins[2])),42);
  // Row 1 Consisting of Pin 1
  lcd_display.CenterPrint(String(convertStateToChar(pins[0])),51);
  
}
//------------------------------------------------------------------------------------------------------

// gets a state of a pin, either 1 or 0 and converts it the following:
// 1 = I Pin is Standing
// 0 = Blankspace Pin has fallen
// everything else = E Anything other than 1 or 0 was given
char convertStateToChar(int state){
  if(state == 1) return 'I';
  else if (state == 0) return ' ';
  return 'E';
  }
