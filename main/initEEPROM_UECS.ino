// void initEEPROM_UECS(void) {
//   int w,a,j,k;
//   w = 9;      // cnd + RLY1..8
//   bool enable;
//   byte room,region,priority;
//   int  order;
//   char ccm_type[20];

//   enable = true;
//   room   = 1;
//   region = 1;
//   order  = 1;
//   priority = 15;
//   for (k=0;k<w;k++) {
//     for (j=0;j<20;j++) {
//       ccm_type[j] = 0;
//     }
//     if (k==0) {
//       strcpy(ccm_type,"cnd.aMC");
//     } else if (k==6) {
//       sprintf(ccm_type,"AirHumFogopr.%d",k);
//     } else {
//       sprintf(ccm_type,"Irriopr.%d",k);
//     }
//     if (k==0) {              // CCMTABLE
//       a = LC_SCH_START;
//     } else {
//       a += LC_SCH_REC_SIZE;
//     }
//     atmem.write(a+LC_VALID,enable);
//     atmem.write(a+LC_ROOM,room);
//     atmem.write(a+LC_REGION,region);
//     atmem.write(a+LC_ORDER,(order&0xff));
//     atmem.write(a+LC_ORDER+1,(order>>8)&0xff);
//     atmem.write(a+LC_PRIORITY,priority);
//     atmem.write(a+LC_LV,LV_A1M0);
//     atmem.write(a+LC_CAST,0);
//     atmem.write(a+LC_SR,'R');
//     for (j=0;j<20;j++) {
//       atmem.write(a+LC_CCMTYPE+j,ccm_type[j]);
//     }
//     for (j=0;j<10;j++) {
//       atmem.write(a+LC_UNIT+j,0);
//     }
//     atmem.write(a+LC_STHR,0);
//     atmem.write(a+LC_STMN,0);
//     atmem.write(a+LC_EDHR,0);
//     atmem.write(a+LC_EDMN,0);
//     atmem.write(a+LC_INMN,0);
//     atmem.write(a+LC_DUMN,0);
//     atmem.write(a+LC_RLY_L,0);
//     atmem.write(a+LC_RLY_H,0);
//   }
// }

