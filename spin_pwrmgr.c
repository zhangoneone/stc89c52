#include<reg52.h>
void idle_mode(){
   PCON |= 0x01;
}
void power_down(){
   PCON |= 0x02;
}
