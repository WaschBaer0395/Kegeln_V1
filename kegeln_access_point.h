//Access Point Data:
#ifndef kegeln_access_point_h
#define kegeln_access_point_h

#include <WiFiNINA.h>
//----- Includes ---------------------------------------------------------------------------------------
#include <SdFat.h>
//------------------------------------------------------------------------------------------------------

//----- Declare Variables ------------------------------------------------------------------------------
#define sd_card_adapter_pin SS
//------------------------------------------------------------------------------------------------------

class kegeln_access_point {
public:
	kegeln_access_point();
  /* Opens a WiFi access point at a given IP & password */
	bool OPEN_AP( char* ssid,  char* password);
  /* Handles POST requests made by connected clients and builds the WiFi login page */
	void HANDLE_AP(char* ssid, char* pass);
  void printWiFiStatus();
  /* Writes the two values ssid & pass to a text file in the root directory of a connected SD drive (separated by a '&' sign) */
  void SAVE_VALUES(String ssid, String pass);
  /* Reads the two values ssid & pass from a text file in the root directory of a connected SD drive */
  void SD_READ(char* ssid, char* password );
  void END_AP();
  /* Initializes the Sd library. Returns true if it successfully initialized, false otherwise */
  bool CHECK_SD();
  /* Decodes and returns a given URL into readable output */
  String URL_DECODE(String input);


private:
	char ssid[32];
	char pass[64];
  char newSSID[32];
  char newPASS[64];
	int keyIndex;        
	String networks[10];
	int numNetworks;                    
	int led;
	int status;
	WiFiServer server;
  int MAX_NETWORKS;
  File wifiFile;
  SdFat sd;
  char data;
};
#endif
