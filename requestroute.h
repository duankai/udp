#ifndef _REQUEST_ROUTE_H
#define _REQUEST_ROUTE_H

#include "corrstar.h"
#include "datatransfer.h"
#include "messagequeue.h"



struct REQUEST_ROUTE_TABLE
{
	CHAR                              chType;
	PCHAR							  pstHeader;    //路由表队列指针
	PCHAR							  pstTail;      //路由表队列指针
	HANDLE_SINGLE                     hSingle;
	struct REQUEST_ROUTE_TABLE      * pstNext;

};

BOOL InsertRouteTable(
	REQUEST_ROUTE_TABLE * stRouteTableInst
	);
/*PCHAR GetRequestName(PCHAR pchRequest, CHAR chFirstCh, CHAR  chLastCh);*/



BOOL MapQequestIntoRoute(DATA_WRAP_HDR_T * dataWrap);


#endif