#include "corrstar.h"
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

#endif
