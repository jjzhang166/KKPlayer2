#include <string.h>
#include "../KKPlayer.h"
#include "live555rtsp.h"
struct live555_buf_info
{
    ourRTSPClient* client;
    live555_av_buf_* avbuf;
	int pos;
	int buflen;
};


static int live555_read_packet(void *opaque, uint8_t *buf, int buf_size)
{ 
  int nRead=0;
  KKPlugin* plu=(KKPlugin*)opaque;

  if(plu->opaque==NULL){
	  live555_buf_info *info=(live555_buf_info*)::malloc(sizeof(live555_buf_info));
	  memset(info,0,sizeof(live555_buf_info));
	  info->client=openlive555rtsp(plu->URL,"live555");
      plu->opaque=info;
  }

  live555_buf_info *info= (live555_buf_info *)plu->opaque;
  ourRTSPClient* client=info->client;
  bool wilx=true;
  if(info->buflen==0)
  {
	  if(info->avbuf)
	  {
		  free(info->avbuf->databuf);
		  free(info->avbuf);
          info->avbuf=0;
		  info->pos=0;
	  }
	  do{
	  
		  info->avbuf=client->m_avbuf_que.avbufpop();
		  
		  if( info->avbuf!=0)
		  {
			  info->buflen=info->avbuf->bulen;

			  wilx=false;
		  }
	  }while(wilx);
    
  }
  
   if(info->buflen!=0)
  {
	  if(buf_size>=info->buflen)
      {
	      memcpy(buf,info->avbuf->databuf,info->buflen);
		  nRead=info->buflen;
		  info->buflen=0;
	  }else{
		  int len=buf_size< info->buflen ? buf_size:info->buflen;
		  memcpy(buf,info->avbuf->databuf+info->pos,len);
		  info->pos+=len;
		  info->buflen-=len;
		  nRead=len;
	  }
  }
   return nRead;
}

static void  live555_WillClose(void *opaque)
{
	if(opaque!=0){
		
	}
}
void Del_live555_Plugin(KKPlugin* p)
{

}
extern "C"{
		KKPlugin* CreateLive555Plugin()
		{			
			KKPlugin* librtmp_plugin =(KKPlugin* ) ::malloc(sizeof(KKPlugin));
			memset(librtmp_plugin,0,sizeof(KKPlugin));
            librtmp_plugin->RealTime=1;
			librtmp_plugin->kkread=live555_read_packet;
			librtmp_plugin->kkPlayerWillClose=live555_WillClose;
			return  librtmp_plugin;
		}
};

void AddLive555PluginInfo()
{
	KKPluginInfo PluInfo;
	memset(&PluInfo,0,sizeof(KKPluginInfo));

	PluInfo.CreKKP=CreateLive555Plugin;
	PluInfo.DelKKp=Del_live555_Plugin;
	strcpy(PluInfo.ptl,"live555");
	KKPlayer::AddKKPluginInfo(PluInfo);
}