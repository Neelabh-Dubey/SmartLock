/*
 * Author Name: Neelabh Dubey
 * Team: ProblemSolver
 * FileName: code_box2
 * 
 * Description: Connecting box2 to any of the available boxes.
 */

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(9, 10);                    // nRF24L01(+) radio attached using Getting Started board 

const byte interruptPin = 2;
RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 011;       // Address of the other node in Octal format
const uint16_t other_node1 = 00; 




void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network/examples/I_am_Box2");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), rec, FALLING);
}

void loop() {
  
  network.update();                          // Check the network regularly

  
  

  char message[] = "I_am_box2";
  Serial.print("Sending_to_box3...");
  
  RF24NetworkHeader header(/*to node*/ other_node1);
  bool  ok = network.write(header,&message,sizeof(message));
  if (ok)
    Serial.println("ok....SENT TO BOX3");
  else
    Serial.println("failed.");
  delay(1000);
  
  Serial.print("Sending_to_box1...");
  
  RF24NetworkHeader header1(/*to node*/ other_node);
  ok = network.write(header,&message,sizeof(message));
  if (ok)
    Serial.println("ok...SENT TO BOX 1");
  else
    Serial.println("failed.");
  delay(1000);

  
}




void rec()
{
   RF24NetworkHeader header;        // If so, grab it and print it out
  // if(header.from_node == 0)
  char message[20];
    network.read(header,&message,sizeof(message));

   // }
   if(header.from_node==00 || header.from_node == 011)
   {
   Serial.print("From Node: ");
   Serial.print(header.from_node);
   Serial.print("    ");
   Serial.print("Message:");
   Serial.println(message);
   }
}
