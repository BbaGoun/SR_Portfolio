#include "pch.h"
#include "CButtonMgr.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CMenu_Set.h"
#include "CScene3_KartBtn.h"
#include "CScene3_CharBtn.h"
#include "CScene3_ColorBtn.h"
#include <CSlotMgr.h>


#include "CScene3_ColorSet.h"
#include "CScene3_ColorSetBG.h"


IMPLEMENT_SINGLETON(CButtonMgr)
CButtonMgr::CButtonMgr()
{
}


CButtonMgr::~CButtonMgr()
{
	Free();
}


void CButtonMgr::ButtonClicked(CGameObject* pObj)
{
	
	if (dynamic_cast<CScene3_KartBtn*>(pObj))
	{
		//if (CSlotMgr::GetInstance()->GetCharSlotShow() == true)
		CSlotMgr::GetInstance()->SetCharSlotShow(false);
		CScene3_ColorSet* pColor = dynamic_cast<CScene3_ColorSet*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSet1"));
		CScene3_ColorSet* pColor2 = dynamic_cast<CScene3_ColorSet*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSet2"));
		CScene3_ColorSetBG* pColorBG = dynamic_cast<CScene3_ColorSetBG*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSetBG"));

		pColor->Set_Show1(false);

		pColor2->Set_Show1(false);
		pColorBG->Set_Show(false);
	}
	else if (dynamic_cast<CScene3_CharBtn*>(pObj))
	{
		//if (CSlotMgr::GetInstance()->GetCartSlotShow() == true)
		CSlotMgr::GetInstance()->SetCartSlotShow(false);
		CScene3_ColorSet* pColor = dynamic_cast<CScene3_ColorSet*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSet1"));
		CScene3_ColorSet* pColor2 = dynamic_cast<CScene3_ColorSet*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSet2"));
		CScene3_ColorSetBG* pColorBG = dynamic_cast<CScene3_ColorSetBG*>(CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_ColorSetBG"));
		
		pColor->Set_Show1(false);
		 
		pColor2->Set_Show1(false);
		pColorBG->Set_Show(false);
	}

	else if (CScene3_ColorBtn* pColorBtn = dynamic_cast<CScene3_ColorBtn*>(pObj))
	{

		CSlotMgr::GetInstance()->SetCartSlotShow(false);
		CSlotMgr::GetInstance()->SetCharSlotShow(false);
	

	}

}

void CButtonMgr::Free()
{
}