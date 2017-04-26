// MangageTrade.cpp : main source file for MangageTrade.exe
//

#include "stdafx.h"

#include "resource.h"
#include "MainFrm.h"
#include <GdiPlus.h>
#include <Gdiplusinit.h>
#include "MainPage/MainDlg.h"
#include "MainPage/SUIVideo.h"
#include "MainPage/KKWkeWebkit.h"
#include "control/kkmclv.h"
#include "Control/kkseek.h"
//#include "DownManage/AVDownManage.h"
#include "Dir/Dir.hpp"
#include "Tool/CFileMgr.h"
#include "Tool/cchinesecode.h"
#include "Tool/FileRelation.h"
#include "../KKPlayerCore/KKPlugin.h"
#include "../KKPlayerCore/KKPlayer.h"
#include "../KKPlayerCore/SqlOp/AVInfomanage.h"
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"Kernel32.lib")

void DeclareDumpFile();
CAppModule _Module;

std::basic_string<TCHAR> g_strModuleFileName;
SOUI::CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
const std::basic_string<TCHAR>& XGetModuleFilename()
{
	if (g_strModuleFileName.empty()){
		if(g_strModuleFileName.empty()){
			TCHAR filename[MAX_PATH] = { 0 };
			::GetModuleFileName(NULL, filename, _countof(filename));
			g_strModuleFileName = filename;
		}
	}
	
	return g_strModuleFileName;
}
std::basic_string<TCHAR> GetModulePath()
{
	std::basic_string<TCHAR> strModuleFileName = XGetModuleFilename();
	unsigned int index = strModuleFileName.find_last_of(_T("\\"));
	if (index != std::string::npos)
	{
		return strModuleFileName.substr(0, index);
	}
	return _T("");
}


HWND G_Parent=NULL;
CMainFrame *pWnd;
int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);
	int nRet = theLoop.Run();
    _Module.RemoveMessageLoop();
	
	return nRet;
}

#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif
SOUI::CMainDlg *m_pDlgMain=NULL;

//F:\ProgramTool\OpenPro\KKPlayer\KKPlayer>uiresbuilder.exe -iuires/uires.idx -puires -rres/KK_res.rc2
std::basic_string<char> GetModulePathA();
void skpngZhuc();



CreateRender pfnCreateRender = NULL;
DelRender pfnDelRender=NULL;
SOUI::SApplication *theApp=NULL;
HINSTANCE GhInstance;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{

	LPWSTR *szArglist = NULL;   
	int nArgs = 0;   
	std::wstring  urlpath;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);   
	if( NULL != szArglist)   
	{   
         for(int i=1;i<nArgs;i++)
		 {
		     urlpath+=*(szArglist+i);
			 if(i!=nArgs-1)
			   urlpath+=L" ";
		 }
	} 
	/*if(urlpath.length()>1)
		::MessageBox(NULL,urlpath.c_str(),L"xxx",0);*/
	LocalFree(szArglist);

    DeclareDumpFile();
    
    GhInstance=hInstance;
	FARPROC spdpia = GetProcAddress(GetModuleHandle(TEXT("user32")), "SetProcessDPIAware");  
	if(spdpia!=NULL)
	{
	   spdpia();
	}

	std::wstring Propath=GetModulePath();
	Propath+=L"\\Db";
	CFileMgr mgr;
	mgr.CreateDirectory(Propath.c_str());
	Propath+=L"\\mv";
	std::string pp;
	CChineseCode::UnicodeToUTF8((wchar_t*)Propath.c_str(),pp);

	CAVInfoManage AVInfoManage;
	AVInfoManage.SetPath((char *)pp.c_str());
	AVInfoManage.InitDb();


	CFileRelation FileRelation;
	bool xx=FileRelation.CheckFileRelation(L".mp4",L"KKPlayer.mp4");
	if( 1){
		Propath=GetModulePath();
        Propath+=_T("\\KKplayer.exe");
		FileRelation.RegisterFileRelation(_T(".mp4"),(LPTSTR)Propath.c_str(),_T("KKPlayer.mp4"),(LPTSTR)Propath.c_str(),_T(".mp4"));
	}
	HMODULE hRender = LoadLibraryA("Render.dll");
	if(hRender){
          pfnCreateRender = (CreateRender)GetProcAddress(hRender, "CreateRender");
		  pfnDelRender = (DelRender)GetProcAddress(hRender, "DelRender");
	}
	//装载
	std::string strPath= GetModulePathA();
	strPath+="\\Plugin";

	std::list<std::string> DllPathInfoList;
	dir::listFiles(DllPathInfoList,strPath,"dll");
	std::list<std::string>::iterator It=DllPathInfoList.begin();
	int Lenxx=sizeof( __KKPluginInfo);
	for (;It!=DllPathInfoList.end();++It)
	{

		//char ptl[32];
		///******创建一个插件******/
		//fCreateKKPlugin CreKKP;
		///**********删除一个插件**************/
		//fDeleteKKPlugin DelKKp;
		///***********下载文件*****************/
		//fKKDownAVFile   KKDownAVFile;
		///************停止下载*****************/
		//fKKStopDownAVFile KKStopDownAVFile;
		//fKKDownAVFileSpeedInfo KKDownAVFileSpeed;
		//fFree KKFree;

		HMODULE	hdll= LoadLibraryA((*It).c_str());
		fCreateKKPlugin        pfn = (fCreateKKPlugin)GetProcAddress(hdll, "CreateKKPlugin");
		fGetPtlHeader          pfGetPtl=(fGetPtlHeader)GetProcAddress(hdll, "GetPtlHeader");
		fDeleteKKPlugin        pDel=(fDeleteKKPlugin)GetProcAddress(hdll, "DeleteKKPlugin");
        fKKDownAVFile          pKKDownAVFile=(fKKDownAVFile)GetProcAddress(hdll, "KKDownAVFile");
		fKKStopDownAVFile      pKKStopDownAVFile=(fKKStopDownAVFile)GetProcAddress(hdll, "KKStopDownAVFile");
		fFree                  pKKFree=(fFree)GetProcAddress(hdll, "KKFree");
        fKKDownAVFileSpeedInfo pKKDownAVFileSpeedInfo=(fKKDownAVFileSpeedInfo)GetProcAddress(hdll, "KKDownAVFileSpeedInfo");
        fKKUrlParser           pKKUrlParser=(fKKUrlParser)GetProcAddress(hdll, "KKUrlParser");
		if(pfn!=NULL&&pfGetPtl!=NULL&& pDel!=NULL)
		{
			
				KKPluginInfo Info;
				pfGetPtl(Info.ptl,32);
				Info.CreKKP= pfn;
				Info.DelKKp=pDel;
                Info.Handle=hdll;
				Info.KKDownAVFile=pKKDownAVFile;
				Info.KKStopDownAVFile=pKKStopDownAVFile;
				Info.KKFree=pKKFree;
				Info.KKDownAVFileSpeedInfo=pKKDownAVFileSpeedInfo;
				Info.KKUrlParser=pKKUrlParser;
				KKPlayer::AddKKPluginInfo(Info);
			
		}else{
			FreeLibrary(hdll);
		}

		int i=0;
		i++;
	}

	int ll=sizeof(ULONG_PTR);
	HRESULT hRes = ::CoInitialize(NULL);
    G_Parent =(HWND)_wtoi(lpstrCmdLine);
	wchar_t abcd[100]=L"";
	wsprintf(abcd,L"%d",G_Parent);
	ATLASSERT(SUCCEEDED(hRes));
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	//HWND h=CreateMessageWindow(hInstance);
	
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();
	Gdiplus::GdiplusStartupInput StartupInput; 
	ULONG_PTR m_gdiplusToken;
	GdiplusStartup(&m_gdiplusToken,&StartupInput,NULL); 


	using namespace SOUI;
	SComMgr * pComMgr = new SComMgr;

	SOUI::CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
	
	pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
    
	
	pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
	pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
	
	theApp=new SApplication(pRenderFactory,hInstance);
    theApp->RegisterWndFactory(TplSWindowFactory<CSuiVideo>());
    theApp->RegisterWndFactory(TplSWindowFactory<CKKmclv>());
    theApp->RegisterWndFactory(TplSWindowFactory<SAVSeekBar>());
	
	 KKWkeLoader wkeLoader;
	 if(wkeLoader.Init(_T("wke.dll")))
	 {
	   theApp->RegisterWndFactory(TplSWindowFactory<KKWkeWebkit>());
	 }

	

	HMODULE hui=LoadLibrary(_T("kkui.dll"));
	if(hui){
		CAutoRefPtr<IResProvider>   pResProvider;
		CreateResProvider(RES_PE,(IObjRef**)&pResProvider);
		BOOL ret=pResProvider->Init((WPARAM)hui,0);
		theApp->AddResProvider(pResProvider);
	}else{
       assert(0);
	}
	
	CAutoRefPtr<ITranslatorMgr> trans;
	pComMgr->CreateTranslator((IObjRef**)&trans);
	if(trans)
	{
	        theApp->SetTranslator(trans);
            pugi::xml_document xmlLang;
            if(theApp->LoadXmlDocment(xmlLang,_T("lang_cn"),_T("translator"))){
                CAutoRefPtr<ITranslator> langCN;
                trans->CreateTranslator(&langCN);
                langCN->Load(&xmlLang.child(L"language"),1);//1=LD_XML
                trans->InstallTranslator(langCN);
            }
	}


	//加载系统资源
	HMODULE hSysResource=LoadLibrary(SYS_NAMED_RESOURCE);
	if(hSysResource){
		CAutoRefPtr<IResProvider> sysSesProvider;
		CreateResProvider(RES_PE,(IObjRef**)&sysSesProvider);
		sysSesProvider->Init((WPARAM)hSysResource,0);
		theApp->LoadSystemNamedResource(sysSesProvider);
	}

	//CAVDownManage Dow;
	//Dow.Start();
	std::wstring path=GetModulePath();
	
	SOUI::SStringT str;
	SOUI::CMainDlg dlgMain;
	m_pDlgMain=&dlgMain;
	dlgMain.Create(NULL,WS_POPUP|WS_MINIMIZEBOX | WS_MAXIMIZEBOX&~WS_CAPTION,0,0,0,0,0);
	dlgMain.GetNative()->SendMessage(WM_INITDIALOG);
	dlgMain.CenterWindow(dlgMain.m_hWnd);
	dlgMain.ShowWindow(SW_SHOWNORMAL);

	if(urlpath.length()>3)
	{
	    char localurl[1024]="";
	    CChineseCode::wcharTochar(urlpath.c_str(), localurl,1024);
		dlgMain.WinTabShow(1);
	    dlgMain.OpenMedia(localurl);
	}
	//int nRet =  Run(lpstrCmdLine, nCmdShow);
	int 	nRet = theApp->Run(dlgMain.m_hWnd);

   // Dow.Stop();
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	_Module.Term();
	::CoUninitialize();

	return nRet;
}
