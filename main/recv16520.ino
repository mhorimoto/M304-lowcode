void UECSupdate16520port(void) {
  extern char uecsbuf[];
  extern bool xmldecode(char *);
  extern st_UECSXML *ptr_uecsxmldata;
  
  int packetSize = UDP16520.parsePacket();
  if (packetSize>10) {
    UDP16520.read(uecsbuf,600-1);
    uecsbuf[packetSize] = NULL;
      Serial.print("UDP16520 size=");
      Serial.println(packetSize);
      Serial.print("UDP16520 IP=");
      Serial.println(UDP16520.remoteIP()) ;
      Serial.print("REMOTE PORT=");
      Serial.println(UDP16520.remotePort());
      Serial.print("TEXT=");
      Serial.println(uecsbuf);
  }
}
