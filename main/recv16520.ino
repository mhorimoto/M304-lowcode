void UECSupdate16520port(void) {
	extern char uecsbuf[];
	extern bool xmldecode(char *);
	extern st_UECSXML *ptr_uecsxmldata;
	extern EthernetUDP UDP16520;
	void match_rro(int);
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
		for (i=0;i<CCM_TBL_CNT_CMP;i++) {
			wdt_reset();
			match_rro(i);
		}
	}
}

//
//  indexで指定されたflb_cmpopeと受信したuecsxmldataから
//  ROOM/REGION/ORDER/CCMTypeが一致しているか検査する関数
//  結果はflb_cmpope[].resultに代入する
//  2.3.7D
//
void match_rro(int id) {
  extern uecsM304cmpope flb_cmpope[];
  extern st_UECSXML *ptr_uecsxmldata;
  extern byte cmpope_result[];
  extern char lbf[];
  extern void sendUECSpacket(int ,char *,int);
  int  i;
  char ft[6];
  float rfval;
  // 2.3.7DBG
  sprintf(lbf,"%d,%d,%d,%s,%d",ptr_uecsxmldata->room,ptr_uecsxmldata->region,ptr_uecsxmldata->order,
  			ptr_uecsxmldata->type,ptr_uecsxmldata->fval);
  Serial.println(lbf);
  if (flb_cmpope[id].valid!=0xff) {
    if (flb_cmpope[id].valid==1) {
      // ROOM
      if ((ptr_uecsxmldata->room==0)||(ptr_uecsxmldata->room==flb_cmpope[id].room)) {
	// REGION
	if ((ptr_uecsxmldata->region==0)||(ptr_uecsxmldata->region==flb_cmpope[id].region)) {
	  // ORDER
	  if ((ptr_uecsxmldata->order==0)||(ptr_uecsxmldata->order==flb_cmpope[id].order)) {
	    // CCM type
	    if (!strncmp(ptr_uecsxmldata->type,flb_cmpope[id].ccm_type,19)) {
	      rfval = float(ptr_uecsxmldata->fval);
              cmpope_lifecnt[id] = flb_cmpope[id].lifecnt;
	      switch(flb_cmpope[id].cmpope) {
	      case R_EQ: // ==
		if (rfval==flb_cmpope[id].fval) {
		  cmpope_result[id] = 1;
		} else {
		  cmpope_result[id] = 0;
		}
		break;
	      case R_GT: // >
		if (rfval>flb_cmpope[id].fval) {
		  cmpope_result[id] = 1;
		} else {
		  cmpope_result[id] = 0;
		}
		break;
	      case R_LT: // <
		if (rfval<flb_cmpope[id].fval) {
		  cmpope_result[id] = 1;
		} else {
		  cmpope_result[id] = 0;
		}
		break;                                                                     
	      case R_GE: // >=
		if (rfval>=flb_cmpope[id].fval) {
		  cmpope_result[id] = 1;
		} else {
		  cmpope_result[id] = 0;
		}
		break;
	      case R_LE: // <=
		if (rfval<=flb_cmpope[id].fval) {
		  cmpope_result[id] = 1;
		} else {
		  cmpope_result[id] = 0;
		}
		break;
	      }
	    }
	  }
	}
      }
    } else {
      //    Serial.println(F("NO MATCH "));
    }
  }
}
