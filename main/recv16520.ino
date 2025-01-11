void UECSupdate16520port(void) {
	extern char uecsbuf[];
    extern void copyRXdata2flb_cmpope(void);
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
        // 受信したデータをflb_cmpope構造体にコピーする。
        // コピーするには条件がある。それは、copyRXdata2flb_cmpop()内にある。 2025/01/03
        copyRXdata2flb_cmpope();
//		float rfval = float(ptr_uecsxmldata->fval);
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

#include <string.h>
void copyRXdata2flb_cmpope(void) {
	extern st_UECSXML *ptr_uecsxmldata;
    extern uecsM304cmpope flb_cmpope[];
    // 受信デバッグ中のメッセージ
    // ID,ROOM,REGION,ORDER,PRIORITY,REMAIN,FVAL
    sprintf(lbf,"RECV:%d,%d,%d,%d,",ptr_uecsxmldata->room,ptr_uecsxmldata->region,ptr_uecsxmldata->order,
                                    ptr_uecsxmldata->priority);
    Serial.print(lbf);
    Serial.println(ptr_uecsxmldata->fval);
    // CCM_TBL_CNT_CMPの分だけ見る
    for(int i=0;i<CCM_TBL_CNT_CMP;i++) {
        wdt_reset();
        if (flb_cmpope[i].valid != 0xff) {
            if (!strncmp(flb_cmpope[i].ccm_type,ptr_uecsxmldata->type,20)) {   // CCMTYPEが合致したら
                Serial.print("M:CCMTYPE>");
                Serial.println(ptr_uecsxmldata->type);
                if ((ptr_uecsxmldata->room==0)||(ptr_uecsxmldata->room==flb_cmpope[i].room)) {  // ROOMが合致したら
                    Serial.print("M:ROOM>");
                    Serial.println(ptr_uecsxmldata->room);
                    if ((ptr_uecsxmldata->region==0)||(ptr_uecsxmldata->region==flb_cmpope[i].region)) {  // REGIONが合致したら
                        Serial.print("M:REGION>");
                        Serial.println(ptr_uecsxmldata->region);
                        Serial.print("E:ORDER>");
                        Serial.print(flb_cmpope[i].order);
                        Serial.print(" / ");
                        Serial.println(ptr_uecsxmldata->order);
                        if ((ptr_uecsxmldata->order==0)||(ptr_uecsxmldata->order==flb_cmpope[i].order)) {  // ORDERが合致したら
                            Serial.print("M:ORDER>");
                            Serial.print(flb_cmpope[i].order);
                            Serial.print(" / ");
                            Serial.println(ptr_uecsxmldata->order);
                            if (ptr_uecsxmldata->priority<=flb_cmpope[i].priority) {  // 優先順位が同じか高かったら
                                Serial.print("M:PRIORITY>");
                                Serial.print(flb_cmpope[i].priority);
                                Serial.print(" / ");
                                Serial.println(ptr_uecsxmldata->priority);
                                flb_cmpope[i].fval = float(ptr_uecsxmldata->fval);    // 受信したデータをflb_cmpope.fvalに記録する
                                flb_cmpope[i].priority = ptr_uecsxmldata->priority;   // 受信した優先順位を記録する
                                flb_cmpope[i].remain = flb_cmpope[i].lifecnt;         // flb_cmpopeにある生存時間をremainにコピーする
                                // デバッグ中のメッセージ
                                // ID,ROOM,REGION,ORDER,PRIORITY,REMAIN,FVAL
                                sprintf(lbf,"%d:%d,%d,%d,%d,%d,",i,flb_cmpope[i].room,flb_cmpope[i].region,flb_cmpope[i].order,
                                            flb_cmpope[i].priority,flb_cmpope[i].remain);
                                Serial.print(lbf);
                                Serial.println(flb_cmpope[i].fval);
                            }
                        }
                    }
                }
            }
        }
    }
}

/* 本当はvoidじゃないので要注意、変更しなければならない。
int comparison_exp(int id,float rfval) {
    int i,x,y,r,j,k,cmpresult;
    byte  cmbcmp;
    float rval; // rval: 比較数値(flb_rx_ccm.cmpval)
    bool combinationCompare(byte,int);

    extern byte cmpope_result[];
    extern uecsM304Sched flb_rx_ccm[];
    extern uecsM304Send flb_tx_ccm[];
    extern uecsM304cmpope flb_cmpope[];

    // cmpresult = 0;  delete D22
    wdt_reset();
    if (flb_rx_ccm[id].valid != 0xff) {
        for (i = 0; i < 5; i++) {
            x = 0;                          // cmpresultを生成するために必要な中間結果
            y = flb_rx_ccm[id].cmpccmid[i]; // 比較するuecsM304cmpopeの位置を決定
            if ((i==0)&&(y==0xff)) {
                i = 100;                     // 最初のcmpccmidが無効ならばあとは評価しない。
            }
            if (y < 0xff) {                  // 比較するCCMTYPEがある場合
                rval = flb_rx_ccm[id].cmpval[i]; // value index;
                // cmbcmp = flb_rx_ccm[id].cmbcmp[i];  // delete D22 直前の比較結果と比較する演算子。最初のi=0のときにはANDなはず。
                switch (flb_rx_ccm[id].cmpope[i]) { // 期待値と比較する演算子を選別する。
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
*/