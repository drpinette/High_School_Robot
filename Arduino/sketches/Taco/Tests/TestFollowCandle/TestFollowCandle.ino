#include <Octagon.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  Serial.begin(9600); 

  delay(2000);

  RC->followCandle(100, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));
}

void loop() {
  // put your main code here, to run repeatedly:

}
