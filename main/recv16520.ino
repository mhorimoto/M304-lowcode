void UECSupdate16520port(void) {
  extern char uecsbuf[];
  extern bool xmldecode(char *);
  extern st_UECSXML *ptr_uecsxmldata;

  int packetSize = UDP16520.parsePacket(),i;
  
  if (packetSize>10) {
    UDP16520.read(uecsbuf,LEN_UECSXML_BUFFER-1);
    uecsbuf[packetSize] = NULL;
      // Serial.print("UDP16520 size=");  2.3.5D
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
      //      Serial.print("Ver="); 2.3.5D
      Serial.println(ptr_uecsxmldata->ver);
      //      Serial.print("Type=");
      Serial.println(ptr_uecsxmldata->type);
      //      Serial.print("TEXTVAL=");
      Serial.println(ptr_uecsxmldata->textval);
      //      Serial.print("Page=");
      Serial.println(ptr_uecsxmldata->page);
      //      Serial.print("ROOM=");
      Serial.print(ptr_uecsxmldata->room);
      //      Serial.print("  REGION=");
      Serial.print(ptr_uecsxmldata->region);
      //      Serial.print("  ORDER=");
      Serial.print(ptr_uecsxmldata->order);
      //      Serial.print("  PRIORITY=");
      Serial.println(ptr_uecsxmldata->priority);
      //      Serial.print("FVAL=");
      Serial.println(ptr_uecsxmldata->fval);
      //      Serial.print("IP=");
      Serial.println(ptr_uecsxmldata->ip);
    } else {
      Serial.println("ERR YXML");
    }
    float rfval = float(ptr_uecsxmldata->fval);
    Serial.println(rfval);
    for (i=0;i<CCM_TBL_CNT_CMP;i++) {
      match_rro(i); // 2.3.7D
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
  extern char lbf[];
  float rfval;

  sprintf(lbf,"E match_rro(%d)",id);
  Serial.println(lbf);
  // ROOM
  if ((ptr_uecsxmldata->room==0)||(ptr_uecsxmldata->room==flb_cmpope[id].room)) {
    // REGION
    if ((ptr_uecsxmldata->region==0)||(ptr_uecsxmldata->region==flb_cmpope[id].region)) {
      // ORDER
      if ((ptr_uecsxmldata->order==0)||(ptr_uecsxmldata->order==flb_cmpope[id].order)) {
	// CCM type
	if (!strcmp(ptr_uecsxmldata->type,flb_cmpope[id].ccm_type)) {
	  rfval = float(ptr_uecsxmldata->fval);
	  Serial.println(rfval);
	  flb_cmpope[id].result = 0;   // Preset false
	  switch(flb_cmpope[id].cmpope) {
	  case R_EQ: // ==
	    if (rfval==flb_cmpope[id].fval) flb_cmpope[id].result = 1;
	    break;
	  case R_GT: // >
	    if (rfval>flb_cmpope[id].fval) flb_cmpope[id].result = 1;
	    break;
	  case R_LT: // <
	    if (rfval<flb_cmpope[id].fval) flb_cmpope[id].result = 1;
	    break;
	  case R_GE: // >=
	    if (rfval>=flb_cmpope[id].fval) flb_cmpope[id].result = 1;
	    break;
	  case R_LE: // <=
	    if (rfval<=flb_cmpope[id].fval) flb_cmpope[id].result = 1;
	    break;
	  }
	}
      }
    }
  }
  sprintf(lbf,"D %d",flb_cmpope[id].result);
  Serial.println(lbf);
}
