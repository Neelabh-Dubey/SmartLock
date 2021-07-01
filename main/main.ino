#define BLYNK_PRINT Serial
#define TINY_GSM_MODEM_SIM800
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

unsigned long time_last = 0;
unsigned long time_now = 0;
int interval = 2000;


#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleTinyGSM.h>

SoftwareSerial SerialAT(12, 13); // TX, RX
TinyGsm modem(SerialAT);

/*Variable for slave server NRF*/
char authorised_access = 'n';
char alert_to_server = '0';
/**********************************/
const int buzzer = 22;
const int led = 23;
const byte ROWS = 4;
const byte COLS = 4;
char data[4]={0};
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 10, 11}; //connect to the column pinouts of the keypad
int count=0;
int i=0,j=0,k=0,checker=0,marker =0, reader =0;
const byte interruptPin = 2; 
char key;
char x;
String password;
char auth[] = "_SNl5zsSKUMEfqbJ1km2p5VfWlK2pQnj";
char apn[]  = "internet";
char user[] = "";
char pass[] = "";
Keypad customkeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/////////////////////////////////////////////////////////////
void buzzer_LED_Alert (){
  digitalWrite(buzzer,HIGH);
  digitalWrite(led,HIGH);
  delay(5000);
  digitalWrite(buzzer,LOW);
  digitalWrite(led,LOW);
}
////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
void transmitData(){
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write('X');        // sends five bytes           
  Wire.endTransmission();    // stop transmitting
  delay(500);
}
//////////////////////////////////////////////////////////////////////
void receiveData(){
    Wire.requestFrom(8, 1);    // request 6 bytes from slave device #8
    while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
    if(c=='w')
    {
      alert_to_server = '1';
      buzzer_LED_Alert();
      
    }
    else if(c=='W')
    {
      alert_to_server = '0';
    }

      Wire.beginTransmission(8); // transmit to device #8
      Wire.write('z');        // sends five bytes           
      Wire.endTransmission();    // stop transmitting
  }
  delay(500);
}
//////////////////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  delay(100);
  SerialAT.begin(9600);
  delay(3000);
  Serial.println("Initializing modem...");
  modem.restart();
  Blynk.begin(auth, modem, apn, user, pass);
  password = (String)random(1000,9999);
  Blynk.virtualWrite(V1,password);
  Serial.println(password);
  Blynk.email("neelabhdubey9@gmail.com", "Alert", password);
  Wire.begin();
  pinMode(buzzer,OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), key_press, RISING);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {          // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15, 10);
  display.println("Welcome  to INDIAN       RAILWAYS---");
  display.display(); 
}
///////////////////////////////////////////////////////////////////////////////////
void key_press()
{
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  i=1;
  reader = 1;
  Serial.println("Choose from menu below ---");
  Serial.println("Press '*' to Enter OTP--");
  Serial.println("Press '#' to Generate OTP--");
}
///////////////////////////////////////////////////////////////////////////////////
void compare(){
 for(k=0;k<4;k++){
    if(password[k]==data[k]){
      checker = 1;
    }
    else checker = 0;
    if(checker == 0){
     Serial.println("Wrong Password");
     display.clearDisplay();
     display.setCursor(10, 10);
     display.println("Wrong Password");
     display.display();
     break;
    }
 }
  if(checker == 1){
   Serial.println("Access Granted");
   display.clearDisplay();
   display.setCursor(10, 10);
   display.println("Access Granted");
   display.display();
   authorised_access = 'y';
   transmitData();
 }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){
  if(i=1){
    while(j<1){
        display.println("* to Enter OTP-");
        display.println("# to Generate OTP-:");
        display.display();
        j++;
    }
     key = customkeypad.getKey();
      if(key){
      Serial.print(key);
      display.print(key);
      display.display();
      }
    if(key == '*'){
      Serial.println("Enter your OTP--");
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Enter your OTP-:");
      display.display();
  }
    if (key =='#'){      
     Serial.println("OTP is send to your registered email id. ");
     display.clearDisplay();
     display.setCursor(0, 10);
     display.println("OTP is send to your registered email id. ");
     display.display();
     marker =1;
   } 
    if (key =='0'|| key =='1'||key =='2'||key =='3'||key =='4'||key =='5'||key =='6'||key =='7'||key =='8'||key =='9'){
      data[count]=key;
     count++;
    if(count ==4){
     Serial.println("  ");
     Serial.println("Verifying......");
     display.println(" ");
     display.println("Verifying.....");
     display.display();
     compare();
     i=0;
    } 
  }     
  }
  if(reader == 1){
     pinMode(interruptPin, INPUT_PULLUP);  
     attachInterrupt(digitalPinToInterrupt(interruptPin),key_press,RISING); 
  }
  if(marker ==1) Blynk.run();
  marker = 0;

   time_last = millis();
   if(time_last-time_now>interval)
   {
    receiveData();
    time_now = millis(); 
   }
  
}
