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
	//if (iDst >= m_vecChildren.size() || iSrc >= m_vecChildren.size())
	//	return;
	//_vec3 vDstPos, vSrcPos;
	//m_vecChildren[iDst]->Get_Transform()->Get_LocalInfo(INFO_POS, &vDstPos);
	//m_vecChildren[iSrc]->Get_Transform()->Get_LocalInfo(INFO_POS, &vSrcPos);
	//
	//m_vecChildren[iDst]->Get_Transform()->Set_Pos(vSrcPos);
	//m_vecChildren[iSrc]->Get_Transform()->Set_Pos(vDstPos);
	//
	//swap(m_vecChildren[iDst], m_vecChildren[iSrc]);
}

void CRankMgr::Free()
{
}