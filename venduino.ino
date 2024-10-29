#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#define led 4
 Servo serv;


// idk what to do for this


int i = 0;
int iCount = 0;
float total = 0;


void iIncoming() {
  iCount = iCount +1;
}
void setup() {
  attachInterrupt(digitalPinToInterrupt(1), iIncoming, RISING);
  serv.attach(9);
  pinMode(led,OUTPUT);
  Serial.begin(9600);
  digitalWrite(led,HIGH);
}
void loop() {
  //attachInterrupt(digitalPinToInterrupt(0), iIncoming, FALLING);
  Serial.println(iCount);
  if (iCount == 1) {
    digitalWrite(led,LOW);
    iCount = 0;
  } else {
    digitalWrite(led, HIGH);
  }

  /* serv.write(270);
   delay(15);
   serv.write(0);
  } else {
    digitalWrite(led,HIGH);
  }*/
}


