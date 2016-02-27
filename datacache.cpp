#include "corrstar.h"
#include "datacache.h"
#include "datatransfer.h"

TABLE_HEAD * g_stTableHeader = NULL;
DATA_WRAP_ST * g_stSendCacheTableHeader = NULL;
DATA_WRAP_ST * g_stSendCacheTableTail = NULL;
INT TABLEID = 1;
INT TABLEID_SEND = 1;
/*
DATA_WRAP_ST 0x01
TRUE         0x02
FALSE        0x03
*/

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////

CHAR InsertIntoTable(
	TABLE_HEAD * stHead, 
	DATA_WRAP_ST * stDWST
	)
{
	//if (TABLEID > USER_COUNT)
	//{
	//	printf("User count has reached it's maximum value:256!\n");
	//	return 0x03;
	//}
	//	
	//if (NULL == stHead)
	//{
	//	stHead = (TABLE_HEAD *)malloc(sizeof(TABLE_HEAD));
	//	if (NULL == stHead)
	//	{
	//		return 0x03;
	//	}
	//	stHead->nRespectSn = 1;
	//	stHead->nRowId = TABLEID++;
	//	stHead->identity = stDWST->pstDw->lPrivl;
	//	stHead->pstNextHead = NULL;
	//	if (stHead->nRespectSn == stDWST->pstDw->llSn)
	//	{
	//		stHead->nRespectSn++;
	//		return 0x01;
	//	}
	//	else
	//	{
	//		stHead->pstDw = stDWST;
	//		stHead->pstTail = stDWST;
	//	}
	//	stHead->nBagCount = 1;
	//	g_stTableHeader = stHead;
	//}
	//else
	//{
	//	/*According identity distinguish users*/
	//	TABLE_HEAD * pstTemp = stHead;
	//	while (NULL != pstTemp)                              //
	//	{
	//		if (pstTemp->identity == stDWST->pstDw->lPrivl)  //找到对应的用户
	//		{
	//			if (pstTemp->nRespectSn == stDWST->pstDw->llSn)
	//			{
	//				return 0x01;
	//			}
	//			if (pstTemp->pstDw == NULL)
	//			{
	//				pstTemp->pstTail = stDWST;
	//			}
	//			else
	//			{
	//				DATA_WRAP_ST *stDwForeTemp = pstTemp->pstDw;      //按照sn号插入列表
	//				while (stDwForeTemp)
	//				{
	//					DATA_WRAP_ST *stDwBackTemp = stDwForeTemp->pstNext;
	//					if (stDwForeTemp->pstDw->llSn < stDWST->pstDw->llSn)
	//					{
	//						if (stDwBackTemp == NULL)
	//						{
	//							pstTemp->pstTail->pstNext = stDWST;
	//							pstTemp->pstTail = stDWST;
	//							break;
	//						}
	//						else
	//						{
	//							if (stDwBackTemp->pstDw->llSn > stDWST->pstDw->llSn)
	//							{
	//								stDWST->pstNext = stDwBackTemp;
	//								stDwForeTemp->pstNext = stDWST;
	//								break;
	//							}
	//							else
	//							{
	//								stDwForeTemp = stDwForeTemp->pstNext;
	//							}
	//						}
	//					}
	//					else
	//					{
	//						stDWST->pstNext = pstTemp->pstDw;
	//						pstTemp->pstDw = stDWST;
	//						break;
	//					}

	//				}
	//			}
	//			pstTemp->nBagCount++;
	//			pstTemp->nRespectSn++;
	//			return 0x02;
	//		}
	//		else if(NULL != pstTemp->pstNextHead)
	//		{
	//			pstTemp = pstTemp->pstNextHead;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//	/* ***** */
	//	TABLE_HEAD * pstCurr = (TABLE_HEAD *)malloc(sizeof(TABLE_HEAD));
	//	if (NULL == pstCurr)
	//	{
	//		return 0x03;
	//	}
	//	pstCurr->nRespectSn = 1;
	//	pstCurr->nRowId = TABLEID++;
	//	pstCurr->identity = stDWST->pstDw->lPrivl;

	//	if (pstTemp->nRespectSn == stDWST->pstDw->llSn)
	//	{
	//		pstCurr->nRespectSn++;
	//		return 0x01;
	//	}
	//	else
	//	{
	//		pstCurr->pstDw = stDWST;
	//		pstCurr->pstTail = stDWST;
	//	}
	//	pstCurr->pstNextHead = NULL;
	//	pstCurr->nBagCount++;

	//	pstTemp->pstNextHead = pstCurr;
	//	
	//}
	return 0x02;

}

//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////

DATA_WRAP_ST * SearchNextDataWrap(DATA_WRAP * pstDWST)
{
	//TABLE_HEAD * pstTemp = g_stTableHeader;
	//while (pstTemp)
	//{
	//	if (pstTemp->identity == pstDWST->lPrivl)  //找到对应的用户
	//	{
	//		if (pstTemp->nRespectSn == pstTemp->pstDw->pstDw->llSn)
	//		{
	//			pstTemp->nBagCount--;
	//			pstTemp->nRespectSn++;
	//			DATA_WRAP_ST * pstDwTmp = pstTemp->pstDw;
	//			pstTemp->pstDw = pstTemp->pstDw->pstNext;
	//			return pstDwTmp;
	//		}
	//		else
	//		{
	//			return NULL;
	//		}
	//	}
	//	else
	//	{
	//		pstTemp = pstTemp->pstNextHead;
	//	}
	//}
	return NULL;


}



CHAR InsertIntoSendCacheTable(
	DATA_WRAP_ST * pstHead, 
	DATA_WRAP_ST * pstDWST,
	DATA_WRAP_ST * pstTail
	)
{		
	if (NULL == pstHead)
	{
		pstHead = (DATA_WRAP_ST *)malloc(sizeof(DATA_WRAP_ST));
		if (NULL == pstHead)
		{
			return 0x03;
		}
		pstHead->pstNext = pstDWST;
		pstTail = pstDWST;
		g_stSendCacheTableTail = pstTail;
		g_stSendCacheTableHeader = pstHead;
	}
	else
	{
		pstTail->pstNext = pstDWST;
		pstTail = pstTail->pstNext;
	}
	return 0x01;
}


/////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////
CHAR DeleteSendCacheDataWrap(
	DATA_WRAP_ST * pstHead,
	DATA_WRAP_RESPONSE * pstDWR
	)
{
/*	DATA_WRAP_ST * pstTemp = pstHead;
	while(pstTemp->pstNext)
	{
		if (pstTemp->pstNext->pstDw->stDWR == pstDWR)
		{
			free(pstTemp->pstNext);
			pstTemp->pstNext = pstTemp->pstNext->pstNext;
			return 0x01;
		}
		else
		{
			pstTemp = pstTemp->pstNext;
		}
	}*/
	return 0x03;
}
