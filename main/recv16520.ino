void UECSupdate16520port(void) {
	extern char uecsbuf[];
	extern bool xmldecode(char *);
	extern st_UECSXML *ptr_uecsxmldata;
	extern EthernetUDP UDP16520;
	//void match_rro(int);
	int packetSize ,i;
  
	packetSize = UDP16520.parsePacket();
	if (packetSize>0) {
		UDP16520.read(uecsbuf,LEN_UECSXML_BUFFER-1);
		uecsbuf[packetSize] = NULL;
		if (xmldecode(&uecsbuf[0])) {
		if (ptr_uecsxmldata->element==ELE_IP) {
			ptr_uecsxmldata->ip = UDP16520.remoteIP();
		}
		} else {
			Serial.println("ERR YXML");
		}
		float rfval = float(ptr_uecsxmldata->fval);
//		sprintf(lbf,"%d,%d,%d,%s,",ptr_uecsxmldata->room,ptr_uecsxmldata->region,ptr_uecsxmldata->order,
//					ptr_uecsxmldata->type);
//		Serial.print(lbf);
//		Serial.println(ptr_uecsxmldata->fval);
//		for (i=0;i<CCM_TBL_CNT_CMP;i++) {
			wdt_reset();
			//match_rro(i);
//		}
	}
}

void copyRXdata2flb_cmpope(st_UECSXML *p) {
    extern uecsM304cmpope flb_cmpope[];

    for(int i=0;i<CCM_TBL_CNT_CMP;i++) {
        if (flb_cmpope[i].valid != 0xff) {

        }
    }
}
void comparison_exp(int id,float rfval) {
    int i,x,y,r,j,k,cmpresult;
    byte  cmbcmp;
    float rval; // rval: 比較数値(flb_rx_ccm.cmpval)
    bool combinationCompare(byte,int);

    extern byte cmpope_result[];
    extern uecsM304Sched flb_rx_ccm[];
    extern uecsM304Send flb_tx_ccm[];
    extern uecsM304cmpope flb_cmpope[];

    cmpresult = 0;
    wdt_reset();
    if (flb_rx_ccm[id].valid != 0xff) {
        for (i = 0; i < 5; i++) {
            x = 0;                          // cmpresultを生成するために必要な中間結果
            y = flb_rx_ccm[id].cmpccmid[i]; // 比較するuecsM304cmpopeの位置を決定
            if ((i==0)&&(y==0xff)) {
                i = 100;
            }
            if (y < 0xff) {                  // 比較するCCMTYPEがある場合
                rval = flb_rx_ccm[id].cmpval[i]; // value index;
                cmbcmp = flb_rx_ccm[id].cmbcmp[i];
                switch (flb_rx_ccm[id].cmpope[i]) {
                case R_EQ: // ==
                    if (flb_cmpope[y].fval == rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                case R_GT: // >
                    if (flb_cmpope[y].fval > rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                case R_LT: // >
                    if (flb_cmpope[y].fval < rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                case R_GE: // >=
                    if (flb_cmpope[y].fval >= rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                case R_LE: // <=
                    if (flb_cmpope[y].fval <= rval) {
                        x = combinationCompare(cmbcmp,1);
                    } else {
                        x = combinationCompare(cmbcmp,0);
                    }
                    break;
                default:
                    i = 5; // 比較演算子が無効の場合、離脱 force exit
                    break;
                }
               // x &= rt;
            } else {
               // x = rt; // y==0xff 比較するCCMTYPEがない場合
            }
        }
    }
}

bool combinationCompare(byte c,int x) {
  switch(c) {
  case R_OR:
    x |= 1;
    break;
  case R_AND:
    x &= 1;
    break;
  default:
    x = 0;
  }
  return(x);
}
