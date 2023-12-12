#include <M304.h>

void opeRUN(int hr,int mn) {
  static int pmn=61;  // Nothing 61minute
  int id, a,i,j,k;
  byte s[2];
  int r;
  char t[81],buf[8];
  extern int rlyttl[];

  //  debugSerialOut(hr,mn,"Enter opeRUN()");
  if (mn!=pmn) {
    pmn = mn;
    for(id=0;id<100;id++) {
      a = LC_SCH_START+(id*LC_SCH_REC_SIZE);
      if (atmem.read(a)!=0xff) {
	timeDecision(id,hr,mn);
      }
    }
    for (r=1;r<9;r++) {
      sendUECSpacket(r,itoa(rlyttl[r-1],buf,DEC));
    }
  }
  //  debugSerialOut(hr,mn,"Exit opeRUN()");
}

void timeDecision(int id,int curhr,int curmn) {
  byte d,sthr,stmn,edhr,edmn,inmn,dumn,s[2];
  int  i,j,k,addr,sttime,edtime,inmntm,dumntm,startmin,curtim,pmin;
  char t[81];
  extern int rlyttl[];
  
  addr = LC_SCH_START+(id*0x40);
  sthr   = (int)atmem.read(addr+LC_STHR);
  stmn   = (int)atmem.read(addr+LC_STMN);
  if (sthr>24) return ERROR;
  sttime = sthr * 60 + stmn;
  edhr   = (int)atmem.read(addr+LC_EDHR);
  edmn   = (int)atmem.read(addr+LC_EDMN);
  if (edhr>24) return ERROR;
  edtime = edhr * 60 + edmn;
  inmntm = (int)atmem.read(addr+LC_INMN);
  dumntm = (int)atmem.read(addr+LC_DUMN);
  if ((inmntm+dumntm)==0) return; // If either is 0, the process is aborted and returns.
  
  curtim = curhr*60+curmn;
  
  for(startmin=sttime;startmin<edtime;startmin+=(inmntm+dumntm)) {
    if (startmin==curtim) {
      s[0] = atmem.read(addr+LC_RLY_L);
      s[1] = atmem.read(addr+LC_RLY_H);
      for(i=0;i<4;i++) {
	j = (s[0]>>(i*2))&0x3;
	k = (s[1]>>(i*2))&0x3;
	if (j) {
	  rlyttl[3-i] = dumntm*60; // RLY1..4
	}
	if (k) {
	  rlyttl[7-i] = dumntm*60; // RLY5..8
	}
      }
    }
  }
}


