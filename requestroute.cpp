#include "requestroute.h"
#include "datatransfer.h"
#include "datacache.h"

REQUEST_ROUTE_TABLE * g_RouteTableHeader = NULL;
REQUEST_ROUTE_TABLE * g_RouteTableTail   = NULL; 

///////////////////////////////////////////////////////////////////////////
//
// @Function: Put the register address to route table, this is called in the 
//            function named RegiFunc.
//
///////////////////////////////////////////////////////////////////////////
BOOL InsertRouteTable(
	REQUEST_ROUTE_TABLE * stRouteTableInst
	)
{
	REQUEST_ROUTE_TABLE * pstSearchPath = g_RouteTableHeader;

	while(pstSearchPath)
	{
		if (stRouteTableInst->chType == pstSearchPath->chType)
		{
			printf("¡¾CORRSTAR¡¿error £ºFunction Register Failure in \"%d\", Reason: request path have defined!\n", stRouteTableInst->chType);
			return FALSE;
		}
		pstSearchPath = pstSearchPath->pstNext;
	}

	if (NULL == g_RouteTableHeader)
	{
		g_RouteTableHeader = stRouteTableInst;
		g_RouteTableTail   = stRouteTableInst;
	}
	else
	{
		g_RouteTableTail ->pstNext = stRouteTableInst;
		g_RouteTableTail = g_RouteTableTail ->pstNext;
	}
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////
//
// @Function: Get Request Name
// @Example: input  "/login?, '/' , '?'"; output "login";
//
///////////////////////////////////////////////////////////////////////////

PCHAR GetRequestName(
	PCHAR pchRequest,    //source string
	CHAR  chFirstCh,     //first char
	CHAR  chLastCh       //last char
	)
{
	if (NULL == pchRequest)
	{
		return FALSE;
	}
	PCHAR pchStart = strchr(pchRequest, chFirstCh); 
	PCHAR pchEnd   = strchr(pchRequest, chLastCh);

	INT nLen = pchEnd-pchStart;
	PCHAR pchName = (PCHAR)malloc(sizeof(CHAR)*nLen);

	INT i = 0;
	for (i; i < nLen - 1; i++)
	{
		pchStart++;
		pchName[i] = *pchStart;
	}
	pchName[i] = '\0';
	return pchName;
}


///////////////////////////////////////////////////////////////////////////
//
// @Function: Put the request information to the according items which are 
//            on the route table.
//
///////////////////////////////////////////////////////////////////////////

BOOL MapQequestIntoRoute(DATA_WRAP_HDR_T *pstDataWrapHead)
{
	REQUEST_ROUTE_TABLE	* pstSearchPath = g_RouteTableHeader;

	if (NULL == pstDataWrapHead)
	{
		return FALSE;
	}

	while(pstSearchPath)
	{
		if (pstDataWrapHead->ulType == pstSearchPath->chType)
		{
			PVOID pstReq; 
			switch(pstDataWrapHead->ulType)
			{
			case LOGIN_CLIENT_REQUEST_TYPE: 
				{
					pstReq = (LOGIN_REQ_T *)malloc(sizeof(LOGIN_REQ_T));
					memcpy(pstReq, pstDataWrapHead, sizeof(LOGIN_REQ_T));

					break;
				}
			case REGIST_CLIENT_REQUEST_TYPE:
				{
					pstReq = (REGIST_REQ_T *)malloc(sizeof(REGIST_REQ_T));
					memcpy(pstReq, pstDataWrapHead, sizeof(REGIST_REQ_T));

					break;
				}
			case CHART_REQUEST_TYPE:
				{
					pstReq = (DATA_WRAP_REQ *)malloc(sizeof(DATA_WRAP_REQ));
					memcpy(pstReq, pstDataWrapHead, sizeof(DATA_WRAP_REQ));
					break;
				}
			case CLIENT_REPORT_STATUS_TYPE:
				{
					pstReq = (DATA_WRAP_CLIENT_REPORT *)malloc(sizeof(DATA_WRAP_CLIENT_REPORT));
					memcpy(pstReq, pstDataWrapHead, sizeof(DATA_WRAP_CLIENT_REPORT));
					break;
				}

			case CLIENT_CHART_LOG_REQ_TYPE:
				{
					pstReq = (DATA_WRAP_CLIENT_CHART_LOG_REQ *)malloc(sizeof(DATA_WRAP_CLIENT_CHART_LOG_REQ));
					memcpy(pstReq, pstDataWrapHead, sizeof(DATA_WRAP_CLIENT_CHART_LOG_REQ));
					break;
				}
			case CLIENT_DATA_FILE_TYPE:
				{
					pstReq = (DATA_FILE_WRAP *)malloc(sizeof(DATA_FILE_WRAP));
					memcpy(pstReq, pstDataWrapHead, sizeof(DATA_FILE_WRAP));
					break;
				}

			}
			MESSAGE_QUEUE stMsgQueInst;
			stMsgQueInst.stDataWrap = pstReq;
			stMsgQueInst.stpNext = NULL;
			QueueIn
				(
				(MESSAGE_QUEUE *)pstSearchPath->pstHeader, 
				stMsgQueInst,
				(MESSAGE_QUEUE *)pstSearchPath->pstTail
				);
#if (!defined(_LINUX_FLAG))
			ReleaseSemaphore(pstSearchPath->hSingle, 1, NULL);
#else
			sem_post(&pstSearchPath->hSingle);
#endif
			return TRUE;
		}
		pstSearchPath = pstSearchPath->pstNext;
	}

	return TRUE;
}