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
    Serial.print("UDP16529 size=");
    Serial.println(packetSize);
    Serial.print("UDP16529 IP=");
    Serial.println(UECS_UDP16529.remoteIP()) ;
    Serial.print("REMOTE PORT=");
    Serial.println(UECS_UDP16529.remotePort());
    Serial.print("TEXT=");
    Serial.println(uecsbuf);
    if (xmldecode(&uecsbuf[0])) {
      if (ptr_uecsxmldata->element==ELE_NODESCAN) {
        res_nodescan(UECS_UDP16529.remoteIP(),UECS_UDP16529.remotePort());
        //        debug_uecsxmldata();
      }
    } else {
      Serial.println("YXML ERROR");
    }
  }
}


void res_nodescan(IPAddress ripa, unsigned int rport) {
  int cfp; // copyFromPROGMEM next pointer
  char buf[80];
  extern int copyFromPROGMEM(char *,const char *);
  extern int ip2chars(char *,IPAddress);
  
  extern const char xmlhead[],res_xmlnode1[];

  sprintf(buf,"%02X%02X%02X%02X%02X%02X",st_m.mac[0],st_m.mac[1],st_m.mac[2],st_m.mac[3],st_m.mac[4],st_m.mac[5]);
  clear_uecsbuf();
  cfp = copyFromPROGMEM(&uecsbuf[0],&xmlhead[0]);
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode1[0]);
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode2[0]);
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode3[0]);
  cfp += copyFromUECSID(&uecsbuf[cfp]);
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode4[0]);
  cfp += ip2chars(&uecsbuf[cfp],st_m.ip);
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode5[0]);
  cfp += copyFromRAM(&uecsbuf[cfp],&buf[0]);
  cfp += copyFromPROGMEM(&uecsbuf[cfp],&res_xmlnode6[0]);

  UECS_UDP16529.beginPacket(ripa, rport);
  UECS_UDP16529.write(uecsbuf);
  UECS_UDP16529.endPacket();
  Serial.print("uecsbuf=");
  Serial.println(uecsbuf);
}

void debug_uecsxmldata(void) {
  Serial.print("ELEMENT=");
  Serial.println(ptr_uecsxmldata->element);
  Serial.print("VER=");
  Serial.println(ptr_uecsxmldata->ver);
  Serial.print("TYPE=");
  Serial.println(ptr_uecsxmldata->type);
  Serial.print("PAGE=");
  Serial.println(ptr_uecsxmldata->page);
  Serial.print("ROOM=");
  Serial.println(ptr_uecsxmldata->room);
  Serial.print("REGION=");
  Serial.println(ptr_uecsxmldata->region);
  Serial.print("ORDER=");
  Serial.println(ptr_uecsxmldata->order);
  Serial.print("PRIORITY=");
  Serial.println(ptr_uecsxmldata->priority);
  Serial.print("TEXTVAL=");
  Serial.println(ptr_uecsxmldata->textval);
  Serial.print("FVAL=");
  Serial.println(ptr_uecsxmldata->fval);
}


/********************************/
/* 16529 Response   *************/
/********************************/

boolean UECSresNodeScan() {
  int i;
  int progPos = 0;
  int startPos = 0;
#ifdef GOGO
  if ( ! UECSFindPGMChar(UECSbuffer,&UECSccm_XMLHEADER[0],&progPos) ) {
    return false;
  }
  startPos+=progPos;
  if ( ! UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_UECSVER_E10[0],&progPos) ) {
    //other ver
    return false;
  }
  startPos+=progPos;
  //NODESCAN
  if ( UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_NODESCAN1[0],&progPos)
       || UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_NODESCAN2[0],&progPos) ) {
    //NODESCAN response
    ClearMainBuffer();
    UDPAddPGMCharToBuffer(&(UECSccm_XMLHEADER[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_UECSVER_E10[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_NODENAME[0])); 
    UDPAddPGMCharToBuffer(&(U_name[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_VENDER[0])); 
    UDPAddPGMCharToBuffer(&(U_vender[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_UECSID[0])); 
    UDPAddPGMCharToBuffer(&(U_uecsid[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_UECSID_IP[0]));
    if ( U_orgAttribute.status & STATUS_SAFEMODE ) {
      UDPAddPGMCharToBuffer(&(UECSdefaultIPAddress[0]));
    } else {
      sprintf( UECStempStr20, "%d.%d.%d.%d",
               U_orgAttribute.ip[0], U_orgAttribute.ip[1], U_orgAttribute.ip[2], U_orgAttribute.ip[3]);
      UDPAddCharToBuffer(UECStempStr20);
    }
    UDPAddPGMCharToBuffer( &(UECSccm_MAC[0]) );
    sprintf( UECStempStr20, "%02X%02X%02X%02X%02X%02X",
             U_orgAttribute.mac[0], U_orgAttribute.mac[1], U_orgAttribute.mac[2],
             U_orgAttribute.mac[3], U_orgAttribute.mac[4], U_orgAttribute.mac[5]);
    UDPAddCharToBuffer( UECStempStr20 );
    UDPAddPGMCharToBuffer( &(UECSccm_NODECLOSE[0]) );
    return true;
  }
  //CCMSCAN
  if ( UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_CCMSCAN[0],&progPos) ) {
    short pageNum=0;
    startPos+=progPos;
    //format of page number written type
    if ( UECSGetValPGMStrAndChr(&UECSbuffer[startPos],
                                &UECSccm_PAGE[0],'\"',&pageNum,&progPos)) {
      startPos+=progPos;
      //check close tag
      if ( !(UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_CCMSCANCLOSE2[0],&progPos)) ) {
        return false;
      }
      //format of page number abridged type
    } else if ( UECSFindPGMChar(&UECSbuffer[startPos],&UECSccm_CCMSCANCLOSE0[0],&progPos) ) {
      pageNum=1;
    } else {
      return false;
    }
    //CCMSCAN response
    ClearMainBuffer();
    UDPAddPGMCharToBuffer(&(UECSccm_XMLHEADER[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_UECSVER_E10[0]));
    UDPAddPGMCharToBuffer(&(UECSccm_CCMNUM[0]));
	
    //count enabled ccm
    short enabledCCMNum=0;
    short returnCCMID=-1;
    for ( i=0; i < U_MAX_CCM; i++ ) {
      if ( U_ccmList[i].ccmLevel != NONE ) {
        enabledCCMNum++;
        if ( enabledCCMNum==pageNum ) {
          returnCCMID=i;
        }
      }
    }
    if ( enabledCCMNum==0 || returnCCMID<0 ) {
      return false;
    }//page num over
    UDPAddValueToBuffer(pageNum);
    UDPAddPGMCharToBuffer(&(UECSccm_TOTAL[0]));
    UDPAddValueToBuffer(enabledCCMNum);
    UDPAddPGMCharToBuffer(&(UECSccm_CLOSETAG[0]));
    UDPAddValueToBuffer(1);//Column number is always 1
    UDPAddPGMCharToBuffer(&(UECSccm_CCMNO[0]));
    UDPAddValueToBuffer(pageNum);//page number = ccm number
    for ( i=0;i<3;i++ )	{
      UDPAddPGMCharToBuffer(UECSattrChar[i]);
      UDPAddValueToBuffer(U_ccmList[returnCCMID].baseAttribute[i]);
    }
    UDPAddPGMCharToBuffer(UECSattrChar[AT_PRIO]);
    UDPAddValueToBuffer(U_ccmList[returnCCMID].baseAttribute[AT_PRIO]);
    UDPAddPGMCharToBuffer(&(UECSccm_CAST[0]));
    UDPAddValueToBuffer(U_ccmList[returnCCMID].decimal);
    UDPAddPGMCharToBuffer(&(UECSccm_UNIT[0]));                          
    UDPAddPGMCharToBuffer((U_ccmList[returnCCMID].unit));
    UDPAddPGMCharToBuffer(&(UECSccm_SR[0]));
    if ( U_ccmList[returnCCMID].sender ) {
      UDPAddPGMCharToBuffer(UECSTxtPartS);
    } else {
      UDPAddPGMCharToBuffer(UECSTxtPartR);
    }              
    UDPAddPGMCharToBuffer(&(UECSccm_LV[0]));                          
    UDPAddPGMCharToBuffer((UECSCCMLEVEL[U_ccmList[returnCCMID].ccmLevel]));
    UDPAddPGMCharToBuffer(&(UECSccm_CLOSETAG[0]));
    UDPAddCharToBuffer(U_ccmList[returnCCMID].typeStr);
    UDPAddPGMCharToBuffer(&(UECSccm_CCMRESCLOSE[0]));  
    return true;
  }
#endif
  return false;
}

bool UECSFindPGMChar(char* targetBuffer,const char *_romword_startStr,int *lastPos) {
  *lastPos=0;
  int startPos=-1;
  int _targetBuffersize=strlen(targetBuffer);
  int _startStrsize=strlen_P(_romword_startStr);
#ifdef GOGO
  if ( _targetBuffersize<_startStrsize ) {
    return false;
  }
  int i,j;
  //-------------start string check
  unsigned char startchr=pgm_read_byte(&_romword_startStr[0]);
  for ( i=0;i<_targetBuffersize-_startStrsize+1;i++ ) {
    //not hit
    if ( targetBuffer[i]!=startchr ) {
      continue;
    }
    //if hit 1 chr ,more check
    for ( j=0;j<_startStrsize;j++ ) {
      if ( targetBuffer[i+j]!=pgm_read_byte(&_romword_startStr[j]) ) {
        break;
      }//not hit!
    }
    //hit all chr
    if ( j==_startStrsize ) {
      startPos=i;
      break;
    }
  }
  if ( startPos<0 ) {
    return false;
  }
  *lastPos = startPos + _startStrsize;
  return true;
#endif
}

