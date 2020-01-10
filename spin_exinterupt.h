#ifndef SPINEXINTERUPTH
#define SPINEXINTERUPTH

#include"spin_common.h"
#include"spin_interupt.h"
void spin_exint0_start(TRIGER_TYPES type);
void spin_exint1_start(TRIGER_TYPES type);
void spin_exint0_stop();
void spin_exint1_stop();
#endif