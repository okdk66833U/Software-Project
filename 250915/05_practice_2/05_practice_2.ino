#define PIN_LED 7
unsigned int toggle;
void setup() {
  pinMode(PIN_LED, OUTPUT);
  toggle=0;
}

void loop() {
  digitalWrite(PIN_LED, toggle);
  delay(1000);
  for(int i=0;i<11;i++){
    toggle=!toggle;
    digitalWrite(PIN_LED, toggle);
    delay(100);
  }
  while(1){}
}
