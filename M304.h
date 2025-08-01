#ifndef _M304_H_
#define _M304_H_
#define _M304_H_V  1321

#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <EEPROM.h>
#include <AT24CX.h>
#include <Wire.h>
#include <DS1307RTC.h>

/*** COMMON ***/

#define TRUE      1
#define FALSE     0
#define ERROR     -1
#define NO        0
#define UNMATCH   0
#define IGNORE    1
#define CHANGE_MAKE  2
#define CHANGE_BREAK 3

/*** Mode via httpd ***/

#define MD_HT_IGNORE  0
#define MD_HT_STORE   1
#define MD_HT_FETCH   2
#define MD_HT_END     3
#define MD_HT_REMOCON 4

/*** CMODE ***/

#define RUN       0
#define CMND      1

#define NETCMND  10
#define RTCCMND  11
#define SCHCMND  12
#define EEPROMCMND 13

/*** CMENU ***/

#define NETCONFIG 0
#define RTCCONFIG 1
#define SCHCONFIG 2
#define EEPROMOPE 3

/*** Define for LCD ***/
#define RS        37
#define RW        38
#define ENA       39
#define DB0       40
#define DB1       41
#define DB2       42
#define DB3       43
#define DB4       44
#define DB5       45
#define DB6       46
#define DB7       47
#define PAGECNT    2

/*** Define for Arrow Key ***/
#define SW_SAFE    3
#define SW_RLY    31
#define SW_ENTER  32
#define SW_UP     33
#define SW_DOWN   34
#define SW_LEFT   35
#define SW_RIGHT  36
#define SELECT_VR A15

/*** Define Relay Ports ***/
#define  RLY1    22
#define  RLY2    23
#define  RLY3    24
#define  RLY4    25
#define  RLY5    26
#define  RLY6    27
#define  RLY7    28
#define  RLY8    29

/*** Ethernet2 Ports ***/
#define NICSS    53

/*** I2C Address ***/
#define AT24LC254_ADDR   0x50
#define AT24LC254_INDEX  0

/*** UECS TEXT ***/
#define CCMFMT "<?xml version=\"1.0\"?><UECS ver=\"1.00-E10\"><DATA type=\"%s\" room=\"%d\" region=\"%d\" order=\"%d\" priority=\"%d\">%s</DATA><IP>%s</IP></UECS>"



////////////////////////////////////////////////////////////////////////////////////////
//  OPERATION MEMORY MAPS //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

/*** EEPROM LOWCORE ASSIGN ***/
#define LC_UECS_ID        0x00
#define LC_MAC            0x06
#define FIX_DHCP_FLAG     0x0c
#define FIXED_IPADDRESS   0x10
#define FIXED_NETMASK     0x14
#define FIXED_DEFGW       0x18
#define FIXED_DNS         0x1c
#define VENDER_NAME       0x40
#define NODE_NAME         0x50
#define LC_DBGMSG         0x60  /* bit pos 0x80: Serial out, 0x40: LCD out, 0x20: 16528/UDP out, 0x02: Info Serial out */
#define   SO_MSG    0x80
#define   LCD_MSG   0x40
#define   UDP_MSG   0x20
#define   SO_INFO   0x02
#define LC_CMODE          0x62  /* Force CMODE Change */

typedef struct stM304 {
  byte mac[6];
  bool dhcpflag=true;
  byte set_ip[4];
  IPAddress ip;
  IPAddress gw;
  IPAddress dns;
  IPAddress subnet;
  int cidr;
};
#define M304V3 1
#ifdef M304V3 // Ver 3.x.x以降のメモリマップ
//
//  SCHEDULE(previous RX) Table
//
#define LC_SCH_START          0x1000
#define LC_SCH_REC_SIZE       0x40 // reserve to 0x3f step by 0x40
#define   LC_SCH_VALID        0x00 // Valid Flag (0x01:valid, 0xff:invalid)
#define   LC_SCH_STHR         0x01 // Start of time (Hour) and validation flag
#define   LC_SCH_STMN         0x02 // Start of time (minute)
#define   LC_SCH_EDHR         0x03 // End of time (hour)
#define   LC_SCH_EDMN         0x04 // End of time (minute)
#define   LC_SCH_MNFLAG       0x05 // MINUTES/SECONDS FLAG (FF:MINUTES,00:SECONDS)
#define   LC_SCH_INMN         0x06 // Interval time (mins/sec) unsigned int (2bytes)
#define   LC_SCH_DUMN         0x08 // During time (mins/sec) unsigned int (2bytes)
#define   LC_SCH_STSC         0x0a // Start of time (Second)
#define   LC_SCH_EDSC         0x0b // End of time (Second)
//define  LC_SCH_DUMMY        0x0c-0x0d (2bytes)
#define   LC_SCH_RLY_L        0x0e // RLY 1..4
#define   LC_SCH_RLY_H        0x0f // RLY 5..8
#define   LC_SCH_CPXCONDS     0x10 // Complex Conditions 16bytes but current 8bytes

#define   LC_SCH_CMPCCMID0    0x10 // compare value opeID 1 byte
#define   LC_SCH_CMPOPE0      0x11 // relational operator 1 byte
#define   LC_SCH_CMPVAL0      0x12 // Numerical values to compare 1 float (4bytes)

#define   LC_SCH_CMBCMP1      0x16 // Combined expression-expression comparison operators 1
#define   LC_SCH_CMPCCMID1    0x17 // compare value opeID 1 byte
#define   LC_SCH_CMPOPE1      0x18 // relational operator 1 byte
#define   LC_SCH_CMPVAL1      0x19 // Numerical values to compare 1 float (4bytes)

#define   LC_SCH_CMBCMP2      0x1d // Combined expression-expression comparison operators 1
#define   LC_SCH_CMPCCMID2    0x1e // compare value opeID 1 byte
#define   LC_SCH_CMPOPE2      0x1f // relational operator 1 byte
#define   LC_SCH_CMPVAL2      0x20 // Numerical values to compare 1 float (4bytes)

#define   LC_SCH_CMBCMP3      0x24 // Combined expression-expression comparison operators 1
#define   LC_SCH_CMPCCMID3    0x25 // compare value opeID 1 byte
#define   LC_SCH_CMPOPE3      0x26 // relational operator 1 byte
#define   LC_SCH_CMPVAL3      0x27 // Numerical values to compare 1 float (4bytes)

#define   LC_SCH_CMBCMP4      0x2b // Combined expression-expression comparison operators 1
#define   LC_SCH_CMPCCMID4    0x2c // compare value opeID 1 byte
#define   LC_SCH_CMPOPE4      0x2d // relational operator 1 byte
#define   LC_SCH_CMPVAL4      0x2e // Numerical values to compare 1 float (4bytes)
//                            0x32-0x3f Reserved

typedef struct uecsM304Sched {
  byte valid;        // 0x00
  byte sthr;         // 0x01
  byte stmn;         // 0x02
  byte edhr;         // 0x03
  byte edmn;         // 0x04
  byte mnflag;       // 0x05
  unsigned int inmn; // 0x06
  unsigned int dumn; // 0x08
  byte stsc;         // 0x09
  byte edsc;         // 0x0a
  byte rly_l;        // 0x0e
  byte rly_h;        // 0x0f
  byte cmbcmp[5];    // 0:R_OR,1:0x16,2:0x1d,3:0x24,4:0x2b
  byte cmpccmid[5];  // 0:0x10, 1:0x17,2:0x1e,3:0x25,4:0x2c
  byte cmpope[5];    // 0:0x11, 1:0x18,2:0x1f,3:0x26,4:0x2d
  float cmpval[5];   // 0:0x12, 1:0x19,2:0x20,3:0x27,4:0x2e
  byte match_result[5]; // load uecsM304cmpope->lifecnt when matched (variable no EEPROM)
};  // 64bytes/1unit

//
//  CCM SEND FORMAT (Previous TX Table) ver:3.x.x
//
#define LC_SEND_START       0x3000   // CCM for data sending (for example cnd.aMC)
#define LC_SEND_REC_SIZE    0x30 // reserve to 0x2f step by 0x30
#define   LC_SEND_VALID     0x00 // Valid Flag (0x01:valid, 0xff:invalid)
#define   LC_SEND_ROOM      0x01
#define   LC_SEND_REGION    0x02
#define   LC_SEND_ORDER     0x03
#define   LC_SEND_PRIORITY  0x05
#define   LC_SEND_LV        0x06 // reference LV define
#define   LC_SEND_CAST      0x07
#define   LC_SEND_CCMTYPE   0x08 // char[20]
#define   LC_SEND_UNIT      0x1c // char[10]

typedef struct uecsM304Send {
  byte valid;        // 0x00
  byte room;         // 0x01
  byte region;       // 0x02
  int  order;        // 0x03
  byte priority;     // 0x05
  byte lv;           // 0x06
  byte cast;         // 0x07
  char ccmtype[20];  // 0x08
  char unit[10];     // 0x1c
};

//  受信CCMに応じた比較演算テーブル  v3.x.x
//  Comparison operation table according to received CCM
//
#define LC_CMPOPE_START     0x5000 // Compare Operators
#define LC_CMPOPE_REC_SIZE  0x20 //
#define   LC_COPE_VALID     0x00
#define   LC_COPE_ROOM      0x01
#define   LC_COPE_REGION    0x02
#define   LC_COPE_ORDER     0x03
#define   LC_COPE_LIFECNT   0x05
#define   LC_COPE_CCMTYPE   0x06 // char[20]
#define   LC_COPE_OPE       0x1a // char[10]
#define   LC_COPE_FVAL      0x1b
#define   LC_COPE_RESULT    0x1f
#define LC_END      0x7fff

typedef struct uecsM304cmpope {
  byte valid;        // 0x00
  byte room;         // 0x01
  byte region;       // 0x02
  uint16_t order;    // 0x03
  byte lifecnt;      // 0x05 sec
  char ccm_type[20]; // 0x06 ASCIZ
  //byte cmpope;       // 0x1a
  float fval;        // 0x1b current recieving data
  byte priority;
  byte remain;
  // LAST            // 0x1f
};  // 32bytes/1unit

////////////// v3.x.x ここまで
//--------------------------------------------------------------------------------------
////////////// v2.6.0D1 までは以下のフォーマット
#else

#define LC_SCH_START 0x1000
#define   LC_VALID        0x00 // Valid Flag (0x01:valid, 0xff:invalid)
#define   LC_ROOM         0x01
#define   LC_REGION       0x02
#define   LC_ORDER        0x03
#define   LC_PRIORITY     0x05
#define   LC_LV           0x06 // reference LV define
#define   LC_CAST         0x07
#define   LC_SR           0x08
#define   LC_CCMTYPE      0x09 // char[20]
#define   LC_UNIT         0x1d // char[10]
#define   LC_STHR         0x27 // Start of time (Hour) and validation flag
#define   LC_STMN         0x28 // Start of time (minute)
#define   LC_EDHR         0x29 // End of time (hour)
#define   LC_EDMN         0x2a // End of time (minute)
#define   LC_INMN         0x2b // Interval time (mins)
#define   LC_DUMN         0x2c // During time (mins)
#define   LC_RLY_L        0x2d // RLY 1..4
#define   LC_RLY_H        0x2e // RLY 5..8
#define   LC_CPXCONDS     0x30 // Complex Conditions 16bytes but current 8bytes
#define   LC_CONDVAL1     0x30 // Condition compare value 1
#define   LC_CONDEXP2     0x31 // Conditional Expression 2
#define   LC_CONDVAL2     0x32 // Condition compare value 2
#define   LC_CONDEXP3     0x33 // Conditional Expression 3
#define   LC_CONDVAL3     0x34 // Condition compare value 3
#define   LC_CONDEXP4     0x35 // Conditional Expression 4
#define   LC_CONDVAL4     0x36 // Condition compare value 4

/*** EEPROM 0x1000..0x2900 CCM DATA ***/
typedef struct uecsM304 {
  byte valid;        // 0x00
  byte room;         // 0x01
  byte region;       // 0x02
  uint16_t order;    // 0x03
  byte priority;     // 0x05
  byte lv;           // 0x06
  byte cast;         // 0x07
  byte sr;           // 0x08
  char ccm_type[20]; // 0x09 ASCIZ
  char unit[10];     // 0x1d ASCIZ
  byte sthr;         // 0x27
  byte stmn;         // 0x28
  byte edhr;         // 0x29
  byte edmn;         // 0x2a
  byte inmn;         // 0x2b
  byte dumn;         // 0x2c
  byte rly_l;        // 0x2d
  byte rly_h;        // 0x2e
  byte alignment;    // 0x2f
  byte dummy[16];    // 0x30-0x3f   // relational operations are written here.
};  // 64bytes/1unit

#define LC_SCH_REC_SIZE   0x40 // reserve to 0x3f step by 0x40
#define LC_SEND_START 0x3000   // CCM for data sending (for example cnd.aMC)
#define LC_SEND_REC_SIZE  0x40 // reserve to 0x3f step by 0x40
#define LC_CMPOPE_START 0x5000 // Compare Operators
#define LC_CMPOPE_REC_SIZE 0x20 //
#define   LC_COPE_VALID    0x00
#define   LC_COPE_ROOM     0x01
#define   LC_COPE_REGION   0x02
#define   LC_COPE_ORDER    0x03
#define   LC_COPE_LIFECNT  0x05
#define   LC_COPE_CCMTYPE  0x06
#define   LC_COPE_OPE      0x1a
#define   LC_COPE_FVAL     0x1b
#define   LC_COPE_RESULT   0x1f
#define LC_END      0x7fff

/*** EEPROM Relational Operators DATA ***/
typedef struct uecsM304cmpope {
  byte valid;        // 0x00
  byte room;         // 0x01
  byte region;       // 0x02
  uint16_t order;    // 0x03
  byte lifecnt;      // 0x05 sec
  char ccm_type[20]; // 0x06 ASCIZ
  byte cmpope;       // 0x1a
  float fval;        // 0x1b
  // LAST            // 0x1f
};  // 32bytes/1unit

#endif
////////////////////////////////////////////////////////////////////////////////////////

/*** LV define ***/
#define LV_A1S0    1      // A-1S-0
#define LV_A1S1    2      // A-1S-1
#define LV_A10S0   3      // A-10S-0
#define LV_A10S1   4      // A-10S-1
#define LV_A1M0    5      // A-1M-0
#define LV_A1M1    6      // A-1M-1
#define LV_B0      7      // B-0
#define LV_B1      8      // B-1
#define LV_S1S0    9      // S-1S-0
#define LV_S1M0   10      // S-1M-0

// Relational Operators

#define R_NULL   0
#define R_EQ     1     // == Equal
#define R_GT     2     // >  Greater Than
#define R_LT     3     // <  Less Than
#define R_GE     4     // >= Greater Than Equal
#define R_LE     5     // <= Less Than Equal
#define R_AND    6     // &  Logical AND
#define R_OR     7     // |  Logical OR
#define R_NE     8     // != Not Equal
#define R_ADDFLG 0x80
#define R_ADD_GT R_ADDFLG|R_GT
#define R_ADD_LT R_ADDFLG|R_LT

/*  Relay operation          */
/*  Group semi 20230615 p.9  */

#define RLY_DNTCARE 0x00
#define RLY_NA      0x01
#define RLY_BOTH    0x01
#define RLY_BREAK   0x02
#define RLY_MAKE    0x03

/*  Cross Key Switch */

#define K_ENT    1
#define K_UP     2
#define K_DOWN   4
#define K_LEFT   8
#define K_RIGHT 16

#define K_DIGIT  32
#define K_XDIGIT 33
#define K_PRINT  34
#define K_ALPHA  35
#define K_ALNUM  36

#define PUSH_SHORT 1000
#define PUSH_LONG  300000

#ifndef _KYBDMEM_
#define _KYBDMEM_
struct KYBDMEM {
  uint8_t   kpos;
  uint16_t  selpos;
  bool      longf;
};
#endif
#ifndef _M304_COMMON_KYBD
#endif

#ifndef _M304_COMMON_DISPLAY
#define _M304_COMMON_DISPLAY 1

class LCDd : public LiquidCrystal {
 private:
  char tarea[PAGECNT][20][4];
 public:
 LCDd():LiquidCrystal(RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7) {
  LiquidCrystal:init(0, RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7);
};
 LCDd(uint8_t rs, uint8_t enable,
      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
      uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7):
  LiquidCrystal(rs,enable,d0,d1,d2,d3,d4,d5,d6,d7){
  LiquidCrystal:init(0, rs,255,enable,d0,d1,d2,d3,d4,d5,d6,d7);
};
  LCDd(uint8_t rs, uint8_t rw, uint8_t enable,
       uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
       uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7):
  LiquidCrystal(rs,rw,enable,d0,d1,d2,d3,d4,d5,d6,d7){};
  LCDd(uint8_t rs, uint8_t rw, uint8_t enable,
      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
  LCDd(uint8_t rs, uint8_t enable,
      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
  void initWriteArea(int);
  int  setWriteChar(int,int,int,char),getDataInt(int,int,int,int),setLine(int,int,char *);
  int  IntRead(int,int,int,int);
  char CharRead(int,int,int);
  void LineWrite(int,int),PageWrite(int),CharWrite(int,int,int,char),IntWrite(int,int,int,int,bool,int);
  void TextWrite(int,int,int,char[]),IPWrite(int,int,int,IPAddress),IPWriteWithCidr(int,int,int,IPAddress,int);
};

#endif

#ifndef _M304_CPP_
  extern void m304Init(void);
  extern void debug_print(void);
#endif
  extern stM304 st_m;
  extern LiquidCrystal lcd;
  extern AT24C256      atmem;
  extern EthernetClient client;
#endif
