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
	void		AddUI(CGameObject* pTargetObj, CGameObject* pRankUI) { m_vecRankUIs.push_back({ pTargetObj, pRankUI }); }
	void		SwapUI(int iDst, int iSrc);
	void		UpdateRank(vector<pair<CGameObject*, TrackLocator>>& vecRank);

private:
	vector<pair<CGameObject*, CGameObject*>>	m_vecRankUIs;

private:
	void Free() override;

};


