// -*- C++ -*-
//  Arduino と外付け AT24C256 の EEPROMを扱うユーティリティプログラム
//

#include <M304.h>


char cha,cmnd[81],lbf[81];
int  cp ;
char *pgname = "M304jp EEPROM Ver2.00";

void setup(void) {
  m304Init();
  Serial.begin(115200);
  Serial.println(pgname);
  sprintf(lbf,"Arduino EEPROM SIZE 0-%03XH(%d bytes)",EEPROM.length()-1,EEPROM.length());
  Serial.println(lbf);
  Serial.println("AT24LC256 EEPROM SIZE 0-7FFFH(32,768bytes)");
  cha = (char)NULL;
  cp  = 0;
}

void loop(void) {
  bool reading;
  String s;
  reading = true;
  cp = 0;
  Serial.print("% ");
  Serial.flush();
  while(reading) {
    if (Serial.available() > 0) {
      cha = Serial.read();
      if ((cha>=0x20)&&(cha<=0x7e)) {
	cmnd[cp] = (char)cha;
	Serial.print(cha);
	cp++;
      } else if (cha==0x08) {
	if (cp>0) {
	  cmnd[cp] = (char)NULL;
	  Serial.write(0x08);
	  Serial.print(" ");
	  Serial.write(0x08);
	  cp--;
	}
      } else if ((cha==0x0a)||(cha==0x0d)) {
	reading = false;
	Serial.println();
	cmnd[cp] = (char)NULL;
	s = String(cmnd);
      }
    }
  }
  if (s.indexOf("dump")==0) {
    cmnd_dump(s,-1);
  } else if (s.indexOf("atdump")==0) {
    cmnd_dump(s,0);
  } else if (s.indexOf("setb")==0) {
    cmnd_setbyte(s,-1);
  } else if (s.indexOf("atsetb")==0) {
    cmnd_setbyte(s,0);
  } else if (s.indexOf("fill")==0) {
    cmnd_fill(s,-1);
  } else if (s.indexOf("atfill")==0) {
    cmnd_fill(s,0);
  }
}
