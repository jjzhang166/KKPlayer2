#include "AvTransferAdapter.h"
#include "../Control/kkmclv.h"
#include "../KKPlayerCore/KKPlayer.h"
#include "../MainPage/AVTransferDlg.h"
#include "json/json.h"
#include "../Tool/cchinesecode.h"
#include "../Tool/curlcode.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <math.h>
#include <iosfwd>
#include <shellapi.h>
namespace SOUI
{
	char strabce[64]="";
	class DataItem
	{
		public:
			DataItem()
			{
				 speed=0;
				 progress=0;
				 filesize=0;
				  Url=L"";
			}
			~DataItem(){}
		   int  speed;
		   long long  progress;
		   long long  filesize;
		   std::wstring  Url;
		   std::string   srcUrl;
		   ///如果支持文件存储路径
		   std::wstring   savefilepath;
		   char ptl[32];
	};
	static std::vector<DataItem> DataItemVector;

	std::string format_thousands_separator(double val)
{
	std::stringstream stream;
	std::string strRet;
	stream.clear();
	stream.imbue(std::locale("chs"));
	stream << std::setprecision(3)<< val;//<<std::fixed << val;
	stream >> strRet;
	return strRet;//std::move(strRet);
}

	char * GetFormtDByMB(char *strabce,long long FileSize)
	{
		
		double dfileSize=0;
		if(FileSize>1024*1024*1024)
		{
			dfileSize=(double)FileSize/(1024*1024*1024);
			sprintf(strabce,"%sGb",format_thousands_separator(dfileSize).c_str());

		}else if(FileSize>1024*1024)
		{
			dfileSize=(double)FileSize/(1024*1024);
			sprintf(strabce,"%sMb",format_thousands_separator(dfileSize).c_str());
		}else{
			dfileSize=(double)FileSize/1024;
			dfileSize+=(FileSize%1024)/1024;
			sprintf(strabce,"%sKb",format_thousands_separator(dfileSize).c_str());
		}
		return strabce;
	}
	



	char * GetFormtDBySpeed(char *strabce,int FileSize)
	{

		double dfileSize=0;
		if(FileSize>1024*1024)
		{
			dfileSize=FileSize/(1024*1024);
			sprintf(strabce,"%s Mb/s",format_thousands_separator(dfileSize).c_str());
		}else if(FileSize>1024){
			dfileSize=(double)FileSize/1024;
			sprintf(strabce,"%s Kb/s",format_thousands_separator(dfileSize).c_str());
		}else{
			sprintf(strabce,"%d B/s",FileSize);
			
		}
		return strabce;
	}
	CDownAVListMcAdapterFix::CDownAVListMcAdapterFix(IMainUI* pIMainUI,SMCListView *pSMCListView,std::string url):
	m_strurl(url),m_pIMainUI(pIMainUI)
	{
	    m_pSMCListView=pSMCListView;
	}
	CDownAVListMcAdapterFix::~CDownAVListMcAdapterFix()
	{
	
	}
	void  CDownAVListMcAdapterFix::Refresh()
	{
	     std::list<KKPluginInfo>  &PlList=KKPlayer::GetKKPluginInfoList();
		 std::list<KKPluginInfo>::iterator It=PlList.begin();
		  wchar_t temp[2048]=L"";
		  DataItemVector.clear();
		 for(;It!=PlList.end();++It)
		 {
			 if(It->KKAllAVFilesSpeedInfo){
				 char *Out=0;
				 It->KKAllAVFilesSpeedInfo(&Out);
				
				 if(Out){

					 Json::Value jsonValue;
					 Json::Reader jsonReader;
					 if(jsonReader.parse(Out,jsonValue))
					 {
						for(int i=0;i<jsonValue.size();i++)
						{
						     Json::Value  speedinfo=jsonValue[i]["speedinfo"];
							 std::string url=jsonValue[i]["url"].asString();
                             memset(temp,0,2048);
							 CChineseCode::charTowchar(url.c_str(),temp, 2048) ; /**/

							 DataItem Item;
							 Item.Url=temp;
							 Item.srcUrl=url;
							 strcpy(Item.ptl,It->ptl);
							 Item.filesize=speedinfo["filesize"].asInt64();
							 Item.speed=speedinfo["speed"].asInt();
							 Item.progress=speedinfo["progress"].asInt64();/* */

							 url=speedinfo["filepath"].asString();
							 url=CURLCode::UrlDecode(url);
							 memset(temp,0,2048);
							 CChineseCode::charTowchar(url.c_str(),temp, 2048) ; /**/
							 Item.savefilepath=temp;
							 DataItemVector.push_back(Item);
						}
					 }
					 It->KKFree(Out);
				 }
			 }
		 }
	}
	int CDownAVListMcAdapterFix::getCount()
	{
		// Refresh();
	     return DataItemVector.size();
	}
    SStringT CDownAVListMcAdapterFix::getSizeText(DWORD dwSize)
	{
	        int num1=dwSize/(1<<20);
			dwSize -= num1 *(1<<20);
			int num2 = dwSize*100/(1<<20);
			return SStringT().Format(_T("%d.%02dM"),num1,num2);
	}
	bool CDownAVListMcAdapterFix::OnItemClick(EventArgs *pEvt)
	{
	      /*  int index=m_pSMCListView->GetSel();
			SItemPanel * pOldSItme=m_pSMCListView->GetItem(index);
			
			SItemPanel * pSItme=(SItemPanel *)pEvt->sender;*/
			return true;
	}
	bool CDownAVListMcAdapterFix::OnItemDelete(EventArgs *pEvt)
	{
		SWindow *pBtn=(SWindow *)pEvt->sender;
		int Id=pBtn->GetUserData();
		if(Id<DataItemVector.size()){
			DataItem &Item=DataItemVector.at(Id);

			   std::list<KKPluginInfo>  &PlList=KKPlayer::GetKKPluginInfoList();
		       std::list<KKPluginInfo>::iterator It=PlList.begin();
		       for(;It!=PlList.end();++It){
				   
                  if( !strncmp(Item.ptl,It->ptl,strlen(Item.ptl)))
				  {

					  std::string temp=Item.ptl;
					  temp+=":";
					  temp+=Item.srcUrl;
					  if(!strncmp(temp.c_str(),m_strurl.c_str(),strlen(m_strurl.c_str())))
					  {
					     m_pIMainUI-> CloseMedia();
					  }
					  if(It->KKDelDownAVFile)
					  {
					      It->KKDelDownAVFile(Item.srcUrl.c_str(),0);
					  }
					  return true;
				  }
		       }
		}
	  return true;
	}
    bool  CDownAVListMcAdapterFix::OnItemPause(EventArgs *pEvt)
	{
		SWindow *pBtn=(SWindow *)pEvt->sender;
        int Id=pBtn->GetUserData();
		if(Id<DataItemVector.size()){
			 DataItem &Item=DataItemVector.at(Id);
			 
			
			 SStringT tiptxt=pBtn->GetToolTipText();
			 bool pause=1;
			 if(tiptxt==L"暂停下载")
			 {
			      pBtn->SetAttribute(L"skin",L"_skin.1PLAY");
                  pBtn->SetToolTipText(L"开始下载");
				  pause=true;
			 }else{
			     pBtn->SetAttribute(L"skin",L"_skin.1PAUSE");
				 pBtn->SetToolTipText(L"暂停下载");
				 pause=false;
			 }

			   std::list<KKPluginInfo>  &PlList=KKPlayer::GetKKPluginInfoList();
		       std::list<KKPluginInfo>::iterator It=PlList.begin();
		       for(;It!=PlList.end();++It){
				   
                  if( !strncmp(Item.ptl,It->ptl,strlen(Item.ptl)))
				  {
					  if(It->KKDelDownAVFile)
					  {
						  It->KKPauseDownAVFile(Item.srcUrl.c_str(),pause);
					    //  It->KKDelDownAVFile(Item.srcUrl.c_str(),0);
					  }
					  return true;
				  }
		       }
		}
	    return true;
	}
	bool  CDownAVListMcAdapterFix::OnItemOpenLocal(EventArgs *pEvt)
    {
        SWindow *pBtn=(SWindow *)pEvt->sender;
        int Id=pBtn->GetUserData();
		if(Id<DataItemVector.size()){
			DataItem &Item=DataItemVector.at(Id);

			std::wstring temp=L"/select, ";
			temp+=Item.savefilepath;
			ShellExecute(NULL,NULL,_T("explorer"),temp.c_str(),NULL,SW_SHOW); 
		}
	   return true;
	}
	void CDownAVListMcAdapterFix::getView(int position, SWindow * pItem,pugi::xml_node xmlTemplate)
	{
		int len=2*1024*4;
		char *temp2=(char*)::malloc(4096);
		wchar_t  *temp = (wchar_t  *)::malloc(len);
	    if(pItem->GetChildrenCount()==0)
		{
			pItem->InitFromXml(xmlTemplate);
		}
	   if(DataItemVector.size()>position)
	   {
			 DataItem &Item=DataItemVector.at(position);
			 SWindow  *pWin= pItem->FindChildByName(L"txt_name");
			 if(pWin)
			 {
				 int index= Item.Url.find_last_of (_T("\\"));
				if(index<0)
				{
					index=Item.Url.find_last_of(L"/");
				}
				index+=1;
				Item.Url=Item.Url.substr(index,Item.Url.length()-index);
				pWin->SetWindowText(Item.Url.c_str());
			 }
	         
			 
			 
			 pWin= pItem->FindChildByName(L"txt_size");
			 if(pWin){
				 memset(temp,0,len);
				 
				 memset(temp2,0,4096);
				 GetFormtDByMB(temp2,Item.filesize);
				 CChineseCode::charTowchar(temp2,temp,1024) ; /**/
				 SStringT Tip=temp;

				 GetFormtDByMB(temp2,Item.progress);
				 CChineseCode::charTowchar(temp2,temp,1024) ; /**/
                 Tip=_T("/")+Tip;
				 Tip=temp+Tip;
				 pWin->SetWindowText(Tip);
                 
			 }
			 pWin= pItem->FindChildByName(L"txt_speed");
			 if(pWin){
				 memset(temp,0,len);	 
				 memset(temp2,0,4096);
				 GetFormtDBySpeed(temp2,Item.speed);
				 CChineseCode::charTowchar(temp2,temp,1024) ; /**/
				 pWin->SetWindowText(temp);
			 } 
             pWin= pItem->FindChildByName(L"btn_pause");
			 if(pWin){
				 pWin->SetUserData(position);
				 pWin->GetEventSet()->subscribeEvent(EVT_CMD, Subscriber(&CDownAVListMcAdapterFix::OnItemPause,this));
			 }

             pWin= pItem->FindChildByName(L"btn_delete");
			 if(pWin){
				     pWin->SetUserData(position);
					 pWin->GetEventSet()->subscribeEvent(EVT_CMD, Subscriber(&CDownAVListMcAdapterFix::OnItemDelete, this));
			 }

             pWin= pItem->FindChildByName(L"btn_openlocate");
			 if(pWin){
				 pWin->SetUserData(position);
				 if(Item.savefilepath.length()>2)
				 {
				    pWin->SetAttribute(L"tip",Item.savefilepath.c_str());
					pWin->SetVisible(1);
					pWin->GetEventSet()->subscribeEvent(EVT_CMD, Subscriber(&CDownAVListMcAdapterFix::OnItemOpenLocal,this));
				 }else{
				    pWin->SetVisible(0);
					//ShellExecute(NULL,NULL,_T("explorer"), "/select, "+strFilePath,NULL,SW_SHOW); 
				 }
			 }
			 
	   }
	   ::free(temp2);
	   ::free(temp);
	   return ;
	}
    bool CDownAVListMcAdapterFix::OnButtonClick(EventArgs *pEvt)
	{
	   return 1;
	}
	SStringW CDownAVListMcAdapterFix::GetColumnName(int iCol) const
	{
	  return SStringW().Format(L"col%d",iCol+1);
	}
	bool CDownAVListMcAdapterFix::OnSort(int iCol,SHDSORTFLAG * stFlags,int nCols)
	{
	   return true;
	}
}