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
//------------------------------------------------------------------------------------------------------


class Wifi_Manager_Class {
public:
  void SETUP_WIFI_CONNECTION(Lcd_Display_Class* lcd_ini);
  void SETUP_MQTT_CONNECTION(Lcd_Display_Class* lcd_ini);
  void   SETUP_RTC();
  float  GET_TARGET_TEMPERATURE() {return target_temperature;}
  String GET_START_TIME()         {return time;}
  String GET_START_DATE()         {return date;}
  String GET_CURRENT_TIME();
  String GET_CURRENT_DATE();
  String GET_CONNECTION_STATUS();
  PubSubClient GET_MQTT_CLIENT();
  
  void SEND_MQTT_MESSAGE(const char* topic, struct data_struct data);
  
  static void subscribeReceive(char* topic, byte* payload, unsigned int length);
  static void END_WIFI_CONNECTION();
  

private:
  float target_temperature;
  char time[9]; 
  char date[11];
  char new_time[9];
  char new_date[11];
  unsigned long epoch = 0;
};


#endif
