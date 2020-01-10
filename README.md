# stc89c52
由于keil自带的reg52头文件不包含看门狗寄存器定义。需要在reg52.h文件中添加一行:
sfr WDT_CONTR = 0xE1;
即可通过编译