

#include "kegeln_access_point.h"
#include "kegeln_struct_for_json.h"
#include "kegeln_wifi_manager.h"
#include "arduino_access_point.h"

//Arduino Libraries:
//#include <DHT.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>

//_____ Internet Stuff ________
Wifi_Manager_Class wifi_manager;

//_____ Misc __________________
Lcd_Display_Class lcd_display;

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
int pins_played = 0;
int rounds_played = 0;
int pins_downed = 0;
//


int sensors[10] = {
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  true,
  };



//----- Arduino Setup Function -------------------------------------------------------------------------
void setup() {

  //_____ Arduino functions ________________________________
  Serial.begin(9600);
  delay(2000);
  //____Check if WiFi Available______

  //INITIALIZE_WIFI_COMMUNICATION(&lcd_display, &wifi_manager);
  Serial.println();
  Serial.println("Setting up Wifi");
  wifi_manager.SETUP_WIFI_CONNECTION(&lcd_display);
  delay(1000);
  Serial.println("Setting up MQTT");
  wifi_manager.SETUP_MQTT_CONNECTION(&lcd_display);
  delay(1000);
  draw_field();


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
}
//------------------------------------------------------------------------------------------------------

//----- Arduino Loop Function --------------------------------------------------------------------------
void loop() {

  wifi_manager.LOOP();

  if(sensors[1] && !digitalRead(Sensor_1)){
    clear_sensor_1();
    Serial.println("  Pin 1 Dropped");
  }
  if(sensors[2] && !digitalRead(Sensor_2)){
    clear_sensor_2();
    Serial.println("  Pin 2 Dropped");
  }
  if(sensors[3] && !digitalRead(Sensor_3)){
    clear_sensor_3();
    Serial.println("  Pin 3 Dropped");
  }
  if(sensors[4] && !digitalRead(Sensor_4)){
    clear_sensor_4();
    Serial.println("  Pin 4 Dropped");
  }
  if(sensors[5] && !digitalRead(Sensor_5)){
    clear_sensor_5();
    Serial.println("  Pin 5 Dropped");
  }
  if(sensors[6] && !digitalRead(Sensor_6)){
    clear_sensor_6();
    Serial.println("  Pin 6 Dropped");
  }
  if(sensors[7] && !digitalRead(Sensor_7)){
    clear_sensor_7();
    Serial.println("  Pin 7 Dropped");
  }
  if(sensors[8] && !digitalRead(Sensor_8)){
    clear_sensor_8();
    Serial.println("  Pin 8 Dropped");
  }
  if(sensors[9] && !digitalRead(Sensor_9)){
    clear_sensor_9();
    Serial.println("  Pin 9 Dropped");
  }
  if(sensors[0] && !digitalRead(Sensor_Round)){
    Serial.println("  Round is over");
    Serial.println("  Packing data");
    DynamicJsonDocument round(1024);
    JsonArray data = round.createNestedArray("sensors");
    for (int i = 1; i <= 9; i++) {
      data.add(sensors[i]);
    }
    round["rounds_played"] = rounds_played;
    round["total_pins_downed"] = pins_played;
    round["pins_downed"] = pins_downed;
    Serial.println("  Data is packed");

    Serial.println("  Sending data over MQTT TO: kegeln/bahn");
    wifi_manager.SEND_MQTT_MESSAGE("Kegelbahn/Kegel", round);
    Serial.println("  Data has been send");
    Serial.println("  Resetting Pins");
    reset_pins_state();
    Serial.println("  Redrawing Screen");
    draw_field();
    Serial.println("  Done! Next round starts now!");
  }

}
//------------------------------------------------------------------------------------------------------

// gets a state of a pin, either 1 or 0 and converts it the following:
// 1 = I Pin is Standing
// 0 = Blankspace Pin has fallen
// everything else = E Anything other than 1 or 0 was given
char convertStateToChar(int state) {
  if (state == 1) return 'I';
  else if (state == 0) return ' ';
  return 'E';
}

void draw_pin(int pos_x, int pos_y) {
  int pin_image[18][8] = {
    { 0, 0, 1, 1, 1, 1, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0 },
    { 0, 0, 2, 2, 2, 2, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 0, 0 },
    { 0, 0, 2, 2, 2, 2, 0, 0 },
    { 0, 1, 1, 1, 1, 1, 1, 0 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1 },
    { 0, 1, 1, 1, 1, 1, 1, 0 }
  };

  draw(pin_image, pos_x, pos_y);
}

void clear_pin(int pos_x, int pos_y) {
  int clear_pin[18][8] = {
    { 2, 0, 0, 0, 0, 0, 2, 0 },
    { 2, 0, 0, 0, 0, 0, 2, 0 },
    { 2, 2, 0, 0, 0, 2, 2, 0 },
    { 2, 2, 0, 0, 0, 2, 2, 0 },
    { 0, 2, 2, 0, 2, 2, 0, 0 },
    { 0, 2, 2, 0, 2, 2, 0, 0 },
    { 0, 0, 2, 2, 2, 0, 0, 0 },
    { 0, 0, 2, 2, 0, 0, 0, 0 },
    { 0, 0, 2, 2, 2, 0, 0, 0 },
    { 0, 0, 2, 2, 2, 0, 0, 0 },
    { 0, 0, 2, 0, 2, 0, 0, 0 },
    { 0, 2, 2, 0, 2, 2, 0, 0 },
    { 0, 2, 0, 0, 0, 2, 0, 0 },
    { 0, 2, 0, 0, 0, 2, 0, 0 },
    { 2, 2, 0, 0, 0, 2, 2, 0 },
    { 2, 0, 0, 0, 0, 0, 2, 0 },
    { 2, 0, 0, 0, 0, 0, 2, 0 },
    { 2, 0, 0, 0, 0, 0, 2, 0 }
  };
  draw(clear_pin, pos_x, pos_y);
}


// draws either a pin or something else
// needs 18x7 array of either 0 1 or 2
// 0 is black, 1 is white, 2 is red
//
void draw(int array[18][8], int pos_x, int pos_y) {
  uint16_t color = 0x0000;
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 8; j++) {
      if (array[i][j] == 1) color = ST7735_WHITE;
      else if (array[i][j] == 2) color = ST7735_BLUE;
      else color = ST7735_BLACK;
      lcd_display.drawPixel(j + pos_x, i + pos_y, color);
    }
  }
}

// a line is 10 pixel tall!
void clear_line(int line) {
  for (int i = 0; i < 10; i++) {
    for (int j = 1; j < 128; j++) {
      lcd_display.drawPixel(j, line + i, ST7735_BLACK);
    }
  }
}

void round_over() {
  if(!sensors[0]){
    sensors[0] = true;
    draw_field();
    rounds_played++;
  }
}

void reset_pins_state() {
  pins_downed = 0;
  for(int i=0;i < 10;i++){
    sensors[i] = true;
  }
  Serial.println("  SensorState Reset");
}


void draw_field() {
  lcd_display.Clear();
  // Row 5 Consisting of Pin 9
  draw_pin(60, 40);
  // Row 4 Consisting of Pin 7 and 8
  draw_pin(36, 60);
  draw_pin(84, 60);
  // Row 3 Consisting of Pin 4 , 5 and 6
  draw_pin(14, 80);
  draw_pin(60, 80);
  draw_pin(106, 80);
  // Row 2 Consisting of Pin 2 and 3
  draw_pin(36, 100);
  draw_pin(84, 100);
  // Row 1 Consisting of Pin 1
  draw_pin(60, 120);
}

void clear_sensor_1() {
  if(sensors[1]){
    clear_pin(60, 120);
    sensors[1] = false;
    pins_played++;
    pins_downed++;
  }
}

void clear_sensor_2() {
  if(sensors[2]){
    clear_pin(36, 100);
    sensors[2] = false;
    pins_played++;
    pins_downed++;
  }
}

void clear_sensor_3() {
  if(sensors[3]){
    clear_pin(84, 100);
    sensors[3] = false;
    pins_played++;
    pins_downed++;
  }
}

void clear_sensor_4() {
  if(sensors[4]){
    clear_pin(14, 80);
    sensors[4] = false;
    pins_played++;
    pins_downed++;
  }
}

void clear_sensor_5() {
  if(sensors[5]){
    clear_pin(60, 80);
    sensors[5] = false;
    pins_played++;
    pins_downed++;
  }
}

void clear_sensor_6() {
  if(sensors[6]){
    clear_pin(106, 80);
    sensors[6] = false;
    pins_played++;
    pins_downed++;
  }
}

void clear_sensor_7() {
  if(sensors[7]){
    clear_pin(36, 60);
    sensors[7] = false;
    pins_played++;
    pins_downed++;
  }
}

void clear_sensor_8() {
  if(sensors[8]){
    clear_pin(84, 60);
    sensors[8] = false;
    pins_played++;
    pins_downed++;
  }
}

void clear_sensor_9() {
  if(sensors[9]){
    clear_pin(60, 40);
    sensors[9] = false;
    pins_played++;
    pins_downed++;
  }
}
