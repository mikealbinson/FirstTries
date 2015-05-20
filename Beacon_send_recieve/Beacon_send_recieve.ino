//By Michael Albinson

/*
* An idea I had that could potentially allow us to work outside the iBeacon protocol
* By having the beacon send out send_creds periodically and having phones running the same BLE 
* source code as on the app RBL released, a phone with bluetooth active could hypothetically run in
* the background until it sees the signal from our Beacon with a unique identifying message. Then the 
* phone could pair with the beacon, send back its ticket credentials, and the beacon could then
* display whether the ticket is valid or not. Not sure if that's actually possible from an iOS perspective
* as I haven't read the source code, but I thought it was an interesting idea
*/

//All libraries need to be install prior to code use

#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>

int CHARS_TO_READ =14, i=0, j=0, carrier; //counters and carrier for 
byte broadcastToken = 0, connectionToken = 0, writeToken = 0, connectionTest = 0, connectionChecker_CONNECTED = 1, connectionChecker_DISCONNECTED = 0, writeByte =0, turnOnWrite;
char sendCharArray[14] = {'s', 'e', 'n', 'd', '_', 'c', 'r', 'e', 'd', 's'};//Ideally, this is the string that casuses the phone's response, which is in barcode format as below (can be up to 14 chars)
String VIPTicket = "WdmMZkFsMooqcy", RegTicket = "75iGkxWAw695wp", VoidTicket = "igJ9PXHhlOVorC"; //Ticket types
char stringBase[14]; // used to extract the ticket code from the BLE shield -- Must array bound change to match CHARS_TO_READ
String finalString= "", stopBroadcast = "stop", stopString ="";//carries our incoming ticket code
char stopBase[4]; //used to parse a 'stop' input from user 




void setup (){
  ble_set_name("Ticketfly"); //Name the BLE
  ble_begin();
  Serial.begin(57600);
  Serial.println ("");
}


void loop (){
  i=0;
  
        while (broadcastToken == 0)
        {
          if (writeToken == 0)
          {  
            Serial.println (F("To begin broadcast type 'broadcast' at any time"));
            writeToken = 1;
          }
          if (Serial.available()){
            broadcastToken = 1;
            turnOnWrite = 0;
            Serial.println ("To stop broadcast, type 'stop'");
          }
          delay (500);
        }
        /*
	connectionTest = ble_connected(); //first check for connection
	if (connectionTest == connectionChecker_CONNECTED){
		if (connectionToken == 0){
                    //if (writeByte = 0){
			Serial.println ("Connected");
			connectionToken = 1;
                        //writeByte = 1; 
                    //}
		}
	}
	else if (connectionTest == connectionChecker_DISCONNECTED){
		if (connectionToken == 1){

			Serial.println (F("No Connection"));
			connectionToken = 0;
		}
	}
        */
	if (broadcastToken == 1) // so let's broadcast
        {
          if (connectionToken = 1) 
          {
            for (i=0;i<CHARS_TO_READ;i++)
            {
              ble_write ( sendCharArray[i] ); 
            } 
          delay (2000);
          }
        }
        
        

    j=0;
    if ( ble_available() )
    {
       while ( ble_available() ){
         carrier =ble_read(); //parses the BLE input ticket status
         Serial.write (carrier); //prints it
         if (j<CHARS_TO_READ) // breaks the status into readable chars
         {
           stringBase[j] = carrier; 
         }
         j++;
      }
      for  (i=0; i<CHARS_TO_READ; i++){
        finalString = finalString + stringBase[i]; //combines the chars into comparable string
      }
      Serial.println("");
      //Serial.println (finalString);

      if (finalString == VIPTicket){ //VIP ticket status
      Serial.println(F("ITS A VIP"));
      Serial.println(F("Good ticket mode begin *indicator on*"));
      delay (5000);
      Serial.println(F("Good ticket mode end *indicator off*"));    
    }
    
    else if (finalString == RegTicket){ // regular ticket status
      Serial.println (F("Normal Ticket"));
      Serial.println (F("Bad ticket mode begin"));
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      delay (1000);
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      delay (1000);
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      Serial.println (F("Bad ticket mode end"));
    }
    
    else if (finalString == VoidTicket){ // Void ticket status
      Serial.println (F("This is a void ticket"));
      Serial.println (F("Bad ticket mode begin"));
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      delay (1000);
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      delay (1000);
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      Serial.println (F("Bad ticket mode end"));
    }
    
    else { // All other unreadable types
      Serial.println (F("Check Ticket"));
      Serial.println (F("Bad ticket mode begin"));
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      delay (1000);
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      delay (1000);
      Serial.println (F("*indicator on*"));
      delay (1000);
      Serial.println (F("*indicator off*"));
      Serial.println (F("Bad ticket mode end"));
    }
    Serial.println();
  
        
          for (i=0; i<CHARS_TO_READ; i++)
          {
            stringBase[i] = 1;
          }
        finalString = "";
      }
        
        
     ble_do_events();

    j=0;
    if (turnOnWrite == 0) // cleans up data output slightly--avoids printing initial 'broadcast' string
    {
       while (Serial.available())
         Serial.read (); 
       turnOnWrite = 1;
    }
      
    if (Serial.available ())//to turn off broadcast--essentially puts the beacon in a standby mode  
    {                       //but it can still be connected to a phone and can still read ticket data input
      while (Serial.available())
      {
        carrier = Serial.read();
        //Serial.write (carrier);
        if (j<4){
          stopBase[j] = carrier;
        }
        j++;
      }
      
      for (i=0; i<4; i++)
      {
        stopString = stopString + stopBase[i];
      }
      Serial.println (stopString);
      
     if (stopString == stopBroadcast)
      {
        broadcastToken = 0;
        Serial.println (F("To restart broadcast type 'broadcast' at any time"));
      }
      
      stopString = "";
      
      for (i=0; i<4; i++)
      {
        stopBase[i] = 1;
      }
      
    }
    
   
}
