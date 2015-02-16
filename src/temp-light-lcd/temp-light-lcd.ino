//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_NeoPixel.h>

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// change these to adjust the range of temperatures you want to measure 
// (these are in Farenheit)
int coldTemp = 60;
int hotTemp  = 80;
int tempSwitch = 0;

#define NeoPixel 3 //Pin for NewPixel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, NeoPixel, NEO_GRB + NEO_KHZ800); //Instance of NeoPixel
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //Instance of temp sensor

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
// if (adc_key_in < 50)   return btnRIGHT;  
// if (adc_key_in < 250)  return btnUP; 
// if (adc_key_in < 450)  return btnDOWN; 
// if (adc_key_in < 650)  return btnLEFT; 
// if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:

 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   

 return btnNONE;  // when all others fail, return this...
}

void updateColor() {
  uint8_t red, blue;
  float temp = mlx.readObjectTempF();
  
  if (temp < coldTemp) temp = coldTemp;
  if (temp > hotTemp) temp = hotTemp;

  // map temperature to red/blue color
  // hotter temp -> more red
  red = map(temp, coldTemp, hotTemp, 0, 255);  
  // hotter temp -> less blue
  blue = map(temp, coldTemp, hotTemp, 255, 0);  

  colorWipe(strip.Color(red, 0, blue), 0);
  
  delay(50); // can adjust this for faster/slower updates
}

void setup()
{
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Temp: ("+ String(coldTemp) +" - "+ String(hotTemp) +")"); // print a simple message
 
 mlx.begin();  
 strip.begin();
 //strip.show(); // Initialize all pixels to 'off'
}
 
void loop()
{
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 lcd.setCursor(0,0);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("Adjust Hot");
     tempSwitch = 0;
     break;
     }
   case btnLEFT:
     {
     lcd.clear();
     lcd.setCursor(0,0); 
     lcd.print("Adjust Cold");
     tempSwitch = 1;
     break;
     }
   case btnUP:
     {
     lcd.clear();
     lcd.setCursor(0,0); 
     if (tempSwitch) {coldTemp++; lcd.print("Cold Temp: " + String(coldTemp));}
     else {hotTemp++; lcd.print("Hot Temp: " + String(hotTemp));}
     break;
     }
   case btnDOWN:
     {
     lcd.clear();
     lcd.setCursor(0,0); 
     if (!tempSwitch) {hotTemp--; lcd.print("Hot Temp: " + String(hotTemp));}
     else {coldTemp--; lcd.print("Cold Temp: " + String(coldTemp));}
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     lcd.clear();
       lcd.setCursor(0,0);
     lcd.print("Temp: ("+ String(coldTemp) +" - "+ String(hotTemp) +")"); // print a simple message
     lcd.setCursor(0,1);
     lcd.print(mlx.readObjectTempF());
     break;
     }
 }
  updateColor();
}
 
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

