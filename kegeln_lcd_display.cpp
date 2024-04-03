//----- Includes ---------------------------------------------------------------------------------------
//Local Headers:
#include "kegeln_lcd_display.h"
#include "splash_logo.cpp"
//Arduino Librarys:
#include <Arduino.h>

//------------------------------------------------------------------------------------------------------

//----- Classes: ---------------------------------------------------------------------------------------
//LiquidCrystal_I2C lcd(0x27, 20, 4);
//Adafruit_SH1106G lcd = Adafruit_SH1106G(128, 64, &Wire, -1);

//RB-TFT1,8-T
#define TFT_CS        17  // Display data/command select
#define TFT_RST       -1  // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC        16  // Hallowing display control pins: chip select

#define TFT_MOSI      18  // Data out
#define TFT_SCLK      19  // Clock out

Adafruit_ST7735 lcd = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// color definitions
const uint16_t  Display_Color_Black        = 0x0000;
const uint16_t  Display_Color_Blue         = 0x001F;
const uint16_t  Display_Color_Red          = 0xF800;
const uint16_t  Display_Color_Green        = 0x07E0;
const uint16_t  Display_Color_Cyan         = 0x07FF;
const uint16_t  Display_Color_Magenta      = 0xF81F;
const uint16_t  Display_Color_Yellow       = 0xFFE0;
const uint16_t  Display_Color_White        = 0xFFFF;

// The colors we actually want to use
uint16_t        Display_Text_Color         = Display_Color_Black;   
uint16_t        Display_Backround_Color    = Display_Color_Blue;

int last_line = 1;


//------------------------------------------------------------------------------------------------------

void Lcd_Display_Class::BEGIN () {  
  lcd.initR(INITR_GREENTAB); // Sets up the link and displays a splash screen
  lcd.fillScreen(ST77XX_BLACK);
  lcd.drawBitmap(0,0,splash_logo,128,160, 0xFFFF); //SPLASH SCREEN
  delay(2000); // Duration of the splash screen, value chosen arbitrarily
  lcd.setCursor(0, 0);
  lcd.setTextWrap(true);
  lcd.fillScreen(ST77XX_BLACK);
}



void Lcd_Display_Class::PRINT(String text) {
  if (sizeof(text) > 20) {text = "Err.: Too much text";}
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 0);
  lcd.print(text);
}


void Lcd_Display_Class::PRINT_BUFFERED(String text){
  if (last_line == 16) {
    last_line = 0;
    Clear();
  }
  else last_line++;

  lcd.setCursor(0, (last_line)*9);
  lcd.print(text);
}

void Lcd_Display_Class::PRINT(const int row, const int line, String text) {
  if (sizeof(text) > 20) {text = "Err.: Too much text";}
  lcd.setCursor(row, line);
  lcd.print(text);
  this->Update();
}

void Lcd_Display_Class::Clear() {
  lcd.fillScreen(ST77XX_BLACK);
}

void Lcd_Display_Class::Update() {
  //lcd.display();
}

void Lcd_Display_Class::drawPixel(int16_t x, int16_t y, uint16_t c){
  lcd.drawPixel(x,y,c);
}

/*
 * prints a given text exactly in the center of the screen
*/
void Lcd_Display_Class::CenterPrint(String text, int posY){
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  lcd.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
  // display on horizontal and vertical center
  lcd.setCursor((SCREEN_WIDTH - width) / 2, (posY));
  lcd.print(text); // text to display
  //lcd.display();
}

