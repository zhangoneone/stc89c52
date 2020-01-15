#ifndef SPINUSERSTARTH
#define SPINUSERSTARTH
#define uchar unsigned char
#define uint unsigned int

void spin_set_F0(uchar value);
void spin_set_RS1(uchar value);
void spin_set_RS0(uchar value);
void spin_set_work_register_group(uchar index);
uchar spin_get_CY();
uchar spin_get_F0();
uchar spin_get_RS1();
uchar spin_get_RS0();
uchar spin_get_OV();
void spin_set_sp(uchar addr);
uchar spin_get_sp();
#endif