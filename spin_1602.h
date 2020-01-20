#include "spin_common.h" 
#include "process.h"
void checkbusy();
void wcode(uchar t); 
void wdata(uchar t);
void put_line1(uchar line1[]);
void put_line2(uchar line2[]);
void InitLCD();
PROCESS_NAME(lcd);
extern process_event_t lcd_update;