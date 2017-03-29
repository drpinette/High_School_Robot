//#define DEBUG
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
  delay(5000);
  
  //Listen for 3.8kHz start signal
  RC->waitForStart();
  Serial.println("Sound Detected!!");
  
  if(RC->readDistanceSonar(EAST_CCW_SONAR) < 12 && RC->readDistanceSonar(EAST_CW_SONAR) < 12)
  {
    //In Correct Starting Position - Do nothing
    Serial.println("Detected facing North");
  }
  else
  {
    Serial.println("Detected facing East");
    RC->rotate(DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 6.0, RC));
    RC->stop();
  }
  //digitalWrite(13, HIGH);
  //delay(1000);

  // Start Maze Navigation
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 12.0, RC));
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
  //_DS("========================checkPoint1"); _NL; 
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 12.0, RC));
  //_DS("========================checkPoint2"); _NL;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 12.0, RC));
  //_DS("========================checkPoint3"); _NL;
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 4.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 12.0, RC));
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), MoreThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), MoreThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CCW), LessThan, 12.0, RC));
  //RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CCW), MoreThan, 12.0, RC));
  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CW), LessThan, 12.0, RC));
  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CCW), LessThan, 12.0, RC));
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 4.0, RC));
  RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), MoreThan, 12.0, RC));
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), LessThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), LessThan, 12.0, RC));
  RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 4.0, RC));
  RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 4.0, RC));
  RC->stop();
 
}

void loop() {
  // put your main code here, to run repeatedly:

} 
