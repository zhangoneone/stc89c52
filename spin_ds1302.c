#include "spin_ds1302.h"
#include"spin_timer.h"
#include"etimer.h"
#include"process.h"
#include"spin_timer.h"
#include"spin_1602.h"
#include<reg52.h>
#include<stdio.h>
 /* p34 IO p35 CE p36 SCLK*/
sbit IODATA=P3^4;
sbit CE=P3^5;
sbit SCLK=P3^6;
#define	RTC_CMD_READ	0x81		/* Read command */
#define	RTC_CMD_WRITE	0x80		/* Write command */

#define RTC_ADDR_RAM0	0x20		/* Address of RAM0 */
#define RTC_ADDR_TCR	0x08		/* Address of trickle charge register */
#define	RTC_ADDR_YEAR	0x06		/* Address of year register */
#define	RTC_ADDR_DAY	0x05		/* Address of day of week register */
#define	RTC_ADDR_MON	0x04		/* Address of month register */
#define	RTC_ADDR_DATE	0x03		/* Address of day of month register */
#define	RTC_ADDR_HOUR	0x02		/* Address of hour register */
#define	RTC_ADDR_MIN	0x01		/* Address of minute register */
#define	RTC_ADDR_SEC	0x00		/* Address of second register */

#define	RTC_IODATA	0x10
#define	RTC_RESET	0x20
#define	RTC_SCLK	0x40

#define set_dp(x)	(P3 = x)
#define get_dp()	(P3)
#define ds1302_set_tx()
#define ds1302_set_rx()

unsigned bcd2bin(unsigned char val)
{
	return (val & 0x0f) + (val >> 4) * 10;
}

unsigned char bin2bcd(unsigned val)
{
	return ((val / 10) << 4) + val % 10;
}

static int ds1302_hw_init(void)
{
	return 0;
}

static void ds1302_reset(void)
{
	set_dp(get_dp() & ~(RTC_RESET | RTC_IODATA | RTC_SCLK));   //三个引脚全部拉低
}

static void ds1302_clock(void)
{
	set_dp(get_dp() | RTC_SCLK);	/* clock high */
	set_dp(get_dp() & ~RTC_SCLK);	/* clock low */
}

static void ds1302_start(void)
{
	set_dp(get_dp() | RTC_RESET);  //reset引脚拉高
}

static void ds1302_stop(void)
{
	set_dp(get_dp() & ~RTC_RESET);	//reset引脚拉低
}

static void ds1302_txbit(int bitt)
{
	set_dp((get_dp() & ~RTC_IODATA) | (bitt ? RTC_IODATA : 0));	 //向引脚输出1个bit
}

static int ds1302_rxbit(void)
{
	return !!(get_dp() & RTC_IODATA);		//从引脚接收1个bit
}


static void ds1302_sendbits(unsigned int val)
{
	int i;

	ds1302_set_tx(); //使能ds1302的接收功能

	for (i = 8; (i); i--, val >>= 1) {
		ds1302_txbit(val & 0x1);
		ds1302_clock();
	}
}

static unsigned int ds1302_recvbits(void)
{
	unsigned int val;
	int i;

	ds1302_set_rx(); //使能ds1302的发送功能

	for (i = 0, val = 0; (i < 8); i++) {
		val |= (ds1302_rxbit() << i);
		ds1302_clock();
	}

	return val;
}

static unsigned int ds1302_readbyte(unsigned int addr)
{
	unsigned int val;

	ds1302_reset();

	ds1302_start();
	ds1302_sendbits(((addr & 0x3f) << 1) | RTC_CMD_READ);
	val = ds1302_recvbits();
	ds1302_stop();

	return val;
}

static void ds1302_writebyte(unsigned int addr, unsigned int val)
{
	ds1302_reset();

	ds1302_start();
	ds1302_sendbits(((addr & 0x3f) << 1) | RTC_CMD_WRITE);
	ds1302_sendbits(val);
	ds1302_stop();
}

int ds1302_rtc_read_time(struct rtc_time *tm)
{
	tm->tm_sec	= bcd2bin(ds1302_readbyte(RTC_ADDR_SEC));
	tm->tm_min	= bcd2bin(ds1302_readbyte(RTC_ADDR_MIN));
	tm->tm_hour	= bcd2bin(ds1302_readbyte(RTC_ADDR_HOUR));
	tm->tm_wday	= bcd2bin(ds1302_readbyte(RTC_ADDR_DAY));
	tm->tm_mday	= bcd2bin(ds1302_readbyte(RTC_ADDR_DATE));
	tm->tm_mon	= bcd2bin(ds1302_readbyte(RTC_ADDR_MON)) - 1;
	tm->tm_year	= bcd2bin(ds1302_readbyte(RTC_ADDR_YEAR));

	if (tm->tm_year < 70)
		tm->tm_year += 100;

//	dev_dbg(dev, "%s: tm is secs=%d, mins=%d, hours=%d, "
//		"mday=%d, mon=%d, year=%d, wday=%d\n",
//		__func__,
//		tm->tm_sec, tm->tm_min, tm->tm_hour,
//		tm->tm_mday, tm->tm_mon + 1, tm->tm_year, tm->tm_wday);
	ds1302_writebyte(RTC_ADDR_SEC, ds1302_readbyte(RTC_ADDR_SEC) & ~0x80);
	return 0;
}

int ds1302_rtc_set_time(struct rtc_time *tm)
{
	/* Stop RTC */
	ds1302_writebyte(RTC_ADDR_SEC, ds1302_readbyte(RTC_ADDR_SEC) | 0x80);

	ds1302_writebyte(RTC_ADDR_SEC, bin2bcd(tm->tm_sec));
	ds1302_writebyte(RTC_ADDR_MIN, bin2bcd(tm->tm_min));
	ds1302_writebyte(RTC_ADDR_HOUR, bin2bcd(tm->tm_hour));
	ds1302_writebyte(RTC_ADDR_DAY, bin2bcd(tm->tm_wday));
	ds1302_writebyte(RTC_ADDR_DATE, bin2bcd(tm->tm_mday));
	ds1302_writebyte(RTC_ADDR_MON, bin2bcd(tm->tm_mon + 1));
	ds1302_writebyte(RTC_ADDR_YEAR, bin2bcd(tm->tm_year % 100));

	/* Start RTC */
	ds1302_writebyte(RTC_ADDR_SEC, ds1302_readbyte(RTC_ADDR_SEC) & ~0x80);

	return 0;
}
extern volatile process_data_t global_dataa;
struct rtc_time default_time={
 	0,
	0,
	0,
	27,
	0,
	2020-1970,
	1,
	0,
	0,
};
static uchar asc[16];
/*20-01-27 04:09:23 */
uchar *time_2_ascii(struct rtc_time *tm,uchar *asc){
   asc[15]=tm->tm_sec%10+0x30;
   asc[14]=tm->tm_sec/10+0x30;

   asc[13]=':';

   asc[12]=tm->tm_min%10+0x30;
   asc[11]=tm->tm_min/10+0x30;

   asc[10]=':';
   
   asc[9]=tm->tm_hour%10+0x30;
   asc[8]=tm->tm_hour/10+0x30;

   asc[7]=tm->tm_mday%10+0x30;
   asc[6]=tm->tm_mday/10+0x30;

   asc[5]='-';

   asc[4]=(tm->tm_mon+1)%10+0x30;
   asc[3]=(tm->tm_mon+1)/10+0x30;

   asc[2]='-';

   asc[1]=(tm->tm_year+1970)%10+0x30;
   asc[0]=((tm->tm_year+1970)%100)/10+0x30;
   return asc;
}
PROCESS(ds1302,"ds1302");//ds1302 rtc
PROCESS_THREAD(ds1302, ev, dataa)
{
	static struct etimer et;
	static struct pt *ptt = &(ds1302.pt);
    PROCESS_BEGIN();
	//延时5s的时钟
	etimer_set(&et,CLOCK_SECOND*1);//当etimer定时最长的那个时间到期了，系统就会挂掉重启，为什么？？
	while(1)
	{
		//等待5s
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et)); //执行到这里，会改变lcd的pt，调查中
		ds1302_rtc_read_time(&default_time);
		time_2_ascii(&default_time,asc);
		put_line1(asc);
		//process_post(&lcd,lcd_update1,asc);//第二次post会导致系统重启，排查原因
		//process_pt=ptt; //用process_post_synch，则要这样处理，避免contiki在51上的bug
		etimer_restart(&et);
	}
   PROCESS_END();
}
