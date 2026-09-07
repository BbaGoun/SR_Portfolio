#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CSlotMgr :
	public CBase
{
	DECLARE_SINGLETON(CSlotMgr)

private:
	explicit CSlotMgr();
	~CSlotMgr() override;

public:
	void		AddCharSlot(CGameObject* pObj, CHAR_TYPE eID) { m_vecCharSlot.push_back({ pObj,eID }); }
	void		CharSlotClicked(CGameObject* pObj);
	bool		GetCharSlotShow();
	void		SetCharSlotShow(bool bShow);

	void		AddCartSlot(CGameObject* pObj, INVEN_SLOT_NUM eID) { m_vecInvenSlot.push_back({ pObj, eID }); }
	void		CartSlotClicked(CGameObject* pObj);
	bool		GetCartSlotShow();
	void		SetCartSlotShow(bool bShow);


private:
	//vector<pair<CGameObject*, int >>	m_vecRankUIs;
	
	vector<pair<CGameObject*, CHAR_TYPE>> m_vecCharSlot;
	vector<pair<CGameObject*, INVEN_SLOT_NUM>> m_vecInvenSlot;


private:
	void Free() override;

};


