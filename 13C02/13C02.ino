#include <Servo.h>

#define PIN_SERVO 10

#define SERVO_SPEED 0.3
#define ANGLE_TARGET 90.0
#define DUTY_TARGET DUTY_NEU
#define DUTY_STEP ((float)(DUTY_TARGET-DUTY_MIN)/ANGLE_TARGET)

//#define SERVO_SPEED 3.0
//#define ANGLE_TARGET 180.0
//#define DUTY_TARGET DUTY_MAX

#define MOVE_DURATION_MS (unsigned long)((ANGLE_TARGET/SERVO_SPEED)*1000.0)
#define DUTY_MIN 553
#define DUTY_NEU 1471
#define DUTY_MAX 2399

Servo myservo;
unsigned long startMs;
bool moveFinished=false;

int angleToDuty(float angle){
  if(angle<0.0)angle=0.0;
  if(angle>ANGLE_TARGET)angle=ANGLE_TARGET;
  return(int)(DUTY_MIN+DUTY_STEP*angle);
}

void setup(){
  myservo.attach(PIN_SERVO);
  myservo.writeMicroseconds(DUTY_MIN);
  startMs=millis();
}

void loop(){
  if(moveFinished)return;
  unsigned long elapsed=millis()-startMs;
  if(elapsed>=MOVE_DURATION_MS){
    myservo.writeMicroseconds(DUTY_TARGET);
    moveFinished=true;
    return;
  }
  float angle=SERVO_SPEED*(elapsed/1000.0f);
  myservo.writeMicroseconds(angleToDuty(angle));
}
