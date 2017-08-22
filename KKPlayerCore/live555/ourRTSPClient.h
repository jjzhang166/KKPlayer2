#ifndef ourRTSPClient_H_
#define ourRTSPClient_H_
#include "../stdafx.h"
#include "StreamClientState.h"
#include "live555_rec_avbuf_que.h"
class ourRTSPClient: public RTSPClient {
public:
        static ourRTSPClient* createNew(UsageEnvironment* env,TaskScheduler*  Scheduler, char const* rtspURL,
				  int verbosityLevel = 0,
				  char const* applicationName = NULL,
				  portNumBits tunnelOverHTTPPortNum = 0);
        void   Start();

		
protected:
  ourRTSPClient(UsageEnvironment* env,TaskScheduler*  Scheduler, char const* rtspURL,
		int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
  static unsigned __stdcall  run_thread(LPVOID lpParameter);
  void run();
    // called only by createNew();
  virtual ~ourRTSPClient();


public:
        StreamClientState         scs;
        live555_rec_avbuf_que     m_avbuf_que;
private:
	 TaskScheduler*         m_pScheduler;
     UsageEnvironment*      m_pEnv;
};
#endif