// By Michael Albinson

/*
* Decided to play with switch cases a little here as I have never really used them before, but each case corresponds
* to an if statement in the BarcodeIntegration.ino sketch, so I combined them in a third sketch that doesn't quite
* work yet. But this functions perfectly on its own.
*/



//All OLED libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//defining the display
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int x;

void setup () {

  // Initializing the Serial connection to PC at 57600 bauds
  Serial.begin(57600);

  // Initializing the LCD
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // clearing the display
  display.clearDisplay();
  delay(1000);
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("TicketFly");
  display.setCursor(10,20);
  display.println("Beacon");
  display.display();
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.clearDisplay();
}

void loop()
{

  x = random (1,4);

  switch (x){
  
  case 1: //VIP case
    Serial.println ("VIP");
     display.setTextSize(5);
    display.setTextColor(WHITE);
    display.setCursor(25,20);
    display.clearDisplay();
    display.println("VIP");
    display.display();
    delay(2000);
    break;
  
  case 2: //Normal case
    Serial.println ("Normal");
    display.setTextSize(5);
    display.setTextColor(WHITE);
    display.setCursor(35,20);
    display.clearDisplay();
    display.println("OK");
    display.display();
    delay(2000);
    break;
    
   case 3: //Void case
    Serial.println ("Void");
    display.setTextSize(9);
    display.setTextColor(WHITE);
    display.setCursor(45,0);
    display.clearDisplay();
    display.println("X");
    display.display();
    delay(2000);
    break;
  
  default: //Unknown/default case
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,0);
    display.clearDisplay();
    display.println("UNKNOWN");
    display.display();
    delay(2000);
    display.clearDisplay();
    display.println("Check Ticket");
    display.display();
    delay(2000);
    break;
  }
  
  Serial.println(x);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("Tfly");
  display.setCursor(10,20);
  display.println("Receiver");
  display.display();
  delay (5000);

}
