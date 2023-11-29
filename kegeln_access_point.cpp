//----- Name (SSID) and Password for Access Point ---------------------------------------------------------------------------------------
#include "kegeln_access_point.h"
//----- Local Headers ---------------------------------------------------------------------------------------
#include "arduino_access_point.h"

kegeln_access_point::kegeln_access_point()
  : ssid(SECRET_SSID),
    pass(SECRET_PASS),
    server(80){

  numNetworks = 0;
  status = WL_IDLE_STATUS;
  MAX_NETWORKS = 10;

}

bool kegeln_access_point::OPEN_AP( char* ssid,  char* password) {
    strncpy(ssid, SECRET_SSID, sizeof(ssid));
    strncpy(pass, SECRET_PASS, sizeof(pass));
    //get local networks
    int numNetworks = WiFi.scanNetworks();

    for (int i = 0; i < numNetworks && i < MAX_NETWORKS; i++) {
        networks[i] = WiFi.SSID(i);
    }

    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        return false;
    }
    status = WiFi.beginAP(ssid,pass);
    delay(10000);
    server.begin();
    return true;
}
//Recognizing Characters like !,#,% or &
String kegeln_access_point::URL_DECODE(String input) {
  String output = "";
  char c;
  for (int i = 0; i < input.length(); i++) {
    c = input.charAt(i);
    if (c == '+') {
      output += ' ';
    } else if (c == '%') {
      char decodedChar = strtol(input.substring(i + 1, i + 3).c_str(), NULL, 16);
      output += decodedChar;
      i += 2;
    } else {
      output += c;
    }
  }
  return output;
}

void kegeln_access_point::HANDLE_AP(char* ssid, char* pass){
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Client connected");
    String currentLine = "";

    while (client.connected() || strcmp(ssid, "SSID") == 0) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        if (c == '\n') {
          if (currentLine.length() == 0) {
            String request = client.readStringUntil('\r');
            client.flush();
            request = URL_DECODE(request);
            String newSSID;
            String newPassword;
            
            // === Handle request ===

            // Read WiFi SSID from request
            int startVariable1 = request.indexOf("SSID=") + 5;
            int endVariable1 = request.indexOf("&", startVariable1);
            newSSID = request.substring(startVariable1, endVariable1);

            // Read WiFi password from request
            int startVariable2 = request.indexOf("PASS=") + 5;
            int endVariable2 = request.indexOf(" ", startVariable2);
            newPassword = request.substring(startVariable2, endVariable2);
            
            // If they are present, write both read values to the SD card  
            if (newSSID.length() > 0 && newPassword.length() > 0) {
              Serial.println("SSID: " + newSSID);
              Serial.println("PASS: " + newPassword);
              SAVE_VALUES(newSSID, newPassword);
              Serial.println(ssid);
              strcpy(ssid,newSSID.c_str());
              Serial.println(ssid);
              strcpy(pass, newPassword.c_str());
              client.stop();
            }

//------------- Build and return WiFi login page--------------------
            client.println("HTTP/1.1 200 OK");

            client.println("Content-type:text/html");

            client.println();
            client.print("<h2>Network Initiation:</h2>");
            client.print("<form action=\"/send\" method=\"POST\">");
            client.println("SSID: <select name=\'SSID\'>");
            client.println("<option> Select</option>");
            for (int i = 0; i < MAX_NETWORKS ; i++) {
            client.print("<option value=\"");
            client.print(networks[i]);
            client.println("\">");
            client.print(networks[i]);
            client.println("</option>");
  }

            client.println("</select><br>");
            client.print("Enter Password: <input name=\"PASS\" type=\"password\"> <br>");
            client.print("<input type=\"submit\"></input> <br>");
            client.print("</form>");

            client.println();
//------ End HTML ---------------------------------------------------------------------
            Serial.println(c);

            break;

          }

          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {    
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("client disconnected");
    }
}

void kegeln_access_point::SD_READ(char* ssid, char* password){
  File wifiFile;
  String local = "";
  sd.begin(sd_card_adapter_pin, SPI_HALF_SPEED);
  wifiFile=sd.open("arduino_wifi.txt");
  if(wifiFile){
    while (wifiFile.read(&data, sizeof(data)) > 0) {
      local+=data;
    }
    wifiFile.close();
    int ssidIdx = local.indexOf("&",0);

    strncpy(ssid, local.c_str(), ssidIdx);
    ssid[ssidIdx] = '\0'; 

    // removing invisible characters like " ","\t","\v","\f","\r","\n" !
    String buff =String(local.c_str() + ssidIdx + 1);
    buff.trim();  
    strcpy(password, buff.c_str());
  }
  else{
    Serial.println("      read error.");
    }
}

void kegeln_access_point::END_AP(){
  WiFi.end();
}
//Optional Save to SD Card
void kegeln_access_point::SAVE_VALUES(String ssid, String pass) {
  String filename="arduino_wifi.txt";
  sd.begin(sd_card_adapter_pin, SPI_HALF_SPEED);
  wifiFile=sd.open(filename,FILE_WRITE);
  if(wifiFile){
    sd.remove(filename);
    wifiFile.close();
    wifiFile = sd.open(filename,FILE_WRITE);
    if(wifiFile){
      wifiFile.println(ssid + "&" + pass);
      wifiFile.close();
      Serial.println("Written to SDCARD");
    }
  }
  else{Serial.println("failed");}
}

bool kegeln_access_point::CHECK_SD() {
  int attempts = 0;
  while (attempts < 3) {
    if (sd.begin(sd_card_adapter_pin,SPI_HALF_SPEED )) {
      return true;
    } else {
      
      delay(2000);
      attempts++;
    }
  }
  
  return false;
}
