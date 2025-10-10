#include <Servo.h>
#define PIN_SERVO 10
#define SERVO_CLOSE 0
#define SERVO_OPEN 90
#define SERVO_SMOOTH 0.08
Servo servo;
unsigned long servoTime=0;
bool servoTimeCheck=false,servoOpened=false;
float servoAngle=SERVO_CLOSE,servoTarget=SERVO_CLOSE;

#define PIN_TRIG 12
#define PIN_ECHO 13
#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 30
#define TIMEOUT ((INTERVAL/2)*1000.0)
#define SCALE (0.001*0.5*SND_VEL)
#define _EMA_ALPHA 0.5
#define DETECT_DISTANCE 150
unsigned long N=30, lastSamplingTime;
float dRaw, dMedian, dEma, samples[30];
int sampleIdx=0;

#define PIN_LED 8

void setup(){
  pinMode(PIN_TRIG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  digitalWrite(PIN_TRIG,LOW);
  pinMode(PIN_LED,OUTPUT);
  
  servo.attach(PIN_SERVO);
  servo.write(SERVO_CLOSE);
}

float getMedian(){
  float temp[N], t;
  for(unsigned long i=0;i<N;i++) temp[i]=samples[i];
  for(unsigned long i=0;i<N-1;i++) for(unsigned long j=i+1;j<N;j++) if(temp[i]>temp[j]){
    t=temp[i];
    temp[i]=temp[j];
    temp[j]=t;
  }
  return temp[N/2];
}

void updateServo(){
  servoAngle=SERVO_SMOOTH*servoTarget+(1-SERVO_SMOOTH)*servoAngle;
  servo.write((int)servoAngle);
}

void loop(){
  if(millis()<lastSamplingTime+INTERVAL) return;
  dRaw=USS_measure(PIN_TRIG,PIN_ECHO);
  
  samples[sampleIdx]=dRaw;
  sampleIdx=(sampleIdx+1)%N;
  dMedian=getMedian();
  dEma=_EMA_ALPHA*dMedian+(1-_EMA_ALPHA)*dEma;
  
  updateServo();

  if(dEma<DETECT_DISTANCE){
    digitalWrite(PIN_LED, LOW);
    if(!servoOpened){
      if(!servoTimeCheck){
        servoTime=millis();
        servoTimeCheck=true;
      }
      if(millis()>servoTime+1000 && servoTimeCheck){
        servoTarget=SERVO_OPEN;
        servoOpened=true;
        servoTimeCheck=false;
      }
    }
    else if(servoTimeCheck){
      servoTimeCheck=false;
    }
  }
  else{
    digitalWrite(PIN_LED, HIGH);
    if(!servoOpened && servoTimeCheck){
      servoTimeCheck=false;
    }
    if(servoOpened){
      if(!servoTimeCheck){
        servoTime=millis();
        servoTimeCheck=true;
      }
      if(millis()>servoTime+1000 && servoTimeCheck){
        servoTarget=SERVO_CLOSE;
        servoOpened=false;
        servoTimeCheck=false;
      }
    }
  }

  lastSamplingTime+=INTERVAL;
}


float USS_measure(int TRIG, int ECHO){
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, TIMEOUT)*SCALE;
}
