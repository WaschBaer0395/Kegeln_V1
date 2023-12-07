//File Security:
#ifndef Wifi_manager_h
#define Wifi_manager_h

//----- Includes ---------------------------------------------------------------------------------------
//Local Headers:
//Arduino Librarys:
#include <Arduino.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "kegeln_lcd_display.h"
#include <ArduinoJson.h>
//------------------------------------------------------------------------------------------------------


class Wifi_Manager_Class {
public:
  void SETUP_WIFI_CONNECTION(Lcd_Display_Class* lcd_ini);
  void SETUP_MQTT_CONNECTION(Lcd_Display_Class* lcd_ini);
  String GET_CONNECTION_STATUS();
  PubSubClient GET_MQTT_CLIENT();
  
  void SEND_MQTT_MESSAGE(const char* topic, DynamicJsonDocument& JSONencoder);
  
  static void subscribeReceive(char* topic, byte* payload, unsigned int length);
  static void END_WIFI_CONNECTION();
  

private:
  unsigned long epoch = 0;
};


#endif
