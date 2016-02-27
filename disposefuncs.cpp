#include "disposefuncs.h"
#include "udpserver.h"
#include "time_m.h"
#include "timer.h"
#include "tree.h"
#include "DB.h"
//extern TABLE_HEAD * g_stTableHeader; //用户数据表头(chart receive cache)
//extern DATA_WRAP_ST * g_stSendCacheTableHeader;  //chart send cache
//extern DATA_WRAP_ST * g_stSendCacheTableTail;  //chart send cache
//extern CRITICAL_SECTION g_stCritSect;
extern UDPServer * pUDP;
extern MySqlDB mysqlDB;
extern TREE_NODE * g_pstRoot;

///////////////////////////////////////////////////////////////////////////
//
// @Function: Client login process function
//
///////////////////////////////////////////////////////////////////////////

CHAR Login(LOGIN_REQ_T *pstDataWrap)
{
	CHAR acSql[1024];
	printf("【CORRSTAR】: %s Login request...\n", GetTime());
	if (NULL == pstDataWrap)
	{
		return ERROR_CODE;
	}
	sprintf(acSql,"select IP from User where UserName='%s' and PassWord='%s' and Status='1'",pstDataWrap->acUserName,pstDataWrap->acPassword);
	MYSQL_RES * pstRst = mysqlDB.SelectTable(acSql);
	
	if (NULL!=pstRst && pstRst->row_count>=1)//查找到该用户已经在线
	{
		MYSQL_ROW sql_row;
		printf("【CORRSTAR】: %s User \"%s\",IP \"%s\" will in outline status.\n", GetTime(), pstDataWrap->acUserName,pstDataWrap->stHdr.stSource.acAddr);
		//发送下线数据包
		for (INT i=0; sql_row = mysql_fetch_row(pstRst); i++)
		{
			DATA_WRAP_HDR_T stHdr;
			stHdr.ulType = CLIENT_STATUS_OUTLINE_TYPE;
			stHdr.stSource.uiPort = UDP_CLIENT_MESSAGE_PORT;
			
			for(INT j=0; j<mysql_num_fields(pstRst);)
			{
				strcpy(stHdr.stSource.acAddr, sql_row[j++]);
			}
			pUDP->Send((PCHAR)&stHdr,sizeof(DATA_WRAP_HDR_T));
		}
	}

	sprintf(acSql,"update User set Status='1',IP='%s',PrivateIdentity='%d',LoginTime='%s' where UserName='%s' and PassWord='%s'",
		pstDataWrap->stHdr.stSource.acAddr,GetTickCount(),GetTime(),pstDataWrap->acUserName,pstDataWrap->acPassword);

	BOOL bRst = mysqlDB.ModifyLoginTable(acSql);
	LOGIN_RSP_T stLoginRsp;
	stLoginRsp.stHdr = pstDataWrap->stHdr;
	if (bRst == FALSE)
	{
		stLoginRsp.iResult = ERROR_CODE;
		printf("【CORRSTAR】: %s Login Failure!\n", GetTime());
	}
	else
	{
		stLoginRsp.iResult = SUCES_CODE;
		printf("【CORRSTAR】: %s Login Successful!\n", GetTime());

		DATA_WRAP_HDR_T * pstdataH = (DATA_WRAP_HDR_T *)malloc(sizeof(DATA_WRAP_HDR_T));
		memcpy(pstdataH, pstDataWrap,sizeof(DATA_WRAP_HDR_T));

		//将此用户加入心跳队列
		SetTimer(CLIENT_STATUS_REPORRT, CLIENT_STATUS_REPORT_INTERVAL, TimerProcessClientReport, pstdataH);
	}
	
	pUDP->Send((PCHAR)&stLoginRsp, sizeof(LOGIN_RSP_T));

	//  所有用户列表     
	SERVER_USERLIST_DATA_WRAP_T stSUDW;
	stSUDW.stHdr = pstDataWrap->stHdr;
	stSUDW.stHdr.ulType = USER_LIST_TYPE;
	mysqlDB.SelectUserList(stSUDW);
	
	pUDP->Send((PCHAR)&stSUDW, sizeof(SERVER_USERLIST_DATA_WRAP_T));

	//此用户是否有离线消息
	sprintf(acSql,"select SourceUser,Message,Time from chartlog where DestUser='%s' and Status='0'",pstDataWrap->acUserName);
	MYSQL_RES * pstRs = mysqlDB.SelectTable(acSql);
	if (NULL == pstRs)
	{
		return SUCES_CODE;
	}
	MYSQL_ROW sql_row;
	for (INT i=0; sql_row = mysql_fetch_row(pstRs); i++)
	{
		DATA_WRAP_REQ stDwr;
		stDwr.stHdr.ulType = CHART_REQUEST_TYPE;
		stDwr.stHdr.stDestin.uiPort = UDP_CLIENT_MESSAGE_PORT;
		strcpy(stDwr.stHdr.stDestin.acAddr, pstDataWrap->stHdr.stSource.acAddr);
		for(INT j=0; j<mysql_num_fields(pstRs);)
		{
			strcpy(stDwr.stHdr.stSource.acUserName, sql_row[j++]);
			strcpy(stDwr.acMsg,sql_row[j++]);
			strcpy(stDwr.acTime,sql_row[j++]);
		}
		pUDP->TransmitSend(&stDwr);
	}

	//将离线消息标记为已发送
	sprintf(acSql,"update chartlog set Status='1' where DestUser='%s' and Status='0'",pstDataWrap->acUserName);
	BOOL bUpdateRst = mysqlDB.ModifyLoginTable(acSql);
	if (FALSE == bUpdateRst)
	{
		return ERROR_CODE;
	}
	return SUCES_CODE;
}

///////////////////////////////////////////////////////////////////////////
//
// @Function: Client regist process function
//
///////////////////////////////////////////////////////////////////////////

CHAR Register(REGIST_REQ_T *pstDataWrap)
{
	CHAR acSql[1024];
	printf("【CORRSTAR】: %s Regist request...\n", GetTime());
	if (NULL == pstDataWrap)
	{
		return REGIST_FAILURE_CODE;
	}

	REGIST_RSP_T stRegistResp;
	stRegistResp.stHdr = pstDataWrap->stHdr;

	sprintf(acSql,"select * from User where UserName='%s'",pstDataWrap->acUserName);
	MYSQL_RES * pstRst = mysqlDB.SelectTable(acSql);

	if ((unsigned long)mysql_num_rows(pstRst)>=1)//该用户名已经存在
	{
		printf("【CORRSTAR】: %s Username \"%s\"has existed, regist failure!\n",GetTime(),pstDataWrap->acUserName);

		stRegistResp.iResult = REGIST_USERNAME_EXIST_CODE;
		pUDP->Send((PCHAR)&stRegistResp, sizeof(REGIST_RSP_T));

		return REGIST_USERNAME_EXIST_CODE;
	}
	else
	{
		sprintf(acSql,"insert into User values('%s','%s','%s','%d','0','%s',NULL,'%s')",
			pstDataWrap->acNickName, pstDataWrap->acUserName, pstDataWrap->acPassword, GetTickCount(), GetTime(), pstDataWrap->stHdr.stSource.acAddr);

		BOOL bRst = mysqlDB.InsertTable(acSql);
		if (bRst == FALSE)
		{
			stRegistResp.iResult = ERROR_CODE;
			printf("【CORRSTAR】: %s Regist Failure!\n",GetTime());
		}
		else
		{
			stRegistResp.iResult = SUCES_CODE;
			printf("【CORRSTAR】: %s Regist Successful!\n",GetTime());
		}

		pUDP->Send((PCHAR)&stRegistResp, sizeof(REGIST_RSP_T));

	}
	
	return REGIST_SUCCESS_CODE;
}

///////////////////////////////////////////////////////////////////////////
//
// @Function: Client chart process function
//
///////////////////////////////////////////////////////////////////////////
CHAR Chart(DATA_WRAP_REQ *pstDataWrap)
{
	CHAR acSql[1024];
	printf("【CORRSTAR】: %s Chart request...\n", GetTime());
	
	if (NULL == pstDataWrap)
	{
		return ERROR_CODE;
	}
	
	DATA_WRAP_RSP stDataReq;
	stDataReq.stHdr = pstDataWrap->stHdr;
	
	//判断此条消息是否离线消息
	sprintf(acSql, "select * from user where UserName='%s' and Status='1'",stDataReq.stHdr.stDestin.acUserName);

	MYSQL_RES * pstRs = mysqlDB.SelectTable(acSql);
	if (NULL == pstRs)
	{
		return ERROR_CODE;
	}
	if (pstRs->row_count>=1)
	{
		sprintf(acSql,"insert into ChartLog values(NULL,'%s','%s','%s','%s','%s',NULL,'1')",
			pstDataWrap->stHdr.stSource.acAddr, pstDataWrap->stHdr.stSource.acUserName, 
			pstDataWrap->stHdr.stDestin.acAddr, pstDataWrap->stHdr.stDestin.acUserName, pstDataWrap->acMsg);

		pUDP->TransmitSend(pstDataWrap);
	}
	else
	{
		sprintf(acSql,"insert into ChartLog values(NULL,'%s','%s','%s','%s','%s',NULL,'0')",
			pstDataWrap->stHdr.stSource.acAddr, pstDataWrap->stHdr.stSource.acUserName, 
			pstDataWrap->stHdr.stDestin.acAddr, pstDataWrap->stHdr.stDestin.acUserName, pstDataWrap->acMsg);
	}
	
	BOOL bRst = mysqlDB.InsertTable(acSql);
	if (bRst == FALSE)
	{
		stDataReq.iResult = ERROR_CODE;
		printf("【CORRSTAR】: %s Message Transmit Failure!\n",GetTime());
	}
	else
	{
		stDataReq.iResult = SUCES_CODE;
		printf("【CORRSTAR】: %s Message Transmit Successful!\n",GetTime());
	}

	stDataReq.stHdr.ulType = CHART_RESPONSE_TYPE;
	pUDP->Send((PCHAR)&stDataReq, sizeof(DATA_WRAP_RSP));

	return SUCES_CODE;
}
///////////////////////////////////////////////////////////////////////////
//
// @Function: Client status report process function
//
///////////////////////////////////////////////////////////////////////////
CHAR ClientStatusReport(DATA_WRAP_CLIENT_REPORT *pstDataWrap)
{
	//printf("【CORRSTAR】: %s Client status report...\n", GetTime());

	if (NULL == pstDataWrap)
	{
		return ERROR_CODE;
	}
	
	//将树中的数据包时间加10秒
	TREE_NODE * pstTree = SearchTree(g_pstRoot, pstDataWrap);
	if (NULL == pstTree)
	{
		return ERROR_CODE;
	}

	pstTree->timer.lMilliSecond = GetTickCount() + CLIENT_STATUS_REPORT_INTERVAL;
	return SUCES_CODE;
}

//定时器更新用户状态
///////////////////////////////////////////////////////////////////////////
//
// @Function: Timer update client's status
//
///////////////////////////////////////////////////////////////////////////

VOID TimerProcessClientReport(PVOID pvDataWrap)
{
	if (NULL == pvDataWrap)
	{
		return;
	}
	DATA_WRAP_HDR_T *pstDataCltRept = (DATA_WRAP_HDR_T *)pvDataWrap;
	printf("【CORRSTAR】: %s Client \"%s\" outline, heartbeat is missing!\n",GetTime(),pstDataCltRept->stSource.acUserName);
	
	//客户状态更新
	CHAR acSql[1024];
	sprintf(acSql,"update user set Status='0' where UserName='%s'",pstDataCltRept->stSource.acUserName);

	BOOL bRst = mysqlDB.ModifyLoginTable(acSql);
	if (bRst == FALSE)
	{
		printf("【CORRSTAR】: %s User \"%s\" Status Update Failure!\n",GetTime(), pstDataCltRept->stSource.acUserName);
	}
	else
	{
		printf("【CORRSTAR】: %s User \"%s\" Status Update Successful!\n",GetTime(), pstDataCltRept->stSource.acUserName);
	}
	
}
//聊天记录的查询
CHAR ClientChartLogSearch(DATA_WRAP_CLIENT_CHART_LOG_REQ * pstDataWrap)
{
	if (NULL == pstDataWrap)
	{
		return ERROR_CODE;
	}
	printf("【CORRSTAR】: %s Chart search. Name:%s, Time: %s - %s, value: %d \n", GetTime(), pstDataWrap->acName, pstDataWrap->acStartTime, pstDataWrap->acEndTime, pstDataWrap->iValue);
	CHAR acSql[1024];
	if (pstDataWrap->iValue == 0)
	{
		sprintf(acSql,"select SourceUser,DestUser,Message,Time from chartlog where SourceUser='%s' or DestUser='%s' and Time>'%s' and Time<'%s'",pstDataWrap->acName, pstDataWrap->acName, pstDataWrap->acStartTime, pstDataWrap->acEndTime);
	}
	else
	{
		sprintf(acSql,"select SourceUser,DestUser,Message,Time from chartlog order by Time desc limit %d", pstDataWrap->iValue);
	}
	
	MYSQL_RES * pstRs = mysqlDB.SelectTable(acSql);
	if (NULL == pstRs)
	{
		return SUCES_CODE;
	}
	MYSQL_ROW sql_row;
	for (INT i=0; sql_row = mysql_fetch_row(pstRs); i++)
	{
		DATA_WRAP_REQ stDwr;
		stDwr.stHdr.ulType = CLIENT_CHART_LOG_REQ_TYPE;
		stDwr.stHdr.stDestin.uiPort = UDP_CLIENT_MESSAGE_PORT;
		strcpy(stDwr.stHdr.stDestin.acAddr, pstDataWrap->stHdr.stSource.acAddr);
		for(INT j=0; j<mysql_num_fields(pstRs);)
		{
			strcpy(stDwr.stHdr.stSource.acUserName, sql_row[j++]);
			strcpy(stDwr.stHdr.stDestin.acUserName, sql_row[j++]);
			strcpy(stDwr.acMsg,sql_row[j++]);
			strcpy(stDwr.acTime,sql_row[j++]);
		}
		pUDP->TransmitSend(&stDwr);
	}
	return SUCES_CODE;
}

FILE * pstFile; 
CHAR FileTransfer(DATA_FILE_WRAP * pstDataWrap)
{
	if (NULL == pstDataWrap)
	{
		return ERROR_CODE;
	}

	if (NULL == pstFile)
	{
		pstFile = fopen("d:qrcodex.bmp","a+");
		
	}
	else
	{
		if (strlen(pstDataWrap->acMsg) == sizeof(pstDataWrap->acMsg))
		{
			fwrite(pstDataWrap->acMsg, sizeof(pstDataWrap->acMsg), 1, pstFile);
		}
		else
		{
			fwrite(pstDataWrap->acMsg, 1, strlen(pstDataWrap->acMsg), pstFile);
		}
	}
	if (feof(pstFile))
	{
		fclose(pstFile);
	}
	return SUCES_CODE;
}