#include <M304.h>

char *pgname = "TEST EEPROM 2.20";
uint8_t addr[100],vl[100],lv[100],sr[100];

void setup(void) {
  int i,a;
  Serial.begin(115200);
  Serial.println(pgname);
  m304Init();
  for(i=0;i<100;i++) {
    a = LC_SCH_START + (i*LC_SCH_REC_SIZE);
    addr[i] = a;
    vl[i] = atmem.read(a+LC_VALID);
    if (vl[i]!=0xff) {
      lv[i] = atmem.read(a+LC_LV);
      sr[i] = atmem.read(a+LC_SR);
    }
  }
}

void loop(void) {
  int i,a;
  unsigned long t1,t2,t3;
  uint8_t mvl,mlv,msr;
  
  Serial.print("START ");
  t1 = millis();
  for(i=0;i<100;i++) {
    mvl = vl[i];
    if (mvl!=0xff) {
      mlv = lv[i];
      msr = sr[i];
    }
  }
  t2 = millis();
  t3 = t2-t1;
  Serial.print("FINISH TIME(msec)=");
  Serial.println(t3);
}
