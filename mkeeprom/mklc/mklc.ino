#include <M304.h>
#if _M304_H_V < 1311
#pragma message("Library M304 is old. Version 1.3.11 or higher is required.")
#else

char *pgname = "M304 makelc Ver1.04 ";
char inputbuf[BUFSIZ],*ptr_inputbuf;
int  cnt;

LCDd lcdd(RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7);

void setup(void) {
  String linebuf[4],inputbuf;

  Serial.begin(115200);
  lcdd.begin(20,4);
  lcdd.clear();
  linebuf[1] = String(pgname);
  lcdd.print(linebuf[1]);
  lcdd.setCursor(0,1);
  lcdd.print("Open SERIAL");
  lcdd.setCursor(0,2);
  lcdd.print("115200bps");
  Serial.println(pgname);
  ptr_inputbuf = &inputbuf[0];
  cnt = 0;
}


void loop(void) {
  char ch;
  static bool cr = true;

  if ((cnt==0) && cr) {
    Serial.print(F("$ "));
    cr = false;
  }
  if (Serial.available() > 0) {
    ch = Serial.read();
    if ((ch=='\r')||(ch=='\n')) {
      cr = true;
      Serial.println();
    } else {
      Serial.print(ch);
      inputbuf[cnt] = ch;
      cnt++;
      inputbuf[cnt] = (char)NULL;
      if (cnt>=BUFSIZ) {
        Serial.println("OV");
        cnt=0;
      }
    }
  }
  if (cr) {
    ptr_inputbuf = &inputbuf[0];
    if ( !strncmp(ptr_inputbuf,"list",BUFSIZ-1) ) {
      Serial.println("LIST COMMAND");
      listCommand();
    }
    Serial.println("OK");
    cnt = 0;
  }
}



void listCommand(void) {
  int i;
  char lbf[80];
  char sbf[21];
  
  // UECS ID
  sprintf(lbf,"UECS ID: %02X %02X %02X %02X %02X %02X",
          atmem.read(LC_UECS_ID),atmem.read(LC_UECS_ID+1),atmem.read(LC_UECS_ID+2),
          atmem.read(LC_UECS_ID+3),atmem.read(LC_UECS_ID+4),atmem.read(LC_UECS_ID+5));
  Serial.println(lbf);
  // MAC Address
  sprintf(lbf,"MAC ADDR: %02X:%02X:%02X:%02X:%02X:%02X",
          atmem.read(LC_MAC),atmem.read(LC_MAC+1),atmem.read(LC_MAC+2),
          atmem.read(LC_MAC+3),atmem.read(LC_MAC+4),atmem.read(LC_MAC+5));
  Serial.println(lbf);
  // FIXED or DHCP
  Serial.print(F("Address allocation method is "));
  if (atmem.read(FIX_DHCP_FLAG)==0) {
    Serial.println(F("STATIC."));
    Serial.println(F("The data below is important."));
  } else {
    Serial.println(F("DHCP."));
    Serial.println(F("The data below are for reference only."));
  }
  // IP Address
  sprintf(lbf,"IP Address/Netmask: %d.%d.%d.%d/%d.%d.%d.%d",
          atmem.read(FIXED_IPADDRESS),atmem.read(FIXED_IPADDRESS+1),
          atmem.read(FIXED_IPADDRESS+2),atmem.read(FIXED_IPADDRESS+3),
          atmem.read(FIXED_NETMASK),atmem.read(FIXED_NETMASK+1),
          atmem.read(FIXED_NETMASK+2),atmem.read(FIXED_NETMASK+3));
  Serial.println(lbf);
  // Default gateway
  sprintf(lbf,"Default Gateway: %d.%d.%d.%d",
          atmem.read(FIXED_DEFGW),atmem.read(FIXED_DEFGW+1),
          atmem.read(FIXED_DEFGW+2),atmem.read(FIXED_DEFGW+3));
  Serial.println(lbf);
  // DNS
  sprintf(lbf,"DNS: %d.%d.%d.%d",
          atmem.read(FIXED_DNS),atmem.read(FIXED_DNS+1),
          atmem.read(FIXED_DNS+2),atmem.read(FIXED_DNS+3));
  Serial.println(lbf);
  // Vender name
  for (i=0;i<16;i++) {
    sbf[i] = atmem.read(VENDER_NAME+i);
    if (sbf[i]==(char)NULL) break;
  }
  sprintf(lbf,"Vender Name: %s",sbf);
  Serial.println(lbf);
  // Node name
  for (i=0;i<16;i++) {
    sbf[i] = atmem.read(NODE_NAME+i);
    if (sbf[i]==(char)NULL) break;
  }
  sprintf(lbf,"Node Name: %s",sbf);
  Serial.println(lbf);
}

#endif
