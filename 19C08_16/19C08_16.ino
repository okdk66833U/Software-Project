#define PIN_IR A0

int freeRam(){
  extern int __heap_start,  *__brkval;
  int v;
  return (int)&v-(__brkval==0?(int)&__heap_start:(int)__brkval);
}

int compare(const void*a, const void*b){
  return (*(unsigned int*)a-*(unsigned int*)b);
}

void print_res(unsigned int val, unsigned long time, int mem){
  Serial.print("값:"); Serial.print(val);
  Serial.print("걸린 시간(us):"); Serial.print(time);
  Serial.print("Mem(B):"); Serial.println(mem);
}

void runTest(const char* name, unsigned int n, float pos, int method){
  Serial.print(name);
  int m1=freeRam();
  unsigned long t1=micros();
  unsigned int res;
  if(method==0){
    unsigned int *arr=(unsigned int*)malloc(sizeof(unsigned int)*n);
    for(int i=0;i<n;i++) arr[i]=analogRead(PIN_IR);
    qsort(arr, n, sizeof(unsigned int), compare);
    res=arr[(int)(n*pos)];
    free(arr);
  }
  else if(method==1){
    unsigned long sum=0;
    unsigned int vMin=1024, vMax=0, val;
    for(int i=0;i<n;i++){
      val=analogRead(PIN_IR);
      sum+=val;
      if(val<vMin) vMin=val;
      if(val>vMax) vMax=val;
    }
    res=(unsigned int)((sum-vMin-vMax)/(n-2));
  }
  else{
    unsigned int *arr=(unsigned int*)malloc(sizeof(unsigned int)*n);
    for(int i=0;i<n;i++) arr[i]=analogRead(PIN_IR);
    int k=(int)(n*pos);
    int l=0, r=n-1;
    while(l<r){
      unsigned int pivot=arr[l+(r-l)/2];
      arr[l+(r-l)/2]=arr[r];
      arr[r]=pivot;
      int i=l;
      for(int j=l;j<r;j++) if(arr[j]<=pivot){
          unsigned int t=arr[i];arr[i]=arr[j];arr[j]=t;
          i++;
      }
      unsigned int t=arr[i];arr[i]=arr[r];arr[r]=t;
      if(i==k){
        res=arr[k];
        break;
      }
      if(i<k) l=i+1;
      else r=i-1;
    }
    if(l>=r) res=arr[k];
    free(arr);
  }
  unsigned long dt=micros()-t1;
  int usedMem=m1-freeRam();
  print_res(res, dt, usedMem);
}

void setup(){
  Serial.begin(1000000);
}

void loop(){
  if(Serial.available()){
    while(Serial.available()) Serial.read();
    Serial.println("\n---------------------------------");
    runTest("[오리지널] ", 20, 0.5, 0);
    runTest("[절삭평균] ", 20, 0.5, 1);
    runTest("[빠른선택] ", 20, 0.5, 2);
    Serial.println("---------------------------------");
  }
}
