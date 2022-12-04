#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); 

LiquidCrystal_I2C lcd(0x27,20,4);
Servo Myservo;

int laserPin1 = 8;
int buttonPin = 2;
int ledPin = 7;
int ledPin2 = 4;
int buzzer = 5;
int pos = 0;

byte lastbuttonState = LOW;
byte laserPin1State = LOW;
byte ledState = LOW;
byte ledState2 = LOW;

unsigned long debounceDuration = 50;
unsigned long lastTimeButtonStateChanged = 0;


void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.clear();
  pinMode(laserPin1, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  SPI.begin();      
  mfrc522.PCD_Init();   
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  Myservo.attach(3);

}

void loop() 
{
  digitalWrite(ledPin2, ledState2);

  byte buttonState = digitalRead(buttonPin);
  buttonState = digitalRead(buttonPin);

  if (millis() - lastTimeButtonStateChanged > debounceDuration) {
    byte buttonState = digitalRead(buttonPin);
    if (buttonState != lastbuttonState) {
      lastTimeButtonStateChanged = millis();
      lastbuttonState = buttonState;
      if (buttonState == LOW) {
        Myservo.write(pos);
      }
    }
  }


  while (ledState2 == LOW) {
    while (laserPin1State == LOW) {
      if ( ! mfrc522.PICC_IsNewCardPresent()) 
      {
        return;
      }
  
      if ( ! mfrc522.PICC_ReadCardSerial()) 
      {
        return;
      }
 
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
      if (content.substring(1) == "43 4F 9E A9") // Make sure you change this with your own UID number
      {
        Serial.println("Authorised access");
        Serial.println();
        laserPin1State = HIGH;
        digitalWrite(laserPin1, laserPin1State);
        ledState = HIGH;
        digitalWrite(ledPin, ledState);
        delay(100);
      }
 
      else if (content.substring(1) == "DA 81 0F B0")  {
        Serial.println(" Access denied");
        delay(100);
      }
    }

    if (laserPin1State == HIGH ){
      delay(100);
      int value = analogRead(A0);
      if (value < 900 && laserPin1State == HIGH) {
        ledState2 = HIGH;
        digitalWrite(ledPin2, ledState2);
        digitalWrite(buzzer, LOW); //Temp
        lcd.backlight();
        lcd.setCursor(1,0);
        lcd.print("Invade!!");
        lcd.setCursor(1,1);
        lcd.print("Warning!!");
        Myservo.write(180);
      }

      if ( ! mfrc522.PICC_IsNewCardPresent()) 
      {
        return;
      }
  
      if ( ! mfrc522.PICC_ReadCardSerial()) 
      {
        return;
      }
 
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
      if (content.substring(1) == "43 4F 9E A9") // Make sure you change this with your own UID number
      {
        Serial.println("Authorised access");
        Serial.println();
        laserPin1State = HIGH;
        digitalWrite(laserPin1, laserPin1State);
        ledState = HIGH;
        digitalWrite(ledPin, ledState);
        delay(100);
      }
 
      else if (content.substring(1) == "DA 81 0F B0")  {
        Serial.println(" Access denied");
        laserPin1State = LOW;
        digitalWrite(laserPin1, laserPin1State);
        ledState = LOW;
        digitalWrite(ledPin, ledState);
        ledState2 = LOW;
        digitalWrite(ledPin2, ledState2); 
        digitalWrite(buzzer, ledState2);
        lcd.begin();
        lcd.clear();
        delay(100);
      }
    
    }
  }

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
    {
      return;
    }
  
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {
      return;
    }
 
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
    if (content.substring(1) == "43 4F 9E A9") // Make sure you change this with your own UID number
    {
      Serial.println("Authorised access");
      Serial.println();
      laserPin1State = HIGH;
      digitalWrite(laserPin1, laserPin1State);
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
      delay(3000);
    }
 
    else if (content.substring(1) == "DA 81 0F B0")  {
      Serial.println(" Access denied");
      laserPin1State = LOW;
      digitalWrite(laserPin1, laserPin1State);
      ledState = LOW;
      digitalWrite(ledPin, ledState);
      ledState2 = LOW;
      digitalWrite(ledPin2, ledState2); 
      digitalWrite(buzzer, ledState2);
      lcd.begin();
      lcd.clear();
      delay(3000);
    }

}