#include <M304.h>
//----------------------------------
//void UECSupdate16529port( UECSTEMPCCM* _tempCCM){
void UECSupdate16529port(void) {
  extern char uecsbuf[];
  extern bool xmldecode(char *);
  extern st_UECSXML *ptr_uecsxmldata;
  
  int packetSize = UECS_UDP16529.parsePacket();
  if (packetSize>10) {
    UECS_UDP16529.read(uecsbuf,600-1);
    uecsbuf[packetSize] = NULL;
    if (debugMsgFlag(SO_MSG)) {
      Serial.print(F("UDP16529 size="));
      Serial.println(packetSize);
      Serial.print(F("UDP16529 IP="));
      Serial.println(UECS_UDP16529.remoteIP()) ;
      Serial.print(F("REMOTE PORT="));
      Serial.println(UECS_UDP16529.remotePort());
      Serial.print(F("TEXT="));
      Serial.println(uecsbuf);
    }
    if (xmldecode(&uecsbuf[0])) {
      if (ptr_uecsxmldata->element==ELE_NODESCAN) {
        res_nodescan(UECS_UDP16529.remoteIP(),UECS_UDP16529.remotePort());
        //        debug_uecsxmldata();
      }
    } else {
      Serial.println(F("YXML ERROR"));
    }
  }
}


void res_nodescan(IPAddress ripa, unsigned int rport) {
  int cfp; // copyFromPROGMEM next pointer
  char buf[80];
  extern int copyFromPROGMEM(char *,const char *);
  extern int ip2chars(char *,IPAddress);
  
  //  extern const char xmlhead[]; // ,res_xmlnode1[];

  sprintf(buf,"%02X%02X%02X%02X%02X%02X",st_m.mac[0],st_m.mac[1],st_m.mac[2],st_m.mac[3],st_m.mac[4],st_m.mac[5]);
  clear_uecsbuf();
  cfp = copyFromPROGMEM(&uecsbuf[0],&xmlhead[0]);          // <?xml version=\"1.0\"?><UECS v
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode1[0]);  // <NODE><NAME>
  cfp += copyFromNAMEVENDER(&uecsbuf[cfp],NODE_NAME);      // NAME
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode2[0]);  // </NAME><VENDER>
  cfp += copyFromNAMEVENDER(&uecsbuf[cfp],VENDER_NAME);    // VENDER
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode3[0]);  // </VENDER><UECSID>
  cfp += copyFromUECSID(&uecsbuf[cfp]);                    // UECSID
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode4[0]);  // </UECSID><IP>
  cfp += ip2chars(&uecsbuf[cfp],st_m.ip);                  // IPAddress
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode5[0]);  // </IP><MAC>
  cfp += copyFromRAM(&uecsbuf[cfp],&buf[0]);               // MACAddress
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode6[0]);  // </MAC></NODE></UECS>

  UECS_UDP16529.beginPacket(ripa, rport);
  UECS_UDP16529.write(uecsbuf);
  UECS_UDP16529.endPacket();
  Serial.print(F("uecsbuf="));
  Serial.println(uecsbuf);
}

void debug_uecsxmldata(void) {
  Serial.print(F("ELEMENT="));
  Serial.println(ptr_uecsxmldata->element);
  Serial.print(F("VER="));
  Serial.println(ptr_uecsxmldata->ver);
  Serial.print(F("TYPE="));
  Serial.println(ptr_uecsxmldata->type);
  Serial.print(F("PAGE="));
  Serial.println(ptr_uecsxmldata->page);
  Serial.print(F("ROOM="));
  Serial.println(ptr_uecsxmldata->room);
  Serial.print(F("REGION="));
  Serial.println(ptr_uecsxmldata->region);
  Serial.print(F("ORDER="));
  Serial.println(ptr_uecsxmldata->order);
  Serial.print(F("PRIORITY="));
  Serial.println(ptr_uecsxmldata->priority);
  Serial.print(F("TEXTVAL="));
  Serial.println(ptr_uecsxmldata->textval);
  Serial.print(F("FVAL="));
  Serial.println(ptr_uecsxmldata->fval);
}


