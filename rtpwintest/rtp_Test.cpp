#include "StdAfx.h"
#include "rtp_Test.h"
#include "kkRtpUdpTransort.h"

rtp_Test::rtp_Test(void)
{
	webrtc::RtpRtcp::Configuration configuration;
	m_pRtpRtcpIns=webrtc::RtpRtcp::CreateRtpRtcp(configuration);
}


rtp_Test::~rtp_Test(void)
{
}

class kkPacedSenderCallback:public webrtc::PacedSender::Callback 
{
 public:
	 kkPacedSenderCallback(){}
	 ~kkPacedSenderCallback(){}
    virtual bool TimeToSendPacket(uint32_t ssrc,
                                  uint16_t sequence_number,
                                  int64_t capture_time_ms,
                                  bool retransmission)
	{
	   return 0;
	}
    // Called when it's a good time to send a padding data.
    // Returns the number of bytes sent.
    virtual size_t TimeToSendPadding(size_t bytes)
	{
	   return 0;
	}
};
kk_Rtp_RtcpImpl::kk_Rtp_RtcpImpl():m_pRtpClock(0)
,m_pRtpRtcpIns(0)
,m_pRTPSender(0)
,m_pRTPSenderVideo(0)
,m_pRTPSenderAudio(0)
{

}
void kk_Rtp_RtcpImpl::init(bool hasAudio,int VideoWidth,int VideoHeight)
{
	m_pRtpClock= new webrtc::SimulatedClock(0);
	webrtc::RtpRtcp::Configuration configuration;
	configuration.clock=m_pRtpClock;
	configuration.audio=hasAudio;
	configuration.id=1;
	kkRtpUdpTransort* pUdpTransort = new kkRtpUdpTransort();
	configuration.outgoing_transport=pUdpTransort;
	/*webrtc::RtpAudioFeedback*                 pRtpAudioFeedback=NULL;
	kkPacedSenderCallback* pkkPacedSenderCallback= new kkPacedSenderCallback();
	webrtc::PacedSender*                      pPaced_sender= new webrtc::PacedSender(m_pRtpClock,pkkPacedSenderCallback,0,0,0);
    webrtc::BitrateStatisticsObserver*        pBitrate_callback=NULL;
    webrtc::FrameCountObserver*               pFrame_count_observer=NULL;
    webrtc::SendSideDelayObserver*           pSend_side_delay_observer=NULL;*/

	m_pRtpRtcpIns=webrtc::RtpRtcp::CreateRtpRtcp(configuration);

	///×¢²áÒôÆµ
	webrtc::CodecInst acodec;
	strcpy(acodec.plname,"mpeg4-generic");
	acodec.pltype=97;
	acodec.plfreq=32000;
	m_pRtpRtcpIns->RegisterSendPayload(acodec);

	webrtc::VideoCodec vCodec;
	strcpy(vCodec.plName,"H264");
	vCodec.plType=96;
	vCodec.width=VideoWidth;
	vCodec.height=VideoHeight;
	vCodec.codecType=webrtc::kVideoCodecH264;
	vCodec.maxFramerate=10;
	m_pRtpRtcpIns->RegisterSendPayload(vCodec);
	//m_pRtpRtcpIns->SendOutgoingData(
	/*m_pRTPSender = new webrtc::RTPSender(         0,					hasAudio,					m_pRtpClock,
		                                          pUdpTransort,			pRtpAudioFeedback,			pPaced_sender,
												  pBitrate_callback,	pFrame_count_observer,		pSend_side_delay_observer
												  );*/
	

	
}