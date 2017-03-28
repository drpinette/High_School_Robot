#include <Motor.h>

Motor::Motor() : curDirection(RELEASE), curSpeed(0), motor(NULL)
{
}

void Motor::run(int direction, int speed)
{
  Serial.print(direction); Serial.print(" "); Serial.println(speed); 
  curDirection = direction;
  curSpeed = speed;
  motor->run(direction);
  motor->setSpeed(speed);
  
}