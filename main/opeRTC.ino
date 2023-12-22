#include <EthernetUdp2.h>
#define NTP_PACKET_SIZE  48 // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
tmElements_t tm;

const char str_opeRTC0[] PROGMEM = "Set RTC";
const char str_opeRTC1[] PROGMEM = "  via NTP server";
const char str_opeRTC2[] PROGMEM = "NO RTC SETUP    ";
const char str_opeRTC3[] PROGMEM = "%d/%02d/%02d  %02d:%02d:%02d";
const char str_opeRTC4[] PROGMEM = "Push ENT Key";
const char str_opeRTC5[] PROGMEM = "OK DONE             ";

const char *const str_opeRTC[] PROGMEM = {
  str_opeRTC0,
  str_opeRTC1,
  str_opeRTC2,
  str_opeRTC3,
  str_opeRTC4,
  str_opeRTC5
};

  
void opeRTC(void) {
  char line1[21],line2[41];
  extern struct KYBDMEM *ptr_crosskey,*getCrossKey(void);
  if (fsf) {
    fsf = false;
    cposp = 0;
    cposx = 0;
    cposy = 0;
    lcdd.initWriteArea(cposp);
    strcpy_P(line1,(char *)pgm_read_word(&(str_opeRTC[0])));
    lcdd.setLine(cposp,0,line1);
    strcpy_P(line1,(char *)pgm_read_word(&(str_opeRTC[1])));
    lcdd.setLine(cposp,1,line1);
    if (RTC.read(tm)==0) {
      strcpy_P(line1,(char *)pgm_read_word(&(str_opeRTC[2])));
      lcdd.setLine(0,2,line1);
    } else {
      strcpy_P(line2,(char *)pgm_read_word(&(str_opeRTC[3])));
      snprintf(line1,21,"%d/%02d/%02d  %02d:%02d:%02d",
               //snprintf(line1,21,line2,
	       tm.Year+1970,tm.Month,tm.Day,tm.Hour,tm.Minute,tm.Second);
      lcdd.setLine(0,2,line1);
      lcdd.LineWrite(0,2);
    }
    //    lcdd.setLine(cposp,3,"Push ENT Key");
    strcpy_P(line1,(char *)pgm_read_word(&(str_opeRTC[4])));
    //    lcdd.setLine(cposp,3,"Push ENT Key");
    lcdd.setLine(cposp,3,line1);
    lcdd.PageWrite(cposp);
  }
  if (ptr_crosskey->kpos & K_ENT) {
    ntpAccess();
    snprintf(line1,21,"%d/%02d/%02d  %02d:%02d:%02d",
	     	 tm.Year+1970,tm.Month,tm.Day,tm.Hour,tm.Minute,tm.Second);
    lcdd.setLine(0,2,line1);
    lcdd.LineWrite(0,2);
    PushEnter(cposp);
    if(RTC.write(tm)) {
      strcpy_P(line1,(char *)pgm_read_word(&(str_opeRTC[5])));
      lcdd.setLine(0,3,line1);
      //      lcdd.setLine(0,3,"OK DONE             ");
      lcdd.LineWrite(0,3);
    }
  }
  delay(100);
  ptr_crosskey = getCrossKey();
  if ((ptr_crosskey->longf==true)&&(ptr_crosskey->kpos & K_LEFT)) {
    ptr_crosskey->longf= false;
    ptr_crosskey->kpos = 0;
    cmode = CMND;
    fsf = true;
    lcdd.clear();
    cposy = 0;
    lcdd.setLine(cposp,cposy,pgname);
    lcdd.PageWrite(cposp);
  }
}

void ntpAccess(void) {
  EthernetUDP Udp;
  unsigned int localPort = 8888;       // local port to listen for UDP packets
  char timeServer[] = "ntp.nict.jp"; // time.nist.gov NTP server
  byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

  Udp.begin(localPort);
  sendNTPpacket(Udp,timeServer); // send an NTP packet to a time server
  delay(700);
  if ( Udp.parsePacket() ) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
  }
  //  Udp.end();
  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  //  Serial.begin(115200);
  //  Serial.print("Seconds since Jan 1 1900 = " );
  //  Serial.println(secsSince1900);

  // now convert NTP time into everyday time:
  //  Serial.print("Unix time = ");
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSince1900 - seventyYears;
  // print Unix time:
  //  Serial.println(epoch);
  epoch+=32400; // JST
  breakTime(epoch,tm);
  // int year = tmYearToCalendar(tm.Year);
  // int month = tm.Month;
  // int day = tm.Day;

  // Serial.print(year);
  // Serial.print("/");
  // Serial.print(month);
  // Serial.print("/");
  // Serial.print(day);
  // Serial.print("  ");
  // Serial.print(tm.Hour);
  // Serial.print(":");
  // Serial.print(tm.Minute);
  // Serial.print(":");
  // Serial.println(tm.Second);
  
  // // print the hour, minute and second:
  // Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
  // Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
  // Serial.print(':');
  // if ( ((epoch % 3600) / 60) < 10 ) {
  //   // In the first 10 minutes of each hour, we'll want a leading '0'
  //   Serial.print('0');
  // }
  // Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
  // Serial.print(':');
  // if ( (epoch % 60) < 10 ) {
  //   // In the first 10 seconds of each minute, we'll want a leading '0'
  //   Serial.print('0');
  // }
  // Serial.println(epoch % 60); // print the second
  // Serial.end();
}


unsigned long sendNTPpacket(EthernetUDP Udp,char* address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
