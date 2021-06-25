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
#include "spin_timer.h"
static volatile  clock_time_t count = 0; /* Uptime in ticks */
static volatile  clock_time_t seconds = 0; /* Uptime in secs */
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
clock_wait(clock_time_t  i)
{
  clock_time_t start;

  start = clock_time();
  while(clock_time() - start < (clock_time_t)i);
}
/*---------------------------------------------------------------------------*/
CCIF clock_time_t
clock_time(void)
{  
  clock_time_t c;
  DISABLE_INTERRUPTS();
  c=count;
  ENABLE_INTERRUPTS();
  return c;
}
/*---------------------------------------------------------------------------*/
CCIF clock_time_t
clock_seconds(void)
{
  clock_time_t s;
  DISABLE_INTERRUPTS();
  s=seconds;
  ENABLE_INTERRUPTS();
  return s;
}

void
clock_init(void)
{
   spin_sysTick();
}
static int counter=0;
volatile char pwm_counter=0;//pwm计数值
const char top=20;//pwm频率是200HZ
extern void pwm_start();
void intersvr1(void) interrupt 1				 //定时器0产生系统时基 每秒中断128次 7.8ms中断1次
{
  counter++;
  pwm_counter++;
  if(pwm_counter%top==0)pwm_counter=0;
  pwm_start();
  if(counter<32)return;
  counter=0;
  DISABLE_INTERRUPTS();
  ++count;
  if(count % CLOCK_CONF_SECOND == 0) {
    ++seconds;
  }

  if(etimer_pending()
      && (etimer_next_expiration_time() - count - 1) > MAX_TICKS) {
    etimer_request_poll();	 //etimer到期，设置poll标志
  }
  ENABLE_INTERRUPTS();
}