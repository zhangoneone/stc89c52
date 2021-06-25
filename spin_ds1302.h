#ifndef SPIN_DS1302_H
#define SPIN_DS1302_H
struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};
int ds1302_rtc_read_time(struct rtc_time *tm);
int ds1302_rtc_set_time(struct rtc_time *tm);
extern struct rtc_time default_time;
PROCESS_NAME(ds1302);
#endif