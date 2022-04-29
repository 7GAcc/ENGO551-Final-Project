/*
 * Zian Zahid Apr 2022
 * Basic RFID scanner code was taken from: http://randomnerdtutorials.com/ and was Created by FILIPEFLOP
 * It was then modified to suit the needs of this project and include a multi-color LED and ultrasonic sensor
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include "SR04.h"
#define TRIG_PIN 2
#define ECHO_PIN 7
#define SS_PIN 10 //SDA
#define RST_PIN 9
SR04 sr04=SR04(ECHO_PIN,TRIG_PIN);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int red=3;
int blue=6;
int green=5;
int r=0;
int g=0;
int b=255;
int start=1;
long a;
long dist=76;
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
}
void loop() 
{
  //If alarm was just turned on/reset, wait a while for user to open and close the door, then get distance between wall and door
  if(start==1){
    start=0;
    delay(5000);
    dist=sr04.Distance();
  }
  a=sr04.Distance();
  RGB_color(r,g,b);
  if (a<(dist-2)||a>(dist+2)){
    r=255;
    g=0;
    b=255;
    Serial.print("Motion Detected\n");
    
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "9C D7 0E 22") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    RGB_color(0,255,0);
    delay(5000);
  }
 
 else   {
    Serial.println(" Access denied");
    if (r==255 && g==0 && b==255){
      RGB_color(255,0,0);
      delay(3000);
    }
    else {
      r=255;
      g=0;
      b=0;
    }
    RGB_color(r,g,b);
  }
} 
void RGB_color(int red_value, int green_value, int blue_value)
{

analogWrite(red, red_value);

analogWrite(green, green_value);

analogWrite(blue, blue_value);

}
