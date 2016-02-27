#ifndef _TIME_M_H
#define _TIME_M_H

#if (!defined(_LINUX_FLAG))
/*WINDOWS*/
#define SLEEP(millSecond) Sleep(millSecond) 
PCHAR GetTime();

#else
/*LINUX*/
#define SLEEP(millSecond) sleep(millSecond/1000) 
LONG GetTickCountT();
#define GetTickCount GetTickCountT
time_t GetTime();
 
#endif

#endif