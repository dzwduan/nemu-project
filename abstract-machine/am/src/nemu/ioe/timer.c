
/*
i8253计时器初始化时会分别注册0x48处长度为8个字节的端口, 
以及0xa1000048处长度为8字节的MMIO空间, 
它们都会映射到RTC寄存器. CPU可以访问这两个寄存器来获得当前时间.
*/

#include <am.h>
#include <nemu.h>
static uint32_t time_high,time_low;
void __am_timer_init() {
  time_high = inl(RTC_ADDR+4);
  time_low = inl(RTC_ADDR);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = (inl(RTC_ADDR+4)-time_high)*1000000 + (inl(RTC_ADDR)-time_low+500);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}

