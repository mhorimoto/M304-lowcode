#define HTTPBUFSIZ  512 // 2.3.6D

void opeHttpd(EthernetClient ec) {
  boolean currentLineIsBlank = true;
  char c,d[5],htbuf[HTTPBUFSIZ],*p_htbuf,prnbuf[BUFSIZ];  // 2.3.6D
  int  mode; // 0:ignore, 1:Store to EEPROM, 2:Fetch from EEPROM, 3:END ope
  int  bufcnt, i;
  int  dlen,daddr,dtype,chksum;
  byte dbyte,flag2;
  
  mode = MD_HT_IGNORE;
  bufcnt = 0;
  chksum = 0;
  flag2  = 0;
  p_htbuf = &htbuf[0];
  wdt_reset();
  for (i=0;i<5;i++) d[i] = (char)0;
  while(ec.connected()) {
    if (ec.available()) {
      c = ec.read();
      if (mode==MD_HT_IGNORE) {
        switch(c) {
        case ':':
          mode = MD_HT_STORE;
          continue;
        case 'L':
          mode = MD_HT_FETCH;
          continue;
        }
      }
      /* STORE ROUTINE */
      if (mode==MD_HT_STORE) {
        htbuf[bufcnt] = c;
        bufcnt++;
        if ( bufcnt > HTTPBUFSIZ ) {
          ec.stop();
          Serial.println(F("Bov1")); // 2.3.5D
          return;
        }
        htbuf[bufcnt] = (char)NULL;
      
        if ( c=='\n' && currentLineIsBlank ) {
	  sendHTTPheader(ec); // 2.3.5D
          // ec.println("HTTP/1.1 200 OK");
          // ec.println("Content-Type: text/html");
          // ec.println("Connection: close");
          // ec.println();
          // ec.println("<!DOCTYPE HTML>");
          // ec.println("<html>");

          ec.print("<h1>MOD</h1>");
          ec.print("<pre>htbuf=");
          ec.println(htbuf);
          strncpy(d,htbuf,2);
          dlen = strtol(d,NULL,16);
          chksum = dlen;
          strncpy(d,&htbuf[2],4);
          d[4]=(char)NULL;
          daddr = strtol(d,NULL,16);

          sprintf(prnbuf,"dlen=%d  chksum=0x%02X  addr=0x%04X",dlen,chksum,daddr);
          ec.println(prnbuf);
          
          strncpy(d,&htbuf[2],2);
          d[2]=(char)NULL;
          chksum += ((strtol(d,NULL,16))&0xff);
          strncpy(d,&htbuf[4],2);
          chksum += ((strtol(d,NULL,16))&0xff);
          strncpy(d,&htbuf[6],2);
          d[2]=(char)NULL;
          dtype = strtol(d,NULL,16);
          chksum += dtype;
          chksum &= 0xff;
          sprintf(prnbuf,"dty=%d  cs=0x%02X  %d  leng=%d  adr=%04X\n",
                  dtype,chksum,strlen(p_htbuf),dlen,daddr);
          ec.print(prnbuf);
          
          for (i=0;i<dlen;i++) {
            strncpy(d,&htbuf[(i*2)+8],2);
            d[2]=(char)NULL;
            dbyte = strtol(d,NULL,16);
            chksum += dbyte;
            chksum &= 0xff;
            if (mode==MD_HT_STORE) {
              mod_EEPROM(daddr+i,dbyte,ec);
            }
          }
          strncpy(d,&htbuf[(i*2)+8],2);
          d[2]=(char)NULL;
          dbyte = strtol(d,NULL,16) ; //>> 8;
          chksum += dbyte;
          htbuf[(i*2)+10] = (char)0;
          ec.print(p_htbuf);
          ec.println("</pre></html>");
          break;
        }
      /* FETCH ROUTINE */
      } else if (mode==MD_HT_FETCH) {
	if (c==0x20) {
	  htbuf[bufcnt] = (char)NULL;
	  flag2 = 1;
	}
	if (flag2==0) {
	  htbuf[bufcnt] = c;
	  bufcnt++;
	}
        if ( bufcnt > HTTPBUFSIZ ) {
          ec.stop();
          Serial.print(bufcnt);
          Serial.println(F(" Bov2")); // 2.3.5D
          return;
        }
        htbuf[bufcnt] = (char)NULL;
        if ( c=='\n' && currentLineIsBlank ) {
	  sendHTTPheader(ec); // 2.3.5D
	  // ec.println("HTTP/1.1 200 OK");
          // ec.println("Content-Type: text/html");
          // ec.println("Connection: close");
          // ec.println();
          // ec.println("<!DOCTYPE HTML>");
          // ec.println("<html>");

          ec.print(F("<h1>FETCH</h1>"));
          ec.print(F("<pre>htbuf="));
          ec.println(htbuf);
          strncpy(d,&htbuf[2],4);
          d[4]=(char)NULL;
          daddr = strtol(d,NULL,16);
          fetch_EEPROM(daddr,ec);
          ec.println(F("</html>"));
          break;
          //          delay(500);
          //          wdt_reset();
          //          ec.stop();
          //          Serial.println("MF9");
          //	  return;
        }
      }
    }
    Serial.print(F("MF6 "));
    Serial.println(bufcnt);
    if (c == '\n') {
      currentLineIsBlank = true;
    } else if (c != '\r') {
      currentLineIsBlank = false;
    }
    Serial.println(F("MF7"));
    wdt_reset();
  }
  delay(500);
  wdt_reset();
  ec.stop();
  Serial.println(F("MF8"));
  //  Serial.println(F("Clinet disconnected")); // 2.3.6D
}
// 2.3.6D
void sendHTTPheader(EthernetClient ec) {
  ec.println(F("HTTP/1.1 200 OK"));
  ec.println(F("Content-Type: text/html"));
  ec.println(F("Connection: close"));
  ec.println();
  ec.println(F("<!DOCTYPE HTML>"));
  ec.println(F("<html>"));
}

void mod_EEPROM(unsigned int addr,byte dt,EthernetClient ec) {
  byte v;
  v = atmem.read(addr);
  if (v==dt) {
    return;
  }
  atmem.write(addr,dt);
  // 2.3.5D
  // ec.print("mE:addr=");
  // ec.print(addr,HEX);
  // ec.print(" data=");
  // ec.print(dt,HEX);
  // ec.println("  Writen");
  return;
}

/*
   1page fetch 1page=256bytes
 */
void fetch_EEPROM(unsigned int addr,EthernetClient ec) {
  int x,y,a;
  uint8_t d;
  char prnbuf[BUFSIZ];
  addr &= 0xff00;
  ec.println(F("<pre>"));
  for (y=0;y<16;y++) {
    sprintf(prnbuf,"0x%04X:",addr+(y*0x10));
    for (x=0;x<16;x++) {
      a = addr + (y*16) + x;
      d = atmem.read(a);
      sprintf(prnbuf,"%s %02X",prnbuf,d);
    }
    ec.println(prnbuf);
  }
  ec.println(F("</pre>"));
}
