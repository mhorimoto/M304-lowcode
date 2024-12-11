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
  extern uecsM304Sched  flb_rx_ccm[];
  extern uecsM304Send   flb_tx_ccm[];
  extern uecsM304cmpope flb_cmpope[];
  extern bool debugMsgFlag(int);
  int i,imax;
  char lbftxt[80],rly1[5],rly5[5],*mnflag,*lv;
  uecsM304Sched  *ptr_flb_rx;
  uecsM304Send   *ptr_flb_tx;
  uecsM304cmpope *ptr_flb_cmpope;
  
  if (k==2) {
    ptr_flb_rx = &flb_rx_ccm[0];
    imax = CCM_TBL_CNT_RX;
  } else if (k==3) {
    ptr_flb_tx = &flb_tx_ccm[0];
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
  case 2: // uecsM304Sched (RX)
    for(i=0;i<imax;i++) {
      if ((f==2)&&(ptr_flb_rx->valid!=1)) {
        ptr_flb_rx++;
        continue;
      }
      switch(ptr_flb_rx->mnflag) {
      case 0:
        mnflag = "SECS";
        break;
      case 0xff:
        mnflag = "MINU";
        break;
      default:
        mnflag = "????";
      }
      conv_relay(ptr_flb_rx->rly_l,&rly1[0]);
      conv_relay(ptr_flb_rx->rly_h,&rly5[0]);
      sprintf(lbftxt,"%2d,%02x,%02d:%02d-%02d:%02d,%s(%02x),%d/%d,(%02x%02x),%02x,%02x,%02x,%f",
              i,ptr_flb_rx->valid,ptr_flb_rx->sthr,ptr_flb_rx->stmn,ptr_flb_rx->edhr,
              ptr_flb_rx->edmn,mnflag,ptr_flb_rx->mnflag,ptr_flb_rx->inmn,ptr_flb_rx->dumn,
              ptr_flb_rx->rly_l,ptr_flb_rx->rly_h,ptr_flb_rx->cmbcmp[0],ptr_flb_rx->cmpccmid[0],
              ptr_flb_rx->cmpope[0],ptr_flb_rx->cmpval[0]);
      ptr_flb_rx++;
      Serial.println(lbftxt);
    }
    break;
      
  case 3: // uecsM304Send (TX)
    for(i=0;i<imax;i++) {
      if ((f==2)&&(ptr_flb_tx->valid!=1)) {
        ptr_flb_tx++;
        continue;
      }
      switch(ptr_flb_tx->lv) {
      case LV_A1S0:
        lv = "A-1S-0";
        break;
      case LV_A1S1:
        lv = "A-1S-1";
        break;
      case LV_A10S0:
        lv = "A-10S-0";
        break;
      case LV_A10S1:
        lv = "A-10S-1";
        break;
      case LV_A1M0:
        lv = "A-1M-0";
        break;
      case LV_A1M1:
        lv = "A-1M-1";
        break;
      case LV_B0:
        lv = "B-0";
        break;
      case  LV_B1:
        lv = "B-1";
        break;
      case LV_S1S0:
        lv = "S-1S-0";
        break;
      case LV_S1M0:
        lv = "S-1M-0";
        break;
      }

      sprintf(lbftxt,"%2d,%d,%d,%d,%d,%d,%s(%d),%d,%s,%s",
              i,ptr_flb_tx->valid,ptr_flb_tx->room,ptr_flb_tx->region,ptr_flb_tx->order,
              ptr_flb_tx->priority,lv,ptr_flb_tx->lv,ptr_flb_tx->cast,
              ptr_flb_tx->ccmtype,ptr_flb_tx->unit);
      Serial.println(lbftxt);
      ptr_flb_tx++;
    }
    break;
  case 4:
    for(i=0;i<CCM_TBL_CNT_CMP;i++) {
      if ((f==2)&&(flb_cmpope[i].valid!=1)) continue;
      Serial.print(flb_cmpope[i].valid);Serial.print(",");
      Serial.print(flb_cmpope[i].room);Serial.print(",");
      Serial.print(flb_cmpope[i].region);Serial.print(",");
      Serial.print(flb_cmpope[i].order);Serial.print(",");
      Serial.print(flb_cmpope[i].lifecnt);Serial.print(",");
      Serial.println(flb_cmpope[i].ccm_type);
      //Serial.println(flb_cmpope[i].cmpope);  // 3.0.0D7から不要 blk_cで設定するので
      //Serial.println(flb_cmpope[i].fval);    // 3.0.0D7から不要 blk_cで設定するので
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

void copyFromLC_uecsM304Sched(uecsM304Sched *tg,int a) {
  int i;
  tg->valid    = atmem.read(a+LC_SCH_VALID);    // 0x00
  tg->sthr     = atmem.read(a+LC_SCH_STHR);     // 0x01
  tg->stmn     = atmem.read(a+LC_SCH_STMN);     // 0x02
  tg->edhr     = atmem.read(a+LC_SCH_EDHR);     // 0x03
  tg->edmn     = atmem.read(a+LC_SCH_EDMN);     // 0x04
  tg->mnflag   = atmem.read(a+LC_SCH_MNFLAG);   // 0x05
  tg->inmn     = atmem.readInt(a+LC_SCH_INMN);  // 0x06
  tg->dumn     = atmem.readInt(a+LC_SCH_DUMN);  // 0x08
  tg->rly_l    = atmem.read(a+LC_SCH_RLY_L);    // 0x0e
  tg->rly_h    = atmem.read(a+LC_SCH_RLY_H);    // 0x0f
  for(i=0;i<5;i++) {
    if (i==0) {
      tg->cmbcmp[i] = R_AND;
    } else {
      tg->cmbcmp[i] = atmem.read(a+LC_SCH_CMBCMP1+((i-1)*7));
    }
    tg->cmpccmid[i] = atmem.read(a+LC_SCH_CMPCCMID0+(i*7));
    tg->cmpope[i]   = atmem.read(a+LC_SCH_CMPOPE0+(i*7));
    tg->cmpval[i]   = atmem.readFloat(a+LC_SCH_CMPVAL0+(i*7));
  }
}

void copyFromLC_uecsM304Send(uecsM304Send *tg,int a) {
    tg->valid    = atmem.read(a+LC_SEND_VALID);    
    tg->room     = atmem.read(a+LC_SEND_ROOM);         // 0x01
    tg->region   = atmem.read(a+LC_SEND_REGION);       // 0x02
    tg->order    = atmem.readInt(a+LC_SEND_ORDER);
    tg->priority = atmem.read(a+LC_SEND_PRIORITY);     // 0x05
    tg->lv       = atmem.read(a+LC_SEND_LV);    
    tg->cast     = atmem.read(a+LC_SEND_CAST);         // 0x07
    atmem.readChars(a+LC_SEND_CCMTYPE,&tg->ccmtype[0],20);
    atmem.readChars(a+LC_SEND_UNIT,&tg->unit[0],10);
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
    tg->lifecnt = atmem.read(a+LC_COPE_LIFECNT);     // 0x05
    for (i=0;i<20;i++) {
        tg->ccm_type[i] = atmem.read(a+LC_COPE_CCMTYPE+i); // 0x06 ASCIZ
    }
    tg->priority = 99;   // Initial value
    tg->remain   = 0;
    
    //tg->cmpope = atmem.read(a+LC_COPE_OPE);         // 0x1a
    //for(i=0;i<4;i++) {
    //  crf.c[i] = atmem.read(a+LC_COPE_FVAL+i);
    //}
    //tg->fval = crf.f;
}

void init_uecsTBL(void) {
    int a,j,w;
    w = atmem.read(LC_DBGMSG);
    debugMsgOutput(1,w); // NET
    // 2.3.5D Read fast lookup buffer for CCM table
    for (j=0;j<CCM_TBL_CNT_RX;j++) {
        a = LC_SCH_START+(j*LC_SCH_REC_SIZE);
        copyFromLC_uecsM304Sched(&flb_rx_ccm[j],a);
    }
    debugMsgOutput(2,w); // rx_ccm display
    for (j=0;j<CCM_TBL_CNT_TX;j++) {
        a = LC_SEND_START+(j*LC_SEND_REC_SIZE);
        copyFromLC_uecsM304Send(&flb_tx_ccm[j],a);
    }
    debugMsgOutput(3,w); // tx_ccm display
    for (j=0;j<CCM_TBL_CNT_CMP;j++) {
        a = LC_CMPOPE_START+(j*LC_CMPOPE_REC_SIZE);
        copyFromLC_uecsM304cmpope(&flb_cmpope[j],a);
    }
    debugMsgOutput(4,w); // cmpope display
}


 void conv_relay(byte a,char o[]) {
  byte b;
  int  i;
  for(i=0;i<4;i++) {
    b = (a>>(i*2))*0x03;
    switch(b) {
    case RLY_DNTCARE:
      o[3-i] = '-';
      break;
    case RLY_BOTH:
      o[3-i] = 'T';
      break;
    case RLY_BREAK:
      o[3-i] = 'B';
      break;
    case RLY_MAKE:
      o[3-i] = 'M';
      break;
    }
  }
  o[4] = (char)NULL;
}
 
