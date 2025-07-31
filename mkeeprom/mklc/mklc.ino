#include <M304.h>
#if _M304_H_V < 1311
#pragma message("Library M304 is old. Version 1.3.11 or higher is required.")
#else

char *pgname = "M304 makelc Ver1.10 ";
char inputbuf[BUFSIZ],*ptr_inputbuf;
int  cnt;

LCDd lcdd(RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7);

void setup(void) {
    String linebuf[4],inputbuf;
    
    Serial.begin(115200);
    lcdd.begin(20,4);
    lcdd.clear();
    linebuf[1] = String(pgname);
    lcdd.print(linebuf[1]);
    lcdd.setCursor(0,1);
    lcdd.print(F("Open SERIAL"));
    lcdd.setCursor(0,2);
    lcdd.print(F("115200bps"));
    Serial.println(pgname);
    ptr_inputbuf = &inputbuf[0];
    cnt = 0;
}


void loop(void) {
    char ch,sc[3];
    byte r;
    int  sl,i;
    static bool cr = true;
    extern void listCommand(void);
    extern void setCommand(char *,int,int);
    extern void clearpage(char *);
    extern void help(void);
    
    if ((cnt==0) && cr) {
        Serial.print(F("$ "));
        cr = false;
    }
    if (Serial.available() > 0) {
        ch = Serial.read();
        if ((ch=='\r')||(ch=='\n')) {
            cr = true;
            Serial.println();
        } else if ((ch==(char)0x08)||(ch==(char)0x7f)) {
            if (cnt>0) {
                cnt--;
                Serial.print(ch);
                Serial.print(" ");
                Serial.print(ch);
            }
        } else if ((ch>=(char)0x20)&&(ch<(char)0x7f)) {
            Serial.print(ch);
            inputbuf[cnt] = ch;
            cnt++;
        }
        inputbuf[cnt] = (char)NULL;
        if (cnt>=BUFSIZ) {
            Serial.println(F("OV"));
            cnt=0;
        }
    }
    
    // Command operation
    if (cr) {
        ptr_inputbuf = &inputbuf[0];
        if ( !strncmp(ptr_inputbuf,"list",BUFSIZ-1) ) {
            Serial.println(F("LIST COMMAND"));
            listCommand();
        }
        if ( !strncmp(ptr_inputbuf,"setid ",6) ) {
            ptr_inputbuf += 6;
            setCommand(ptr_inputbuf,12,LC_UECS_ID);
        }
        if ( !strncmp(ptr_inputbuf,"setmac ",7) ) {
            ptr_inputbuf += 7;
            setCommand(ptr_inputbuf,12,LC_MAC);
        }
        if ( !strncmp(ptr_inputbuf,"setdhcp ",8) ) {
            ptr_inputbuf += 8;
            setCommand(ptr_inputbuf,1,FIX_DHCP_FLAG);
        }
        if ( !strncmp(ptr_inputbuf,"setip ",6) ) {
            ptr_inputbuf += 6;
            setCommand(ptr_inputbuf,15,FIXED_IPADDRESS);
        }
        if ( !strncmp(ptr_inputbuf,"setmask ",8) ) {
            ptr_inputbuf += 8;
            setCommand(ptr_inputbuf,15,FIXED_NETMASK);
        }
        if ( !strncmp(ptr_inputbuf,"setgw ",6) ) {
            ptr_inputbuf += 6;
            setCommand(ptr_inputbuf,15,FIXED_DEFGW);
        }
        if ( !strncmp(ptr_inputbuf,"setdns ",7) ) {
            ptr_inputbuf += 7;
            setCommand(ptr_inputbuf,15,FIXED_DNS);
        }
        if ( !strncmp(ptr_inputbuf,"setvender ",10) ) {
            ptr_inputbuf += 10;
            setCommand(ptr_inputbuf,16,VENDER_NAME);
        }
        if ( !strncmp(ptr_inputbuf,"setnodename ",12) ) {
            ptr_inputbuf += 12;
            setCommand(ptr_inputbuf,16,NODE_NAME);
        }
        if ( !strncmp(ptr_inputbuf,"setdbgmsg ",10) ) {
            ptr_inputbuf += 12;
            setCommand(ptr_inputbuf,16,LC_DBGMSG);
        }
        if ( !strncmp(ptr_inputbuf,"clearpage ",10) ) {
            ptr_inputbuf += 10;
            clearpage(ptr_inputbuf);
        }
        if ( !strncmp(ptr_inputbuf,"help",4)) {
            help();
        }
        Serial.println(F("OK"));
        cnt = 0;
    }
}
#endif
