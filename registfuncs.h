
#ifndef _REGIST_FUNCS_H
#define _REGIST_FUNCS_H


#include "corrstar.h"
#include "requestroute.h"
extern CRITICAL_SECTION g_stCritSect;

template <typename DATA_TYPE_TP>
struct REGI_FUNC_TRANSFER_PARAMTS
{
	CHAR (* RequestHandler)(DATA_TYPE_TP * dataWrap);

#if (!defined(_LINUX_FLAG))
	HANDLE_SINGLE hSingle;
#else
	HANDLE_SINGLE * hSingle;
#endif

	MESSAGE_QUEUE * stQeuHeader;
}; 


template <typename DATA_TYPE_TP>
BOOL RegiFunc(CHAR chType,
	CHAR (* RequestHandler)(DATA_TYPE_TP *));

#if (!defined(_LINUX_FLAH))
template <typename DATA_TYPE_TP>
DWORD WINAPI RegiFuncStart(LPVOID lp)
{
	REGI_FUNC_TRANSFER_PARAMTS<DATA_TYPE_TP> *stRFTP = (REGI_FUNC_TRANSFER_PARAMTS<DATA_TYPE_TP> *)lp;

	while(1)
	{
		WaitForSingleObject(stRFTP->hSingle, INFINITE);
		EnterCriticalSection(&g_stCritSect);

		DATA_TYPE_TP * pstDataWrap = (DATA_TYPE_TP *)stRFTP->stQeuHeader->stpNext->stDataWrap;
		if (pstDataWrap == NULL)
		{
			printf("Data Wrap is empty!\n");
			LeaveCriticalSection(&g_stCritSect);
			continue;
		}
		stRFTP->stQeuHeader->stpNext = stRFTP->stQeuHeader->stpNext->stpNext;

		CHAR chHanRst = stRFTP->RequestHandler(pstDataWrap);

		free(pstDataWrap);
		pstDataWrap = NULL;

		LeaveCriticalSection(&g_stCritSect);
	}
}
#else
LPVOID RegiFuncStart(LPVOID lp)
{

}

#endif


template <typename DATA_TYPE_TP>
BOOL RegiFunc(CHAR chType,
	CHAR (* RequestHandler)(DATA_TYPE_TP * stDataWrap))
{
	REGI_FUNC_TRANSFER_PARAMTS<DATA_TYPE_TP> *stRFTP = (REGI_FUNC_TRANSFER_PARAMTS<DATA_TYPE_TP> *)malloc(sizeof(REGI_FUNC_TRANSFER_PARAMTS<DATA_TYPE_TP>));

	if (NULL == stRFTP)
	{
		return FALSE;
	}
	//create infomation queue
	
	REQUEST_ROUTE_TABLE * stReqRotTblInst = (REQUEST_ROUTE_TABLE *)malloc(sizeof(REQUEST_ROUTE_TABLE));

	stReqRotTblInst->chType = chType;
	stReqRotTblInst->pstNext = NULL;

	MESSAGE_QUEUE * pstHeaderTmp = (MESSAGE_QUEUE *)malloc(sizeof(MESSAGE_QUEUE));
	pstHeaderTmp->stpNext = NULL;

	stReqRotTblInst->pstHeader = (PCHAR)pstHeaderTmp;

	MESSAGE_QUEUE * pstTailTmp = (MESSAGE_QUEUE *)malloc(sizeof(MESSAGE_QUEUE));
	pstTailTmp->stpNext = NULL;

	stReqRotTblInst->pstTail = (PCHAR)pstTailTmp;

	//stReqRotTblInst->pstTail = NULL;

#if (!defined(_LINUX_FLAG))
	stReqRotTblInst->hSingle = CREATE_SEMAPHORE(0, 1000);
#else
	CREATE_SEMAPHORE(&stReqRotTblInst->hSingle);
#endif
	
	BOOL nInstRst = InsertRouteTable(stReqRotTblInst);
	if (FALSE == nInstRst)
	{
		free(stRFTP);
		stRFTP = NULL;

		free(stReqRotTblInst->pstHeader);
		stReqRotTblInst->pstHeader = NULL;

		free(stReqRotTblInst->pstTail);
		stReqRotTblInst->pstTail = NULL;

		return FALSE;
	}
	
	stRFTP->RequestHandler = RequestHandler;
#if (!defined(_LINUX_FLAG))
	stRFTP->hSingle = stReqRotTblInst->hSingle;
	stRFTP->stQeuHeader = (MESSAGE_QUEUE *)stReqRotTblInst->pstHeader;
	HANDLE hThread = CREATE_THREAD(0, RegiFuncStart<DATA_TYPE_TP>, stRFTP);
#else
	stRFTP->hSingle = &stReqRotTblInst->hSingle;
	pthread_t nPid;
	CREATE_THREAD(&nPid, RegiFuncStart, stRFTP);
#endif

	return TRUE;
}

#endif

