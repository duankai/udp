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
// @Date 2014-06-18													   //
//																	   //
/////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//
// The external head files
//
/////////////////////////////////////////////////////////////////////////////
#include "corrstar.h"
#include "udpserver.h"
#include "time_m.h"
#include "requestroute.h"
#include "datacache.h"


extern CRITICAL_SECTION g_stCritSect;
extern DATA_WRAP_ST * g_stSendCacheTableHeader;
/////////////////////////////////////////////////////////////////////////////
//
// @Name: UDPServer()
// @Description: This is the constructor of class UDPServer.
//
/////////////////////////////////////////////////////////////////////////////

UDPServer::UDPServer(INT nPort)
{
	ACS = VERSION();

	_stSocket = socket(AF_INET,SOCK_DGRAM,0);
	if (INVALID_SOCKET == _stSocket)
	{
		ACS = FALSE;
		CLOSE_SOCKET(_stSocket);

#if (!defined(_cplusplus_linux))
		WSACleanup();
#endif
		return ;
	}


#if (!defined(_cplusplus_linux))
		_stSockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
#else
		_stSockAddr.sin_addr.s_addr = INADDR_ANY;
#endif
	
	_stSockAddr.sin_family = AF_INET;
	_stSockAddr.sin_port = htons(nPort);

	INT nBindRst = bind(_stSocket, (SOCKADDR*)&_stSockAddr, sizeof(SOCKADDR));
	if (SOCKET_ERROR == nBindRst)
	{
		ACS = FALSE;
		CLOSE_SOCKET(_stSocket);

#if (!defined(_cplusplus_linux))
		WSACleanup();
#endif
		return;
	}
	
}

/***********************************************************************/
/*
/* @Name:Send()
/*
/* @Function: Response message when the UDP server received information.
/*
/* @Author: Duan Kai
/*
/* @Create Date: 2014-06-18
/*
/* @Modified Info:
/*
/***********************************************************************/

VOID UDPServer::Send(
	PCHAR pstDataWrap,
	INT   nLen
	)
{
	DATA_WRAP_HDR_T * pstDHerd = (DATA_WRAP_HDR_T *)pstDataWrap;

	printf("¡¾CORRSTAR¡¿: %s send Msg to client: %s \n",GetTime(), pstDHerd->stSource.acAddr);
	//printf("\n");

	stAddrClient.sin_port = htons(pstDHerd->stSource.uiPort);
	stAddrClient.sin_addr.S_un.S_addr = inet_addr(pstDHerd->stSource.acAddr);
	sendto(_stSocket, pstDataWrap, nLen, 0, (SOCKADDR*)&stAddrClient, sizeof(SOCKADDR));
}

/***********************************************************************/
/*
/* @Name:TransmitSend()
/*
/* @Function: Transmit chart messages or file wraps to other client.
/*
/* @Author: Duan Kai
/*
/* @Create Date: 2014-06-18
/*
/* @Modified Info:
/*
/***********************************************************************/

VOID UDPServer::TransmitSend(
	DATA_WRAP_REQ *pstDataWrap
	)
{
	DATA_WRAP_HDR_T * pstDHerd = (DATA_WRAP_HDR_T *)pstDataWrap;

	printf("¡¾CORRSTAR¡¿: %s send Msg to client: %s \n", GetTime() ,pstDHerd->stDestin.acAddr);
	//printf("\n");

	stAddrClient.sin_port = htons(pstDHerd->stDestin.uiPort);
	stAddrClient.sin_addr.S_un.S_addr = inet_addr(pstDHerd->stDestin.acAddr);

	sendto(_stSocket, (PCHAR)pstDataWrap, sizeof(DATA_WRAP_REQ), 0, (SOCKADDR*)&stAddrClient, sizeof(SOCKADDR));
}

/***********************************************************************/
/*
/* @Name:~UDPServer()
/*
/* @Function: This is the destoryed function of class UDPServer.
/*
/* @Author: Duan Kai
/*
/* @Create Date: 2014-06-18
/*
/* @Modified Info:
/*
/***********************************************************************/

UDPServer::~UDPServer()
{
	CLOSE_SOCKET(_stSocket);
#if (!defined(_cplusplus_linux))
	WSACleanup();
#endif
}

/***********************************************************************/
/*
/* @Name:_Receive
/*
/* @Function: Receive message from client.
/*
/* @Author: Duan Kai
/*
/* @Create Date: 2014-06-18
/*
/* @Modified Info:
/*
/***********************************************************************/

VOID UDPServer::Receive()
{
	static DATA_WRAP_REQ *pstDwBuf = NULL;

	if (!pstDwBuf)
	{
		pstDwBuf = (DATA_WRAP_REQ *)malloc(sizeof(DATA_WRAP_REQ));
		if (!pstDwBuf)
		{
			return;
		}
	}

	INT nLen = sizeof(SOCKADDR);

#if (!defined(_LINUX_FLAG))
	INT nRcvFrmRst = recvfrom(_stSocket, (PCHAR)pstDwBuf, sizeof(DATA_WRAP_REQ), 0, (SOCKADDR*)&stAddrClient, &nLen);
#else
	INT nRcvFrmRst = recvfrom(_stSocket, (PCHAR)pstDwBuf, sizeof(DATA_WRAP_REQ), 0, (SOCKADDR*)&stAddrClient, (socklen_t *)&nLen);
#endif	
	if (-1 == nRcvFrmRst)
	{
		return;
	}

	stAddrClient.sin_port = htons(pstDwBuf->stHdr.stSource.uiPort);
	sendto(_stSocket, "QWE", 4, 0, (SOCKADDR*)&stAddrClient, sizeof(SOCKADDR));

	EnterCriticalSection(&g_stCritSect);
	MapQequestIntoRoute((DATA_WRAP_HDR_T *)pstDwBuf);
	LeaveCriticalSection(&g_stCritSect);
	
}

/***********************************************************************/
/*
/* @Function: Message distribution function
/*
/* @Author: Duan Kai
/*
/* @Create Date: 2014-06-20
/*
/* @Modified Info:
/*
/***********************************************************************/



#if (!defined(_cplusplus_linux))


BOOL WIN_VERSION()
{
	BOOL ACS = TRUE;
	WORD wVersionRequested;
	WSADATA wsaData;
	
	wVersionRequested = MAKEWORD(2,0);
	INT nErr = WSAStartup(wVersionRequested, &wsaData);
	if (0 != nErr) 
	{
		ACS = FALSE;
	}

	if (LOBYTE(wsaData.wVersion) != 2 ||
	    HIBYTE(wsaData.wVersion) != 0) 
	{
		ACS = FALSE;
		WSACleanup( );
	}
	return ACS;

}
#endif
