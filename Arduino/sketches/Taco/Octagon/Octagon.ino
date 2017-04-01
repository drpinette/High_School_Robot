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
  delay(3000);
  
  //Listen for 3.8kHz start signal
  Serial.println("Waiting for sound start...");
  digitalWrite(13, HIGH);
  delay(2000);
  // RC->waitForStart();
  digitalWrite(13, LOW);
  Serial.println("Sound Detected!!");
  
  if(RC->readDistanceSonar(EAST_CCW_SONAR) < 12 && RC->readDistanceSonar(EAST_CW_SONAR) < 12)
  {
    //In Correct Starting Position - Do nothing
    Serial.println("Detected facing North");
  }
  else
  {
    Serial.println("Detected facing East");
    delay(10);
    Serial.println(RC->readDistanceSonar(EAST_CCW_SONAR));
    Serial.println(RC->sonarIdAt(East, NoSide, CCW));
    RC->rotate(100, new DistanceFor(EAST_CCW_SONAR, LessThan, 6.0, RC));  //RC->sonarIdAt(East, NoSide, CCW)
    RC->stop();
    Serial.println("Done Rotating");
  }
  //digitalWrite(13, HIGH);
  //delay(1000);

  // Start Maze Navigation
  // Move down center of maze
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 9, RC)); //Front Sensor Loses Wall
  RC->stop();
  delay(2000);
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), MoreThan, 9, RC)); //Back Sensor Loses Wall
  RC->stop();
  delay(2000);
  
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 8.0, RC)); //Front Sensor Finds Wall
  RC->stop();
  delay(2000);
  
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 8.0, RC));  //Back Sensor Finds Wall
  RC->stop();
  delay(2000);
  
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 9, RC));  //Front Sensor Loses Wall
  RC->stop();
  delay(2000);
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 5.0, RC));   //Move until north hits wall.
  RC->stop();
  delay(2000);
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, 21.0, RC));
  RC->stop();
  delay(2000);
  RC->rotate(100, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 6.0, RC));
  RC->stop();
  
  // Check for Candle
  if(RC->readUv(LEFT_UV) >= 80 || RC->readUv(RIGHT_UV) >= 80)
   {
      digitalWrite(13, HIGH);
      //RC->followCandle(100, BrightFor(RC->uvIdAt(leftUvSensor), MoreThan, 300, RC));
   }
  else 
   {
      //Do Nothing, Move to next room
   }
  
  RC->followWall(Right, East, 100, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 9.0, RC));  //Front Sensor finds wall edge
  RC->stop();
  delay(2000);
  RC->followWall(Right, East, 100, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 9.0, RC));  //Back Sensor finds wall edge
  RC->stop();
  delay(2000);
  RC->followWall(Right, East, 100, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 9.0, RC));  //Front Sensor finds wall edge of 2nd Room
  RC->stop();
  delay(2000);
  RC->followWall(Right, East, 100, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 9.0, RC));  //Back Sensor finds wall edge of 2nd Room
  RC->stop();
  delay(2000);
  // Check for candle
  RC->rotate(DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 6.0, RC));
  RC->stop();

  
   
//  //Move South to center of maze
//  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 12.0, RC));
//  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
//  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), MoreThan, 12.0, RC));
//  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), MoreThan, 12.0, RC));
//  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CCW), LessThan, 12.0, RC));
//  
//  //RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CCW), MoreThan, 12.0, RC));
//  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CW), LessThan, 12.0, RC));
//  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CCW), LessThan, 12.0, RC));
//  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), LessThan, 4.0, RC));
//  RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
//  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), MoreThan, 12.0, RC));
//  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 12.0, RC));
//  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 12.0, RC));
//  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), LessThan, 12.0, RC));
//  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), LessThan, 12.0, RC));
//  RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
//  RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 4.0, RC));
//  RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 4.0, RC));
//  RC->stop();
// 
}

void loop() {
  // put your main code here, to run repeatedly:

} 
