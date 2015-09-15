#include "liveMedia.hh"
#include "GroupsockHelper.hh"
#include "BasicUsageEnvironment.hh"
#include <time.h>
#include "MutexQueue.cpp"
#include "tinyxml.h"
#include "tinystr.h"
#include <stdio.h> 
#include <string.h> 
#include <iostream>
#include <string.h>
#include <stdlib.h>  
#include <errno.h> 
#if	(__VXWORKS_OS__ || __LINUX_OS__)
#include <netdb.h>
#include <unistd.h> 
#include <unistd.h>  
#include <dirent.h>  
#include <mysql/mysql.h>
#elif __WIN32_OS__
#include <direct.h>
#include <windows.h>
#include <WinSock2.h>
#include "include\mysql.h"
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib, "ws2_32.lib")
#endif
using namespace std;



struct ssession{
	char mac[100];
	char cfd[10];
	char rtspurl[300];
	char deviceip[20];
	int height;
	int width;
};


class StreamClientState {
public:
	StreamClientState();
	virtual ~StreamClientState();

public:
	MediaSubsessionIterator* iter;
	MediaSession* session;
	MediaSubsession* subsession;
	TaskToken streamTimerTask;
	double duration;
	bool flag;
};

class ourRTSPClient : public RTSPClient {
public:
	static ourRTSPClient* createNew(struct ssession * ts, int verbosityLevel = 0, char const* applicationName = NULL, portNumBits tunnelOverHTTPPortNum = 0);
	void createPeriodicOutputFiles0();
	void createOutputFiles0(char const * periodicFilenameSuffix);
	int createVideoFile();
	void closeMediaSinks0();
	void startStorage();
	void stopStorage();
	int run();
	int stop();
	int sendstartreply(const char * msg);
	friend void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString) ;
protected:
	ourRTSPClient(UsageEnvironment& env,struct ssession * ts, int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
	virtual ~ourRTSPClient();
	static void *run_thread(void * orc);
public:
	time_t lt;
	struct tm * starttime;
	pthread_t rpt;
	StreamClientState scs;
	struct ssession *ss;
	TaskToken periodicFileOutputTask0;
	unsigned fileOutputInterval; 
	unsigned fileOutputSecondsSoFar;
	char eventLoopWatchVariable;
	Medium* Out0;
	double initialSeekTime0;
	float scale0;
	double endTime0;
	char date_t[20];
	//char start_t[20];
	int start_t[3];
	char end_t[20];
	char videoname[200];
	unsigned rtspClientCount ; 
};


int MkDir(char *dir);
bool LoadConfig(char * xmlFile);
ourRTSPClient * lookupClientByRTSPURL(const char * rtspurl);
void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);
void subsessionAfterPlaying(void* clientData); 
void subsessionByeHandler(void* clientData); 
void streamTimerHandler(void* clientData); 
void periodicFileOutputTimerHandler0(RTSPClient * rtspClient);
void sessionAfterPlaying0(void * rtspClient);
void setupNextSubsession(RTSPClient* rtspClient); 
void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);
ourRTSPClient * lookupClientByRTSPURL(const char * rtspurl); 
UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient);
UsageEnvironment& operator<<(UsageEnvironment& env, const MediaSubsession& subsession);