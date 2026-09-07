#pragma once
#include "CGameObject.h"

#include "CTexture.h"

class CButtonMgr :
	public CBase
{
	DECLARE_SINGLETON(CButtonMgr)

private:
	explicit CButtonMgr();
	~CButtonMgr() override;

public:
	void		AddBtntoVec(CGameObject* pObj) { m_vecButton.push_back({ pObj}); }
	void		ButtonClicked(CGameObject* pObj);



private:
	//vector<pair<CGameObject*, int >>	m_vecRankUIs;

	vector<CGameObject*> m_vecButton;

private:
	void Free() override;

};


