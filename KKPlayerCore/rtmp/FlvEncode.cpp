#include <stdlib.h>
#include <string.h>
#include "FlvEncode.h"
#include "stdint.h"

//<stdint.h>
namespace KKMEDIA
{
	#define NALU_TYPE_SLICE 1
	#define NALU_TYPE_DPA 2
	#define NALU_TYPE_DPB 3
	#define NALU_TYPE_DPC 4
	#define NALU_TYPE_IDR 5
	#define NALU_TYPE_SEI 6          
	#define NALU_TYPE_SPS 7
	#define NALU_TYPE_PPS 8
	#define NALU_TYPE_AUD 9
	#define NALU_TYPE_EOSEQ 10
	#define NALU_TYPE_EOSTREAM 11
	#define NALU_TYPE_FILL 12

	char *put_byte( char *output, uint8_t nVal )  
	{  
		output[0] = nVal;  
		return output+1;  
	}  
	char *put_be16(char *output, uint16_t nVal )  
	{  
		output[1] = nVal & 0xff;  
		output[0] = nVal >> 8;  
		return output+2;  
	}  
	char *put_be24(char *output,uint32_t nVal )  
	{  
		output[2] = nVal & 0xff;  
		output[1] = nVal >> 8;  
		output[0] = nVal >> 16;  
		return output+3;  
	}  
	char *put_be32(char *output, uint32_t nVal )  
	{  
		output[3] = nVal & 0xff;  
		output[2] = nVal >> 8;  
		output[1] = nVal >> 16;  
		output[0] = nVal >> 24;  
		return output+4;  
	}  
	char *put_be64( char *output, uint64_t nVal )  
	{  
		output=put_be32( output, nVal >> 32 );  
		output=put_be32( output, nVal );  
		return output;  
	}  
	char *put_amf_bool(char *output, int b)
	{
		*output=0x01;
		*(++output)= !!b;
		return ++output;
	}
	char *put_amf_string( char *c, const char *str )  
	{  
		uint16_t len = strlen( str );  
		c=put_be16( c, len );  
		memcpy(c,str,len);  
		return c+len;  
	} 
	
	char *put_amf_double( char *c, double d )  
	{  
		*c++ = 0;  /* type: Number */  
		{  
			unsigned char *ci, *co;  
			ci = (unsigned char *)&d;  
			co = (unsigned char *)c;  
			co[0] = ci[7];  
			co[1] = ci[6];  
			co[2] = ci[5];  
			co[3] = ci[4];  
			co[4] = ci[3];  
			co[5] = ci[2];  
			co[6] = ci[1];  
			co[7] = ci[0];  
		}  
		return c+8;  
	}

	FlvEncode::FlvEncode(void)
	{
	}

	FlvEncode::~FlvEncode(void)
	{
	}
	FLV_HEADER FlvEncode::CreateFLVHeader(char StreamInfo)
	{
		FLV_HEADER header;
		memset(&header,0,sizeof(FLV_HEADER));
		header.FLV[0]=0x46;
		header.FLV[1]=0x4c;
		header.FLV[2]=0x56;
		header.Ver=0x01;
		header.StreamInfo=StreamInfo;
		header.HeaderLen=0;
		char *pp=(char *)&header.HeaderLen;
		*(pp+3)=0x09;
		return header;
	}
	//http://blog.csdn.net/zengraoli/article/details/7742278
	void *FlvEncode::CreateFLVMetaData(METADATA *lpMetaData,int &OutLen)
	{
		char *body =(char *)::malloc(1024);

		char * p = (char *)body;
	
		p = put_byte( p, 2 );
		p = put_amf_string( p, "onMetaData" );

		//amf 2 type 1bytes：固定为0x08
		p = put_byte(p, 0x08);  
	    //array size 4bytes：表示接下来的metadata array data 中有多少组数据
		{
			p=put_be32(p,11);
			/*p = put_byte(p, 0x00);
			p = put_byte(p, 0x00);
			p = put_byte(p, 0x00);
			p = put_byte(p, 0x08);*/
			/**p++ =0x00 ;
			*p++ =0x00 ;
			*p++ =0x00 ;
			*p++ =0x08 ;*/
		}
		p =put_amf_string( p, "duration" );
		p =put_amf_double( p, lpMetaData->duration); //lpMetaData->nAudioFmt

		
		//①前面2bytes表示，第N个数组的名字所占的bytes
		//视频m_nCurPos
		p =put_amf_string( p, "width");
		p =put_amf_double( p, lpMetaData->nWidth);

		p =put_amf_string( p, "height");
		p =put_amf_double( p, lpMetaData->nHeight);

		p =put_amf_string( p, "framerate" );    //视频帧率
		p =put_amf_double( p, lpMetaData->nFrameRate); 

		//p =put_amf_string( p, "videodatarate" );//视频数据速率
		//p =put_amf_double( p, lpMetaData->nVideoDataRate); 

		p =put_amf_string( p, "videocodecid" );
		p =put_amf_double( p,  7);//lpMetaData->nVideoFmt
		
		//音频
		p =put_amf_string( p, "audiodatarate");//音频数据速率
		p =put_amf_double( p, lpMetaData->nAudioDatarate);

		p =put_amf_string( p, "audiosamplerate");
		p =put_amf_double( p, lpMetaData->nAudioSampleRate); //音频采样率

		p =put_amf_string( p, "audiosamplesize" );
		p =put_amf_double( p, lpMetaData->nAudioSampleSize); //音频采样精度

		p =put_amf_string( p, "stereo" );//是否为立体声
		//p =put_be32( p,1 ); //lpMetaData->nAudioChannels
		p=put_amf_bool(p, 1);

		p =put_amf_string( p, "audiocodecid" ); //音频编码方式
		p =put_amf_double( p, 2); //lpMetaData->nAudioFmt

		p =put_amf_string( p, "filesize" );
		p =put_amf_double( p, lpMetaData->filesize); //lpMetaData->nAudioFmt

		//p =put_amf_string( p, "" );
		//p =put_byte( p, 9);
		OutLen=0;
		for(int i=0;i<1024;i++)
		{
			OutLen++;
			if(p==body+i)
			{
				break;
			}
		}
		return body;
	}

	void *FlvEncode::FlvMemcpy(void* dest,size_t destLen, const void* src, size_t n)
	{
		char*     d= (char*) dest;
		const char*  s= (const char*)src;
		s=s+n-1;
		while(n--&&destLen--)
		{
			*d++=*s--;
		}
		return dest;
	}

	int FlvEncode::GetNALULen(const unsigned  char *src,int srcLen)
	{
		int len=0;
		int Index=0;
		unsigned char* pdata=(unsigned  char *)src;
		while(len<srcLen)
		{
			
			if(
				(
				*pdata==0x00&&
				*(1+pdata)==0x00&&
				*(2+pdata)==0x01
				)
				||
				(
					*pdata==0x00&&
					*(1+pdata)==0x00&&
					*(2+pdata)==0x00&&
					*(3+pdata)==0x01
				)
			)
			{
               break;
			}
			len++;
			pdata++;
		}
		return len;
	}

	int FlvEncode::GetH264SeparatorLen(const unsigned  char *src,int srcLen)
	{
		if(*src!=0x00)
			return 0;
		unsigned char* pdata=(unsigned  char *)src;
		
		if(
			*pdata==0x00&&
			*(1+pdata)==0x00&&
			*(2+pdata)==0x01
			)
		{
			return 3;
		}
		if(
			*pdata==0x00&&
			*(1+pdata)==0x00&&
			*(2+pdata)==0x00&&
			*(3+pdata)==0x01
			)
		{
			return 4;
		}
		return 0;
	}
	flv_packet  FlvEncode::GetFlvHeader()
	{
	        KKMEDIA::FLV_HEADER flvheader=CreateFLVHeader(0x04);

            KKMEDIA::METADATA MetaData;
			memset(&MetaData,0,sizeof(MetaData));
			MetaData.nHeight=480;
			MetaData.nWidth=640;
			MetaData.nFrameRate=0;
			MetaData.nVideoDataRate=0;
			MetaData.nVideoCodecId=7;

			int OutLen;
			void *pIfo=CreateFLVMetaData(&MetaData,OutLen);

			KKMEDIA::FLV_TAG_HEADER Tag_Head;
			memset(&Tag_Head,0,sizeof(Tag_Head));
			Tag_Head.TagType=0x12;
			FlvMemcpy(&Tag_Head.TagDtatLen,3,&OutLen,3);
			//PreTagLen=

			int TagDataLen=sizeof(flvheader)+OutLen+100;
            char *pTagData =(char *)::malloc(TagDataLen);
			memset(pTagData,0,TagDataLen);
			memcpy(pTagData,&flvheader,sizeof(flvheader));


			int lex=sizeof(flvheader);
			memcpy((pTagData+lex),&Tag_Head,sizeof(Tag_Head));


			int lex2=OutLen;
			memcpy((pTagData+lex),pIfo,OutLen);


			flv_packet flvpacket;
			flvpacket.buf=(unsigned char*)pTagData;
			flvpacket.bufLen=TagDataLen;
			flvpacket.taglen=OutLen+11;
			return flvpacket;
			/**/
		//	MetaData.nVideoFmt=
			
			
			
			
			/*int OutLen;
			void *pIfo=flvEc.CreateFLVMetaData(&MetaData,OutLen);

			KKMEDIA::FLV_TAG_HEADER Tag_Head;
			memset(&Tag_Head,0,sizeof(Tag_Head));
			Tag_Head.TagType=0x12;
			flvEc.FlvMemcpy(&Tag_Head.TagDtatLen,3,&OutLen,3);
			fwrite(&Tag_Head,sizeof(KKMEDIA::FLV_TAG_HEADER), 1, local_recod->fp_h264_420_major);
			fwrite(pIfo,OutLen, 1, local_recod->fp_h264_420_major);
			PreTagLen=OutLen+11;*/
	}
	flv_packet FlvEncode::GetVideoPacket(const unsigned  char *buf,int bufsize,unsigned int pts,int &nPreTagLen)
	{
		    flv_packet flvpacket;
		    int TagDataLen=bufsize+1000;
            char *pTagData =(char *)::malloc(TagDataLen);
		    unsigned char* pDataNALU=(unsigned char*)buf;
	           
			unsigned char NALUType=*pDataNALU&0x1f;
			unsigned char aud=0;
			int NaluLen=0;

			char AVCPacket[5]={0x00};
			char H264Type=0x00;
		    char AVType=0xFF;
			int RemainLen=bufsize;

           if(NALUType==0x05){
				H264Type=0x05;
				AVType=0x17;//关键帧
				AVCPacket[0]=0x01;
			}else if(NALUType==0x41){
					H264Type=0x41;
					AVType=0x27;       //P帧 重要         type = 1 
					AVCPacket[0]=0x01;
			}else if(NALUType==0x01)
			{
				 H264Type=0x00;
				 AVType=0x27; 	// B帧     不重要        type = 1 
				 AVCPacket[0]=0x01;
			}else if(*pDataNALU==0x06)//增强信息帧 sei
			{
			
			}else {
				//NaluLen= GetNALULen(pDataNALU,RemainLen,&RemainLen);
			}
			KKMEDIA::FLV_TAG_HEADER Tag_Head;
			memset(&Tag_Head,0,sizeof(Tag_Head));
			//前一个Tag长度
			FlvMemcpy(&Tag_Head.PreTagLen,4,&nPreTagLen,4);
			FlvMemcpy(&Tag_Head.Timestamp,3,&pts,3);
			//flvEc.FlvMemcpy(&Tag_Head.streamID,3,&stream.wStreamId,3);

			KKMEDIA::FLV_TAG_HEADER *ppppp=&Tag_Head;
			Tag_Head.TagType=0x09;
            
			int datalen=0;
			memcpy(pTagData+datalen,&Tag_Head,sizeof(KKMEDIA::FLV_TAG_HEADER));
			datalen+=sizeof(KKMEDIA::FLV_TAG_HEADER);


			//AVCPacket
			{//视频类型
				AVCPacket[0]=AVType;
				AVCPacket[1]=0x01;
				memcpy(pTagData+datalen,AVCPacket,5);
				datalen+=5;
			}
			//AVC 格式 SEI 跳过
			if (H264Type==0x06)//SEI信息
			{
				/*memcpy(pTagData+datalen,seiLen,4);
				datalen+=4;
				memcpy(pTagData+datalen,pSeiData,SEILen);
				datalen+=datalen;*/
			}

			//NALU单元长度,大端
			FlvMemcpy(pTagData+datalen,4,&RemainLen, 4);
			datalen+=4;

			//NALU数据
			memcpy(pTagData+datalen,pDataNALU, RemainLen);
			datalen+=RemainLen;

			nPreTagLen=datalen-4;
			
			TagDataLen=datalen-15;//(11+4)
			//Tag数据区大小
			FlvMemcpy(pTagData+5,3,&TagDataLen,3);

			flvpacket.buf=(unsigned char*)pTagData;
			flvpacket.bufLen=datalen;
			flvpacket.taglen=nPreTagLen;
			return flvpacket;
	}
}