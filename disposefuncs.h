#ifndef _DISPOSE_FUNCS_H
#define _DISPOSE_FUNCS_H

#include "corrstar.h"
#include "datatransfer.h"

CHAR Login(LOGIN_REQ_T *dataWrap);

CHAR Register(REGIST_REQ_T *dataWrap);

CHAR Chart(DATA_WRAP_REQ *dataWrap);

CHAR ClientStatusReport(DATA_WRAP_CLIENT_REPORT *pstDataWrap);

VOID TimerProcessClientReport(PVOID pvDataWrap);

CHAR ClientChartLogSearch(DATA_WRAP_CLIENT_CHART_LOG_REQ * pstDataWrap);

CHAR FileTransfer(DATA_FILE_WRAP * pstDataWrap);

#endif 