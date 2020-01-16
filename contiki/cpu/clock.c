/*
 * Copyright (c) 2009, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *         Implementation of the clock functions for the cc253x.
 *         Ported over from the cc243x original.
 * \author
 *         Zach Shelby (zach@sensinode.com) - original (cc243x)
 *         George Oikonomou - <oikonomou@users.sourceforge.net> - cc2530 port
 */
#include "sfr-bits.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "sys/energest.h"
#include<reg52.h>
#include "spin_gpio.h"
static volatile CC_AT_DATA  clock_time_t count = 0; /* Uptime in ticks */
static volatile CC_AT_DATA  clock_time_t seconds = 0; /* Uptime in secs */
/*---------------------------------------------------------------------------*/
/**
 * Each iteration is ~1.0xy usec, so this function delays for roughly len usec
 */
void
clock_delay_usec(uint16_t len)
{
  DISABLE_INTERRUPTS();
  while(len--) {
    ASM(nop);
  }
  ENABLE_INTERRUPTS();
}
/*---------------------------------------------------------------------------*/
/*
 * Wait for a multiple of ~8 ms (a tick)
 */
void
clock_wait(clock_time_t i)
{
  clock_time_t xdata start;

  start = clock_time();
  while(clock_time() - start < (clock_time_t)i);
}
/*---------------------------------------------------------------------------*/
CCIF clock_time_t
clock_time(void)
{
  return count;
}
/*---------------------------------------------------------------------------*/
CCIF unsigned long
clock_seconds(void)
{
  return seconds;
}
/*---------------------------------------------------------------------------*/
/*
 * There is some ambiguity between TI cc2530 software examples and information
 * in the datasheet.
 *
 * TI examples appear to be writing to SLEEPCMD, initialising hardware in a
 * fashion semi-similar to cc2430
 *
 * However, the datasheet claims that those bits in SLEEPCMD are reserved
 *
 * The code here goes by the datasheet (ignore TI examples) and seems to work.
 */
void
clock_init(void)
{
    TMOD &= 0xf0;
	TMOD |= 0x01;		//设置定时器模式 16位计数模式 7.8ms中断1次
	TL0=0xEC;
	TH0=0xE3;
	TR0=1;
	ET0=1;//使能定时器0
	EA=1;
}
void
clock_isr(void) interrupt 1				 //定时器0产生系统时基 每秒中断128次 7.8ms中断1次
{
  spin_set_gpio_bit_value(GPIO2,1,0);
  DISABLE_INTERRUPTS();
    /*重装初始值*/
  TL0=0xEC;
  TH0=0xE3;
  TF0 = 0;
  TR0=1;//使能定时器0
  ++count;
  /* Make sure the CLOCK_CONF_SECOND is a power of two, to ensure
     that the modulo operation below becomes a logical and and not
     an expensive divide. Algorithm from Wikipedia:
     http://en.wikipedia.org/wiki/Power_of_two */
#if (CLOCK_CONF_SECOND & (CLOCK_CONF_SECOND - 1)) != 0
#error CLOCK_CONF_SECOND must be a power of two (i.e., 1, 2, 4, 8, 16, 32, 64, ...).
#error Change CLOCK_CONF_SECOND in contiki-conf.h.
#endif
  if(count % CLOCK_CONF_SECOND == 0) {
    ++seconds;
  }

  if(etimer_pending()
      && (etimer_next_expiration_time() - count - 1) > MAX_TICKS) {
    etimer_request_poll();
  }
  ENABLE_INTERRUPTS();
}