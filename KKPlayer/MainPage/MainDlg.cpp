// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include <dwmapi.h>
#include <atldlgs.h>
#include "SuiVideo.h"
#include "../Tool/cchinesecode.h"
#include <control/SCmnCtrl.h>
#include <control/SSliderBar.h>
#pragma comment(lib,"dwmapi.lib")
namespace SOUI
{
	CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
	{
		m_bLayoutInited=FALSE;
		m_PlayerState=0;
	} 

	CMainDlg::~CMainDlg()
	{
	}

	int CMainDlg::OnCreate( LPCREATESTRUCT lpCreateStruct )
	{
		// 		MARGINS mar = {5,5,30,5};
		// 		DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
		SetMsgHandled(FALSE);
		
		
		return 0;
	}

	void CMainDlg::OnShowWindow( BOOL bShow, UINT nStatus )
	{
		if(bShow)
		{
 			//AnimateHostWindow(200,AW_CENTER);
		}
	}


	BOOL CMainDlg::OnInitDialog( HWND hWnd, LPARAM lParam )
	{
		m_bLayoutInited=TRUE;

        SetMsgHandled(FALSE);
		//SSliderBar *VolBar=(SSliderBar *)FindChildByName(L"AvAudio");
		//bool okx=VolBar->GetEventSet()->subscribeEvent(SSliderBar::SC_THUMB,Subscriber(&CMainDlg::OnSliderVideo,this));

		return 0;
	}
	void CMainDlg::OnAVPlay()
	{
		SImageButton* pPlay=(SImageButton*)FindChildByName(L"AVPlayBtn");
		CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
		if(m_PlayerState==1){
            pPlay->SetAttribute(L"skin",L"_skin.PLAY",TRUE);
			av->SetPlayStat(1);
			pPlay->UpdateWindow();
			m_PlayerState=2;
		}else if(m_PlayerState==2){
                pPlay->SetAttribute(L"skin",L"_skin.Pause",TRUE);
				av->SetPlayStat(1);
				m_PlayerState=1;
				pPlay->UpdateWindow();
		}

	}
	void CMainDlg::OnTimer(char cTimerID)
	{
        if(m_PlayerState!=0)
		{
             CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
			 MEDIA_INFO info=av->GetMeadiaInfo();
			 SSliderBar *SeekBar=(SSliderBar *)FindChildByName(L"slider_video_Seek");
			 
			 std::wstring CurTimeStr;
			
			 int h=(info.CurTime/(60*60));
			 int m=(info.CurTime%(60*60))/(60);
			 int s=((info.CurTime%(60*60))%(60));
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
			 CurTimeStr+=L"/";

			

			 
			  int Min=0;
			  int Max=0;
			  SeekBar->GetRange(&Min,&Max);
			  static std::wstring hh=L"00:00:00";
			  if(info.TotalTime>0)
			  {
                  SeekBar->SetRange(0,info.TotalTime);
				  h=info.TotalTime/(60*60);
				  m=(info.TotalTime%(60*60))/60;
				  s=((info.TotalTime%(60*60))%60);


				  if(h<10)
				  {
					  swprintf_s(strtmp,L"0%d:",h);
					  hh=strtmp;
				  }
				  else{
					  swprintf_s(strtmp,L"%d:",m);
					  hh=strtmp;
				  }
				  if(m<10){
					  swprintf_s(strtmp,L"0%d:",m);
					  hh+=strtmp;
				  }
				  else{
					  swprintf_s(strtmp,L"%d:",m);
					 hh+=strtmp;
				  }
				  if(s<10){
					  swprintf_s(strtmp,L"0%d:",s);
					 hh+=strtmp;
				  }
				  else{
					  swprintf_s(strtmp,L"%d",s);
					  hh+=strtmp;
				  }
			  }
			  if(info.CurTime>m_CumrTime|| m_SeekTimer>1)
			  {
			     
                 SeekBar->SetValue(info.CurTime);
				 m_CumrTime=info.CurTime;
			  }
			  m_SeekTimer++;
			  CurTimeStr+=hh;
			  SStatic* AV_CurTimeTxt=( SStatic  *)FindChildByName(L"AV_CurTimeTxt");
			  AV_CurTimeTxt->SetWindowText(CurTimeStr.c_str());

			  
		}
	}
	bool  CMainDlg::OnSliderVideo(EventArgs *pEvt)
	{
		EventSliderPos *pEvt2 = sobj_cast<EventSliderPos>(pEvt);
		SASSERT(pEvt2);
		SSliderBar * pSlider = sobj_cast<SSliderBar>(pEvt->sender);
		SASSERT(pSlider);


		SSliderBar *VolBar=(SSliderBar *)FindChildByName(L"slider_video");
		if(m_PlayerState!=0)
		{
			m_SeekTimer=0;
			int vol=pEvt2->nPos;//-m_CumrTime;
			m_CumrTime=pEvt2->nPos;
			CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
			av->AvSeek(vol);
		}/**/
		return true;
	}
	bool CMainDlg::OnSliderAudio(EventArgs *pEvt)
	{
		EventSliderPos *pEvt2 = sobj_cast<EventSliderPos>(pEvt);
		SASSERT(pEvt2);
		SSliderBar * pSlider = sobj_cast<SSliderBar>(pEvt->sender);
		SASSERT(pSlider);


		SSliderBar *VolBar=(SSliderBar *)FindChildByName(L"slider_video");
		if(m_PlayerState!=0)
		{
			long vol=pEvt2->nPos*10 -10000;
			CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
			av->SetVolume(vol);
		}/**/
		return true;
	}
    void CMainDlg::OnFolder()
	{
		m_CumrTime=0;
		SStatic* AV_CurTimeTxt=( SStatic  *)FindChildByName(L"AV_CurTimeTxt");
		AV_CurTimeTxt->SetWindowText(L"00:00:00/00:00:00");

		SSliderBar *SeekBar=(SSliderBar *)FindChildByName(L"slider_video_Seek");
		SeekBar->SetRange(0,10000);
		SeekBar->SetValue(0);
		wchar_t* filter = L"文件(*.mp4; *.avi; *.flv)\0*.mp4;*.avi; *.rmvb;*.flv\0全部 (*.*)\0*.*\0\0";  
		CFileDialog dlg(true, 0, 0, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST, filter, m_hWnd);  
		if(dlg.DoModal() == IDOK)
		{  
			CSuiVideo* av=(CSuiVideo*)FindChildByName(L"AVMainWnd");
			char path[1024]="";
			CChineseCode::wcharTochar(dlg.m_szFileName,path,1024);
			this->KillTimer(1);
			int ret=av->OpenMeida(path);
			if(ret==-1)
			{
			     MessageBox(m_hWnd, L"文件打开错误", 0, MB_OK); 
			     m_PlayerState=0;
			     SImageButton* pPlay=(SImageButton*)FindChildByName(L"AVPlayBtn");
			     pPlay->SetAttribute(L"skin",L"_skin.PLAY",TRUE);
				 pPlay->UpdateWindow();
			}else
			{

				// SeekBar->SetRange(0,1);
				 m_PlayerState=1;
                 SImageButton* pPlay=(SImageButton*)FindChildByName(L"AVPlayBtn");
				 pPlay->SetAttribute(L"skin",L"_skin.Pause",TRUE);
				 pPlay->UpdateWindow();
				 this->SetTimer(1,1000);
			}
			
		}  
	}
	void CMainDlg::OnFileList()
	{
		MessageBox(m_hWnd, L"此功能暂时未实现", 0, MB_OK);  
	}
}