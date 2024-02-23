// NORM01
//
// Program to measure how many seconds it takes to OVF using TIMER1.
// Since it is 16 bits, the count to OVF is 65536.
// The number of milliseconds is measured when this is reached,
// and the number of counts required for one second is calculated.
//
void setup() {
  Serial.begin(115200);
  TCCR1A = 0; // Initialize
  TCCR1B = 0; // Initialize
  TCCR1B |= (1 << CS10); // CS10 -> 1 prescaler = 1
  TIMSK1 |= (1 << TOIE1); //TOIE -> 1 enable overflow
}

ISR(TIMER1_OVF_vect) {
  Serial.println(millis());
}

void loop() {
}
