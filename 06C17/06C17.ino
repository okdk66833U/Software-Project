#define LED 7
unsigned long period, duty, second=1000000UL, highTime, lowTime, nowTime, startTime;

void setup() {
  pinMode(LED, OUTPUT);
}

void set_period(unsigned long p){
  period=constrain(p, 100UL, 10000UL);
}

void set_duty(unsigned long duty){
  highTime=(period*duty)/100UL, lowTime=period-highTime;
  digitalWrite(LED, 1);
  if (highTime>0) delayMicroseconds(highTime);
  digitalWrite(LED, 0);
  if (lowTime>0) delayMicroseconds(lowTime);
}

void loop() {
  set_period(10000UL);
  //set_period(1000UL);
  //set_period(100UL);
  
  startTime=micros();
  while((micros()-startTime)<second){
    nowTime=micros()-startTime, duty=((nowTime<(second/2UL))?nowTime:(second-nowTime))*100UL/(second/2UL);
    set_duty(duty);
  }
}
