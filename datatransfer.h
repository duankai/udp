#ifndef _DATA_TRANSFER_H
#define _DATA_TRANSFER_H
#include "corrstar.h"

#pragma pack(push, 4)

//only used in client replay 
typedef struct DATA_WRAP_RESPONSE
{
	LLONG        llSn;
	LONG         lPrivl;
	LLONG        lTime;
	/*BOOL operator==(DATA_WRAP_RESPONSE * pstDWR)
	{
		if (
			this->llSn   == pstDWR->llSn &&
			this->lPrivl == pstDWR->lPrivl &&
			this->lTime  == pstDWR->lTime
			)
		{
			return TRUE;
		}
		return FALSE;
	}*/

}DATA_WRAP_RESPONSE;

typedef struct
{
	USHORT uiPort;
	CHAR   acAddr[16];
	CHAR   acUserName[32];
}ADDRESS_PORT_T;

//the common data wrap
typedef struct 
{
	UINT32              ulType;
	UINT32              ulSn;
	ADDRESS_PORT_T      stSource;
	ADDRESS_PORT_T      stDestin;
	LONG                lPrivl;       //相当于用户的唯一标识
	LONG                lLen;			
}DATA_WRAP_HDR_T;


//Largest data wrap, which is used to chart request
typedef struct
{
	DATA_WRAP_HDR_T		stHdr;
	CHAR                acTime[32];
	CHAR                acMsg[32*1024];
}DATA_WRAP_REQ;

typedef struct
{
	DATA_WRAP_HDR_T		stHdr;
	INT32               iResult;
}DATA_WRAP_RSP;

// login request
typedef struct
{
	DATA_WRAP_HDR_T		stHdr;
	CHAR                acUserName[32];
	CHAR                acPassword[32];
}LOGIN_REQ_T;


//login response
typedef struct
{
	DATA_WRAP_HDR_T		stHdr;
	INT32				iResult;	
}LOGIN_RSP_T;

//register request
typedef struct 
{
	DATA_WRAP_HDR_T		stHdr;
	CHAR                acUserName[32];
	CHAR                acPassword[32];
	CHAR                acNickName[32];
}REGIST_REQ_T;


//register response
typedef struct 
{
	DATA_WRAP_HDR_T		stHdr;
	INT32               iResult;
}REGIST_RSP_T;


//返回所有用户列表的数据包
typedef struct
{
	DATA_WRAP_HDR_T		stHdr;
	CHAR                acUserName[256][32];
	CHAR                acNickName[256][32];
	CHAR                acIP[256][32];
	INT32				nStatus[256];
	INT32               nLenth;
}SERVER_USERLIST_DATA_WRAP_T;


//客户端发送心跳
// only used in client status report
typedef struct DATA_WRAP_CLIENT_REPORT
{
	DATA_WRAP_HDR_T		stHdr;
	UINT32              iStatus;
}DATA_WRAP_CLIENT_REPORT;

//聊天记录请求包
typedef struct DATA_WRAP_CLIENT_CHART_LOG_REQ
{
	DATA_WRAP_HDR_T		stHdr;
	CHAR                acName[32];
	CHAR                acStartTime[32];
	CHAR                acEndTime[32];
	INT                 iValue;            //0:查询聊天记录；>0:加载最近的iValue条聊天记录
}DATA_WRAP_CLIENT_CHART_LOG_REQ;



// only used in transfer file data
typedef struct DATA_FILE_WRAP
{
	DATA_WRAP_HDR_T		stHdr;
	CHAR                acMsg[32*1024];
}DATA_FILE_WRAP;

#pragma pack(pop)

#endif