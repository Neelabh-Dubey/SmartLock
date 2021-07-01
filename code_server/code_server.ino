/*
 * Author Name: Neelabh Dubey
 * Team: ProblemSolver
 * FileName: code_server
 * 
 * Description: Connection of server with boxes in compartment
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>



RF24 radio(9, 10);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 02;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 00;   // Address of the other node in Octal format

const byte interruptPin = 2;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network/examples/I_m_SERVER");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), rec, FALLING);
}

void loop(void){
  
  network.update();                  // Check the network regularly
}



void rec()
{
   RF24NetworkHeader header;        // If so, grab it and print it out
  // if(header.from_node == 01)
  char message[20];
  network.read(header,&message,sizeof(message));

   // }
   if(header.from_node==00)
   {
     Serial.print("From Node: ");
     Serial.print(header.from_node);
     Serial.print("    ");
     Serial.print("Message:");
     Serial.println(message);
   }
   
}
