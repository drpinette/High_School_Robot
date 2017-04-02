//#define DEBUG
#include <Octagon.h>
#include <Condition.h>
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>

RobotController* RC;

void setup() {
  int x = 500;
  RC = new RobotController();
  RC->initialize();
  Serial.begin(9600);
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


  //Check for dog
  bool dog = false;
  
  //dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 10;
  //dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CW)) < 10;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 4.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 15;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 5.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 15;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 6.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 15;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 7.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 15;
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 3.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CW)) < 15;
  RC->stop();
  if(dog == false)
  {
  RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CCW), MoreThan, 6.5, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CW)) < 15;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 4.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 15;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 5.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 15;
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), MoreThan, 6.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 15;
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 3.0, RC));
  dog |= RC->readDistanceSonar(RC->sonarIdAt(West, NoSide, CCW)) < 15;
  RC->stop();
  }

  //Return to starting position
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 4.0, RC));

  // Start Maze Navigation
  if(!dog)
  {
    RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), LessThan, 12.0, RC));
    RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 12.0, RC));
    RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), MoreThan, 12.0, RC));
    RC->go(West, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
    delay(300);
    RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));

    //test for UV
    if(RC->readUv(LEFT_UV) >= 80)
    {
    //Room 4 Candle A
      digitalWrite(13, HIGH);
      RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
      delay(300);
      RC->stop();
      RC->extinguish(true);
      RC->extinguish(false);
      return;
    }
    float distanceToWall = RC->readDistanceSonar(WEST_CW_SONAR);
    RC->rotate(100, CW, new DistanceFor(SOUTH_CCW_SONAR, LessThan, distanceToWall + .25, RC));
    RC->stop();
    if(RC->readUv(LEFT_UV) >= 80 || RC->readUv(RIGHT_UV) >= 80)
    {
      //room 4 candle B
      digitalWrite(13, HIGH);
      if(RC->readUv(LEFT_UV) > RC->readUv(RIGHT_UV))
      {
        RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));
        RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
        delay(300);
        RC->stop();
        RC->extinguish(true);
        RC->extinguish(false);
        return;
      }
      else
      {
        //room 4 candle C
        RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 5.0, RC));
        RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
        delay(350);
        RC->stop();
        RC->extinguish(true);
        RC->extinguish(false);
        return;
      }
    }
    else
    {
      //rotate back North
      distanceToWall = RC->readDistanceSonar(WEST_CW_SONAR);
      RC->rotate(100, CCW, new DistanceFor(NORTH_CW_SONAR, LessThan, distanceToWall + .25, RC));
      RC->stop();
    }
    //move out of room 4
    RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 5.0, RC));
    RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, 5.0, RC));
    //back at original starting position
  }

  // Move down center of maze
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 9, RC)); //Front Sensor Loses Wall
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), MoreThan, 9, RC)); //Back Sensor Loses Wall
  
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), LessThan, 9.0, RC)); //Front Sensor Finds Wall
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CW), LessThan, 9.0, RC));  //Back Sensor Finds Wall
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Right, CCW), MoreThan, 9, RC));  //Front Sensor Loses Wall
  RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 4.0, RC));   //Move until north hits wall.
  
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CCW), LessThan, 24.0, RC));
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CW), LessThan, 24.0, RC));
  float distanceToWall = RC->readDistanceSonar(EAST_CW_SONAR);
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, distanceToWall - 6.0, RC));
  
  //rotate test for candle in room 2
  RC->maxUv = RC->readUv(RIGHT_UV);
  distanceToWall = RC->readDistanceSonar(NORTH_CW_SONAR);
  RC->rotate(100, CW, new DistanceFor(WEST_CCW_SONAR, LessThan, distanceToWall, RC));
  RC->stop();
  delay(x);
  if(RC->readUv(LEFT_UV) >= 80)
  {
    //Room 2 candle A
    digitalWrite(13, HIGH);
    RC->followWall(Left, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Left, CCW), LessThan, 7.0, RC));
    RC->move(North, DEFAULT_SPEED,new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 7.0, RC)); 
    RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
    delay(300);
    RC->stop();
    RC->extinguish(true);
    RC->extinguish(false);
    return;
  }
  distanceToWall = RC->readDistanceSonar(WEST_CW_SONAR);
  RC->rotate(100, CW, new DistanceFor(SOUTH_CCW_SONAR, LessThan, distanceToWall + .25, RC));
  RC->stop();
  delay(x);
  if(RC->maxUv >= 80)
    {
      //Room 2 Candle C
      digitalWrite(13, HIGH);
      RC->move(North, DEFAULT_SPEED,new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 7.0, RC));
      RC->stop(); 
      if(RC->readUv(RIGHT_UV) >= 150)
      {
        RC->move(North, DEFAULT_SPEED,new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 7.0, RC)); 
        RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
        delay(300);
        RC->stop(); 
        RC->extinguish(true);
        RC->extinguish(false);
        return;
      }
      else {
        distanceToWall = RC->readDistanceSonar(EAST_CCW_SONAR);
        RC->rotate(100, CCW, new DistanceFor(SOUTH_CW_SONAR, LessThan, distanceToWall + .25, RC));
        RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 7.0, RC));
        RC->stop();
        RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
        delay(300);
        RC->stop(); 
        RC->extinguish(true);
        RC->extinguish(false);
        return;
      }
    }
  else {
    distanceToWall = RC->readDistanceSonar(SOUTH_CW_SONAR);
    RC->rotate(100, CW, new DistanceFor(EAST_CCW_SONAR, LessThan, distanceToWall + .25, RC));
    RC->stop();
  }

  
  //Finish rotation for room 2, slide into room 1.
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 9.0, RC)); // move inside hallway
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, 9.0, RC));
   RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 9.0, RC)); //inside hallway, moving inside room
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CCW), LessThan, 9.0, RC)); // should be inside room 1
  //should be barely inside room 1

  //Slide into room 1 some more
  distanceToWall = RC->readDistanceSonar(NORTH_CW_SONAR);
  RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, distanceToWall - 7.0, RC));
  RC->stop();
 
  //test for Room 1 Candle A
   if(RC->readUv(RIGHT_UV) >= 80)
  {
    digitalWrite(13, HIGH);
    RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 5.0, RC));
    RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
    delay(300);
    RC->stop();
    RC->extinguish(true);
    RC->extinguish(false);
    return;
  }
  //rotate for Room 1 Candle B
  distanceToWall = RC->readDistanceSonar(EAST_CW_SONAR);
  RC->rotate(100, CCW, new DistanceFor(SOUTH_CW_SONAR, LessThan, distanceToWall + .5, RC));
  RC->stop();
  //Test for Room 1 Candle B
  if(RC->readUv(LEFT_UV) >= 80)
  {
    digitalWrite(13, HIGH);
    RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC)); 
    RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
    delay(300);
    RC->stop();
    RC->extinguish(true);
    RC->extinguish(false);
    return;
  }
  //rotate so that we can move out of room 1
  distanceToWall = RC->readDistanceSonar(SOUTH_CW_SONAR);
  RC->rotate(100, CCW, new DistanceFor(NORTH_CW_SONAR, LessThan, distanceToWall, RC));
  RC->stop();

  //follow wall out of room 1


  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CCW), LessThan, 9.0, RC));
  RC->stop();
  delay(x);
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 9.0, RC));
  RC->stop();
  delay(x);
  RC->move(East, DEFAULT_SPEED, new MoveBy(RC->sonarIdAt(East, NoSide, CW), 5.0, RC));

  //move down the corridor, robot's nose is facing North
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), MoreThan, 15.0, RC));
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), MoreThan, 8.0, RC));
  RC->move(South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CCW), MoreThan, 12.0, RC));
  RC->go(South, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(200);
  RC->stop();

  //move into corridor for room 3
  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CW), LessThan, 8.0, RC));
  RC->move(East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CCW), LessThan, 8.0, RC));
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CCW), MoreThan, 12.0, RC));
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Right, CW), MoreThan, 12.0, RC));
  RC->followWall(Left, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, NoSide, CW), LessThan, 5.0, RC));
  //Robot is right in front of room 3, move into room 3, heading is North
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CCW), LessThan, 18.0, RC));
  RC->stop();
  delay(x);
  RC->followWall(Left, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Right, CW), LessThan, 18.0, RC));
  RC->go(South, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
  delay(200);
  RC->stop();
  //rotate robot 90 degrees CCW facing towards Room 3 Candle A


  //FIND CANDLE CODE Room 3
  RC->maxUv = RC->readUv(RIGHT_UV);
  distanceToWall = RC->readDistanceSonar(EAST_CW_SONAR);
  RC->rotate(100, CW, new DistanceFor(NORTH_CCW_SONAR, LessThan, distanceToWall, RC));
  RC->rotate(100, CW, new DistanceFor(WEST_CCW_SONAR, LessThan, distanceToWall, RC));
  RC->stop();
  if(RC->readUv(LEFT_UV) >= 80)
  {
    //Room 3 candle C
    digitalWrite(13, HIGH);
    RC->followWall(Left, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, Left, CCW), LessThan, 7.0, RC));
    RC->move(North, DEFAULT_SPEED,new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 7.0, RC)); 
    RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
    delay(300);
    RC->stop();
    RC->extinguish(true);
    RC->extinguish(false);
    return;
  }
  distanceToWall = RC->readDistanceSonar(WEST_CW_SONAR);
  RC->rotate(100, CW, new DistanceFor(SOUTH_CCW_SONAR, LessThan, distanceToWall + .25, RC));
  RC->stop();
  delay(x);
  if(RC->maxUv >= 80)
    {
      //Room 2 Candle C
      digitalWrite(13, HIGH);
      RC->move(North, DEFAULT_SPEED,new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 7.0, RC));
      RC->stop(); 
      if(RC->readUv(RIGHT_UV) >= 150)
      {
        RC->move(North, DEFAULT_SPEED,new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 7.0, RC)); 
        RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
        delay(300);
        RC->stop(); 
        RC->extinguish(true);
        RC->extinguish(false);
        return;
      }
      else {
        RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 5.0, RC));
        RC->stop();
        RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
        delay(300);
        RC->stop(); 
        RC->extinguish(true);
        RC->extinguish(false);
        return;
      }
    }
  else {
    distanceToWall = RC->readDistanceSonar(SOUTH_CW_SONAR);
    RC->rotate(100, CW, new DistanceFor(EAST_CCW_SONAR, LessThan, distanceToWall + .25, RC));
    RC->stop();
    delay(x);
  }

    //leaving room 3
    RC->followWall(Right, North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));
    RC->followWall(Right, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CCW), MoreThan, 8.0, RC));
    RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Right, CW), MoreThan, 12.0, RC));
   //moved into intersection
    RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Left, CW), LessThan, 8.0, RC));
    RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Left, CCW), LessThan, 8.0, RC));
    //moved out of intersection
    RC->followWall(Left, West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, Left, CW), MoreThan, 8.0, RC));
    RC->move(West, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(West, NoSide, CW), LessThan, 5.0, RC));
    RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, Left, CW), MoreThan, 12.0, RC));
    RC->followWall(Right, South, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(South, NoSide, CW), LessThan, 5.0, RC));
    RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CW), LessThan, 24.0, RC));
    RC->followWall(Right, East, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(East, Left, CCW), LessThan, 24.0, RC));
    RC->go(East, DEFAULT_SPEED, NoSide, 0, NoRotation, 0);
    delay(700);
    RC->stop();
    delay(x);
    //move into room 4
    RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));
    if(RC->readUv(LEFT_UV) >= 80)
    {
    //Room 4 Candle A
      digitalWrite(13, HIGH);
      RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
      delay(225);
      RC->stop();
      RC->extinguish(true);
      RC->extinguish(false);
      return;
    }
    distanceToWall = RC->readDistanceSonar(WEST_CW_SONAR);
    RC->rotate(100, CW, new DistanceFor(SOUTH_CCW_SONAR, LessThan, distanceToWall + .25, RC));
    RC->stop();
    if(RC->readUv(LEFT_UV) >= 80 || RC->readUv(RIGHT_UV) >= 80)
    {
      //room 4 candle B
      digitalWrite(13, HIGH);
      if(RC->readUv(LEFT_UV) > RC->readUv(RIGHT_UV))
      {
        RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CCW), LessThan, 5.0, RC));
        RC->go(North, 0, NoSide, 0, CCW, DEFAULT_SPEED);
        delay(300);
        RC->stop();
        RC->extinguish(true);
        RC->extinguish(false);
        return;
      }
      else
      {
        //room 4 candle C
        RC->move(North, DEFAULT_SPEED, new DistanceFor(RC->sonarIdAt(North, NoSide, CW), LessThan, 5.0, RC));
        RC->go(North, 0, NoSide, 0, CW, DEFAULT_SPEED);
        delay(350);
        RC->stop();
        RC->extinguish(true);
        RC->extinguish(false);
        return;
      }
    }
    RC->stop();
    
    delay(800);

}

void loop() {
  // put your main code here, to run repeatedly:

} 
