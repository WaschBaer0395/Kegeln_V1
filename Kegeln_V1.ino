

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
const byte Sensor_Round = 14; 
const byte Sensor_1 = 21;
const byte Sensor_2 = 2;
const byte Sensor_3 = 3;
const byte Sensor_4 = 4;
const byte Sensor_5 = 5;
const byte Sensor_6 = 6;
const byte Sensor_7 = 7;
const byte Sensor_8 = 8;
const byte Sensor_9 = 20;

//____Round counters
int current_round = 1;
int total_round = 0;
int total_pins_fallen = 0;
int duds = 0;
//
bool sensor_1 = true;
bool sensor_2 = true;
bool sensor_3 = true;
bool sensor_4 = true;
bool sensor_5 = true;
bool sensor_6 = true;
bool sensor_7 = true;
bool sensor_8 = true;
bool sensor_9 = true;
bool sensor_round = true;



//----- Arduino Setup Function -------------------------------------------------------------------------
void setup() {

  //_____ Arduino functions ________________________________
  Serial.begin(9600);

  //____Check if WiFi Available______

  //INITIALIZE_WIFI_COMMUNICATION(&lcd_display, &wifi_manager);
  wifi_manager.SETUP_WIFI_CONNECTION(&lcd_display);
  delay(1000);
  wifi_manager.SETUP_MQTT_CONNECTION(&lcd_display);
  delay(2000);
  
  //_____Defining Pinmodes for connected Sensors
  pinMode(Sensor_Round, INPUT_PULLUP);
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
  draw_field();
}
//------------------------------------------------------------------------------------------------------

//----- Arduino Loop Function --------------------------------------------------------------------------
void loop() {
  bool state_round = digitalRead(Sensor_Round);
  bool state_1 = digitalRead(Sensor_1);
  bool state_2 = digitalRead(Sensor_2);
  bool state_3 = digitalRead(Sensor_3);
  bool state_4 = digitalRead(Sensor_4);
  bool state_5 = digitalRead(Sensor_5);
  bool state_6 = digitalRead(Sensor_6);
  bool state_7 = digitalRead(Sensor_7);
  bool state_8 = digitalRead(Sensor_8);
  bool state_9 = digitalRead(Sensor_9);

  //Serial.println(state_round);

  if (!state_round){
    draw_field();
    sensor_1 =true;
    sensor_2 =true;
    sensor_3 =true;
    sensor_4 =true;
    sensor_5 =true;
    sensor_6 =true;
    sensor_7 =true;
    sensor_8 =true;
    sensor_9 =true;
    }
  else{  
    if (sensor_1 && !state_1){
      clear_sensor_1();
      Serial.println("cleared sensor 1");
      sensor_1 = false;
    }
    if (sensor_2 && !state_2){
      clear_sensor_2();
      sensor_2 = false;
    }
    if (sensor_3 && !state_3){
      clear_sensor_3();
      sensor_3 = false;
    }
    if (sensor_4 && !state_4){
      clear_sensor_4();
      sensor_4 = false;
    }
    if (sensor_5 && !state_5){
      clear_sensor_5();
      sensor_5 = false;
    }
    if (sensor_6 && !state_6){
      clear_sensor_6();
      sensor_6 = false;
    }
    if (sensor_7 && !state_7){
      clear_sensor_7();
      sensor_7 = false;
    }
    if (sensor_8 && !state_8){
      clear_sensor_8();
      sensor_8 = false;
    }
    if (sensor_9 && !state_9){
      clear_sensor_9();
      sensor_9 = false;
    }
  }
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

void draw_pin(int pos_x, int pos_y){
  int pin_image [18][8] = {
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 2, 2, 2, 2, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 2, 2, 2, 2, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 0}
  };
  
  draw(pin_image, pos_x, pos_y);
}

void clear_pin(int pos_x, int pos_y){
  int clear_pin [18][8] = {
    {2, 0, 0, 0, 0, 0, 2, 0},
    {2, 0, 0, 0, 0, 0, 2, 0},
    {2, 2, 0, 0, 0, 2, 2, 0},
    {2, 2, 0, 0, 0, 2, 2, 0},
    {0, 2, 2, 0, 2, 2, 0, 0},
    {0, 2, 2, 0, 2, 2, 0, 0},
    {0, 0, 2, 2, 2, 0, 0, 0},
    {0, 0, 2, 2, 0, 0, 0, 0},
    {0, 0, 2, 2, 2, 0, 0, 0},
    {0, 0, 2, 2, 2, 0, 0, 0},
    {0, 0, 2, 0, 2, 0, 0, 0},
    {0, 2, 2, 0, 2, 2, 0, 0},
    {0, 2, 0, 0, 0, 2, 0, 0},
    {0, 2, 0, 0, 0, 2, 0, 0},
    {2, 2, 0, 0, 0, 2, 2, 0},
    {2, 0, 0, 0, 0, 0, 2, 0},
    {2, 0, 0, 0, 0, 0, 2, 0},
    {2, 0, 0, 0, 0, 0, 2, 0}
  };
  draw(clear_pin, pos_x, pos_y);
}


// draws either a pin or something else
// needs 18x7 array of either 0 1 or 2
// 0 is black, 1 is white, 2 is red
//
void draw(int array[18][8], int pos_x, int pos_y){
  uint16_t color = 0x0000;
  for(int i = 0; i<18;i++){
    for(int j = 0;j<8;j++){
      if(array[i][j]==1) color = ST7735_WHITE; 
      else if (array[i][j]==2) color = ST7735_BLUE;
      else color = ST7735_BLACK;
      lcd_display.drawPixel(j+pos_x, i+pos_y, color);
    }
  }
}

// a line is 10 pixel tall!
void clear_line(int line){
  for(int i = 0; i<10;i++){
    for(int j = 1;j<128;j++){
        lcd_display.drawPixel(j, line+i, ST7735_BLACK);
      }
  }
}

void draw_field(){
  // Row 5 Consisting of Pin 9 
  draw_pin(60,40);
  // Row 4 Consisting of Pin 7 and 8
  draw_pin(36,60);
  draw_pin(84,60);
  // Row 3 Consisting of Pin 4 , 5 and 6
  draw_pin(14,80);
  draw_pin(60,80);
  draw_pin(106,80);
  // Row 2 Consisting of Pin 2 and 3
  draw_pin(36,100);
  draw_pin(84,100);
  // Row 1 Consisting of Pin 1
  draw_pin(60,120);
}

void clear_sensor_1(){
  Serial.println("Cleared Pin 1");
  clear_pin(60,120);
}

void clear_sensor_2(){
  Serial.println("Cleared Pin 2");
  clear_pin(36,100);
}

void clear_sensor_3(){
  Serial.println("Cleared Pin 3");
  clear_pin(84,100);
}

void clear_sensor_4(){
  Serial.println("Cleared Pin 4");
  clear_pin(14,80);
}

void clear_sensor_5(){
  Serial.println("Cleared Pin 5");
  clear_pin(60,80);
}

void clear_sensor_6(){
  Serial.println("Cleared Pin 6");
  clear_pin(106,80);
}

void clear_sensor_7(){
  Serial.println("Cleared Pin 7");
  clear_pin(36,60);
}

void clear_sensor_8(){
  Serial.println("Cleared Pin 8");
  clear_pin(84,60);
}

void clear_sensor_9(){
  Serial.println("Cleared Pin 9");
  clear_pin(60,40);
}
