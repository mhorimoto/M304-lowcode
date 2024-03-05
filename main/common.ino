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
  extern bool debugMsgFlag(int);
  
  if (debugMsgFlag(SO_MSG)||f) {
    Serial.begin(115200);
    switch(kind) {
    case 1:  // st_m display
      Serial.println(F("1-NET"));
      _dump_flb(1,f);
      break;
    case 2:  // flb_rx_ccm display
      Serial.println(F("2-RX CCM"));
      _dump_flb(2,f);
      break;
    case 3:  // flb_tx_ccm display
      Serial.println(F("3-TX CCM"));
      _dump_flb(3,f);
      break;
    case 4:  // flb_cmpope display
      Serial.println(F("4-CMPOPE"));
      _dump_flb(4,f);
      break;
    }
  }
}

void _dump_flb(int k, int f) {
  extern uecsM304  flb_rx_ccm[],flb_tx_ccm[];
  extern uecsM304cmpope flb_cmpope[];
  extern bool debugMsgFlag(int);
  int i,imax;
  uecsM304 *flb;
  
  if (k==2) {
    flb = &flb_rx_ccm[0];
    imax = CCM_TBL_CNT_RX;
  } else if (k==3) {
    flb = &flb_tx_ccm[0];
    imax = CCM_TBL_CNT_TX;
  }
  switch(k) {
  case 1:
    Serial.println(st_m.gw);
    Serial.println(st_m.ip);
    Serial.println(st_m.dns);
    Serial.println(st_m.subnet);
    Serial.println(st_m.cidr);
    Serial.println(broadcastIP);
    break;
  case 2:
  case 3:
    for(i=0;i<imax;i++) {
      if ((f==2)&&(flb->valid!=1)) continue;
      //      sprintf(lbftxt,"%d,%d,%d,%d,%d,%d,%d,%c,%s,%s,%d,%d,%d,%d,%d,%d,%02x,%02x",
      //              flb->valid,flb->room,flb->region,flb->order,flb->priority,flb->lv,
      //              flb->cast,flb->sr,flb->ccm_type,flb->unit,
      //              flb->sthr,flb->stmn,flb->edhr,flb->edmn,flb->inmn,flb->dumn,
      //              flb->rly_l,flb->rly_h);
      //      Serial.println(lbftxt);
      Serial.print(flb->valid);
      Serial.print(F(","));
      Serial.print(flb->room);
      Serial.print(F(","));
      Serial.print(flb->region);
      Serial.print(F(","));
      Serial.print(flb->order);
      Serial.print(F(","));
      Serial.print(flb->priority);
      Serial.print(F(","));
      Serial.print(flb->lv);
      Serial.print(F(","));
      Serial.print(flb->cast);
      Serial.print(F(","));
      Serial.print((char)flb->sr);
      Serial.print(F(","));
      Serial.print(flb->ccm_type);
      Serial.print(F(","));
      Serial.print(flb->unit);
      Serial.print(F(","));
      Serial.print(flb->sthr);
      Serial.print(F(","));
      Serial.print(flb->stmn);
      Serial.print(F(","));
      Serial.print(flb->edhr);
      Serial.print(F(","));
      Serial.print(flb->edmn);
      Serial.print(F(","));
      Serial.print(flb->inmn);
      Serial.print(F(","));
      Serial.print(flb->dumn);
      Serial.print(F(","));
      Serial.print(flb->rly_l,BIN);
      Serial.print(F(","));
      Serial.println(flb->rly_h,BIN);
      flb++;
    }
    break;
  case 4:
    for(i=0;i<CCM_TBL_CNT_CMP;i++) {
      if ((f==2)&&(flb_cmpope[i].valid!=1)) continue;
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

void copyVersionCode(unsigned int addr,char *pgn) {
  int i;
  uint8_t c;
  
  for(i=0;i<16;i++) {
    c = (uint8_t)(*pgn);
    eeprom_update(addr+i,c);
    if (c==0) break;
    pgn++;
  }
}

void eeprom_update(unsigned int a,uint8_t v) {
  uint8_t rv;
  rv = atmem.read(a);
  if (rv!=v) {
    atmem.write(a,v);
  }
}
