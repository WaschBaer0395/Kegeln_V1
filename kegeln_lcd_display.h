//File Security:
#ifndef lcd_display_h
#define lcd_display_h

// Others
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//----- Includes ---------------------------------------------------------------------------------------
//Local Headers:

//Arduino Librarys:
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

//------------------------------------------------------------------------------------------------------


class Lcd_Display_Class {
public:
  //Lcd_Display_Class();
  void PRINT(String text);
  void PRINT(const int line, const int row, String text);
  void Clear();
  void BEGIN();
  void Update();
  void CenterPrint(String text, int posY);
  
private:
};


#endif
