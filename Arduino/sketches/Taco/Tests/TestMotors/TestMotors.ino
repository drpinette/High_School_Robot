#define DEBUG
#include <Octagon.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  delay(3000);
  Serial.begin(9600);
  
  RC->go(North, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(2000);
  RC->stop();
 
//  RC->go(East, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
//  delay(2000);
//  RC->stop();
//  
//  RC->go(South, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
//  delay(2000);
//  RC->stop();
//
//  RC->go(West, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
//  delay(2000);
//  RC->stop();
// 
//  RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
//  delay(2000);
//  RC->stop();
//  RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
//  delay(2000);
//  RC->stop();
  
}

void loop() {
  // put your main code here, to run repeatedly:

} 
