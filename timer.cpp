#include "corrstar.h"
#include "timer.h"
#include "tree.h"

extern TREE_NODE * g_pstRoot;

extern TREE_NODE * g_pstSmallestNode;

#if (!defined(_cplusplus_linux))
DWORD WINAPI TimerThread(LPVOID lp);
#else
LPVOID TimerThread(LPVOID lp);
#endif

#if (!defined(_cplusplus_linux))
VOID SetTimer(
	INT     uiTimerID,      //the identify of timer
	LONG    lMilliSecond,   //the millisecond of timer
	VOID    (*_lpProcessFunction)(PVOID pvData),
	PVOID   pvData
	)
{
	TIMER stTimerInstc;

	stTimerInstc.lpProcessFunction = _lpProcessFunction;
	stTimerInstc.lMilliSecond = GetTickCount() + lMilliSecond;
	stTimerInstc.uiTimerID = uiTimerID;
	stTimerInstc.pvData = pvData;
	
	TREE_NODE * pstNode = (TREE_NODE *)malloc(sizeof(TREE_NODE));

	if (NULL == pstNode)
	{
		return;
	}

	pstNode->pstLeftChild = NULL;
	pstNode->pstParent = NULL;
	pstNode->pstRightChild = NULL;
	pstNode->timer = stTimerInstc;

	InsertTree(g_pstRoot, pstNode);

}


#endif

///////////////////////////////////////////////////////////////////////////
//
// @Function: set timer in the thread, it stands for business threads
//
///////////////////////////////////////////////////////////////////////////

#if (!defined(_cplusplus_linux))
DWORD WINAPI scanTimerQueue(LPVOID lp)
#else
LPVOID scanTimerQueue(LPVOID lp)
#endif
{
	LONG ulCurrTime;
	while(1){

		ulCurrTime = GetTickCount();
		if (NULL != g_pstSmallestNode && 
			g_pstSmallestNode->timer.lMilliSecond <= ulCurrTime)
		{

			TREE_NODE * pstTimerNode = DeleteSmallestNode();

			if (NULL != pstTimerNode)
			{
				pstTimerNode->timer.lpProcessFunction(pstTimerNode->timer.pvData);
				
				if (pstTimerNode == g_pstRoot)//delete root, and then root point to NULL
				{
					g_pstRoot = NULL;
				}

				free(pstTimerNode);
				pstTimerNode = NULL;
			}

		}		
		SLEEP(100);
	}
	return 0;
}
#if (defined(_cplusplus_linux))

LONG GetTickCountT()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec*1000+ts.tv_nsec/1000000);
}

/*long long GetTickCount()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec*1000+ts.tv_nsec/(1000*1000));
} */

/*
time_t GetTime()
{
	time_t nT;
	time(&nT);
	return nT;
}
#else
CHAR chTime[20];
PCHAR GetTime()
{
	SYSTEMTIME stSysTime;
	GetLocalTime(&stSysTime);
	sprintf(chTime, "%d-%d-%d %d:%d:%d", stSysTime.wYear, stSysTime.wMonth, 
			stSysTime.wDay, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);
	//printf("%s",stSysTime);
	return chTime;
}
*/
#endif  
