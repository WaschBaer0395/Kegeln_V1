//File Security:
#ifndef lcd_display_h
#define lcd_display_h

// Others
#define SCREEN_WIDTH 128 //128 // OLED display width, in pixels
#define SCREEN_HEIGHT 160 //64 // OLED display height, in pixels

//----- Includes ---------------------------------------------------------------------------------------
//Local Headers:

//Arduino Librarys:
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

//------------------------------------------------------------------------------------------------------


class Lcd_Display_Class {
public:
  //Lcd_Display_Class();
  void PRINT(String text);
  void PRINT_BUFFERED(String text);
  void PRINT(const int line, const int row, String text);
  void drawPixel(int16_t x, int16_t y, uint16_t c);
  void Clear();
  void BEGIN();
  void Update();
  void CenterPrint(String text, int posY);
  
private:
};


#endif
