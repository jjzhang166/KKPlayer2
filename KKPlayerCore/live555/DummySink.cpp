#include "DummySink.h"
#include <string>
#include "../platforms.h"

#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 8294400
DummySink* DummySink::createNew(UsageEnvironment& env, MediaSubsession& subsession, live555_rec_avbuf_que* avbuf_que,char const* streamId) {
  return new DummySink(env, subsession,avbuf_que, streamId);
}

DummySink::DummySink(UsageEnvironment& env, MediaSubsession& subsession,live555_rec_avbuf_que* avbuf_que, char const* streamId)
  : MediaSink(env),
  m_pavbuf_que(avbuf_que),
    fSubsession(subsession) {
  fStreamId = strDup(streamId);
  fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
}

DummySink::~DummySink() {
  delete[] fReceiveBuffer;
  delete[] fStreamId;
}

void DummySink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned durationInMicroseconds)
{
  DummySink* sink = (DummySink*)clientData;
  sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

// If you don't want to see debugging output for each received frame, then comment out the following line:
#define DEBUG_PRINT_EACH_RECEIVED_FRAME 1

void DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
				  struct timeval presentationTime, unsigned /*durationInMicroseconds*/) {

	if(m_pavbuf_que!=NULL){
		
		std::string mediumName=fSubsession.mediumName();
		unsigned int pts=presentationTime.tv_sec*1000+presentationTime.tv_usec/1000;
		bool isvideo=false;
		if(mediumName!="audio")
		{
			isvideo=true;
		}
	
		m_pavbuf_que->avbufpush(pts,isvideo,(const char*)fReceiveBuffer,frameSize);
	}
  continuePlaying();
}

Boolean DummySink::continuePlaying() {
  if (fSource == NULL) return False; // sanity check (should not happen)

  // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
  fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
                        afterGettingFrame, this,
                        onSourceClosure, this);
  return True;
}
