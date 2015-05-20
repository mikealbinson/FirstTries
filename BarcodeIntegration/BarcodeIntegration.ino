
//By Michael Albinson
//Barcode string integration using only the arduino BLE sheild (only currently using three example barcode strings
//this could fairly easily be expanded

//all necessary libraries will need to be downloaded and added to the arduino library folder before it can be run

// include the necesary RBL setup files
#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);

int CHARS_TO_READ = 14; // max of 16
int carrier, a=0;
String VIPTicket = "WdmMZkFsMooqcy", RegTicket = "75iGkxWAw695wp", VoidTicket = "igJ9PXHhlOVorC"; //Ticket types
char stringBase[14]; // used to extract the ticket code from the BLE shield -- Must array bound change to match CHARS_TO_READ
String finalString= "";//carries our incoming ticket code



void setup()
{  
  ble_set_name("Ticketfly"); //Name the BLE
  ble_begin(); // initializes functions of the RBL setup files
  Serial.begin(57600); //serial window baud
  while (!Serial);{}
  
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
      Serial.println("ITS A VIP");
      Serial.println("Good ticket mode begin *indicator on*");
      delay (5000);
      Serial.println("Good ticket mode end *indicator off*");    
    }
    
    else if (finalString == RegTicket){ // regular ticket status
      Serial.println ("Normal Ticket");
      Serial.println ("Bad ticket mode begin");
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
      delay (1000);
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
      delay (1000);
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
      Serial.println ("Bad ticket mode end");
    }
    
    else if (finalString == VoidTicket){ // Void ticket status
      Serial.println ("This is a void ticket");
      Serial.println ("Bad ticket mode begin");
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
      delay (1000);
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
      delay (1000);
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
      Serial.println ("Bad ticket mode end");
    }
    
    else { // All other unreadable types
      Serial.println ("Check Ticket");
      Serial.println ("Bad ticket mode begin");
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
      delay (1000);
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
      delay (1000);
      Serial.println ("*indicator on*");
      delay (1000);
      Serial.println ("*indicator off*");
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
}
