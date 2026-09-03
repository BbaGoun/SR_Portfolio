#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CRankMgr :
	public CBase
{
	DECLARE_SINGLETON(CRankMgr)

private:
	explicit CRankMgr();
	~CRankMgr() override;

public:
	void		AddUI(CGameObject* pRankUI, int iNum) { m_vecRankUIs.push_back({ pRankUI,iNum }); }
	void		SwapUI(int iDst, int iSrc);
	void		UpdateRank(vector<pair<CGameObject*, int >>);
private:
	vector<pair<CGameObject*, int >>	m_vecRankUIs;

private:
	void Free() override;

};


