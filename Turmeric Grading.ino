
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "ThingSpeak.h"
const char* ssid = "mahi";   // your network SSID (name)
const char* password = "mahi1026";   // your network password

WiFiClient  client;

unsigned long myChannelNumber =   2028997;
const char * myWriteAPIKey = "RY1WSUQNMZWS8GNU";
#define FIELD1 1

unsigned long lastTime = 0;
unsigned long timerDelay = 3;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

#define outPin 15
#define s0 19
#define s1 18
#define s2 2
#define s3 4
int out1, out2, out3;
boolean DEBUG = true;

// Variables
int red, grn, blu;
String color ="";
long startTiming = 0;
long elapsedTime =0;


void setup(){
  Serial.begin(115200);  
 
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT); //out from sensor becomes input to arduino

  // Setting frequency scaling to 100%
  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);
  ThingSpeak.begin(client);

  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // Print a message on both lines of the LCD.
  lcd.setCursor(1,0);   //Set cursor to character 2 on line 0
  lcd.print("Turmeric");
  lcd.setCursor (1,1);
  lcd.print("Grading");
  delay(2000);  
  lcd.clear();  
  startTiming = millis();
  WiFi.mode(WIFI_STA);  
}

void loop(){
  getColor();
  showDataLCD();
   
  if(DEBUG)
     
  elapsedTime = millis()-startTiming;
  if (elapsedTime > 1000) {
    
    startTiming = millis();
  }
  if ((millis() - lastTime) > timerDelay) {
   
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password);
        delay(5000);    
      }
      Serial.println("\nConnected.");
    }
  }
   
    {
      if (color == "A - GRADE")
     {
       out1 == HIGH;
      }
     else if (color == "B - GRADE")     
     {       
       out2 == HIGH;
      }
     else 
     {
       out3 == HIGH;
      }
      
    ThingSpeak.setField(1, red);
    ThingSpeak.setField(2, grn);
    ThingSpeak.setField(3, blu);
    ThingSpeak.setField(4, out1);
    ThingSpeak.setField(5, out2);
    ThingSpeak.setField(6, out3);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
       
    lastTime = millis();
  }
    

}
/* read RGB components */
void readRGB()
{
  red = 0, grn=0, blu=0;
 
  int n = 10;
  for (int i = 0; i < n; ++i){
    //read red component
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    red = red + pulseIn(outPin, LOW);
 
   //read green component
    digitalWrite(s2, HIGH);
    digitalWrite(s3, HIGH);
    grn = grn + pulseIn(outPin, LOW);
   
   //let's read blue component
    digitalWrite(s2, LOW);
    digitalWrite(s3, HIGH);
    blu = blu + pulseIn(outPin, LOW);
  }
  red = red/n;
  grn = grn/n;
  blu = blu/n;

}  
///***************************************************
//* Showing capured data at LCD
//****************************************************/
void showDataLCD(void){
lcd.clear();
lcd.setCursor (0,4);

lcd.print("GRADE: ");  
lcd.print(color);  
}



void getColor(){  
  readRGB();

    if(red > 40 && red < 50 && grn > 55 && grn < 65 && blu >  45 && blu < 55) color = "A - GRADE";//BRIGHT YELLOW ORANGE COLOR
else if(red > 55 && red < 65 && grn > 75 && grn < 85 && blu > 65 && blu < 75) color = "B - GRADE";//LIGHT YELLOW ORANGE COLOR
else  color = "UNDER GRADE";// FOR ANYOTHER COLOR CODE
        
}
