#define DEBUG
#include <Null.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  
  RC-> runMotor(0, FORWARD, DEFAULT_SPEED);
  delay(3000);
  RC-> runMotor(0, BRAKE, 0);
  delay(3000);
  RC-> runMotor(1, FORWARD, DEFAULT_SPEED);
  delay(3000);
  RC-> runMotor(1, BRAKE, 0);
  delay(3000);
  RC-> runMotor(2, FORWARD, DEFAULT_SPEED);
  delay(3000);
  RC-> runMotor(2, BRAKE, 0);
  delay(3000);
  RC-> runMotor(3, FORWARD, DEFAULT_SPEED);
  delay(3000);
  RC-> runMotor(3, BRAKE, 0);
  RC->stop();
}


void loop() {
  // put your main code here, to run repeatedly:

} 
