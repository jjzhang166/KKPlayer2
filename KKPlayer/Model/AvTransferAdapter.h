#ifndef AvTransferAdapter_H_
#define AvTransferAdapter_H_
#include "../stdafx.h"

#include <control/SCmnCtrl.h>
#include <helper/SAdapterBase.h>
#include <map>
#include <helper/SAdapterBase.h>
#include <control/SMCListView.h>
#include <string>
#include "../MainPage/IMainUI.h"
namespace SOUI
{
	class CKKmclv;
	typedef struct SORTCTX
	{
		int iCol;
		SHDSORTFLAG stFlag;
	}SORTCTX;
	class CDownAVListMcAdapterFix : public SMcAdapterBase
	{
		public:
			CDownAVListMcAdapterFix(IMainUI* pIMainUI,SMCListView *pSMCListView,std::string url);
				~CDownAVListMcAdapterFix();
				void Refresh();
				int getCount();

				SStringT getSizeText(DWORD dwSize);
				bool OnItemClick(EventArgs *pEvt);
				virtual void getView(int position, SWindow * pItem,pugi::xml_node xmlTemplate);

				bool OnButtonClick(EventArgs *pEvt);
				SStringW GetColumnName(int iCol) const;
				
				bool OnSort(int iCol,SHDSORTFLAG * stFlags,int nCols);

				bool OnItemDelete(EventArgs *pEvt);
				bool OnItemPause(EventArgs *pEvt);
				bool OnItemOpenLocal(EventArgs *pEvt);
		private:
			SMCListView *m_pSMCListView;
			std::string m_strurl;
			IMainUI*    m_pIMainUI;
	};
}
#endif