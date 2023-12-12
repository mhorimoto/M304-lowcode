void UECSupdate16520port(void) {
  extern char uecsbuf[];
  extern bool xmldecode(char *);
  extern st_UECSXML *ptr_uecsxmldata;
  
  int packetSize = UDP16520.parsePacket();
  if (packetSize>10) {
    UDP16520.read(uecsbuf,600-1);
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
  }
}
