#define LED 7

unsigned long duty=0UL;
unsigned long period=10000UL;

void setup(){
  pinMode(LED, OUTPUT);
}

void set_period(unsigned long p){
  period=p;
  bool direction=true;
  unsigned long i=0UL;
  
  while(1){
    duty=i;
    set_duty(duty);

    if(direction){
      i++;
      if(i>100UL){
        i=100UL;
        direction=false;
      }
    } 
    else{
      if(!i) break;
      i--;
    }
  }
}

void set_duty(unsigned long d){
  duty=d;
  unsigned long hTime=(period*duty)/100UL;
  unsigned long lTime=period-hTime;
  
  digitalWrite(LED, 1);
  if(hTime>0){
    delayMicroseconds(hTime);
  }
  digitalWrite(LED, 0);
  if(lTime>0){
    delayMicroseconds(lTime);
  }
}

void loop(){
  set_period(10000UL);
  //set_period(1000UL);
  //set_period(100UL);
}
