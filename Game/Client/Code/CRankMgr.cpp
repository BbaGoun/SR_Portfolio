#include "pch.h"
#include "CRankMgr.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CManagement.h"
#include "CDInputMgr.h"

IMPLEMENT_SINGLETON(CRankMgr)
CRankMgr::CRankMgr()
{
}


CRankMgr::~CRankMgr()
{
	Free();
}

void CRankMgr::SwapUI(int iDst, int iSrc)
{
	if (iDst >= m_vecRankUIs.size() || iSrc >= m_vecRankUIs.size())
		return;

	_vec3 vDstPos, vSrcPos;
	m_vecRankUIs[iDst].second->Get_Transform()->Get_LocalInfo(INFO_POS, &vDstPos);
	m_vecRankUIs[iSrc].second->Get_Transform()->Get_LocalInfo(INFO_POS, &vSrcPos);
	
	m_vecRankUIs[iDst].second->Get_Transform()->Set_Pos(vSrcPos);
	m_vecRankUIs[iSrc].second->Get_Transform()->Set_Pos(vDstPos);
	
	swap(m_vecRankUIs[iDst], m_vecRankUIs[iSrc]);
}

void CRankMgr::UpdateRank(vector<pair<CGameObject*, TrackLocator>>& vecRank)
{
	for (int i = 0; i < vecRank.size(); ++i) {
		for (int j = 0; j < m_vecRankUIs.size(); ++j) {
			if (m_vecRankUIs[j].first == vecRank[i].first)
				SwapUI(i, j);
		}
	}
}

void CRankMgr::Free()
{
}