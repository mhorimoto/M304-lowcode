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

char *itoaddr(IPAddress a) {
  char ia[16],*iap;
  iap = &ia[0];
  sprintf(iap,"%d.%d.%d.%d",a[0],a[1],a[2],a[3]);
  return(iap);
}

void debugSerialOut(int a,int b,char *c) {
  extern bool fsf;
  extern bool debugMsgFlag(int);
  char t[45];

  if (debugMsgFlag(SO_MSG)) {
    sprintf(t,"%s cmode=%d/%d/%d",c,a,b,fsf,c);
    Serial.println(t);
  }
}

void debugMsgOutput(int kind,int f=0) {
  extern uecsM304  flb_rx_ccm[],flb_tx_ccm[];
  extern uecsM304cmpope flb_cmpope[];
  extern bool debugMsgFlag(int);

  int i;
  
  if (debugMsgFlag(SO_MSG)||f) {
    Serial.begin(115200);
    switch(kind) {
    case 1:  // st_m display
      Serial.println(F("1-NET"));
      Serial.println(st_m.gw);
      Serial.println(st_m.ip);
      Serial.println(st_m.dns);
      Serial.println(st_m.subnet);
      Serial.println(st_m.cidr);
      Serial.println(broadcastIP);
      break;
    case 2:  // flb_rx_ccm display
      Serial.println(F("2-RX CCM"));
      for(i=0;i<CCM_TBL_CNT_RX;i++) {
        Serial.println(flb_rx_ccm[i].valid);
        Serial.println(flb_rx_ccm[i].room);
        Serial.println(flb_rx_ccm[i].region);
        Serial.println(flb_rx_ccm[i].order);
        Serial.println(flb_rx_ccm[i].priority);
        Serial.println(flb_rx_ccm[i].lv);
        Serial.println(flb_rx_ccm[i].cast);
        Serial.println(flb_rx_ccm[i].sr);
        Serial.println(flb_rx_ccm[i].ccm_type);
        Serial.println(flb_rx_ccm[i].unit);
        Serial.println(flb_rx_ccm[i].sthr);
        Serial.println(flb_rx_ccm[i].stmn);
        Serial.println(flb_rx_ccm[i].edhr);
        Serial.println(flb_rx_ccm[i].edmn);
        Serial.println(flb_rx_ccm[i].inmn);
        Serial.println(flb_rx_ccm[i].dumn);
        Serial.println(flb_rx_ccm[i].rly_l,HEX);
        Serial.println(flb_rx_ccm[i].rly_h,HEX);
      }
      break;
    case 3:  // flb_tx_ccm display
      Serial.println(F("3-TX CCM"));
      for(i=0;i<CCM_TBL_CNT_TX;i++) {
        Serial.println(flb_tx_ccm[i].valid);
        Serial.println(flb_tx_ccm[i].room);
        Serial.println(flb_tx_ccm[i].region);
        Serial.println(flb_tx_ccm[i].order);
        Serial.println(flb_tx_ccm[i].priority);
        Serial.println(flb_tx_ccm[i].lv);
        Serial.println(flb_tx_ccm[i].cast);
        Serial.println(flb_tx_ccm[i].sr);
        Serial.println(flb_tx_ccm[i].ccm_type);
        Serial.println(flb_tx_ccm[i].unit);
        Serial.println(flb_tx_ccm[i].sthr);
        Serial.println(flb_tx_ccm[i].stmn);
        Serial.println(flb_tx_ccm[i].edhr);
        Serial.println(flb_tx_ccm[i].edmn);
        Serial.println(flb_tx_ccm[i].inmn);
        Serial.println(flb_tx_ccm[i].dumn);
        Serial.println(flb_tx_ccm[i].rly_l,HEX);
        Serial.println(flb_tx_ccm[i].rly_h,HEX);
      }
      break;
    case 4:  // flb_cmpope display
      Serial.println(F("4-CMPOPE"));
      for(i=0;i<CCM_TBL_CNT_CMP;i++) {
	if ((f==2)&&(flb_cmpope[i].valid==1)) {
	  Serial.println(flb_cmpope[i].valid);
	  Serial.println(flb_cmpope[i].room);
	  Serial.println(flb_cmpope[i].region);
	  Serial.println(flb_cmpope[i].order);
	  Serial.println(flb_cmpope[i].priority);
	  Serial.println(flb_cmpope[i].ccm_type);
	  Serial.println(flb_cmpope[i].cmpope);
	  Serial.println(flb_cmpope[i].fval);
	}
      }
      break;
    }
  }
}


void floatingtest(float a) {
  union CHARFLOAT crf;
  int k;
  
  crf.f = a;
  for (k=0;k<4;k++) {
    Serial.print((crf.c[k]&0xff),HEX);
    Serial.print(" ");
  }
  Serial.println();
}
