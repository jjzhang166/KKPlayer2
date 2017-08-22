#include "ourRTSPClient.h"
// Implementation of "ourRTSPClient":
ourRTSPClient* ourRTSPClient::createNew(UsageEnvironment* env,TaskScheduler*  Scheduler, char const* rtspURL,
					int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum) {
  return new ourRTSPClient(env,Scheduler, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

ourRTSPClient::ourRTSPClient(UsageEnvironment* env,TaskScheduler*  Scheduler, char const* rtspURL,
			     int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
  : RTSPClient(*env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) 
{
	 m_pScheduler=Scheduler;
     m_pEnv=env;
}

ourRTSPClient::~ourRTSPClient() {
}


void ourRTSPClient::Start()
{
    unsigned	int Addr;
    HANDLE ThreadHandel=(HANDLE)_beginthreadex(NULL, NULL, run_thread, (LPVOID)this, 0,&Addr);
}

unsigned __stdcall   ourRTSPClient::run_thread(LPVOID lpParameter)
{
	ourRTSPClient* Client=(ourRTSPClient*)lpParameter;
	Client->run();
   return 0;
}
void ourRTSPClient::run()
{
  char eventLoopWatchVariable = 0;
  m_pEnv->taskScheduler().doEventLoop(&eventLoopWatchVariable);
  m_pEnv->reclaim(); 
  m_pEnv= NULL;
  delete m_pScheduler; 
  m_pScheduler = NULL;
}