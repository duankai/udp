/////////////////////////////////////////////////////////////////////////
//																	   //
//																	   //
// COPYRIGHT NOTICE                                                    //
// Copyright (c) 2014                                                  //
//                                                                     //
// @File timer.h                                                       //
// @Brief The cpp file of timer      				                   //
//																	   //
// @Version 1.0 													   //
// @Author Duan Kai													   //
// @Date 2014-06-11													   //
//																	   //
/////////////////////////////////////////////////////////////////////////


#ifndef _TIMER_H
#define _TIMER_H
#pragma pack(push, 4)

#define SLPPE_MILLISECOND 1

#if (!defined(_cplusplus_linux))
/*WINDOWS*/
#define SLEEP(millSecond) Sleep(millSecond) 
#define SCAN_TIME_QUEUE scanTimerQueue
DWORD WINAPI scanTimerQueue(LPVOID lp);

#define CREATESEMAPHORE(sem_t) CreateSemaphore(NULL, 0, 1, NULL)
PCHAR GetTime();

#else
/*LINUX*/
#define SLEEP(millSecond) sleep(millSecond/1000) 
#define SCAN_TIME_QUEUE scanTimerQueue
LPVOID scanTimerQueue(LPVOID lp);
LONG GetTickCountT();
#define CREATESEMAPHORE(sem_t) sem_init(sem_t, 0, 0)
#define GetTickCount GetTickCountT

 
#endif

typedef struct TIMER
{
	INT              uiTimerID;
	LONG             lMilliSecond;
	VOID	        (* lpProcessFunction)(PVOID pvData);
	PVOID            pvData;
} TIMER;


VOID SetTimer(
	INT     uiTimerID,      //the identify of timer
	LONG    lMilliSecond,   //the millisecond of timer
	VOID    (*_lpProcessFunction)(PVOID pvData),
	PVOID   pvData
	);
#pragma pack(pop)

#endif
