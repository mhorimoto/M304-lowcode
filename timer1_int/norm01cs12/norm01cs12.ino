// NORM01
//
// Program to measure how many seconds it takes to OVF using TIMER1.
// Since it is 16 bits, the count to OVF is 65536.
// The number of milliseconds is measured when this is reached,
// and the number of counts required for one second is calculated.
//

#define  JD7  7

volatile int sema = 0;

void setup() {
  Serial.begin(115200);
  pinMode(JD7,OUTPUT);
  digitalWrite(JD7,sema);
  TCCR1A = 0; // Initialize
  TCCR1B = 0; // Initialize
  TCCR1B |= (1 << CS12); // CS12 -> 1 prescaler = 256
  TIMSK1 |= (1 << TOIE1); //TOIE -> 1 enable overflow
}

ISR(TIMER1_OVF_vect) {
  Serial.println(millis());
  if (sema==0) {
    sema = 1;
  } else {
    sema = 0;
  }
  digitalWrite(JD7,sema);
}

void loop() {
}
