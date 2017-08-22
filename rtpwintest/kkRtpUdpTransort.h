#ifndef  kkRtpUdpTransort_H_
#define  kkRtpUdpTransort_H_
#include "../rtp_rtcp/common_types.h"
class kkRtpUdpTransort:public webrtc::Transport
{
public:
	kkRtpUdpTransort(void);
	~kkRtpUdpTransort(void);
public:
	virtual int SendPacket(int channel, const void *data, size_t len);
    virtual int SendRTCPPacket(int channel, const void *data, size_t len);
};
#endif

