/////////////////////////////////////////////////////////////////////////
//																	   //
//																	   //
// COPYRIGHT NOTICE                                                    //
// Copyright (c) 2014                                                  //
//                                                                     //
// @File MESSAGE_QUEUE.h                                                //
// @header file of udpserver.cpp    				                   //
//																	   //
// @Version 1.0 													   //
// @Author Duan Kai													   //
// @Date 2014-06-18													   //
//																	   //
/////////////////////////////////////////////////////////////////////////

#ifndef _MESSAGE_QUEUE_H
#define _MESSAGE_QUEUE_H

#include "corrstar.h"
#include "datatransfer.h"

///////////////////////////////////////////////////////////////////////////
//
// @Function: Struct of Message Queue
//
///////////////////////////////////////////////////////////////////////////


struct MESSAGE_QUEUE
{
	LLONG                  nID;
	PVOID                  stDataWrap; 
	struct MESSAGE_QUEUE * stpNext;
};
BOOL QueueIn(
	MESSAGE_QUEUE * pstMsgHead, 
	MESSAGE_QUEUE   stMsgInstance,
	MESSAGE_QUEUE * pstMsgTail
	);

MESSAGE_QUEUE * QueueOut(
	MESSAGE_QUEUE * pstMsgHead
	);

/**/
#endif