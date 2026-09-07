#include "pch.h"
#include "CSlotMgr.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include <CScene3_CharSlot.h>
#include <CUI_InvenSlot.h>

IMPLEMENT_SINGLETON(CSlotMgr)
CSlotMgr::CSlotMgr()
{
}


CSlotMgr::~CSlotMgr()
{
	Free();
}


void CSlotMgr::CharSlotClicked(CGameObject* pObj)
{
	for (auto& pSlot : m_vecCharSlot)
	{
		if (pObj != pSlot.first)
		{
			CScene3_CharSlot* pCharSlot = static_cast<CScene3_CharSlot*>(pSlot.first);
			if(pCharSlot->GetBGSelected() == true)
				pCharSlot->SetBGSelected(false);
		}
	}
}

bool CSlotMgr::GetCharSlotShow()
{
	for (auto& pCharSlot : m_vecCharSlot)
	{
		if (static_cast<CScene3_CharSlot*>(pCharSlot.first)->Get_Show() == false)
			return false;
	}
	return true;
	// 일단 이 함수는 캐릭터 슬롯을 순회하면서 하나라도 숨겨져있다면 전체가 숨겨져있다고 판단하는 함수에요
	// 어차피 슬롯들은 모두 보여지고 모두 숨겨지니까
	// 이제 모두 보이게하고 모두 숨기게 하는 함수를 만들어야해요
}

void CSlotMgr::SetCharSlotShow(bool bShow)
{
	//한번 해보실래요?? 
	// 매개변수 bShow값으로 m_vecCharSlot의 요소들의 m_bShow값을 바꾸는 함수를 만들어보세요
	//ㄱㄱ
	for (auto& pCharSlot : m_vecCharSlot)
	{
		static_cast<CScene3_CharSlot*>(pCharSlot.first)->Set_Show(bShow);
	}
	// 궁금한 부분이 있나요??

}

void	CSlotMgr::CartSlotClicked(CGameObject* pObj)
{
	for (auto& pSlot : m_vecInvenSlot)
		if (pObj != pSlot.first)
		{
			CUI_InvenSlot* pCartSlot = static_cast<CUI_InvenSlot*>(pSlot.first);
			if (pCartSlot->GetBGSelected() == true)
			{
				pCartSlot->SetBGSelected(false);
			}
		}
}

bool CSlotMgr::GetCartSlotShow()
{
	for (auto& pCartSlot : m_vecInvenSlot)
	{
		if (static_cast<CUI_InvenSlot*>(pCartSlot.first)->Get_Show() == false)
			return false;
	}
	return true;
}

void CSlotMgr::SetCartSlotShow(bool bShow)
{
	for (auto& pCartSlot : m_vecInvenSlot)
	{
		static_cast<CUI_InvenSlot*>(pCartSlot.first)->Set_Show(bShow);
	}
}

void CSlotMgr::Free()
{
}