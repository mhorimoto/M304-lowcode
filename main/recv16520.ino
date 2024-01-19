void UECSupdate16520port(void) {
  extern char uecsbuf[];
  extern bool xmldecode(char *);
  extern st_UECSXML *ptr_uecsxmldata;
  extern EthernetUDP UDP16520;
  void match_rro(int);
  int packetSize ,i;
  
  packetSize = UDP16520.parsePacket();
  // Ver2.4.aDbg-06  packetSize>0
  if (packetSize>0) {
    UDP16520.read(uecsbuf,LEN_UECSXML_BUFFER-1);
    uecsbuf[packetSize] = NULL;
    // Serial.print("UDP16520 size=");  //2.3.5D
    // Serial.println(packetSize);
    // Serial.print("UDP16520 IP=");
    // Serial.println(UDP16520.remoteIP()) ;
    // Serial.print("REMOTE PORT=");
    // Serial.println(UDP16520.remotePort());
    // Serial.print("TEXT=");
    // Serial.println(uecsbuf);
    if (xmldecode(&uecsbuf[0])) {
      if (ptr_uecsxmldata->element==ELE_IP) {
        ptr_uecsxmldata->ip = UDP16520.remoteIP();
      }
      // //      Serial.print("Ver="); 2.3.5D
      //      Serial.println(ptr_uecsxmldata->ver);
      // //      Serial.print("Type=");
      Serial.print(now());
      Serial.print("  ");
      Serial.println(ptr_uecsxmldata->type);
      // //      Serial.print("TEXTVAL=");
      //      Serial.println(ptr_uecsxmldata->textval);
      // //      Serial.print("Page=");
      //      Serial.println(ptr_uecsxmldata->page);
      // //      Serial.print("ROOM=");
      //      Serial.println(ptr_uecsxmldata->room);
      // //      Serial.print("  REGION=");
      //      Serial.println(ptr_uecsxmldata->region);
      // //      Serial.print("  ORDER=");
      //      Serial.println(ptr_uecsxmldata->order);
      // //      Serial.print("  PRIORITY=");
      //      Serial.println(ptr_uecsxmldata->priority);
      // //      Serial.print("FVAL=");
      //      Serial.println(ptr_uecsxmldata->fval);
      // //      Serial.print("IP=");
      //      Serial.println(ptr_uecsxmldata->ip);
    } else {
      Serial.println("ERR YXML");
    }
    float rfval = float(ptr_uecsxmldata->fval);
    //    Serial.println(rfval);
    // Ver2.4.aDbg-04 removed
    for (i=0;i<CCM_TBL_CNT_CMP;i++) {
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
  
  float rfval;
  // 2.3.7DBG
  if (flb_cmpope[id].valid!=0xff) {
    sprintf(lbf,"E match_rro(%d) valid=%d",id,flb_cmpope[id].valid);
    Serial.println(lbf);
    if (flb_cmpope[id].valid==1) {
      // ROOM
      if ((ptr_uecsxmldata->room==0)||(ptr_uecsxmldata->room==flb_cmpope[id].room)) {
	Serial.println(F("MATCH ROOM"));
	// REGION
	if ((ptr_uecsxmldata->region==0)||(ptr_uecsxmldata->region==flb_cmpope[id].region)) {
	  Serial.println(F("MATCH REGION"));
	  // ORDER
	  if ((ptr_uecsxmldata->order==0)||(ptr_uecsxmldata->order==flb_cmpope[id].order)) {
	    Serial.println(F("MATCH ORDER"));
	    // CCM type
	    if (!strncmp(ptr_uecsxmldata->type,flb_cmpope[id].ccm_type,19)) {
	      Serial.println(F("MATCH CCM"));
	      rfval = float(ptr_uecsxmldata->fval);
	      Serial.println(rfval);
	      cmpope_result[id] = 0;   // Preset false
	      switch(flb_cmpope[id].cmpope) {
	      case R_EQ: // ==
		if (rfval==flb_cmpope[id].fval) cmpope_result[id] = 1;
		break;
	      case R_GT: // >
		if (rfval>flb_cmpope[id].fval) cmpope_result[id] = 1;
		break;
	      case R_LT: // <
		if (rfval<flb_cmpope[id].fval) cmpope_result[id] = 1;
		break;
	      case R_GE: // >=
		if (rfval>=flb_cmpope[id].fval) cmpope_result[id] = 1;
		break;
	      case R_LE: // <=
		if (rfval<=flb_cmpope[id].fval) cmpope_result[id] = 1;
		break;
	      }
	    }
	  }
	}
      }
      sprintf(lbf,"D %d",cmpope_result[id]);
      Serial.println(lbf);
    } else {
      //    Serial.println(F("NO MATCH "));
      ;    
    }
  }
}
