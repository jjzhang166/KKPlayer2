#include "live555_rec_avbuf_que.h"
#include "../Rtmp/FlvEncode.h"
static int GetNALULen(unsigned char *src,int srcLen,int *remainLen)
{
	int len=0;
	int Index=0;
	unsigned char* pdata=src;
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
	*remainLen=srcLen-len;
	return len;
}

static int  GetH264SeparatorLen(unsigned char *src,int srcLen)
{
	if(*src!=0x00)
		return 0;
	unsigned char* pdata=src;
		
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
live555_rec_avbuf_que::live555_rec_avbuf_que()
{
       m_nPreTagLen=0;
}
live555_av_buf_*  live555_rec_avbuf_que::avbufpop()
{
	live555_av_buf_* Item=NULL;
    m_Lock.Lock();
	if(av_buf_que.size()>0)
	{
		Item=av_buf_que.front();
		av_buf_que.pop();
	}
	m_Lock.Unlock();
	return Item;
}
FILE* m_pFlvFile=0;
void  live555_rec_avbuf_que::avbufpush(unsigned int pts,bool video,const char* buf,int bufsize)
{
	live555_av_buf_* item=(live555_av_buf_*)::malloc(sizeof(live555_av_buf_));
	
		


	if(video){
		       
				        unsigned char* pDataNALU=(unsigned char*)buf;
	           
						unsigned char NALUType=*pDataNALU&0x1f;
						unsigned char aud=0;
						
						if(m_nPreTagLen==0)
						{
							m_pFlvFile=fopen("d:\\test22.flv", "wb");;
							KKMEDIA::flv_packet pa=m_flvEc.GetFlvHeader();
							m_nPreTagLen=pa.taglen;
                                    live555_av_buf_* item2=(live555_av_buf_*)::malloc(sizeof(live555_av_buf_));
									item2->databuf=pa.buf;
									item2->bulen=pa.bufLen;
									item2->isvideo=true;
									item2->pts=pts; 
									fwrite(item2->databuf,item2->bulen, 1, m_pFlvFile);
									m_Lock.Lock();
									av_buf_que.push(item2);
									m_Lock.Unlock();
						}

					
						if(NALUType==0x07)           //sequence parameter sps.
						{
							    int TagDataLen=bufsize+1000;
								char *pTagData =(char *)::malloc(TagDataLen);	
	                            char AVCPacket[5]={0x00};
								char H264Type=0x00;
								char AVType=0x17;
								int RemainLen=bufsize;
                                int NaluLen=0;
							    KKMEDIA::AVC_DEC_CON_REC avc_dec_con_rec;
								avc_dec_con_rec.cfgVersion=0x01;

								avc_dec_con_rec.reserved6_lengthSizeMinusOne2=0xFF;//11111,111
								avc_dec_con_rec.reserved3_numOfSPS5=0xE1;//000,00001->111,0,0001=
								avc_dec_con_rec.numOfPPS=1;

								unsigned char *temp=(unsigned char *)pDataNALU;
								avc_dec_con_rec.spsLength=m_flvEc.GetNALULen(temp,100);
								unsigned char *pSpsData=(unsigned char*)::malloc(avc_dec_con_rec.spsLength);
								//sps数据
								memcpy(pSpsData,temp,avc_dec_con_rec.spsLength);
								avc_dec_con_rec.sps=pSpsData;

								temp+=avc_dec_con_rec.spsLength;
								RemainLen-=avc_dec_con_rec.spsLength;
							    NaluLen=m_flvEc.GetH264SeparatorLen(temp,4);
								temp+=NaluLen;
                                RemainLen-=NaluLen;
								NALUType=*temp&0x1f;
								if(NALUType==0x08)     //picture parameter pps
								{
									    avc_dec_con_rec.ppsLength=GetNALULen(temp,RemainLen,&RemainLen);
										char *pPPSData=(char*)::malloc(avc_dec_con_rec.ppsLength);
										memcpy(pPPSData,temp,avc_dec_con_rec.ppsLength);
										avc_dec_con_rec.pps=pPPSData;
										temp+=avc_dec_con_rec.ppsLength;
										RemainLen-=avc_dec_con_rec.ppsLength;
								}
						

								KKMEDIA::FLV_TAG_HEADER Tag_Head;
								memset(&Tag_Head,0,sizeof(Tag_Head));
								m_flvEc.FlvMemcpy(&Tag_Head.PreTagLen,4,&m_nPreTagLen,4);

								KKMEDIA::FLV_TAG_HEADER *ppppp=&Tag_Head;
								Tag_Head.TagType=0x09;///视频
								Tag_Head.ExpandTimeStamp=0;

								///tag头+tag数据区的长度
								int TempLen=0;
								//Tag头
								memcpy(pTagData,&Tag_Head,sizeof(KKMEDIA::FLV_TAG_HEADER));
								TempLen+=sizeof(KKMEDIA::FLV_TAG_HEADER);


								memset(&AVCPacket,0,5);
								//视频类型
								AVCPacket[0]=AVType;//0x17
								memcpy(pTagData+TempLen,&AVCPacket,5);
								TempLen+=5;

								//Version
								memcpy(pTagData+TempLen,&avc_dec_con_rec.cfgVersion,1);
								TempLen+=1;

								char *pSps=(char*)avc_dec_con_rec.sps;
								avc_dec_con_rec.avcProfile=*(pSps+1);
								//profile
								memcpy(pTagData+TempLen,&avc_dec_con_rec.avcProfile,1);
								TempLen+=1;
								//profile compat
								avc_dec_con_rec.profileCompatibility=*(pSps+2);
								memcpy(pTagData+TempLen,&avc_dec_con_rec.profileCompatibility,1);
								TempLen+=1;
								//Level
								avc_dec_con_rec.avcLevel=*(pSps+3);
								memcpy(pTagData+TempLen,&avc_dec_con_rec.avcLevel,1);
								TempLen+=1;
								///* 6 bits reserved (111111) + 2 bits nal size length - 1 (11) */
								avc_dec_con_rec.reserved6_lengthSizeMinusOne2=0xFF;
								memcpy(pTagData+TempLen,&avc_dec_con_rec.reserved6_lengthSizeMinusOne2,1);
								TempLen+=1;
								//* 3 bits reserved (111) + 5 bits number of sps (00001) */
								avc_dec_con_rec.reserved3_numOfSPS5=0xe1;
								memcpy(pTagData+TempLen,&avc_dec_con_rec.reserved3_numOfSPS5,1);
								TempLen+=1;


								//sps长度
								//memcpy(pTagData+TempLen,&avc_dec_con_rec.spsLength,2);
								m_flvEc.FlvMemcpy(pTagData+TempLen,2,&avc_dec_con_rec.spsLength,2);
								TempLen+=2;
								//sps数据
								memcpy(pTagData+TempLen,avc_dec_con_rec.sps,avc_dec_con_rec.spsLength);
								TempLen+=avc_dec_con_rec.spsLength;

								//pps
								//个数 单位字节
								avc_dec_con_rec.numOfPPS=1;
								memcpy(pTagData+TempLen,&avc_dec_con_rec.numOfPPS,1);
								TempLen+=1;

								//pps 长度 大端，低位高字节
								//memcpy(pTagData+TempLen,&avc_dec_con_rec.ppsLength,2);
								m_flvEc.FlvMemcpy(pTagData+TempLen,2,&avc_dec_con_rec.ppsLength,2);
								TempLen+=2;
								//数据
								memcpy(pTagData+TempLen,avc_dec_con_rec.pps,avc_dec_con_rec.ppsLength);
								TempLen+=avc_dec_con_rec.ppsLength;

								m_nPreTagLen=TempLen-4;///（tag长度值）

								TagDataLen=TempLen-15;//(11+4(tag长度值+tag的头)

								//Tag 数据区长度
								m_flvEc.FlvMemcpy(pTagData+5,3,&TagDataLen,3);
								item->databuf=(unsigned char*)pTagData;
								 
								item->bulen=TempLen;
								item->isvideo=true;
								item->pts=pts; 
								fwrite(item->databuf, item->bulen, 1, m_pFlvFile);
								m_Lock.Lock();
								av_buf_que.push(item);
								m_Lock.Unlock();
                               
							
								///拷贝剩余的帧
								if(RemainLen>0)
								{
									NaluLen= m_flvEc.GetH264SeparatorLen(temp,4);
									temp+=NaluLen;
									RemainLen-=NaluLen;
									NALUType=*temp&0x1f;
									KKMEDIA::flv_packet pa=m_flvEc.GetVideoPacket(temp,RemainLen,pts,m_nPreTagLen);

                                    live555_av_buf_* item2=(live555_av_buf_*)::malloc(sizeof(live555_av_buf_));
									item2->databuf=pa.buf;
									item2->bulen=pa.bufLen;
									item2->isvideo=true;
									item2->pts=pts;

									fwrite(item2->databuf, item2->bulen, 1, m_pFlvFile);
									m_Lock.Lock();
									av_buf_que.push(item2);
									m_Lock.Unlock();
								}


						}else if(NALUType==0x05|| NALUType==0x41||NALUType==0x01)
						{
							        KKMEDIA::flv_packet pa=m_flvEc.GetVideoPacket(pDataNALU,bufsize,pts,m_nPreTagLen);

                                   
									item->databuf=pa.buf;
									item->bulen=pa.bufLen;
									item->isvideo=true;
									item->pts=pts;

									fwrite(item->databuf, item->bulen, 1, m_pFlvFile);
									m_Lock.Lock();
									av_buf_que.push(item);
									m_Lock.Unlock();

						}else if(*pDataNALU==0x06)//增强信息帧 sei
						{
						
						}else {
							//NaluLen= GetNALULen(pDataNALU,RemainLen,&RemainLen);
						}
						

	}
	
}




//int RemainLen=bufsize;
//				unsigned char* pDataNALU=(unsigned char*)buf;
//	            while(RemainLen!=0)
//				{
//						int SepLen= GetH264SeparatorLen(pDataNALU,4);
//						char H264Type=0x00;
//				
//						pDataNALU=(pDataNALU+SepLen);
//						RemainLen=RemainLen-SepLen;
//						unsigned char NALUType=*pDataNALU&0x1f;
//						unsigned char aud=0;
//
//						int NaluLen=0;
//						if(NALUType==0x07)           //sequence parameter sps.
//						{
//							NaluLen= GetNALULen(pDataNALU,RemainLen,&RemainLen);
//						
//						}else if(NALUType==0x08)     //picture parameter pps
//						{
//							NaluLen= GetNALULen(pDataNALU,RemainLen,&RemainLen);
//						}else(NALUType==0x05){
//							//H264Type=0x05;
//							//AVType=0x17;//关键帧
//							//AVCPacket[0]=0x01;
//						}else if(NALUType==0x41){
//								//H264Type=0x41;
//								//AVType=0x27;       //P帧 重要         type = 1 
//								//AVCPacket[0]=0x01;
//						}else if(NALUType==0x01)
//						{
//							 //H264Type=0x00;
//							 //AVType=0x27; 	// B帧     不重要        type = 1 
//							 //AVCPacket[0]=0x01;
//						}else if(*pDataNALU==0x06)//增强信息帧 sei
//						{
//						
//						}else {
//							NaluLen= GetNALULen(pDataNALU,RemainLen,&RemainLen);
//						}
//						pDataNALU=(pDataNALU+NaluLen);
//						
//				}