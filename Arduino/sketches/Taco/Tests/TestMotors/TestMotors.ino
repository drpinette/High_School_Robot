#define DEBUG
#include <Null.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  delay(1000);
  Serial.begin(9600);
 
  RC->go(North, 0, Left, SLOW_SPEED, NoRotation, 0);
  delay(4000);
  RC->stop();
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
/* 
  RC->go(South, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(4000);
  RC->stop();
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);

  RC->go(East, 0, Left, DEFAULT_SPEED, NoRotation, 0);
  delay(4000);
  RC->stop();
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
 
  RC->go(West, 0, Left, DEFAULT_SPEED, NoRotation, 0);
  delay(4000);
  RC->stop();
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  */
}

void loop() {
  // put your main code here, to run repeatedly:

} 
