/////////////////////////////////////////////////////////////////////////
//																	   //
//																	   //
// COPYRIGHT NOTICE                                                    //
// Copyright (c) 2014                                                  //
//                                                                     //
// @File UDPServer.cpp                                                 //
// @Brief The cpp file of UDP Server				                   //
//																	   //
// @Version 1.0 													   //
// @Author Duan Kai													   //
// @Date 2014-07-03													   //
//																	   //
/////////////////////////////////////////////////////////////////////////

#ifndef _CORR_STAR_H
#define _CORR_STAR_H

//#define _LINUX_FLAG

/************************************************************************
**
** @Description : some common necessary header files
**
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <cType.h>
#include <time.h>
/************************************************************************
**
** @Description : some necessary header files in different system linux 
**				  and windows
**
************************************************************************/
#if (!defined(_LINUX_FLAG))
/*WINDOWS*/
#include <WinSock2.h>
#pragma comment(lib,"ws2_32")

#else
/*LINUX*/
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#endif


//数据包请求类型定义
#define LOGIN_CLIENT_REQUEST_TYPE      1        //login
#define REGIST_CLIENT_REQUEST_TYPE     2        //regist
#define CHART_REQUEST_TYPE             6        //chart

#define CLIENT_REPORT_STATUS_TYPE      7        //report

#define CLIENT_STATUS_OUTLINE_TYPE     11       //client outline

#define CHART_RESPONSE_TYPE            16       //client response 

#define USER_LIST_TYPE				   17       //client list

#define CLIENT_CHART_LOG_REQ_TYPE      18       //聊天记录查询类型

#define CLIENT_DATA_FILE_TYPE          20       //file transfer type

//普通结果
#define ERROR_CODE 0x00
#define SUCES_CODE 0x01

//用户注册返回代码
#define REGIST_SUCCESS_CODE        0x01      //success
#define REGIST_USERNAME_EXIST_CODE 0x02      //user name has existed
#define REGIST_FAILURE_CODE        0x00      //regist fail



//定时器类型
#define CLIENT_STATUS_REPORRT  0x00         //客户端状态主动上报




//客户端主动上报等待时间
#define CLIENT_STATUS_REPORT_INTERVAL 6000
/************************************************************************
**
** @Description : some common necessary types
**
************************************************************************/
#ifndef VOID
#define VOID void
#endif

#ifndef PVOID
#define PVOID void *
#endif

#ifndef INT
#define INT int
#endif

#ifndef SINT
#define SINT short int
#endif

#ifndef ULONG
#define ULONG unsigned long
#endif

#ifndef LLONG
#define LLONG long long int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef ERROR
#define ERROR (-1)
#endif

#ifndef CHAR
#define CHAR char
#endif

#ifndef PCHAR
#define PCHAR char *
#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef PARMTS
#define PARMTS ...
#endif

#ifndef ACCESS
typedef int  ACCESS;
#endif


/************************************************************************
**
** @Description : some necessary types in different system linux 
**				  and windows
**
************************************************************************/



#if (!defined(_LINUX_FLAG))
/*WINDOWS*/
#ifndef HANDLE_SINGLE
#define HANDLE_SINGLE HANDLE
#endif

#else
/*LINUX*/

#ifndef SOCKET
#define SOCKET int
#endif

#ifndef SOCKADDR_IN
#define SOCKADDR_IN struct sockaddr_in
#endif

#ifndef SOCKADDR
#define SOCKADDR struct sockaddr
#endif

#ifndef HANDLE
#define HANDLE INT
#endif

#ifndef HANDLE_SINGLE
#define HANDLE_SINGLE sem_t
#endif

#endif

/************************************************************************
**
** @Description : some necessary functions in different system linux 
**				  and windows
**
************************************************************************/


#if (!defined(_LINUX_FLAG))
/*WINDOWS*/

#define CREATE_THREAD(nPid, pFunc, pParmts) CreateThread(NULL, 0, pFunc, pParmts, 0, NULL)
#define CREATE_SEMAPHORE(nInit, nMax) CreateSemaphore(NULL, nInit, nMax, NULL)
#define sprintf sprintf_s
#else
/*LINUX*/
#define CREATE_THREAD(nPid, pFunc, pParmts) pthread_create(nPid, NULL,pFunc, pParmts) 
#define CREATE_SEMAPHORE (sem) sem_init(sem, 0, 0)

#define CRITICAL_SECTION pthread_mutex_t
#define InitializeCriticalSection(mutex) pthread_mutex_init(mutex, NULL)
#define EnterCriticalSection(mutex) pthread_mutex_lock(mutex)
#define LeaveCriticalSection(mutex) pthread_mutex_unlock(mutex)
#define DeleteCriticalSection(mutex) pthread_mutex_destroy(mutex)
#endif

/*epilogue*/
#endif