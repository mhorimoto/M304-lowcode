#include <M304.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <yxml.h>
#include <string.h>

#if _M304_H_V < 138
#pragma message("Library M304 is old. Version 1.3.8 or higher is required.")
#else
uint8_t mcusr_mirror __attribute__ ((section (".noinit")));
void get_mcusr(void)     \
  __attribute__((naked)) \
  __attribute__((section(".init3")));
void get_mcusr(void) {
  mcusr_mirror = MCUSR;
  MCUSR = 0;
  wdt_disable();
}

char *pgname = "M304 Ver2.3.4D";

typedef struct irrM304 {
  byte id,sthr,stmn,edhr,edmn,inmn,dumn,rly[8];
};

irrM304 irr_m;

#define ELE_UECS      0b00000001
#define ELE_NODESCAN  0b00000010
#define ELE_CCMSCAN   0b00000100
#define ELE_DATA      0b00001000
#define ELE_REQUEST   0b00010000
#define ELE_SEARCH    0b00100000
#define ELE_IP        0b01000000

#define ATTR_VER      1
#define ATTR_PAGE     2
#define ATTR_TYPE     3
#define ATTR_ROOM     4
#define ATTR_REGION   5
#define ATTR_ORDER    6
#define ATTR_PRIORITY 7

#define LEN_UECSXML_VER     20
#define LEN_UECSXML_TYPE    20
#define LEN_UECSXML_TEXTVAL 20
#define LEN_UECSXML_BUFFER  512

#define CCM_TBL_CNT_RX  30
#define CCM_TBL_CNT_TX  30

char uecsbuf[LEN_UECSXML_BUFFER+1];
const char xmlhead[] PROGMEM = "<?xml version=\"1.0\"?><UECS ver=\"1.00-E10\">$";
const char res_xmlnode1[] PROGMEM = "<NODE><NAME>$";
const char res_xmlnode2[] PROGMEM = "</NAME><VENDER>$";
const char res_xmlnode3[] PROGMEM = "</VENDER><UECSID>$";
const char res_xmlnode4[] PROGMEM = "</UECSID><IP>$";
const char res_xmlnode5[] PROGMEM = "</IP><MAC>$";
const char res_xmlnode6[] PROGMEM = "</MAC></NODE></UECS>$";


typedef struct st_UECSXML {
  byte element;
  char ver[LEN_UECSXML_VER+1];
  char type[LEN_UECSXML_TYPE+1];
  byte page,room,region,priority;
  int  order;
  char textval[LEN_UECSXML_TEXTVAL+1];
  float fval;
  IPAddress ip;
};

st_UECSXML uecsxmldata,*ptr_uecsxmldata;

LCDd lcdd(RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7);
EthernetUDP UDP16520;
EthernetUDP UECS_UDP16529;
EthernetUDP UECS_UDP16521;
EthernetServer httpd(80);
//EthernetClient UECSclient;
IPAddress broadcastIP;

int cposx,cposy,cposp;
int cmode=RUN;
int cmenu=NETCONFIG;
int rlyttl[8];

bool cf,fsf=true;
byte ip[4] = { 192,168,0,177 };
char lbf[81];
extern bool debugMsgFlag(int);

struct st_FAST_LOOKUP_BUFFER_FOR_CCMTBL {
  uint8_t valid,lv;
} flb_rx_ccm[CCM_TBL_CNT_RX],flb_tx_ccm[CCM_TBL_CNT_TX];

void setup(void) {
  extern int mask2cidr(IPAddress);
  int a,w,j;
  char ccm_type[21];
  IPAddress hostip,subnet,gateway,dns;
  m304Init();
  clear_uecsbuf();
  ptr_uecsxmldata = &uecsxmldata;
  lcdd.begin(20,4);
  if (is_dhcp()) {
    if (Ethernet.begin(st_m.mac)==0) {
      lcdd.setLine(0,2,"NO NET MODE");
      lcdd.LineWrite(0,2);
      st_m.dhcpflag = true;
    }
  } else {
    Serial.begin(115200);
    st_m.ip     = getIPAddressFromEEPROM(FIXED_IPADDRESS);
    st_m.subnet = getIPAddressFromEEPROM(FIXED_NETMASK);
    st_m.gw     = getIPAddressFromEEPROM(FIXED_DEFGW);
    st_m.dns    = getIPAddressFromEEPROM(FIXED_DNS);
    st_m.cidr   = mask2cidr(st_m.subnet);
    st_m.dhcpflag = false;
    lcdd.setLine(0,2,"STATIC IP ADDRESS");
    lcdd.LineWrite(0,2);
    Ethernet.begin(st_m.mac,st_m.ip,st_m.dns,st_m.gw,st_m.subnet);
    st_m.dhcpflag = false;
  }
  configure_wdt();
  msgRun1st();
  wdt_reset();
  if (debugMsgFlag(SO_MSG)) {
    Serial.begin(115200);
    Serial.println(st_m.gw);
    Serial.println(st_m.ip);
    Serial.println(st_m.dns);
    Serial.println(st_m.subnet);
    Serial.println(st_m.cidr);
    Serial.println(broadcastIP);
  }
  for(w=0;w<8;w++) {
    rlyttl[w] = 0;
  }
  for (j=0;j<20;j++) {
    ccm_type[j] = 0;
  }

  //  Initialize the CCM table when the initialization switch is in the up position.
  //  Very very dangerous
  j = digitalRead(SW_SAFE);
  if (j==LOW) {
    if ((atmem.read(LC_SEND_START)==0xff)&&(atmem.read(LC_SCH_START)==0xff)) {  // CCMTABLE
        initEEPROM_UECS();
	lcdd.setLine(0,3,"init EEPROM         ");
	lcdd.LineWrite(0,3);
    }
  }

  // Read fast lookup buffer for CCM table
  for (j=0;j<CCM_TBL_CNT_RX;j++) {
    a = LC_SCH_START+(j*LC_SCH_REC_SIZE);
    flb_rx_ccm[j].valid = atmem.read(a+LC_VALID);    
    flb_rx_ccm[j].lv    = atmem.read(a+LC_LV);    
  }
  for (j=0;j<CCM_TBL_CNT_TX;j++) {
    a = LC_SEND_START+(j*LC_SCH_REC_SIZE);
    flb_tx_ccm[j].valid = atmem.read(a+LC_VALID);    
    flb_tx_ccm[j].lv    = atmem.read(a+LC_LV);    
  }
  
  UDP16520.begin(16520);
  UECS_UDP16529.begin(16529);
  httpd.begin();
  sendUECSpacket(0,"2048"); // setup completed 0x800
  Serial.begin(115200);
  Serial.println(pgname);
}


void loop(void) {
  int x,y,z,id,hr,mi,mx,io,minsec,j;
  char ca,line1[21];
  static char pca;
  static int prvsec;
  extern struct KYBDMEM *ptr_crosskey,*getCrossKey(void);
  extern void opeSCH(void),opeRTC(void),opeNET(void),opeRUN(int,int),opeHttpd(EthernetClient);
  extern void UECSupdate16529port(void) ;
  uint8_t InputDataButtom(int,int,int,int,uint8_t,int mi='0',int mx='9');
  tmElements_t tm;

  EthernetClient httpClient = httpd.available();
  wdt_reset();
  if ( httpClient ) {
    opeHttpd(httpClient);
  }
  UECSupdate16520port() ;
  UECSupdate16529port() ;
  if (digitalRead(SW_SAFE)==0) {
    lcdd.setLine(0,1,"  EEPROM Operation  ");
    lcdd.LineWrite(0,1);
    opeEEPROM();
  }
  switch(cmode) {
  case RUN:
    if (fsf) {
      msgRun1st();
      fsf = false;
    }
    if (RTC.read(tm)==0) {
      lcdd.setLine(cposp,1,"NO RTC PLS SETUP    ");
      lcdd.LineWrite(cposp,1);
    } else {
      if (prvsec!=tm.Second) {
	prvsec = tm.Second;
	snprintf(line1,21,"%d/%02d/%02d  %02d:%02d:%02d",	
	 tm.Year+1970,tm.Month,tm.Day,tm.Hour,tm.Minute,tm.Second);
	lcdd.setLine(cposp,1,line1);
	lcdd.LineWrite(cposp,1);
	opeRUN(tm.Hour,tm.Minute);
	minsec = 0;
	for (x=0;x<8;x++) {
	  if (rlyttl[x]>0) {
	    if (minsec==0) minsec = rlyttl[x];
	    if (minsec>rlyttl[x]) {
	      minsec = rlyttl[x];
	    }
	    digitalWrite(RLY1+x,LOW);
	    rlyttl[x]--;
	  } else {
	    digitalWrite(RLY1+x,HIGH);
	  }
	}
	if (minsec>0) {
	  snprintf(line1,21,"REMAINING=%3d",minsec);
	} else {
	  snprintf(line1,21,"ALL BREAK          ");
	}
	lcdd.setLine(cposp,3,line1);
	lcdd.LineWrite(cposp,3);
        //
        sendUECSpacket(0,"0");
        //
      }
    }
    ptr_crosskey = getCrossKey();
    if (ptr_crosskey->longf==true) {
      ptr_crosskey->longf=false;
      ptr_crosskey->kpos=0;
      cmode=CMND;
      fsf = true;
    }
    wdt_reset();
    break;
    //################################################################
  case CMND:
    cf = false;
    if (fsf) {
      debugSerialOut(cmode,cmenu,"Begin point CMND with fsf");
      msgCmnd1st();
    }
    wdt_reset();
    ptr_crosskey = getCrossKey();
    if ((ptr_crosskey->longf==true)&&(ptr_crosskey->kpos & K_LEFT)) {
      debugSerialOut(cmode,cmenu,"Begin point CMND with K_LEFT");
      cmode = RUN;
      fsf = true;
      ptr_crosskey->longf=false;
      ptr_crosskey->kpos=0;
    }
    if (ptr_crosskey->kpos & K_UP) {
      ptr_crosskey->kpos &= ~K_UP;
      cmenu++;
      if (cmenu>EEPROMOPE) cmenu=NETCONFIG;
      cf = true;
      debugSerialOut(cmode,cmenu,"K_UP");      
    }
    if (ptr_crosskey->kpos & K_DOWN) {
      ptr_crosskey->kpos &= ~K_DOWN;
      cmenu--;
      if (cmenu<NETCONFIG) cmenu=EEPROMOPE;
      cf = true;
      debugSerialOut(cmode,cmenu,"K_DOWN");      
    }
    if (ptr_crosskey->kpos & K_ENT) {
      ptr_crosskey->kpos &= ~K_ENT;
      fsf   = true;
      switch(cmenu) {
      case NETCONFIG:
        cmode = NETCMND;
        //opeNET();
	break;
      case RTCCONFIG:
        cmode = RTCCMND;
        //opeRTC();
	break;
      case SCHCONFIG:
        cmode = SCHCMND;
        //opeSCH();
	break;
      case EEPROMOPE:
        cmode = EEPROMCMND;
        break;
      default:
	lcdd.clear();
	cmode = CMND;
	break;
      }
    }
    switch(cmenu) {
    case NETCONFIG: // NET CONFIG
      lcdd.setLine(0,1,"  Network config    ");
      break;
    case RTCCONFIG: // RTC CONFIG
      lcdd.setLine(0,1,"  RTC config        ");
      break;
    case SCHCONFIG: // Schedule CONFIG
      lcdd.setLine(0,1,"  SCHEDULE config   ");
      break;
    case EEPROMOPE: // EEPROM Operation
      lcdd.setLine(0,1,"  EEPROM Operation  ");
      break;
    }
    if (cf) {
      lcdd.LineWrite(0,1);
      cf = false;
    }
    break;
    //################################################################
  case NETCMND:
    wdt_reset();
    opeNET();
    break;
  case RTCCMND:
    wdt_reset();
    opeRTC();
    break;
  case SCHCMND:
    wdt_reset();
    opeSCH();
    break;
  case EEPROMCMND:
    wdt_reset();
    opeEEPROM();
    break;
  }
}


void PushEnter(int p) {
  int x,y;
  extern struct KYBDMEM *ptr_crosskey;
  ptr_crosskey->kpos &= ~K_ENT;  // Reset Flag
  if (!debugMsgFlag(SO_MSG)) {
    Serial.begin(115200);
  }
  Serial.println("+--------------------+");
  for (y=0;y<4;y++) {
    for (x=0;x<20;x++) {
      Serial.print(lcdd.CharRead(p,x,y));
    }
    Serial.println();
  }
  Serial.println("+--------------------+");
  if (!debugMsgFlag(SO_MSG)) {
    Serial.end();
  }
}


int InputArithA(int p,int x,int y,int w,int min,int max,bool zp) {
  int v,va,i;
  static int pv=-1000;
  va = analogRead(SELECT_VR);
  v = map(va,0,1023,min,max);
  if (pv!=v) {
    lcdd.IntWrite(p,x,y,w,zp,v);
    pv = v;
  }
  return(v);
}

uint8_t InputDataButtom(int p,int x,int y,int k,uint8_t ud,int mi='0',int mx='9') {
  char c;
  extern struct KYBDMEM *ptr_crosskey;
  c = lcdd.CharRead(p,x,y);
  if (ud==K_UP) {
    ptr_crosskey->kpos &= ~K_UP;  // Reset Flag
    c++;

    switch(k) {
    case K_DIGIT:
      if (c<mi) {
        c = mi;
      } else if (c>mx) {
        c = mi;
      }
    }
    lcdd.CharWrite(p,x,y,c);
  } else if (ud==K_DOWN) {
    ptr_crosskey->kpos &= ~K_DOWN;  // Reset Flag
    c--;
    switch(k) {
    case K_DIGIT:
      if (c<mi) {
        c = mx;
      } else if (c>mx) {
        c = mx;
      }
    }
    lcdd.CharWrite(p,x,y,c);
  }
  return(-1);
}

void msgRun1st(void) {
  extern int mask2cidr(IPAddress);
  int cidr,i;
  
  lcdd.initWriteArea(0);
  lcdd.initWriteArea(1);
  lcdd.cursor();
  lcdd.blink();
  lcdd.clear();
  lcdd.setLine(0,0,pgname);
  cposp = 0;
  cposx = 0;
  cposy = 2;
  lcdd.PageWrite(cposp);
  lcdd.setCursor(cposx,cposy);
  if (Ethernet.maintain()!=0) {
    lcdd.setLine(0,2,"NIC IS NO W5500");
    lcdd.LineWrite(0,2);
  } else {
    lcdd.TextWrite(0,0,2,"IP:");
    st_m.gw = Ethernet.gatewayIP();
    st_m.ip = Ethernet.localIP();
    st_m.dns = Ethernet.dnsServerIP();
    st_m.subnet = Ethernet.subnetMask();
    st_m.cidr   = mask2cidr(st_m.subnet);
    for(i=0;i<4;i++) {
      broadcastIP[i] = ~st_m.subnet[i]|st_m.ip[i];
    }
    cposx = 3;
    lcdd.IPWrite(0,cposx,2,st_m.ip);
  }
}

void msgCmnd1st(void) {
  extern struct KYBDMEM *ptr_crosskey,*getCrossKey(void);
  lcdd.noBlink();
  lcdd.setLine(0,0,"Choose Menu         ");
  lcdd.setLine(0,2,"UP/DOWN/ENT Key use ");
  lcdd.setLine(0,3,"Exit:LEFT Key push  ");
  cposp = 0;
  cposx = 0;
  cposy = 1;
  lcdd.PageWrite(cposp);
  lcdd.setCursor(cposx,cposy); // NO NEED break
  fsf = false;
  cf = true;
  ptr_crosskey->longf=false;
  ptr_crosskey->kpos=0;
}

void debugSerialOut(int a,int b,char *c) {
  char t[80];
  extern bool fsf;
  if (debugMsgFlag(SO_MSG)) {
    sprintf(t,"cmode=%d  cmenu=%d  fsf=%d  key=%s",a,b,fsf,c);
    Serial.println(t);
  }
}

char *itoaddr(IPAddress a) {
  char ia[16],*iap;
  iap = &ia[0];
  sprintf(iap,"%d.%d.%d.%d",a[0],a[1],a[2],a[3]);
  return(iap);
}

void initEEPROM_UECS(void) {
  int w,a,j,k;
  w = 9;      // cnd + RLY1..8
  bool enable;
  byte room,region,priority;
  int  order;
  char ccm_type[20];

  enable = true;
  room   = 1;
  region = 1;
  order  = 1;
  priority = 15;
  for (k=0;k<w;k++) {
    for (j=0;j<20;j++) {
      ccm_type[j] = 0;
    }
    if (k==0) {
      strcpy(ccm_type,"cnd.aMC");
    } else if (k==6) {
      sprintf(ccm_type,"AirHumFogopr.%d",k);
    } else {
      sprintf(ccm_type,"Irriopr.%d",k);
    }
    if (k==0) {              // CCMTABLE
      a = LC_SCH_START;
    } else {
      a += LC_SCH_REC_SIZE;
    }
    atmem.write(a+LC_VALID,enable);
    atmem.write(a+LC_ROOM,room);
    atmem.write(a+LC_REGION,region);
    atmem.write(a+LC_ORDER,(order&0xff));
    atmem.write(a+LC_ORDER+1,(order>>8)&0xff);
    atmem.write(a+LC_PRIORITY,priority);
    atmem.write(a+LC_LV,LV_A1M0);
    atmem.write(a+LC_CAST,0);
    atmem.write(a+LC_SR,'R');
    for (j=0;j<20;j++) {
      atmem.write(a+LC_CCMTYPE+j,ccm_type[j]);
    }
    for (j=0;j<10;j++) {
      atmem.write(a+LC_UNIT+j,0);
    }
    atmem.write(a+LC_STHR,0);
    atmem.write(a+LC_STMN,0);
    atmem.write(a+LC_EDHR,0);
    atmem.write(a+LC_EDMN,0);
    atmem.write(a+LC_INMN,0);
    atmem.write(a+LC_DUMN,0);
    atmem.write(a+LC_RLY_L,0);
    atmem.write(a+LC_RLY_H,0);
  }
}

void sendUECSpacket(int id,char *v) {
  char t[256];
  char *xmlDT;
  bool enable;
  byte room,region,priority;
  int  order,x,a,j;
  char ccm_type[20];
  byte ordh,ordl;
  for(x=0;x<256;x++) {
    t[x] = (char)NULL;
  }
  a = LC_SCH_START+(id*0x40);    // CCMTABLE
  xmlDT = CCMFMT;
  enable = atmem.read(a+LC_VALID);
  if (enable!=1) {
    return;
  }
  room = atmem.read(a+LC_ROOM);
  region = atmem.read(a+LC_REGION);
  ordl = atmem.read(a+LC_ORDER);
  ordh = atmem.read(a+LC_ORDER+1);
  order = (ordh<<8)+ordl;
  priority = atmem.read(a+LC_PRIORITY);
  for (j=0;j<20;j++) {
    ccm_type[j] = atmem.read(a+LC_CCMTYPE+j);
  }
  sprintf(t,xmlDT,ccm_type,room,region,
          order,priority,v,itoaddr(st_m.ip));
  
  if (debugMsgFlag(SO_MSG)) {
    Serial.println(t);
  }
  UDP16520.beginPacket(broadcastIP, 16520);
  UDP16520.write(t);
  UDP16520.endPacket();
}

int copyFromNAMEVENDER(char *dest,char *src) {
  int a,r,l;
  char b[17],bb;
  r = 0;
  for(a=src;a<(src+16);a++) {
    bb = atmem.read(a);
    if (isPrintable(bb)) {
      b[r] = bb;
    } else {
      b[r] = '*';
      if (bb==(char)NULL) {
        //        b[r] = bb;
        break;
      }
    }
    r++;
  }
  for (a=0;a<r;a++) {
    *dest = b[a];
    dest++;
  }
  return(r);
}

int copyFromUECSID(char *dest) {
  int a,r;
  char b[13];
  a = LC_UECS_ID;
  sprintf(b,"%02X%02X%02X%02X%02X%02X",atmem.read(a),
          atmem.read(a+1),atmem.read(a+2),atmem.read(a+3),atmem.read(a+4),atmem.read(a+5));
  for (a=0;a<12;a++) {
    *dest = b[a];
    dest++;
  }
  return(a);
}

int ip2chars(char *dest,IPAddress ip) {
  char ibuf[16];
  int i1,i2,i3,i4,r;
  i1 = ip & 0xff;
  i2 = (ip & 0xff00) >> 8;
  i3 = (ip & 0xff0000) >> 16;
  i4 = (ip & 0xff000000) >> 24;
  sprintf(ibuf,"%d.%d.%d.%d",i1,i2,i3,i4);
  r = copyFromRAM(dest,ibuf);
  return(r);
}

int copyFromRAM(char *dest,char *src) {
  int i,l;
  l = strlen(src);
  for (i=0;i<l;i++) {
    *dest = *src;
    dest++;
    src++;
  }
  return(i);
}


int copyFromPROGMEM(char *dest,const char *src) {
  int i,l;
  char c;

  for(i=0;i<50;i++) {
    c = pgm_read_byte_near(src);
    if (c=='$') break;
    *dest = c;
    dest++;
    src++;
  }
  return(i);
}

void clear_uecsbuf(void) {
  int i;
  for (i=0;i<LEN_UECSXML_BUFFER;i++) {
    uecsbuf[i] = NULL;
  }
}

boolean is_dhcp(void) {
  byte f;
  f = atmem.read(FIX_DHCP_FLAG);
  if (f==0x00) {
    return(false);
  }
  return(true);
}

IPAddress getIPAddressFromEEPROM(int a) {
  IPAddress r;
  r = IPAddress(atmem.read(a),atmem.read(a+1),atmem.read(a+2),atmem.read(a+3));
  return(r);
}

void configure_wdt(void) {
  cli();                           // disable interrupts for changing the registers
  MCUSR = 0;                       // reset status register flags
                                   // Put timer in interrupt-only mode:
  WDTCSR |= 0b00011000;            // Set WDCE (5th from left) and WDE (4th from left) to enter config mode,
                                   // using bitwise OR assignment (leaves other bits unchanged).
  WDTCSR =  0b00001000 | 0b100001; // clr WDIE: interrupt enabled
                                   // set WDE: reset disabled
                                   // and set delay interval (right side of bar) to 8 seconds
  sei();                           // re-enable interrupts
                                   // reminder of the definitions for the time before firing
                                   // delay interval patterns:
                                   //  16 ms:     0b000000
                                   //  500 ms:    0b000101
                                   //  1 second:  0b000110
                                   //  2 seconds: 0b000111
                                   //  4 seconds: 0b100000
                                   //  8 seconds: 0b100001
}

#endif
