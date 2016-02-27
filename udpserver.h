/////////////////////////////////////////////////////////////////////////
//																	   //
//																	   //
// COPYRIGHT NOTICE                                                    //
// Copyright (c) 2014                                                  //
//                                                                     //
// @File udpserver.h                                                   //
// @header file of udpserver.cpp    				                   //
//																	   //
// @Version 1.0 													   //
// @Author Duan Kai													   //
// @Date 2014-06-18													   //
//																	   //
/////////////////////////////////////////////////////////////////////////



#ifndef _UDP_SERVER
#define _UDP_SERVER

#include "corrstar.h"
#include "messagequeue.h"
#include "datatransfer.h"

#define UDP_PORT                   6000      //common port
#define UDP_FILE_PORT              9000      //file port
#define UDP_FILE_PORT_RESPONSE     9001     

#define UDP_SERVER_RESPONSE_PORT   9002      //replay  chart  
#define UDP_CLIENT_STATUS_PORT     10000
#define UDP_RESPONSE_PORT          60000     //client response

#define UDP_CLIENT_MESSAGE_PORT    4000

BOOL WIN_VERSION();

#if (!defined(_cplusplus_linux))
#define CLOSE_SOCKET(s) closesocket(s)
#define VERSION() WIN_VERSION()
#else
#define CLOSE_SOCKET(s) shutdown(s, 2)
#define VERSION() TRUE
#endif


#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif 


typedef struct DISPOSE_PARAS_TRANSFER
{
	/*MESSAGE_QUEUE  * pstMsgHead;
	MESSAGE_QUEUE  * pstMsgTail;
*/
	PCHAR   pstMsgHead;
	PCHAR   pstMsgTail;
}DISPOSE_PARAS_TRANSFER;


/////////////////////////////////////////////////////////////////////////
//
// @Description : UDPServer class
//
/////////////////////////////////////////////////////////////////////////

class UDPServer
{
public:
	UDPServer(INT nPort = UDP_PORT);
	VOID Send(PCHAR pstDataWrap, INT nLen);
	VOID TransmitSend(DATA_WRAP_REQ *pstDataWrap);
	~UDPServer();
	VOID Receive();
public:
	ACCESS ACS;   //flag indicate the UDPServer initialize success or fail

private:
	SOCKADDR_IN stAddrClient;
	SOCKET _stSocket;
	SOCKADDR_IN _stSockAddr;
	//CHAR _chTime[20];
};

#endif