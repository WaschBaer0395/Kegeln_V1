//----- Includes ---------------------------------------------------------------------------------------
//Local Headers:
#include "kegeln_lcd_display.h"
//Arduino Librarys:
#include <Arduino.h>
//------------------------------------------------------------------------------------------------------

//----- Classes: ---------------------------------------------------------------------------------------
  //LiquidCrystal_I2C lcd(0x27, 20, 4);
//Adafruit_SH1106G lcd = Adafruit_SH1106G(128, 64, &Wire, -1);

#define TFT_CS        17 // Hallowing display control pins: chip select
#define TFT_RST       16 // Display reset
#define TFT_DC        15 // Display data/command select
#define TFT_BACKLIGHT  7 // Display backlight pin

Adafruit_ST7735 lcd = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

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

//------------------------------------------------------------------------------------------------------

void Lcd_Display_Class::BEGIN () {  
  delay(250); // Delay for OLED bootup
  //lcd.initR(INITR_HALLOWING); // Sets up the link and displays a splash screen
  //lcd.display(); // display function has to be called everytime something is drawn/written
  delay(2000); // Duration of the splash screen, value chosen arbitrarily
  lcd.setFont();
  lcd.fillScreen(Display_Backround_Color);
  lcd.setTextColor(Display_Text_Color);
  lcd.setTextSize(1);
  //lcd.clearDisplay();

  //lcd.setTextSize(1); // Argument works as a multiplier (e.g. 1 = standard size, 2 = double size)
  //lcd.setTextColor(SH110X_WHITE, SH110X_BLACK); // Monochrome OLED Display
  //lcd.display(); //update display                //den Wert (0) eintragen
}



void Lcd_Display_Class::PRINT(String text) {
  if (sizeof(text) > 20) {text = "Err.: Too much text";}
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 0);
  lcd.print(text);
  //lcd.display();
  /*
  delay(2000);
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  */
}

void Lcd_Display_Class::PRINT(const int row, const int line, String text) {
  if (sizeof(text) > 20) {text = "Err.: Too much text";}
  lcd.setCursor(row, line);
  lcd.print(text);
  this->Update();
}

void Lcd_Display_Class::Clear() {
  //lcd.clearDisplay();
}

void Lcd_Display_Class::Update() {
  //lcd.display();
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

