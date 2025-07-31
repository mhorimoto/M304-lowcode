#include <M304.h>

void listCommand(void) {
    int i;
    char lbf[80];
    char sbf[21];
    byte b;

    // UECS ID
    sprintf(lbf,"UECS ID: %02X %02X %02X %02X %02X %02X",
        atmem.read(LC_UECS_ID),atmem.read(LC_UECS_ID+1),atmem.read(LC_UECS_ID+2),
        atmem.read(LC_UECS_ID+3),atmem.read(LC_UECS_ID+4),atmem.read(LC_UECS_ID+5));
        Serial.println(lbf);
    // MAC Address
    sprintf(lbf,"MAC ADDR: %02X:%02X:%02X:%02X:%02X:%02X",
        atmem.read(LC_MAC),atmem.read(LC_MAC+1),atmem.read(LC_MAC+2),
        atmem.read(LC_MAC+3),atmem.read(LC_MAC+4),atmem.read(LC_MAC+5));
    Serial.println(lbf);
    // FIXED or DHCP
    Serial.print(F("Address allocation method is "));
    if (atmem.read(FIX_DHCP_FLAG)==0) {
        Serial.println(F("STATIC."));
        Serial.println(F("The data below is important."));
    } else {
        Serial.println(F("DHCP."));
        Serial.println(F("The data below are for reference only."));
    }
    // IP Address
    sprintf(lbf,"IP Address/Netmask: %d.%d.%d.%d/%d.%d.%d.%d",
        atmem.read(FIXED_IPADDRESS),atmem.read(FIXED_IPADDRESS+1),
        atmem.read(FIXED_IPADDRESS+2),atmem.read(FIXED_IPADDRESS+3),
        atmem.read(FIXED_NETMASK),atmem.read(FIXED_NETMASK+1),
        atmem.read(FIXED_NETMASK+2),atmem.read(FIXED_NETMASK+3));
    Serial.println(lbf);
    // Default gateway
    sprintf(lbf,"Default Gateway: %d.%d.%d.%d",
       atmem.read(FIXED_DEFGW),atmem.read(FIXED_DEFGW+1),
       atmem.read(FIXED_DEFGW+2),atmem.read(FIXED_DEFGW+3));
    Serial.println(lbf);
    // DNS
    sprintf(lbf,"DNS: %d.%d.%d.%d",
        atmem.read(FIXED_DNS),atmem.read(FIXED_DNS+1),
        atmem.read(FIXED_DNS+2),atmem.read(FIXED_DNS+3));
    Serial.println(lbf);
    // Vendor name
    for (i=0;i<16;i++) {
        sbf[i] = atmem.read(VENDER_NAME+i);
        if (sbf[i]==(char)NULL) break;
    }
    sprintf(lbf,"Vender Name: %s",sbf);
    Serial.println(lbf);
    // Node name
    for (i=0;i<16;i++) {
        sbf[i] = atmem.read(NODE_NAME+i);
        if (sbf[i]==(char)NULL) break;
    }
    sprintf(lbf,"Node Name: %s",sbf);
    Serial.println(lbf);
    // Debug Flag
    b = atmem.read(LC_DBGMSG);
    switch(b) {
        case 0xff:
            sprintf(lbf,"Not defined. must choose an option.");
            break;
        case 0x80:
            sprintf(lbf,"Serial output.");
            break;
        case 0x40:
            sprintf(lbf,"LCD output.");
            break;
        case 0x20:
            sprintf(lbf,"UDP outout.");
            break;
        default:
            sprintf(lbf,"Defined unknown data %02X",b);
            break;
    }
    Serial.print("Debug message: ");
    Serial.println(lbf);
}
                    
