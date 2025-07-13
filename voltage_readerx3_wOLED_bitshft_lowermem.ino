 /*
Default I²C pins on ATtiny1624 (SOIC-14):
      SDA  PIN_PB1 
      SCL  PIN_PB0
  If you wired the bus to PB1/PB0 instead, add Wire.swap(1);
*/
//  Name of new lib: SSD1306AsciiWire
//using adafruit lib:
//Sketch uses 15971 bytes (97%) of program storage space. Maximum is 16384 bytes.

/* new lib, uses less flash memory about 279, 13%

// Simple I2C test for ebay 128x64 oled.

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;
//------------------------------------------------------------------------------
void setup() {
  Wire.begin();         
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();  
  oled.setFont(Adafruit5x7);  

  uint32_t m = micros();
  oled.clear();  
  oled.println("Hello world!");
  oled.println("A long line may be truncated");
  oled.println();
  oled.set2X();
  oled.println("2X demo");
  oled.set1X();
  oled.print("\nmicros: ");
  oled.print(micros() - m);
}
//------------------------------------------------------------------------------
void loop() {}


*/
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define UART_TX_PIN 7
#define I2C_ADDRESS 0x3C

//instantiate oled. 
SSD1306AsciiWire oled;

unsigned int adc0,adc1,adc2 = 0;
float adc0map,adc1map,adc2map = 0;

unsigned long millis0=0;//initial ms reading
unsigned long millis1 = 0; // ms reading to compare
unsigned long int next_millis = 0;

int x = 2;

// example. return ( (float)x * 20.0f / 16535.0f ) - 10.0f;
float scaleToPlusMinus10(unsigned int x2){
  x2 = x2 >> 8;
  return -10 + (float)x2 * 20.0 / 256  ;
}

void setup() {
  millis0 = millis();
  pinMode(PIN_PA1, INPUT); // use chip pin 11 as ADC; RV1; adc0
  pinMode(PIN_PA2, INPUT); // use chip pin 12 as ADC; RV2; adc1
  pinMode(PIN_PA3, INPUT); // use chip pin 13 as ADC; RV3; adc2
//start begin statement here.
  pinMode(7, OUTPUT);  // TX
  pinMode(PIN_PA4, OUTPUT); // PIN2 LED
  
  Wire.begin();   
  Serial.begin(9600);

  oled.begin(&Adafruit128x64, I2C_ADDRESS); 
  Wire.setClock(400000L);  
  
  
  //create leftmost strings
  oled.setFont(TimesNewRoman16_bold);
  oled.setCursor(0,0);
  oled.println("OUT1: ");
  oled.setCursor(0,28);
  oled.println("OUT2: ");
  oled.setCursor(0,56);
  oled.println("OUT3: ");
  oled.println("---audioDiWHY---");

 // oled.setFont(lcdnums12x16);  

}  //end setup 

void loop() {

  millis1 = millis();

  //toggle LED without using delay() stupidity
  
  if ((millis1 - millis0) > 250) {      
    int state = digitalRead(PIN_PA3);       // Read current state
    digitalWrite(PIN_PA4, !state);  // toggle LED
    millis0 = millis(); 
    x++;

    if (x > 10) {
      Serial.println();
      x = 0;
    }    
    
  //read ADC's
  adc0 = analogReadEnh(PIN_PA1,16);
  adc1 = analogReadEnh(PIN_PA2,16);
  adc2 = analogReadEnh(PIN_PA3,16);  
  
    //write values
  // display.setCursor(40, 0);

  // put a black rectangle where the # needs to go.display
  //int x = 40, y = 0;               // top-left corner of text
  // int w = 88, h = 16;              // rectangle just large enough
  
 //oled.clear();
  

  
  
  adc0map = scaleToPlusMinus10(adc0);
  Serial.println(adc0);
  Serial.println(adc0map);
  oled.setCursor(60,0);
  oled.print(adc0map,2);
  oled.println("V");
  
    //y = 16;               // top-left corner of text
                // rectangle just large enough
  

  // display.setCursor(40, 16 );
  oled.setCursor(60,16);
  adc1map = scaleToPlusMinus10(adc1);
  Serial.println(adc1);
  Serial.println(adc1map);
  oled.print(adc1map,2);
  oled.println("V");

  
  //y = 32;               // top-left corner of text
                // rectangle just large enough
  // display.fillRect(x, y, w, h, SSD1306_BLACK);

  // display.setCursor(40, 32);
    //display.println(adc2map);
  oled.setCursor(60,32);
  adc2map = scaleToPlusMinus10(adc2);
  Serial.println(adc2);
  Serial.println(adc2map);
  oled.print(adc2map,2);
  oled.print("V");
  
  delay(50); 
  } // end millis if
    
}  // end main loop
