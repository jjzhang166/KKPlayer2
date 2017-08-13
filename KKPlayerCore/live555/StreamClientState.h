#ifndef StreamClientState_H_
#define StreamClientState_H_
#include "live555.h"
class StreamClientState
{
public:
	  StreamClientState();
	  virtual ~StreamClientState();

public:
	  MediaSubsessionIterator* iter;
	  MediaSession* session;
	  MediaSubsession* subsession;
	  TaskToken streamTimerTask;
	  double duration;
};

#endif