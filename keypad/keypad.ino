/*
 * Author Name: Neelabh Dubey
 * Team Name: ProblemSolver
 * Filename: keypad
 * 
 * Description: Data input for OTP at main box in train
 */


#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
 unsigned long time_last = 0;
 unsigned long time_now = 0;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 10, 11}; //connect to the column pinouts of the keypad
int count=0;
int i=0;
const byte interruptPin2 = 2; 
int interval = 250;
char key;

Keypad customkeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(115200);
  pinMode(interruptPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), key_press, RISING);
}

void key_press()
{
  detachInterrupt(digitalPinToInterrupt(interruptPin2));
  i=1;
  Serial.println("Choose from menu below ---");
  Serial.println("Press '*' to Enter OTP--");
  Serial.println("Press '#' to Generate OTP--");
  
}


void loop(){
   if(i=1){
      key = customkeypad.getKey();
      if(key){
      Serial.println(key);
      }
  if(key == '*'){
      Serial.println("Enter your OTP--");
  }
   if (key =='#'){
   Serial.println("OTP is send to your registered email id. ");
  } 
  if (key =='0'|| key =='1'||key =='2'||key =='3'||key =='4'||key =='5'||key =='6'||key =='7'||key =='8'||key =='9'){
   count++;
   if(count ==4){
        Serial.println("Verifying......");
        i=0;
  } 
  }     
 }

      pinMode(interruptPin2, INPUT_PULLUP);  
     attachInterrupt(digitalPinToInterrupt(interruptPin2),key_press,RISING);
}
