#include "corrstar.h"
#include "datatransfer.h"
#include "registfuncs.h"
#include "disposefuncs.h"
#include "udpserver.h"
#include "DB.h"
#include "timer.h"
#include <json\json.h>

CRITICAL_SECTION g_stCritSect;
UDPServer * pUDP;
MySqlDB mysqlDB;


BOOL CorrStarInit()
{
	//启动必须的线程
	
#if (defined(_cplusplus_linux))
	pthread_t nPid;
#endif

	HANDLE hScan = CREATE_THREAD(&nPid, SCAN_TIME_QUEUE, NULL);

	//注册功能模块函数
	if (FALSE == RegiFunc<LOGIN_REQ_T>(LOGIN_CLIENT_REQUEST_TYPE,Login))
	{
		return FALSE;
	}
	if (FALSE == RegiFunc<REGIST_REQ_T>(REGIST_CLIENT_REQUEST_TYPE,Register))
	{
		return FALSE;
	}
	if (FALSE == RegiFunc<DATA_WRAP_REQ>(CHART_REQUEST_TYPE,Chart))
	{
		return FALSE;
	}
	if (FALSE == RegiFunc<DATA_WRAP_CLIENT_REPORT>(CLIENT_REPORT_STATUS_TYPE,ClientStatusReport))
	{
		return FALSE;
	}
	if (FALSE == RegiFunc<DATA_WRAP_CLIENT_CHART_LOG_REQ>(CLIENT_CHART_LOG_REQ_TYPE,ClientChartLogSearch))
	{
		return FALSE;
	}
	if (FALSE == RegiFunc<DATA_FILE_WRAP>(CLIENT_DATA_FILE_TYPE, FileTransfer))
	{
		return FALSE;
	}
	return TRUE;
}


VOID main()
{
	//Json::Value json_tmp;
	//json_tmp["name"] = Json::Value("duankai");
	//Json::FastWriter fast_write;
	//std::cout << fast_write.write(json_tmp)<<std::endl;

	//run();
	//char x[456] = "this";
	//printf("%d,", strlen(x));
	//printf("%d,", sizeof(x));
	//--------------------------------------------

	printf("/////////////////////////////////////////////////////////////////////////\n");
	printf("//                                                                     //\n");
	printf("// COPYRIGHT NOTICE          ||||||  |||||||   ||||||  ||||||  ||||||  //\n");
	printf("// Copyright (c) 2014        ||      ||  ||      ||      ||      ||    //\n");
	printf("//                           ||||    ||||||      ||      ||      ||    //\n");
	printf("// @UDP                      ||      ||   ||     ||      ||      ||    //\n");
	printf("//                           ||      ||    ||  ||||||    ||      ||    //\n");
	printf("// @Version 1.0                                                        //\n");
	printf("// @Author FRITT                                                       //\n");
	printf("// @Date 2014-07-15                                                    //\n");
	printf("//                                                                     //\n");
	printf("/////////////////////////////////////////////////////////////////////////\n");
	printf("\n");
	printf("UDP server have started in port 6000...\n");
	printf("\n");

	InitializeCriticalSection(&g_stCritSect);
	if (FALSE == CorrStarInit())
	{
		return ;
	}
	
	pUDP = new UDPServer();

	while(1)
	{
		pUDP->Receive();
	}

	DeleteCriticalSection(&g_stCritSect);
}