# stc89c52
由于keil自带的reg52头文件不包含看门狗寄存器定义。需要在reg52.h文件中添加一行:
sfr WDT_CONTR = 0xE1;
即可通过编译

本分支移植了Contiki RTOS并运行了9个任务，使用了contiki的核心功能，同时实现了所有的51单片机的片内外设驱动。代码中也使用了一些较为中高级的c语言技巧和用法。
实现了一个多任务万年历和呼吸灯的demo作为演示效果。
适合想要初步入门51单片机后，想提高c语言实战能力，单片机项目综合能力，rtos原理学习和应用能力人群。
嵌入式软件方向的同学的学习建议：
step1 contiki+51
step2 stm32+freertos
step3 arm+linux
step4 音视频处理 ADAS fpga算法 异构计算+人工智能 AIOT...

本项目全部代码功能占用rom约15k，占用ram约800byte。
经过裁剪之后，可以占用8k以内rom和512byte以内ram。
