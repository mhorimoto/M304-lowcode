#include <M304.h>

void clearpage(char *p) {
  int a,c;
  a = strtol(p,0,16);
  if (a>0x7fff) {
    Serial.println(F("OUT of RANGE"));
    return;
  }
  a &= 0xff00;
  for(c=a;c<(a+0x100);c++) {
    atmem.write(c,0xff);
    if ((c & 0xf)==0) {
      Serial.print(F("=== 0x"));
      Serial.print(c,HEX);
      Serial.println(F(" Done ==="));
    }
  }
}
