#ifndef RAIN_H_
#define RAIN_H_
unsigned char rain_buf[100]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,
0x32,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,/*"r",0*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,
0x44,0x0C,0x34,0x44,0x4C,0x36,0x00,0x00,/*"a",1*/

0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,
0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,/*"i",2*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,
0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,/*"n",3*/
} ;
unsigned char norain_buf[150]={

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,
0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,/*"n",0*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,
0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,/*"o",1*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,/*"_",2*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,
0x32,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,/*"r",3*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,
0x44,0x0C,0x34,0x44,0x4C,0x36,0x00,0x00,/*"a",4*/

0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,
0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,/*"i",5*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,
0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,/*"n",6*/

};

#endif
