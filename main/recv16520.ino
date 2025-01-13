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
        wdt_reset();
	}
}

#include <string.h>
void copyRXdata2flb_cmpope(void) {
	extern st_UECSXML *ptr_uecsxmldata;
    extern uecsM304cmpope flb_cmpope[];
    // CCM_TBL_CNT_CMPの分だけ見る
    for(int i=0;i<CCM_TBL_CNT_CMP;i++) {
        wdt_reset();
        if (flb_cmpope[i].valid != 0xff) {
            if (!strncmp(flb_cmpope[i].ccm_type,ptr_uecsxmldata->type,20)) {   // CCMTYPEが合致したら
                if ((ptr_uecsxmldata->room==0)||(ptr_uecsxmldata->room==flb_cmpope[i].room)) {  // ROOMが合致したら
                    if ((ptr_uecsxmldata->region==0)||(ptr_uecsxmldata->region==flb_cmpope[i].region)) {  // REGIONが合致したら
                        if ((ptr_uecsxmldata->order==0)||(ptr_uecsxmldata->order==flb_cmpope[i].order)) {  // ORDERが合致したら
                            if (ptr_uecsxmldata->priority<=flb_cmpope[i].priority) {  // 優先順位が同じか高かったら
                                flb_cmpope[i].fval = float(ptr_uecsxmldata->fval);    // 受信したデータをflb_cmpope.fvalに記録する
                                flb_cmpope[i].priority = ptr_uecsxmldata->priority;   // 受信した優先順位を記録する
                                flb_cmpope[i].remain = flb_cmpope[i].lifecnt;         // flb_cmpopeにある生存時間をremainにコピーする
                            }
                        }
                    }
                }
            }
        }
    }
}

