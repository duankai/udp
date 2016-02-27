
#include "DB.h"


MySqlDB::MySqlDB(PCHAR pacUserName ,
	PCHAR pacPassword ,
	PCHAR pacDataBase ,
	PCHAR pacHostIP ,
	UINT32 nPort)
{
	_pacUserName = pacUserName;
	_pacPassword = pacPassword;
	_pacDataBase = pacDataBase;
	_pacHostIP   = pacHostIP;
	_nPort       = nPort;

	mysql_init(&_stConn);
	if (!mysql_real_connect(&_stConn,_pacHostIP,_pacUserName,_pacPassword,_pacDataBase,_nPort,NULL,0))
	{
		printf("Connection failed!\n");
		return;
	}

}

MySqlDB::~MySqlDB()
{
	mysql_close(&_stConn);
}

BOOL MySqlDB::ModifyLoginTable(PCHAR sql)
{
	mysql_query(&_stConn, "SET NAMES GBK");
	
	_nRs = mysql_query(&_stConn,sql);
	
	if (!_nRs &&
		mysql_affected_rows(&_stConn)
		)
	{
		return TRUE;
	}
	return FALSE;

}

BOOL MySqlDB::InsertTable(PCHAR sql)
{
	mysql_query(&_stConn, "SET NAMES GBK");
	
	_nRs = mysql_query(&_stConn,sql);
	if (!_nRs)
	{
		_pstResult = mysql_store_result(&_stConn);

		if (_pstResult!=NULL && 
			_pstResult->row_count == 0)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
		
	}
	return FALSE;

}


MYSQL_RES * MySqlDB::SelectTable(PCHAR sql)
{
	_nRs = mysql_query(&_stConn, sql);
	if (!_nRs)
	{
		_pstResult = mysql_store_result(&_stConn);
		return _pstResult;
	}
	else
	{
		return NULL;
	}
}


//查询所有用户列表
SERVER_USERLIST_DATA_WRAP_T & MySqlDB::SelectUserList(SERVER_USERLIST_DATA_WRAP_T & stServUserList)
{
	INT nRs = mysql_query(&_stConn, "select UserName, NickName, Status, IP from User");
	if (!_nRs)
	{
		MYSQL_RES * pstRes = mysql_store_result(&_stConn);
		INT nFields = mysql_num_fields(pstRes);
		stServUserList.nLenth = pstRes->row_count;
		MYSQL_ROW sql_row;

		for (INT i = 0; sql_row = mysql_fetch_row(pstRes); i++)
		{
			for(INT j=0; j<nFields;)
			{
				strcpy(stServUserList.acUserName[i], sql_row[j++]);
				strcpy(stServUserList.acNickName[i], sql_row[j++]);
				stServUserList.nStatus[i] = atoi(sql_row[j++]);
				strcpy(stServUserList.acIP[i], sql_row[j++]);
			}

		}
	}
	return stServUserList;
}




//void run(){
//	const char user[] = "duankai";         //username
//	const char pswd[] = "123456";         //password
//	const char host[] = "192.168.100.129";    //or"127.0.0.1"
//	const char table[] = "dktest";        //database
//	unsigned int port = 3306;           //server port        
//	MYSQL myCont;
//	MYSQL_RES *result;
//	MYSQL_ROW sql_row;
//	MYSQL_FIELD *fd;
//	char column[32][32];
//	int res;
//	mysql_init(&myCont);
//	if(mysql_real_connect(&myCont,host,user,pswd,table,port,NULL,0))
//	{
//		mysql_query(&myCont, "SET NAMES GBK");
//		res=mysql_query(&myCont,"select * from table1");
//		mysql_query(&myCont,"select * from table1 where w=1");
//		if(!res)
//		{
//			result=mysql_store_result(&myCont);
//			if(result)
//			{
//				int i,j;
//				std::cout<<"number of result: "<<(unsigned long)mysql_num_rows(result)<<std::endl;
//				for(i=0;fd=mysql_fetch_field(result);i++)
//				{
//					strcpy(column[i],fd->name);
//				}
//				j=mysql_num_fields(result);
//				for(i=0;i<j;i++)
//				{
//					printf("%s\t",column[i]);
//				}
//				printf("\n");
//				while(sql_row=mysql_fetch_row(result))
//				{
//					for(i=0;i<j;i++)
//					{
//						printf("%s\n",sql_row[i]);
//					}
//					printf("\n");
//				}
//			}
//		}
//		else
//		{
//	
//		}
//	}
//	else
//	{
//	}
//	if(result!=NULL) mysql_free_result(result);//
//	mysql_close(&myCont);//
//}