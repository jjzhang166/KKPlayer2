#ifndef rtp_rtcp_test
#define rtp_rtcp_test
#include "rtp_Test.h"
#include "../rtp_rtcp/rtp_rtcpinterface/rtp_rtcp.h"
#include "../rtp_rtcp/RtcRtpsource/rtp_sender.h"
#include "../rtp_rtcp/RtcRtpsource/rtp_sender_video.h"
#include "../rtp_rtcp/RtcRtpsource/rtp_sender_audio.h"

class rtp_Test
{
public:
	rtp_Test(void);
	~rtp_Test(void);
private:
	 webrtc::RtpRtcp* m_pRtpRtcpIns;
};

//web
class kk_Rtp_RtcpImpl
{
public:
	kk_Rtp_RtcpImpl();
	void init(bool hasAudio,int VideoWidth,int VideoHeight);
private:
	 webrtc::Clock*                 m_pRtpClock;
	 webrtc::RtpRtcp*               m_pRtpRtcpIns;
	 ///rtp数据发送者
	 webrtc::RTPSender*             m_pRTPSender;
	 webrtc::RTPSenderVideo*        m_pRTPSenderVideo;
	 webrtc::RTPSenderAudio*        m_pRTPSenderAudio;
};
#endif

