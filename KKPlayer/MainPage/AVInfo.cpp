#include "AVInfo.h"
#include "MainDlg.h"
#include "../Tool/cchinesecode.h"
namespace SOUI
{
	CAVInfo::CAVInfo(MEDIA_INFO Info): SDomodalwnd(_T("LAYOUT:XML_AVINFO"),0)
		,m_Info(Info)
	{
           
	}
	CAVInfo::~CAVInfo()
	{
        
	}
	void CAVInfo::Init()
	{
	     SetAVInfo(m_Info);
	}
	void  CAVInfo::SetAVInfo(MEDIA_INFO &Info)
	{
		//
		//richedit
        SRichEdit* pSRichEdit= (SRichEdit*)this->FindChildByName("EditAVInfo");
		
		char strtemp[1024]="";

		if(strlen(Info.videoinfo.codecname)>1&&strlen(Info.audioinfo.codecname)>1)
		{
		    ::sprintf(strtemp,"视频流信息:\n+视频编码:%s\n+平均码率:%d\n+视频帧率:%d\n\n音频流信息:\n+音频编码:%s\n+平均码率:%d\n+采样帧率:%d Hz\n+声 道 数:%d",
			Info.videoinfo.codecname,Info.videoinfo.bitrate,Info.videoinfo.framerate,
            Info.audioinfo.codecname,Info.audioinfo.bitrate,Info.audioinfo.sample_rate, Info.audioinfo.channels
			);
		}else if(strlen(Info.videoinfo.codecname)>1)
		{
		   ::sprintf(strtemp,"视频流信息:\n+视频编码:%s\n+平均码率:%d\n+视频帧率:%d",
			Info.videoinfo.codecname,Info.videoinfo.bitrate,Info.videoinfo.framerate
			);
		}else if(strlen(Info.audioinfo.codecname)>1){
		   ::sprintf(strtemp,"音频流信息:\n+音频编码:%s\n+平均码率:%d\n+采样帧率:%d Hz\n+声 道 数:%d",
            Info.audioinfo.codecname,Info.audioinfo.bitrate,Info.audioinfo.sample_rate, Info.audioinfo.channels
			);
		}
		
		wchar_t abcd[1024]=L"";
		CChineseCode::charTowchar(strtemp,abcd,1024);
		pSRichEdit->SetWindowText(abcd);

      
		CChineseCode::charTowchar(Info.AVRes,abcd,1024);
		this->FindChildByName("TxtAvRes")->SetWindowText(abcd);


		std::wstring CurTimeStr;
		int h=(Info.TotalTime/(60*60));
		int m=(Info.TotalTime%(60*60))/(60);
		int s=((Info.TotalTime%(60*60))%(60));
		wchar_t strtmp[100]=L"";
		if(h<10){
			swprintf_s(strtmp,L"0%d:",h);
			CurTimeStr=strtmp;
		}
		else{
			swprintf_s(strtmp,L"%d:",h);
			CurTimeStr=strtmp;
		}
		if(m<10){
			swprintf_s(strtmp,L"0%d:",m);
			CurTimeStr+=strtmp;
		}
		else{				  
			swprintf_s(strtmp,L"%d:",m);
			CurTimeStr+=strtmp;
		}

		if(s<10){
			swprintf_s(strtmp,L"0%d",s);
			CurTimeStr+=strtmp;
		}
		else{
			swprintf_s(strtmp,L"%d",s);
			CurTimeStr+=strtmp;
		}

		
		this->FindChildByName("TxtAvTime")->SetWindowText(CurTimeStr.c_str());


		if(Info.AvFile!=NULL){
			CChineseCode::charTowchar(Info.AvFile,abcd,1024);
			this->FindChildByName("TxtFilePath")->SetWindowText(abcd);
		}
		
	    CurTimeStr=abcd;
		int index=CurTimeStr.find_last_of(L".");
		if(index>-1)
		{
            CurTimeStr=CurTimeStr.substr(index+1,CurTimeStr.length()-index-1);
			this->FindChildByName("AVFileType")->SetWindowText(CurTimeStr.c_str());
		}
		

		if(Info.FileSize>1024*1024*1024)
		{
			swprintf_s(abcd,L"%.3fGb", (double)Info.FileSize/1024/1024/1024);
		}else
		if(Info.FileSize>1024*1024)
		{
			swprintf_s(abcd,L"%dMb", Info.FileSize/1024/1024);
		}else
		{
             swprintf_s(abcd,L"%dKb", Info.FileSize/1024);
		}
		
		this->FindChildByName("TxtFileLen")->SetWindowText(abcd);
		
		
	}
	void CAVInfo::OnLButtonDown(UINT nFlags, CPoint point)
	{
		
		 this->OnMouseEvent(WM_LBUTTONDOWN,nFlags,MAKELPARAM(point.x,point.y));
		
         //m_pDlgMain->ShowMiniUI(false);
	}

	void CAVInfo::OnClose()
	{
		this->PostMessage(WM_CLOSE,0,0);
	}
	void CAVInfo::OnMinimize()
	{
		
	}
}