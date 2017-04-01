//#define DEBUG
#include <Octagon.h>
#include <Condition.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  int x = 0;
  RC = new RobotController();
  RC->initialize();
  Serial.begin(9600);
  delay(3000);
  RC->extinguish(false);
  delay(5000);
  RC->extinguish(true);
  RC->extinguish(false);
  //Listen for 3.8kHz start signal
  Serial.println("Waiting for sound start...");
  digitalWrite(13, HIGH);
  delay(2000);
  // RC->waitForStart();
  digitalWrite(13, LOW);
  Serial.println("Sound Detected!!");
  
  if(RC->readDistanceSonar(EAST_CCW_SONAR) < 14 && RC->readDistanceSonar(EAST_CW_SONAR) < 14)
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
    RC->rotate(100, CW, new DistanceFor(EAST_CCW_SONAR, LessThan, 6.0, RC));  //RC->sonarIdAt(East, NoSide, CCW)
    RC->stop();
    Serial.println("Done Rotating");
  }
  //digitalWrite(13, HIGH);
  //delay(1000);

  // Start Maze Navigation
  // Move down center of maze
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 9, RC)); //Front Sensor Loses Wall
  //Serial.println("-------------------Front Sensor Loses Wall--------------------------");
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), MoreThan, 9, RC)); //Back Sensor Loses Wall
  //Serial.println("-------------------Back sensor loses wall---------------------");
  
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 9.0, RC)); //Front Sensor Finds Wall
  //Serial.println("-------------------Front sensor finds wall-------------------");
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 9.0, RC));  //Back Sensor Finds Wall
 // Serial.println("------------------------Back sensor finds wall-------------------");
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 9, RC));  //Front Sensor Loses Wall
 // Serial.println("---------------------------Front sensor loses wall-----------------------");
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 4.0, RC));   //Move until north hits wall.
//  Serial.println("-----------------------Move until North hits wall-----------------------");
  
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 5.0, RC));
 // Serial.println("-----------------------------------Move East until front sensor detects wall-----------------------");
  RC->stop();
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 5.0, RC));
 // Serial.println("--------------------------------Move East until back sensor detects wall----------------");
  RC->stop();
  float distanceToWall = RC->readDistanceSonar(EAST_CW_SONAR);
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, distanceToWall - 6.0, RC));
  
  //rotate test for candle in room 2
  RC->maxUv = RC->readUv(RIGHT_UV);
  RC->rotate(100, CW, new DistanceFor(WEST_CCW_SONAR, LessThan, RC->readDistanceSonar(NORTH_CW_SONAR), RC));
  RC->stop();
  if(RC->readUv(LEFT_UV) >= 80)
  {
    digitalWrite(13, HIGH);
    RC->followWall(Left, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Left, CCW), LessThan, 7.0, RC));
    RC->followCandle(DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 5.0, RC));
    RC->stop();
    delay(9999999999999999);
  }
  distanceToWall = RC->readDistanceSonar(WEST_CW_SONAR);
  RC->rotate(100, CW, new DistanceFor(SOUTH_CCW_SONAR, LessThan, distanceToWall, RC));
  if(RC->maxUv >= 80)
    {
      digitalWrite(13, HIGH);
      RC->move(North, DEFAULT_SPEED,new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 7.0, RC)); 
     //RC->followCandle(DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 5.0, RC));
      if(RC->readUv(RIGHT_UV) >= 150)
      {
        RC->stop();
        //RC->followCandle(DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 5.0, RC));
        RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
        delay(200);
        RC->stop(); 
        delay(9999999999999999);
      }
      else {
        distanceToWall = RC->readDistanceSonar(EAST_CCW_SONAR);
        RC->rotate(100, CCW, new DistanceFor(SOUTH_CW_SONAR, LessThan, distanceToWall, RC));
        RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 5.0, RC));
        RC->stop();
        delay(99999);
      }
    }
  else {
    distanceToWall = RC->readDistanceSonar(SOUTH_CCW_SONAR);
    RC->rotate(100, CCW, new DistanceFor(WEST_CW_SONAR, LessThan, distanceToWall, RC));
    distanceToWall = RC->readDistanceSonar(WEST_CW_SONAR);
    RC->rotate(100, CCW, new DistanceFor(NORTH_CW_SONAR, LessThan, distanceToWall, RC));
  }
  RC->stop();
  delay(9999999999);
  //Finish rotation for room 2, slide into room 1.
  RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 9.0, RC));
  RC->stop();
  delay(x);
  RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 9.0, RC));
  RC->stop();
  delay(x);
   RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 9.0, RC)); //inside hallway
  RC->stop();
  delay(x);
  RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 9.0, RC));
  RC->stop();
  delay(x);
  //Enter into room 1  
  digitalWrite(12, HIGH);
  delay(10000000);


 // RC->maxUv = RC->readUv(RIGHT_SENSOR);
  
  RC->rotate(100, CW, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 6.0, RC));
  
  RC->stop();
  
  // Check for Candle
  if(RC->readUv(LEFT_UV) >= 80 || RC->readUv(RIGHT_UV) >= 80)
   {
      digitalWrite(13, HIGH);
      RC->followCandle(100, new False());
   }
  else 
   {
      //Do Nothing, Move to next room
   }
  
  RC->followWall(Right, East, 100, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 9.0, RC));  //Front Sensor finds wall edge
  RC->stop();
  delay(x);
  RC->followWall(Right, East, 100, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 9.0, RC));  //Back Sensor finds wall edge
  RC->stop();
  delay(x);
  RC->followWall(Right, East, 100, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 9.0, RC));  //Front Sensor finds wall edge of 2nd Room
  RC->stop();
  delay(x);
  RC->followWall(Right, East, 100, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 9.0, RC));  //Back Sensor finds wall edge of 2nd Room
  RC->stop();
  delay(x);
  // Check for candle
  RC->rotate(DEFAULT_SPEED, CW, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 6.0, RC));
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
