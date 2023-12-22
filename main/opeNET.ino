const char str_opeNET0[] PROGMEM = "NET Config          ";
const char str_opeNET1[] PROGMEM = "DHCP";
const char str_opeNET2[] PROGMEM = "STATIC";
const char str_opeNET3[] PROGMEM = " GW:";
const char str_opeNET4[] PROGMEM = "DNS:";
const char str_opeNET5[] PROGMEM = "EXIT opeNET()";
const char *const str_opeNET[] PROGMEM = {
  str_opeNET0,
  str_opeNET1,
  str_opeNET2,
  str_opeNET3,
  str_opeNET4,
  str_opeNET5
};

void opeNET(void) {
  int z,id,hr,mi,mx,io;
  char line1[21];
  extern struct KYBDMEM *ptr_crosskey,*getCrossKey(void);
  extern bool fsf;
  
  if (fsf) {
    fsf = false;
    cposp = 1;
    cposx = 0;
    cposy = 1;
    strcpy_P(line1,(char *)pgm_read_word(&(str_opeNET[0])));
    //    lcdd.setLine(cposp,0,"NET Config          ");
    lcdd.setLine(cposp,0,line1);
    if (st_m.dhcpflag) {
      strcpy_P(line1,(char *)pgm_read_word(&(str_opeNET[1])));
      lcdd.TextWrite(cposp,14,0,line1);
      //      lcdd.TextWrite(cposp,14,0,"DHCP");
    } else {
      strcpy_P(line1,(char *)pgm_read_word(&(str_opeNET[2])));
      lcdd.TextWrite(cposp,14,0,line1);
      //      lcdd.TextWrite(cposp,14,0,"STATIC");
    }
    lcdd.IPWriteWithCidr(cposp,cposx,cposy,st_m.ip,st_m.cidr);
    cposy++;
    cposx=0;
    strcpy_P(line1,(char *)pgm_read_word(&(str_opeNET[3])));
    lcdd.TextWrite(cposp,cposx,cposy,line1);
    //    lcdd.TextWrite(cposp,cposx,cposy," GW:");
    cposx=4;
    lcdd.IPWrite(cposp,cposx,cposy,st_m.gw);
    cposy++;
    cposx=0;
    strcpy_P(line1,(char *)pgm_read_word(&(str_opeNET[4])));
    lcdd.TextWrite(cposp,cposx,cposy,line1);
    //    lcdd.TextWrite(cposp,cposx,cposy,"DNS:");
    cposx=4;
    lcdd.IPWrite(cposp,cposx,cposy,st_m.dns);
    lcdd.PageWrite(cposp);
    lcdd.setCursor(cposx,cposy); // NO NEED break
    cposp = 0;
  }
  ptr_crosskey = getCrossKey();
  if ((ptr_crosskey->longf==true)&&(ptr_crosskey->kpos & K_LEFT)) {
    ptr_crosskey->longf= false;
    ptr_crosskey->kpos = 0;
    cmode = CMND;
    fsf = true;
    lcdd.clear();
    cposy = 0;
    cposp = 0;
    lcdd.setLine(cposp,cposy,pgname);
    lcdd.PageWrite(cposp);
    strcpy_P(line1,(char *)pgm_read_word(&(str_opeNET[5])));
    //    debugSerialOut(cmode,cmenu,F("EXIT opeNET()"));
    debugSerialOut(cmode,cmenu,line1);
  }
}
