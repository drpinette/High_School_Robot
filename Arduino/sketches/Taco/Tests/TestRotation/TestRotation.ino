#define DEBUG
#include <Octagon.h>
#include <Condition.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  RC = new RobotController();
  RC->initialize();
  Serial.begin(9600);
  delay(3000);
  
  if(RC->readDistanceSonar(EAST_CCW_SONAR) < 12 && RC->readDistanceSonar(EAST_CW_SONAR) < 12)
  {
    //In Correct Starting Position - Do nothing
    Serial.println("Detected facing North");
  }
  else
  {
    Serial.println("Detected facing East");
    Serial.println(RC->readDistanceSonar(EAST_CCW_SONAR));
    Serial.println(RC->sonarIdAt(East, NoSide, CCW));

    RC->rotate(255, new DistanceFor(EAST_CCW_SONAR, LessThan, 6.0, RC));
    //RC->rotate(255, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 6.0, RC));
    RC->stop();
    Serial.println("Done Rotating");
  }
} 

void loop() {
  // put your main code here, to run repeatedly:

} 
