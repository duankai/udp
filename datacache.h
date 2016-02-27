#ifndef _DATA_CACHE_H
#define _DATA_CACHE_H
#include "datacache.h"

//���256���û���ÿ���û�����100�����ݰ�

#define USER_COUNT 256
#define BAG_COUNT  100


//���ݰ��ṹ
typedef struct DATA_WRAP_ST
{
	struct DATA_WRAP    * pstDw;
	
	struct DATA_WRAP_ST * pstNext;
}DATA_WRAP_ST;


//·�ɱ�ṹ
typedef struct TABLE_HEAD
{
	INT                   nRowId;
	LLONG                 identity;
	struct DATA_WRAP_ST * pstDw;
	struct TABLE_HEAD   * pstNextHead;
	struct DATA_WRAP_ST * pstTail;
	INT					  nBagCount;
	LLONG                 nRespectSn;
}TABLE_HEAD;

CHAR InsertIntoTable(
	TABLE_HEAD * stHead, 
	DATA_WRAP_ST * stDWST
	);

DATA_WRAP_ST * SearchNextDataWrap(
	DATA_WRAP * stDWST
	);

CHAR InsertIntoSendCacheTable(
	DATA_WRAP_ST * stHead, 
	DATA_WRAP_ST * stDWST,
	DATA_WRAP_ST * stTail
	);

#endif