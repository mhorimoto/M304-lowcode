#define HTTPBUFSIZ  128

void opeHttpd(EthernetClient ec) {
  boolean currentLineIsBlank = true;
  char c,d[5],htbuf[HTTPBUFSIZ],*p_htbuf;
  int  mode; // 0:ignore, 1:Store to EEPROM, 2:Fetch from EEPROM, 3:END ope
  int  bufcnt, i;
  int  dlen,daddr,dtype,chksum;
  byte dbyte;
  
  mode = 0;
  bufcnt = 0;
  chksum = 0;
  p_htbuf = &htbuf[0];
  for (i=0;i<5;i++) d[i] = (char)0;
  while(ec.connected()) {
    if (ec.available()) {
      c = ec.read();
      if (c==':') {
        mode = 1;
        continue;
      }
      if (mode==1) {
        htbuf[bufcnt] = c;
        bufcnt++;
        if ( bufcnt > HTTPBUFSIZ ) {
          ec.stop();
          Serial.println("Buffer overflow");
          return;
        }
        htbuf[bufcnt] = (char)NULL;
      }
      if ( c=='\n' && currentLineIsBlank ) {
        ec.println("HTTP/1.1 200 OK");
        ec.println("Content-Type: text/html");
        ec.println("Connection: close");
        ec.println();
        ec.println("<!DOCTYPE HTML>");
        ec.println("<html>");
        // output the value of each analog input pin
        ec.print("<h1>M304</h1>");
        ec.print("<p> is here.</p>htbuf=");
        ec.println(htbuf);
        strncpy(d,htbuf,2);
        dlen = strtol(d,NULL,16);
        chksum = dlen;
        //        ec.print("dlen=");
        //        ec.print(dlen);
        //        ec.print("  chksum=");
        //        ec.println(chksum,HEX);
          
        strncpy(d,&htbuf[2],4);
        daddr = strtol(d,NULL,16);

        strncpy(d,&htbuf[2],2);
        chksum += ((strtol(d,NULL,16)>>8)&0xff);
        //        ec.print("addrh  chksum=");
        //        ec.println(chksum,HEX);
        strncpy(d,&htbuf[4],2);
        chksum += ((strtol(d,NULL,16)>>8)&0xff);
        //        ec.print("addrL  chksum=");
        //        ec.println(chksum,HEX);
        
        strncpy(d,&htbuf[6],2);
        dtype = strtol(d,NULL,16);
        chksum += dtype;
        chksum &= 0xff;
        ec.print("<p>");
        ec.print(strlen(p_htbuf));
        ec.print(" chars length=");
        ec.print(dlen);
        ec.print("bytes address=");
        ec.println(daddr);
        for (i=0;i<dlen;i++) {
          strncpy(d,&htbuf[(i*2)+8],2);
          dbyte = strtol(d,NULL,16) >> 8;
          chksum += dbyte;
          ec.print(dbyte,HEX);
          ec.print("  chksum=");
          ec.println(chksum,HEX);
          if (mode==1) {
            mod_EEPROM(daddr+i,dbyte,ec);
          }
        }
        strncpy(d,&htbuf[(i*2)+8],2);
        dbyte = strtol(d,NULL,16) >> 8;
        chksum += dbyte;
        ec.println(dbyte,HEX);
        ec.print("chksum=");
        ec.println(chksum,HEX);
        ec.print(p_htbuf);
        ec.println("</p>");
        ec.println("</html>");
        break;
      }
    }
    if (c == '\n') {
      currentLineIsBlank = true;
    } else if (c != '\r') {
      currentLineIsBlank = false;
    }
  }
  delay(1);
  ec.stop();
  Serial.println("Clinet disconnected");
}

void mod_EEPROM(unsigned int addr,byte dt,EthernetClient ec) {
  byte v;
  v = atmem.read(addr);
  if (v==dt) {
    return;
  }
  atmem.write(addr,dt);
  ec.print("mE:addr=");
  ec.print(addr,HEX);
  ec.print(" data=");
  ec.print(dt,HEX);
  ec.println("  Writen");
  return;
}
