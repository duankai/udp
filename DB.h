#ifndef _DB_H
#define _DB_H

#include <iostream>
#include "corrstar.h"
#include "datatransfer.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h> 


class MySqlDB
{
public:
	MySqlDB(
		PCHAR pacUserName = "duankai",
		PCHAR pacPassword = "duankai",
		PCHAR pacDataBase = "corrstar",
		PCHAR pacHostIP   = "192.168.100.220",
		UINT32 nPort      = 3306
		);
	~MySqlDB();
	SERVER_USERLIST_DATA_WRAP_T & SelectUserList(SERVER_USERLIST_DATA_WRAP_T & stServUserList);
	BOOL ModifyLoginTable(PCHAR sql);
	BOOL InsertTable(PCHAR sql);
	MYSQL_RES * SelectTable(PCHAR sql);
private:
	PCHAR  _pacUserName;
	PCHAR  _pacPassword;
	PCHAR  _pacDataBase;
	PCHAR  _pacHostIP;
	UINT32 _nPort;

	MYSQL          _stConn;
	MYSQL_RES   *  _pstResult;
	MYSQL_ROW      _sqlRow;
	MYSQL_FIELD *  _pstfd;
	CHAR           _column[32][32];
	INT            _nRs;
};

#endif 