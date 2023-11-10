void opeSCH(void) {
  int  x,z,y,w,hr,mi,mx,io,ic;
  unsigned int addr;
  byte id,sthr,stmn,edhr,edmn,inmn,dumn,rly[8];
  char eebuf[32],lcdbuf[21];
  extern struct KYBDMEM *ptr_crosskey,*getCrossKey(void);
  if (fsf) {
    fsf = false;
    cposp = 0;
    cposx = 0;
    cposy = 1;
    lcdd.setLine(cposp,0,"Set Timer           ");
    lcdd.setLine(cposp,3," 0:BREAK  1:MAKE    ");
    getSCHData(cposp,0);
    lcdd.PageWrite(cposp);
    lcdd.setCursor(cposx,cposy);
  }
  if (ptr_crosskey->kpos & K_RIGHT) {
    cposx++;
    ptr_crosskey->kpos &= ~K_RIGHT;  // Reset Flag
    switch(cposy) {
    case 1:
      if (cposx>19) {
        cposx=4;
        cposy=2;
      } else {
        switch(cposx) {
        case 2:
        case 5:
        case 8:
        case 11:
        case 14:
        case 17:
          cposx++;
        }
      }
      break;
    case 2:
      if (cposx>11) {
        cposx=0;
        cposy=1;
	io = lcdd.IntRead(cposp,0,1,2);
      }
      break;
    default:
      cposx=0;
      cposy=1;
      io = lcdd.IntRead(cposp,0,1,2);
      break;
    }
  }
  if (ptr_crosskey->kpos & K_LEFT) {
    cposx--;
    ptr_crosskey->kpos &= ~K_LEFT;  // Reset Flag
    switch(cposy) {
    case 1:
      if (cposx<0) {
        cposy=2;
        cposx=11;
      } else {
        switch(cposx) {
        case 2:
        case 5:
        case 8:
        case 11:
        case 14:
        case 17:
          cposx--;
          break;
        }
      }
      break;
    case 2:
      if (cposx<4) {
        cposy=1;
        cposx=19;
      }
      break;
    default:
      cposx=0;
      cposy=1;
      io = lcdd.IntRead(cposp,0,1,2);
      break;
    }
  }
  lcdd.setCursor(cposx,cposy);
  if (cposy==1) {
    mi = '0';
    switch(cposx) {
    case 3:  // Hour
    case 9:
      mx = '2';
      break;
    case 4:
    case 10:
      if (lcdd.CharRead(cposp,cposx-1,cposy)=='2') {
	mx = '3';
      } else {
	mx = '9';
      }
      break;
    case 6:  // Minite
    case 12:
    case 15:
    case 18:
      mx = '5';
      break;
    default:
      mx = '9';
      io = lcdd.IntRead(cposp,0,1,2);
    }
    InputDataButtom(cposp,cposx,cposy,K_DIGIT,ptr_crosskey->kpos,mi,mx);
  } else if (cposy==2) {
    if ((cposx>=4)&&(cposx<12)) {
      mi = '0';
      mx = '1';
      InputDataButtom(cposp,cposx,cposy,K_DIGIT,ptr_crosskey->kpos,mi,mx);
    }
  }

  if (ptr_crosskey->kpos & K_ENT) {
    PushEnter(cposp);
    id = lcdd.IntRead(cposp,0,1,2);
    sthr = lcdd.IntRead(cposp,3,1,2);
    stmn = lcdd.IntRead(cposp,6,1,2);
    edhr = lcdd.IntRead(cposp,9,1,2);
    edmn = lcdd.IntRead(cposp,12,1,2);
    inmn = lcdd.IntRead(cposp,15,1,2);
    dumn = lcdd.IntRead(cposp,18,1,2);
    rly[0]=0;rly[1]=0;
    for(y=0;y<2;y++) {
      for(z=0;z<4;z++) {
        w = 4*y+z;
        if (lcdd.IntRead(cposp,w+4,2,1)==1) {
          x = 3;
        } else {
          x = 0;
        }
        rly[y]|=x<<((3-z)*2);
      }
    }
    addr = LC_SCH_START+id*LC_SCH_REC_SIZE;
    if ((sthr==0)&&(stmn==0)&&(edhr==0)&&(edmn==0)||(dumn==0)) {
      atmem.write(addr+LC_VALID,0xff);
    } else {
      atmem.write(addr+LC_STHR,sthr);
      atmem.write(addr+LC_STMN,stmn);
      atmem.write(addr+LC_EDHR,edhr);
      atmem.write(addr+LC_EDMN,edmn);
      atmem.write(addr+LC_INMN,inmn);
      atmem.write(addr+LC_DUMN ,dumn);
      atmem.write(addr+LC_RLY_L,rly[0]);
      atmem.write(addr+LC_RLY_H,rly[1]);
    }
    sprintf(eebuf,"EEPROM %04XH %02X%02X,%02X%02X,%02X%02X,%02X%02X",
            id,sthr,stmn,edhr,edmn,inmn,dumn,rly[0],rly[1]);
    debugSerialOut(cmode,cmenu,eebuf);
  }
  delay(100);
  ic = lcdd.IntRead(cposp,0,1,2);
  if (ic!=io) {
    getSCHData(cposp,ic);
    lcdd.PageWrite(cposp);
  }
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

void getSCHData(int p,int id) {
  int x,y,z;
  unsigned int addr;
  byte sthr,stmn,edhr,edmn,inmn,dumn,rly[2],rlyb[2];
  char lcdbuf[21];
  addr = LC_SCH_START+id*LC_SCH_REC_SIZE;
  sthr = atmem.read(addr+LC_STHR);
  stmn = atmem.read(addr+LC_STMN);
  edhr = atmem.read(addr+LC_EDHR);
  edmn = atmem.read(addr+LC_EDMN);
  inmn = atmem.read(addr+LC_INMN);
  dumn = atmem.read(addr+LC_DUMN);
  rly[0] = atmem.read(addr+LC_RLY_L);
  rly[1] = atmem.read(addr+LC_RLY_H);
  if ((sthr==0xff)||((sthr==0)&&(stmn==0)&&(edhr==0)&&(edmn==0))) {
    sthr = 0;
    stmn = 0;
    edhr = 0;
    edmn = 0;
    inmn = 0;
    dumn = 0;
    rly[0] = 0;
    rly[1] = 0;
  }
  sprintf(lcdbuf,"%02d %02d:%02d %02d:%02d %02d-%02d",
	  id,sthr,stmn,edhr,edmn,inmn,dumn);
  lcdd.setLine(p,1,lcdbuf);
  lcdd.setLine(p,2,"RLY:00000000  OK=ENT");
  x = 4;
  for(z=0;z<2;z++) {
    for(y=6;y>=0;y-=2) {
      if ((rly[z]>>y)&0x1) {
	lcdd.setWriteChar(p,x,2,'1');
      } else {
	lcdd.setWriteChar(p,x,2,'0');
      }
      x++;
    }
  }
}
