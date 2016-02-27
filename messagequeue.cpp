#include "messagequeue.h"


///////////////////////////////////////////////////////////////////////////
//
// @Function: Create message queue, input data to the queue
//
///////////////////////////////////////////////////////////////////////////


BOOL QueueIn(
	MESSAGE_QUEUE * pstMsgHead, 
	MESSAGE_QUEUE   stMsgInstance,
	MESSAGE_QUEUE * pstMsgTail
	)
{
	if (NULL == pstMsgHead)
	{
		return FALSE;
	}

	struct MESSAGE_QUEUE * pCurrent = (struct MESSAGE_QUEUE *)malloc(sizeof(struct MESSAGE_QUEUE));
	if (NULL == pCurrent)
	{
		return FALSE;
	}
	pCurrent->stDataWrap = stMsgInstance.stDataWrap;
	pCurrent->nID = stMsgInstance.nID;
	pCurrent->stpNext = NULL;
	
	if (NULL == pstMsgHead->stpNext)
	{
		pstMsgHead->stpNext = pCurrent;
		pstMsgTail->stpNext = pCurrent;
	}
	else
	{
		pstMsgTail->stpNext->stpNext = pCurrent;

		pstMsgTail->stpNext = pCurrent;
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////
//
// @Function: Put Message Out of Queue
//
///////////////////////////////////////////////////////////////////////////


MESSAGE_QUEUE * QueueOut(
	MESSAGE_QUEUE * pstMsgHead
	)
{
	MESSAGE_QUEUE * stpTmp = NULL;

	if (NULL != pstMsgHead->stpNext)
	{
		stpTmp = pstMsgHead->stpNext;
		pstMsgHead->stpNext = pstMsgHead->stpNext->stpNext;
	}

	return stpTmp;
}




