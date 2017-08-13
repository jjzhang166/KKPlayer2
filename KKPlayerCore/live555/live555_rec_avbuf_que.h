#ifndef live555_rec_avbuf_que_H_
#define live555_rec_avbuf_que_H_
#include <queue>
#include "../KKLock.h"
#include "../Rtmp/FlvEncode.h"
struct live555_av_buf_
{
    unsigned char* databuf;
	int            bulen;
	bool           isvideo;
	int            pts;
};
//enum avtype
//{
//    h264=0,
//	aac=1
//}
class   live555_rec_avbuf_que
{
public:
	     live555_rec_avbuf_que();
         live555_av_buf_*  avbufpop();
		 void   avbufpush(unsigned int pts,bool video,const char* buf,int bufsize);
private:
	     CKKLock m_Lock;
         std::queue< live555_av_buf_*>   av_buf_que;
		 KKMEDIA::FlvEncode              m_flvEc;
		 int                             m_nPreTagLen;
};
#endif