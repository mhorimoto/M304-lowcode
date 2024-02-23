#include <M304.h>

#if _M304_H_V < 1311
#pragma message("Library M304 is old. Version 1.3.11 or higher is required.")
#else

#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <yxml.h>
#include <string.h>

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));
void get_mcusr(void)     \
  __attribute__((naked)) \
  __attribute__((section(".init3")));
void get_mcusr(void) {
  mcusr_mirror = MCUSR;
  MCUSR = 0;
  wdt_disable();
}

char *pgname = "M304 Ver2.5.1Dbg03";

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
#define CCM_TBL_CNT_TX  10
#define CCM_TBL_CNT_CMP 10

#define TCNT1_1SEC      3036   // TNCT1 value for 1sec

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

union CHARFLOAT {
  float f;
  char  c[4];
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

char lbf[81];
extern bool debugMsgFlag(int);
extern void debugMsgOutput(int,int);

// 2.3.5D
uecsM304  flb_rx_ccm[CCM_TBL_CNT_RX],flb_tx_ccm[CCM_TBL_CNT_TX];
uecsM304cmpope flb_cmpope[CCM_TBL_CNT_CMP];
// 2.3.7DBG5
byte cmpope_result[CCM_TBL_CNT_CMP];

const char str_main0[] PROGMEM  = "NO NET MODE         ";
const char str_main1[] PROGMEM  = "STATIC IP ADDRESS   ";
const char str_main2[] PROGMEM  = "  EEPROM Operation  ";
const char str_main3[] PROGMEM  = "NO RTC PLS SETUP    ";
const char str_main4[] PROGMEM  = "  Network config    ";
const char str_main5[] PROGMEM  = "  RTC config        ";
const char str_main6[] PROGMEM  = "  SCHEDULE config   ";
const char str_main7[] PROGMEM  = "NIC IS NO W5500     ";
const char str_main8[] PROGMEM  = "IP:";
const char str_main9[] PROGMEM  = "Choose Menu         ";
const char str_main10[] PROGMEM = "UP/DOWN/ENT Key use ";
const char str_main11[] PROGMEM = "Exit:LEFT Key push  ";
const char str_main12[] PROGMEM = "ALL BREAK           ";
const char str_main13[] PROGMEM = "BpCMNDw/fsf";

const char *const str_main[] PROGMEM = {
  str_main0, str_main1, str_main2, str_main3, str_main4, str_main5, str_main6, str_main7,
  str_main8, str_main9, str_main10, str_main11, str_main12, str_main13
};

volatile int period1sec = 0;
volatile int period10sec = 0;
volatile int period60sec = 0;
volatile int period1hour = 0;
volatile time_t cepoch,pepoch;

void setup(void) {
  extern int mask2cidr(IPAddress);
  extern boolean is_dhcp(void);
  extern void clear_uecsbuf(void);
  
  int a,w,j,k;
  char ccm_type[21],line1[21];
  IPAddress hostip,subnet,gateway,dns;
  float tesfval;
  tmElements_t tm;
  
  m304Init();
  Serial.begin(115200);
  clear_uecsbuf();
  //  floating test routine 2.4.6Db
  //  floatingtest(387.0);
  
  ptr_uecsxmldata = &uecsxmldata;
  lcdd.begin(20,4);
  if (is_dhcp()) {
    debugMsgOutput(1,0); // st_m display
    if (Ethernet.begin(st_m.mac)==0) {
      // "NO NET MODE"
      strcpy_P(line1,(char *)pgm_read_word(&(str_main[0])));
      lcdd.setLine(0,2,line1);
      lcdd.LineWrite(0,2);
      st_m.dhcpflag = true;
    }
  } else {
    st_m.ip     = getIPAddressFromEEPROM(FIXED_IPADDRESS);
    st_m.subnet = getIPAddressFromEEPROM(FIXED_NETMASK);
    st_m.gw     = getIPAddressFromEEPROM(FIXED_DEFGW);
    st_m.dns    = getIPAddressFromEEPROM(FIXED_DNS);
    st_m.cidr   = mask2cidr(st_m.subnet);
    st_m.dhcpflag = false;
    strcpy_P(line1,(char *)pgm_read_word(&(str_main[1])));
    lcdd.setLine(0,2,line1);
    lcdd.LineWrite(0,2);
    Ethernet.begin(st_m.mac,st_m.ip,st_m.dns,st_m.gw,st_m.subnet);
    st_m.dhcpflag = false;
  }
  configure_wdt();
  msgRun1st();
  wdt_reset();
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
      //        initEEPROM_UECS();
      //	lcdd.setLine(0,3,"init EEPROM         ");
      //	lcdd.LineWrite(0,3);
    }
  }
  w = atmem.read(LC_DBGMSG);
  debugMsgOutput(1,w); // NET
  // 2.3.5D Read fast lookup buffer for CCM table
  for (j=0;j<CCM_TBL_CNT_RX;j++) {
    a = LC_SCH_START+(j*LC_SCH_REC_SIZE);
    copyFromLC_uecsM304(&flb_rx_ccm[j],a);
  }
  debugMsgOutput(2,w); // rx_ccm display
  for (j=0;j<CCM_TBL_CNT_TX;j++) {
    a = LC_SEND_START+(j*LC_SEND_REC_SIZE);
    copyFromLC_uecsM304(&flb_tx_ccm[j],a);
  }
  debugMsgOutput(3,w); // tx_ccm display
  for (j=0;j<CCM_TBL_CNT_CMP;j++) {
    a = LC_CMPOPE_START+(j*LC_CMPOPE_REC_SIZE);
    copyFromLC_uecsM304cmpope(&flb_cmpope[j],a);
  }
  debugMsgOutput(4,w); // cmpope display
  
  UDP16520.begin(16520);
  UECS_UDP16529.begin(16529);
  httpd.begin();
  sendUECSpacket(0,"2048"); // setup completed 0x800
  //  Serial.begin(115200);
  Serial.println(pgname);
  if (RTC.read(tm)==0) {
    // "NO RTC PLS SETUP    "
    strcpy_P(line1,(char *)pgm_read_word(&(str_main[3])));
    lcdd.setLine(cposp,1,line1);
    lcdd.LineWrite(cposp,1);
  }
  pepoch = 0;
  cepoch = RTC.get();
  pinMode(7,OUTPUT);
  //
  // Setup Timer1 Interrupt
  //
  TCCR1A  = 0;
  TCCR1B  = 0;
  //  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);  //CTCmode //prescaler to 1024
  //  OCR1A   = 15625-1;
  TCNT1 = TCNT1_1SEC ;
  TCCR1B |= (1 << CS12); // CS12 -> 1 prescaler = 256
  TIMSK1 |= (1 << OCIE1A);
}


void loop(void) {
  int x,y,z,id,hr,mi,mx,io,minsec,j;
  char ca,line1[21];
  static char pca;
  static int prvsec;
  extern struct KYBDMEM *ptr_crosskey,*getCrossKey(void);
  extern void opeSCH(void),opeRTC(void),opeNET(void),opeRUN(int,int),opeHttpd(EthernetClient);
  extern void UECSupdate16529port(void) ;
  extern void debugSerialOut(int,int,char*);
  uint8_t InputDataButtom(int,int,int,int,uint8_t,int mi='0',int mx='9');
  tmElements_t tm;

  EthernetClient httpClient = httpd.available();
  wdt_reset();
  //2.4.aDbg-01
  if ( httpClient ) {
    opeHttpd(httpClient);
  }
  UECSupdate16520port() ;
  // 2.4.aDbg-03
  UECSupdate16529port() ;
  if (digitalRead(SW_SAFE)==0) {
    // "  EEPROM Operation  "
    strcpy_P(line1,(char *)pgm_read_word(&(str_main[2])));
    lcdd.setLine(0,1,line1);
    lcdd.LineWrite(0,1);
    opeEEPROM();
  }
  switch(cmode) {
  case RUN:
    if (fsf) {
      msgRun1st();
      fsf = false;
    }
    //    if (RTC.read(tm)==0) {
    //      // "NO RTC PLS SETUP    "
    //      strcpy_P(line1,(char *)pgm_read_word(&(str_main[3])));
    //      lcdd.setLine(cposp,1,line1);
    //      lcdd.LineWrite(cposp,1);
    //    } else {
    if (period1sec==1) {
      period1sec = 0;
      breakTime(cepoch,tm);
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
        strcpy_P(line1,(char *)pgm_read_word(&(str_main[12])));
      }
      lcdd.setLine(cposp,3,line1);
      lcdd.LineWrite(cposp,3);
      //
      sendUECSpacket(0,"0");
      // Ver2.4.aDbg-02
      UECSupdate16520port() ;
    }
    ptr_crosskey = getCrossKey();
    if (ptr_crosskey->longf==true) {
      ptr_crosskey->longf=false;
      ptr_crosskey->kpos=0;
      cmode=CMND;
      fsf = true;
    }
    if (period1hour==1) {
      period1hour = 0;
      cepoch = RTC.get();
      Serial.print("1hour  ");
      Serial.println(cepoch);
    }
    wdt_reset();
    break;
    //################################################################
  case CMND:
    cf = false;
    if (fsf) {
      #ifdef DEBUG
      // "BpCMNDw/fsf"
      strcpy_P(line1,(char *)pgm_read_word(&(str_main[12])));
      debugSerialOut(cmode,cmenu,line1);
      #endif
      msgCmnd1st();
    }
    wdt_reset();
    ptr_crosskey = getCrossKey();
    if ((ptr_crosskey->longf==true)&&(ptr_crosskey->kpos & K_LEFT)) {
      #ifdef DEBUG
      debugSerialOut(cmode,cmenu,"BpCMNDw/K_LEFT");
      #endif
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
      #ifdef DEBUG
      debugSerialOut(cmode,cmenu,"K_UP");
      #endif
    }
    if (ptr_crosskey->kpos & K_DOWN) {
      ptr_crosskey->kpos &= ~K_DOWN;
      cmenu--;
      if (cmenu<NETCONFIG) cmenu=EEPROMOPE;
      cf = true;
      #ifdef DEBUG
      debugSerialOut(cmode,cmenu,"K_DOWN");
      #endif
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
      strcpy_P(line1,(char *)pgm_read_word(&(str_main[4])));
      lcdd.setLine(0,1,line1);
      break;
    case RTCCONFIG: // RTC CONFIG
      strcpy_P(line1,(char *)pgm_read_word(&(str_main[5])));
      lcdd.setLine(0,1,line1);
      break;
    case SCHCONFIG: // Schedule CONFIG
      strcpy_P(line1,(char *)pgm_read_word(&(str_main[6])));
      lcdd.setLine(0,1,line1);
      break;
    case EEPROMOPE: // EEPROM Operation
      strcpy_P(line1,(char *)pgm_read_word(&(str_main[2])));
      lcdd.setLine(0,1,line1);
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
  Serial.println(F("+--------------------+"));
  for (y=0;y<4;y++) {
    for (x=0;x<20;x++) {
      Serial.print(lcdd.CharRead(p,x,y));
    }
    Serial.println();
  }
  Serial.println(F("+--------------------+"));
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
  char line1[21];
  
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
    strcpy_P(line1,(char *)pgm_read_word(&(str_main[7])));
    lcdd.setLine(0,2,line1);
    lcdd.LineWrite(0,2);
  } else {
    strcpy_P(line1,(char *)pgm_read_word(&(str_main[8])));
    lcdd.TextWrite(0,0,2,line1);
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
  char line1[21];
  
  lcdd.noBlink();
  strcpy_P(line1,(char *)pgm_read_word(&(str_main[9])));
  lcdd.setLine(0,0,line1); // "Choose Menu         "
  strcpy_P(line1,(char *)pgm_read_word(&(str_main[10])));
  lcdd.setLine(0,2,line1); // "UP/DOWN/ENT Key use "
  strcpy_P(line1,(char *)pgm_read_word(&(str_main[11])));
  lcdd.setLine(0,3,line1); // "Exit:LEFT Key push  "
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

void sendUECSpacket(int id,char *v) {
  extern char *itoaddr(IPAddress);
  char t[256];
  char *xmlDT;
  byte enable;
  byte room,region,priority;
  int  order,x,a,j;
  char ccm_type[20];
  byte ordh,ordl;
  for(x=0;x<256;x++) {
    t[x] = (char)NULL;
  }
  a = LC_SEND_START+(id*LC_SEND_REC_SIZE);    // CCMTABLE
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

IPAddress getIPAddressFromEEPROM(int a) {
  IPAddress r;
  r = IPAddress(atmem.read(a),atmem.read(a+1),atmem.read(a+2),atmem.read(a+3));
  return(r);
}

// 2.3.5D
void copyFromLC_uecsM304(uecsM304 *tg,int a) {
  int i;
  uint8_t ordl,ordh;
  tg->valid    = atmem.read(a+LC_VALID);    
  tg->room     = atmem.read(a+LC_ROOM);         // 0x01
  tg->region   = atmem.read(a+LC_REGION);       // 0x02
  ordl = atmem.read(a+LC_ORDER);
  ordh = atmem.read(a+LC_ORDER+1);
  tg->order    = (ordh<<8)+ordl;
  tg->priority = atmem.read(a+LC_PRIORITY);     // 0x05
  tg->lv       = atmem.read(a+LC_LV);    
  tg->cast     = atmem.read(a+LC_CAST);         // 0x07
  tg->sr       = atmem.read(a+LC_SR);           // 0x08
  for (i=0;i<20;i++) {
    tg->ccm_type[i] = atmem.read(a+LC_CCMTYPE+i); // 0x09 ASCIZ
  }
  for (i=0;i<10;i++) {
    tg->unit[i] = atmem.read(a+LC_UNIT+i);      // 0x1d ASCIZ
  }
  tg->sthr     = atmem.read(a+LC_STHR);         // 0x27
  tg->stmn     = atmem.read(a+LC_STMN);         // 0x28
  tg->edhr     = atmem.read(a+LC_EDHR);         // 0x29
  tg->edmn     = atmem.read(a+LC_EDMN);         // 0x2a
  tg->inmn     = atmem.read(a+LC_INMN);         // 0x2b
  tg->dumn     = atmem.read(a+LC_DUMN);         // 0x2c
  tg->rly_l    = atmem.read(a+LC_RLY_L);        // 0x2d
  tg->rly_h    = atmem.read(a+LC_RLY_H);        // 0x2e
}

void copyFromLC_uecsM304cmpope(uecsM304cmpope *tg,int a) {
  int i;
  uint8_t ordl,ordh;
  union CHARFLOAT crf;
  tg->valid    = atmem.read(a+LC_COPE_VALID);    
  tg->room     = atmem.read(a+LC_COPE_ROOM);         // 0x01
  tg->region   = atmem.read(a+LC_COPE_REGION);       // 0x02
  ordl = atmem.read(a+LC_COPE_ORDER);
  ordh = atmem.read(a+LC_COPE_ORDER+1);
  tg->order    = (ordh<<8)+ordl;
  tg->priority = atmem.read(a+LC_COPE_PRIORITY);     // 0x05
  for (i=0;i<20;i++) {
    tg->ccm_type[i] = atmem.read(a+LC_COPE_CCMTYPE+i); // 0x06 ASCIZ
  }
  tg->cmpope = atmem.read(a+LC_COPE_OPE);         // 0x1a
  for(i=0;i<4;i++) {
    crf.c[i] = atmem.read(a+LC_COPE_FVAL+i);
  }
  tg->fval = crf.f;
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

ISR(TIMER1_COMPA_vect) {
  static byte cnt10=0,cnt60=0,cnt1h=0;
  extern time_t cepoch,pepoch;
  cnt10++;
  cnt60++;
  cnt1h++;
  digitalWrite(7,cnt1h&1);
  period1sec = 1;
  cepoch++;
  pepoch++;
  if (cnt10 >= 10) {
    cnt10 = 0;
    period10sec = 1;
    Serial.print(F("10sec  "));
    Serial.println(millis());
  }
  if (cnt60 >= 60) {
    cnt60 = 0;
    period60sec = 1;
    Serial.print(F("60sec  "));
    Serial.println(millis());
  }
  if (cnt1h >= 3600) {
    cnt1h = 0;
    period1hour = 1;
  }
  TCNT1 = TCNT1_1SEC;
}

#endif
