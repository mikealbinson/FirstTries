// By Michael Albinson

/*
* Decided I might as well put all my work into one sketch even though it inexplicably still doesn't work together
* even though each works perfectly on their own. I cut out the random number generator, but otherwise it's all about
* the same as in TicketConditionals.ino and BarcodeIntegration.ino -- This is what it should look like
*/

// include the necesary RBL setup files
#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int CHARS_TO_READ = 14; // max of 16
int carrier, a=0;
String VIPTicket = "WdmMZkFsMooqcy", RegTicket = "75iGkxWAw695wp", VoidTicket = "igJ9PXHhlOVorC"; //Ticket types
char stringBase[14]; // used to extract the ticket code from the BLE shield -- Must change array bound to match CHARS_TO_READ
String finalString= "";//carries our incoming ticket code



void setup()
{  
  ble_set_name("Ticketfly"); //Name the BLE
  ble_begin(); // initializes functions of the RBL setup files
  Serial.begin(57600); //serial window baud
  while (!Serial);{}
  
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // clearing the display
  display.clearDisplay();
  delay(1000);
  
  display.setTextSize(2); // Fun initialization thingy, can be cut and it will not change the code below
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("TicketFly");
  display.setCursor(10,20);
  display.println("Beacon");
  display.display();
 
  display.startscrollright(0x00, 0x0F); //moves the words around the screen when it first starts up
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
  
  a=0;
  if ( ble_available() )
  {
    while ( ble_available() ){
      carrier =ble_read(); //parses the BLE input ticket status
      Serial.write (carrier); //prints it
      if (a<CHARS_TO_READ) // breaks the status into readable chars
       {
        stringBase[a] = carrier; 
       }
       a++;
    }
    for  (a=0; a<CHARS_TO_READ; a++){
    finalString = finalString + stringBase[a]; //combines the chars into comparable string
  }
   Serial.println("");
   
   // Serial.println (INCOMING_MSG); these two can be brought in to see the input ticket status
   //Serial.println();

    if (finalString == VIPTicket){ //VIP ticket status
      Serial.println("It's a VIP");
      display.setTextSize(5);
      display.setTextColor(WHITE);
      display.setCursor(25,20);
      display.clearDisplay();
      display.println("VIP");
      display.display();
      delay(2000);   
    }
    
    else if (finalString == RegTicket){ // regular ticket status
      Serial.println ("Normal Ticket");
      display.setTextSize(5);
      display.setTextColor(WHITE);
      display.setCursor(35,20);
      display.clearDisplay();
      display.println("OK");
      display.display();
      delay(2000);
    }
    
    else if (finalString == VoidTicket){ // Void ticket status
      Serial.println ("This is a void ticket");
      Serial.println ("Bad ticket mode begin");
      display.setTextSize(9);
      display.setTextColor(WHITE);
      display.setCursor(45,0);
      display.clearDisplay();
      display.println("X");
      display.display();
      delay(2000);
      Serial.println ("Bad ticket mode end");
    }
    
    else { // All other unreadable types
      Serial.println ("Check Ticket");
      Serial.println ("Bad ticket mode begin");
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(45,0);
      display.clearDisplay();
      display.println("Check ticket");
      display.display();
      delay(2000);
      Serial.println ("Bad ticket mode end");
    }
    Serial.println();
  }

  else {
  //Serial.println ("No Ticket Detected *No Indicator*");
  delay(1000);
  }
  
  if ( Serial.available() ) // allows the console to send messages back to the phone/bluetooth device
  {
    delay(5);
    
    while ( Serial.available() )
        ble_write( Serial.read() );
  }
  ble_do_events();
  
  //"Clean" stringBase and INCOMING_MSG
  for (a=0; a<CHARS_TO_READ; a++){
    stringBase[a] = 1;
  }
  finalString= "";
  
  //resets the screen to read "Tfly Receiver" at the end of each iteration
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("Tfly");
  display.setCursor(10,20);
  display.println("Receiver")
  display.display();
  delay (5000); // delay can be shortened as desired, it just allows for the screen to display the message longer
                // and demonstrate that it is moving on to its next ticket read
  
}
