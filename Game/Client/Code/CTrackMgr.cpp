#include "pch.h"
#include "Engine_Define.h"
#include "CTrackMgr.h"

IMPLEMENT_SINGLETON(CTrackMgr)

CTrackMgr::CTrackMgr()
{
	
}

CTrackMgr::~CTrackMgr()
{
	Free();
}

void CTrackMgr::Register_Track(CGameObject* pGraphObj)
{
	m_pTGraph = pGraphObj->Get_Component<CTrackGraph>();
}

void CTrackMgr::Register_Player(CCart* pPlayer)
{
	TrackLocator TL;
	TL.edgeId = 0;
	TL.bValid = false;

	m_vecPlayer.push_back({ pPlayer, TL });
}

void CTrackMgr::Update_Locator()
{
	m_tempRanking.clear();

	for (auto& p : m_vecPlayer) {
		_vec3 vPos;
		p.first->Get_Transform()->Get_Info(INFO_POS, &vPos);

		TrackLocator TL;
		float lateral;
		if (m_pTGraph->ProjectPosition(vPos, p.second, TL, &lateral))
			p.second = TL;
		else
			p.second.bValid = false;
		
		m_tempRanking.push_back({ p.first, p.second });
	}

	float fLapLength = m_pTGraph->Get_LapLength();

	sort(m_tempRanking.begin(), m_tempRanking.end(), 
		[&](pair<CGameObject*, TrackLocator> a, pair<CGameObject*, TrackLocator> b)->bool{
			return
				(a.second.iLap * fLapLength + a.second.s) >
				(b.second.iLap * fLapLength + b.second.s);
		});

	Update_RankingUI();
}

void CTrackMgr::Update_RankingUI()
{
	cout << "==========================\n";
	for (auto& p : m_tempRanking) {
		cout << "Lap : " << p.second.iLap << " s : " << p.second.s << "\n";
	}
}

void CTrackMgr::Free()
{
}