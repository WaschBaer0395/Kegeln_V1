//----- Includes ---------------------------------------------------------------------------------------
//Local Headers:
#include "kegeln_wifi_manager.h"
#include "kegeln_struct_for_json.h"
#include "kegeln_access_point.h"
#include "arduino_access_point.h"
//#include "arduino_wifi.h"
//Arduino Librarys:
#include <Arduino.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <RTCZero.h>
#include <ArduinoJson.h>
//------------------------------------------------------------------------------------------------------

//----- Declare Variables ------------------------------------------------------------------------------
  //float new_target_temperature = 15.0;

  const int GMT = 2;
//Placeholder for read data
   char* ssid = nullptr;     // Netzwerk WLAN SSID (name) - HSD
   char* pass= nullptr;     // WLAN WPA Passwort - HSD
//Accesspoint Data
  char ssid_ap[] = SECRET_SSID;
  char password_ap[] = SECRET_PASS;
  int status = WL_IDLE_STATUS;             // WLAN Status
                // WL_NO_SHIELD = 255,
                // WL_NO_MODULE = WL_NO_SHIELD,
                // WL_IDLE_STATUS = 0,
                // WL_NO_SSID_AVAIL = 1,
                // WL_SCAN_COMPLETED = 2,
                // WL_CONNECTED = 3,
                // WL_CONNECT_FAILED = 4,
                // WL_CONNECTION_LOST = 5,
                // WL_DISCONNECTED = 6,
                // WL_AP_LISTENING = 7,
                // WL_AP_CONNECTED = 8,
                // WL_AP_FAILED = 9

  WiFiClient wifiClient;

            // WLAN Client

  const char* mqttServer = "192.168.178.17"; // IP MQTT Broker

  int number_of_tries = 0;                 // Counter for number of connection attempts
  int max_number_of_tries = 3;
//------------------------------------------------------------------------------------------------------


//----- Classes: ---------------------------------------------------------------------------------------
  PubSubClient mqttClient(wifiClient);
  RTCZero rtc;
  kegeln_access_point accessPoint;
  File wifiFile;
//------------------------------------------------------------------------------------------------------

//----- Declare Variables ------------------------------------------------------------------------------
float wifi_target_temperature = 15.0f;
//------------------------------------------------------------------------------------------------------


void Wifi_Manager_Class::SETUP_WIFI_CONNECTION(Lcd_Display_Class* lcd_ini) {

  ssid = new char[32];
  pass = new char[64];
  strcpy(ssid, "SSID");
  strcpy(pass, "PASS");
  
  lcd_ini->BEGIN();

  if (WiFi.status() == WL_NO_MODULE){
   Serial.println("WLAN Error!");      
  }
  else{
    String fv = WiFi.firmwareVersion();

    if (fv < WIFI_FIRMWARE_LATEST_VERSION){
      // return "WLAN FW Outdated!";      
    }

    //Read WiFi Info from SD Card:
    //First Connecting Attempt:
    
    Serial.println("First Connecting Attempt:");
    if(accessPoint.CHECK_SD()){
      Serial.println("  SD Module found\n   Reading SD Card:");
      lcd_ini->Clear();
      lcd_ini->PRINT(0,0,"SD Module found\nReading SD Card:");
      accessPoint.SD_READ(ssid, pass);

      if(strcmp(ssid, "SSID") != 0 ){
        Serial.println("    SD Read Success");
        lcd_ini->PRINT(0,9,"SD Read Success");
      }
      Serial.println("    Status: " + String(status));
      status = WiFi.begin(ssid, pass);
      
    }
    //One Time Connection
    else{
      Serial.println("  SD Module not found");
      accessPoint.OPEN_AP(ssid_ap, password_ap);
      lcd_ini->Clear();
      lcd_ini->PRINT("SD Module not found \nTrying temporal connection: \nUse 192.168.4.1");
      Serial.println(ssid_ap);
      Serial.println(password_ap);
      while(strcmp(ssid, "SSID") == 0){
        accessPoint.HANDLE_AP(ssid, pass);
      }
      status = WiFi.begin(ssid, pass);
      Serial.println(status);
    }
    lcd_ini->Clear();
    lcd_ini->PRINT(
      "S: " + String(ssid) + "\n" + 
      "P: " + String(pass) + "\n");
    Serial.println(
      " S: " + String(ssid) + "\n" + 
      " P: " + String(pass) + "\n"); 
    while (status != WL_CONNECTED && number_of_tries < max_number_of_tries) {
      number_of_tries++;
      lcd_ini->PRINT(0,18,
        "Try NR. " + String(number_of_tries) + "\n" +
        "STATUS: " + String(status));
      Serial.println(
        "-------------------------------------\n"
        "-- Try NR. " + String(number_of_tries) + "\n" +
        "-- STATUS: " + String(status) + 
        "\n-------------------------------------");
      status = WiFi.begin(ssid, pass);
      delay(10000);
    }

    if(status == WL_CONNECTED){
      Serial.println("  connected!");
      lcd_ini->Clear();         
      lcd_ini->PRINT("WLAN Connected!");
      number_of_tries = 0; // Either here or just before if-clause
    }
    //Second Attempt:
    else{
      lcd_ini->Clear();
      lcd_ini->PRINT("WLAN Failed! \nCreating Access Point...");
      accessPoint.OPEN_AP(ssid_ap, password_ap);
      Serial.println("  WLAN Failed! \nCreated Access Point... \nUse 192.168.4.1");
      char* tempS;
      char* tempP;
      int s = 0;
      bool sdDataRead= false;

      while(status != WL_CONNECTED && s<10 && !sdDataRead){
        accessPoint.HANDLE_AP(ssid, pass);
        tempS = ""; // Reset tempS
        tempP = ""; // Reset tempP
        accessPoint.SD_READ(tempS, tempP);
        if (strlen(tempS)>0) {
          status = WiFi.begin(tempS, tempP);
          sdDataRead = true;
          ++s;
        }
        delay(500);
      }
      SETUP_WIFI_CONNECTION(lcd_ini); //Trying it again
      }
    }
    delete[] ssid;
    delete[] pass;

    return;
}

void Wifi_Manager_Class::SETUP_MQTT_CONNECTION(Lcd_Display_Class* lcd_ini) {
  if(status==WL_CONNECTED){
    mqttClient.setServer(mqttServer, 1883);

      if(mqttClient.connect("ArduinoClient")){      // Sets up connection to broker
        mqttClient.setCallback(this->subscribeReceive);   // Assign Event Handler/Callback function
        
        mqttClient.subscribe("kegelbahn", 1);
        
        lcd_ini->Clear();
        lcd_ini->PRINT("MQTT Connected!");
        Serial.println("MQTT Connected!");
      }
      else{
        lcd_ini->Clear();
        lcd_ini->PRINT("MQTT Failed!");
        Serial.println("MQTT Failed!");
      }

      return;
  }
}

PubSubClient Wifi_Manager_Class::GET_MQTT_CLIENT() {
  return mqttClient;
}

String Wifi_Manager_Class::GET_CONNECTION_STATUS() {
  if(status==WL_CONNECTED) {
    return "Connected";
  }
  else {
    return "Not Connected";
  }
}


void Wifi_Manager_Class::SETUP_RTC() {
  if(status==WL_CONNECTED){
      rtc.begin();
      
      int i = 0;
      while (epoch == 0 && number_of_tries < max_number_of_tries) {
        while (epoch == 0 && i<10) {
          epoch = WiFi.getTime();
          ++i;
          delay(500);
        }
        number_of_tries++;
      }

      if (epoch != 0) {
        rtc.setEpoch(epoch);
        sprintf(time, "%02d:%02d:%02d", rtc.getHours() + GMT, rtc.getMinutes(), rtc.getSeconds());
        sprintf(date, "%02d.%02d.%02d", rtc.getDay(), rtc.getMonth(), rtc.getYear()); 
      }
  }
}


String Wifi_Manager_Class::GET_CURRENT_TIME() {
  sprintf(new_time, "%02d:%02d:%02d", rtc.getHours() + GMT, rtc.getMinutes(), rtc.getSeconds());
  return new_time;
}


void Wifi_Manager_Class::SEND_MQTT_MESSAGE(const char* topic, struct data_struct data) {
  DynamicJsonDocument JSONencoder (1024);  


  JSONencoder["time_stamp"] = this->GET_CURRENT_TIME();


  char JSONmessageBuffer[300];  
  
  serializeJson(JSONencoder, JSONmessageBuffer);
  
  mqttClient.publish(topic, JSONmessageBuffer);
  
}


void Wifi_Manager_Class::subscribeReceive(char* topic, byte* payload, unsigned int length) {
  //if(strcmp(topic,"setTs")==0){              // No need to check topic as it's the only one
    String s = String((char*)payload);
    float f = s.toFloat();
    wifi_target_temperature = f;
  //}
  //Receive from WebServer
  //Library in git ReadMe
}


void Wifi_Manager_Class::END_WIFI_CONNECTION() {
  if(status==WL_CONNECTED){
    WiFi.end();
  }
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
