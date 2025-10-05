#define PIN_TRIG 12
#define PIN_ECHO 13
#define SND_VEL 346.0
#define INTERVAL 25
#define PULSE_DURATION 10
#define _DIST_MIN 100
#define _DIST_MAX 300
#define TIMEOUT ((INTERVAL/2)*1000.0)
#define SCALE (0.001*0.5*SND_VEL)
#define _EMA_ALPHA 0.5

unsigned long N=30, lastSamplingTime;
float dRaw, dMedian, dEma, samples[30];
int sampleIdx=0;

void setup(){
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);
  Serial.begin(57600);
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

void loop(){
  if(millis()<lastSamplingTime+INTERVAL) return;
  dRaw=USS_measure(PIN_TRIG, PIN_ECHO);
  
  if((dRaw==0.0) || (dRaw<_DIST_MIN)) samples[sampleIdx]=_DIST_MIN;
  else if(dRaw>_DIST_MAX) samples[sampleIdx]=_DIST_MAX;
  else samples[sampleIdx]=dRaw;
  // if(dRaw<_DIST_MIN) samples[sampleIdx]=_DIST_MIN;
  // else if(dRaw>_DIST_MAX) samples[sampleIdx]=_DIST_MAX;
  // else samples[sampleIdx]=dRaw;
  
  sampleIdx=(sampleIdx+1)%N;
  
  dMedian=getMedian();
  dEma=_EMA_ALPHA*dMedian+(1-_EMA_ALPHA)*dEma;

  Serial.print("Min:"); Serial.print(_DIST_MIN);
  Serial.print(",raw:"); Serial.print(min(dRaw, _DIST_MAX+100));
  Serial.print(",ema:"); Serial.print(min(dEma, _DIST_MAX+100));
  Serial.print(",median:"); Serial.print(min(dMedian, _DIST_MAX+100));
  Serial.print(",Max:"); Serial.print(_DIST_MAX);
  Serial.println("");
  lastSamplingTime+=INTERVAL;
}


float USS_measure(int TRIG, int ECHO){
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, TIMEOUT)*SCALE;
}
