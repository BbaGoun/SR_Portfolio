#include "pch.h"
#include "Engine_Define.h"
#include "CTrackMgr.h"
#include "CRankMgr.h"
#include "CUI_Laps.h"
#include "CPlayTimeMgr.h"

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

void CTrackMgr::Register_Bot(CCartBot* pBot)
{
	TrackLocator TL;
	TL.edgeId = 0;
	TL.bValid = false;

	m_vecBot.push_back({ pBot, TL });
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
		
		if (p.second.iLap >= m_iMaxLap)
		{
			if (!m_bAlreadyGoal) {
				CPlayTimeMgr::GetInstance()->SetCountDownStart();
				m_bAlreadyGoal = true;
			}
			p.first->SetActive(false);
		}

		m_tempRanking.push_back({ p.first, p.second });
	}

	for (auto& p : m_vecBot) {
		_vec3 vPos;
		p.first->Get_Transform()->Get_Info(INFO_POS, &vPos);

		TrackLocator TL;
		float lateral;
		if (m_pTGraph->ProjectPosition(vPos, p.second, TL, &lateral))
			p.second = TL;
		else
			p.second.bValid = false;

		if (p.second.iLap >= m_iMaxLap)
		{
			if (!m_bAlreadyGoal) {
				CPlayTimeMgr::GetInstance()->SetCountDownStart();
				m_bAlreadyGoal = true;
			}
			p.first->SetActive(false);
		}

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
	Update_LapUI();
}

TrackPose CTrackMgr::Compute_TargetPose(CGameObject* pObj, float lookAhead)
{
	TrackPose TP;

	auto it = find_if(m_tempRanking.begin(), m_tempRanking.end(), [&](pair<CGameObject*, TrackLocator> p)->bool {
		return p.first == pObj;
		});

	if (it == m_tempRanking.end())
		return TP;

	m_pTGraph->EvaluatePose(it->second, lookAhead, TP);

	return TP;
}

void CTrackMgr::Update_RankingUI()
{
	CRankMgr::GetInstance()->UpdateRank(m_tempRanking);
	cout << "==========================\n";
	for (auto& p : m_vecPlayer) {
		cout << "Lap : " << p.second.iLap << " s : " << p.second.s << "\n";
	}
}

void CTrackMgr::Update_LapUI()
{
	CGameObject* pUILap = CManagement::GetInstance()->Find_GameObjectByTag(L"UI", L"UI_Lap");
	for (auto& p : m_vecPlayer) {
		if(pUILap)
			static_cast<CUI_Laps*>(pUILap)->Set_LapInfo(p.second.iLap, m_iMaxLap);
	}
}

void CTrackMgr::Free()
{
}