#define DEBUG
#include <Octagon.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  Serial.begin(9600);
}

void loop() {
  _DS(RC->readDistanceSonar(NORTH_CCW_SONAR));
  delay(10);
  _DS(RC->readDistanceSonar(NORTH_CW_SONAR));
  delay(10);
  _DS(RC->readDistanceSonar(EAST_CCW_SONAR));
  delay(10);
  _DS(RC->readDistanceSonar(EAST_CW_SONAR));
  delay(10);
  _DS(RC->readDistanceSonar(SOUTH_CCW_SONAR));
  delay(10);
  _DS(RC->readDistanceSonar(SOUTH_CW_SONAR));
  delay(10);
  _DS(RC->readDistanceSonar(WEST_CCW_SONAR));
  delay(10);
  _DS(RC->readDistanceSonar(WEST_CW_SONAR));
  delay(10);
  _NL;
  delay(1000);
}
