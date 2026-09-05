#include "pch.h"
#include "Engine_Define.h"
#include "CTrackMgr.h"
#include "CRankMgr.h"
#include "CUI_Laps.h"
#include "CPlayTimeMgr.h"
#include "CManagement.h"

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

void CTrackMgr::Register_Hazard(CGameObject* pObj, ITEM_TYPE eID)
{
	_vec3 vPos;
	pObj->Get_Transform()->Get_Info(INFO_POS, &vPos);

	TrackLocator prevPL;

	TrackLocator TL;
	float lateral;
	if (!m_pTGraph->ProjectPosition(vPos, prevPL, TL, &lateral)) {
		pObj->GetLayer()->Delete_GameObject(pObj);
		return;
	}

	HazardRecord HR;
	HR.edgeId = TL.edgeId;
	HR.u = TL.u;
	HR.lateral = lateral;
	HR.eType = eID;
	HR.pOwner = pObj;

	switch (eID) {
	case ITEM_BANANA:
		HR.radius = 3.f;
		break;
	}

	m_hazardRecords.push_back(HR);
}

void CTrackMgr::Delete_Hazard(CGameObject* pObj)
{
	auto it = find_if(m_hazardRecords.begin(), m_hazardRecords.end(), [&](HazardRecord HR)->bool {
		return HR.pOwner == pObj;
		});

	if (it == m_hazardRecords.end())
		return;

	m_hazardRecords.erase(it);
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

TrackPose CTrackMgr::Compute_TargetPose(CGameObject* pObj, float lookAhead, bool bDodge)
{
	TrackPose TP;

	auto it = find_if(m_tempRanking.begin(), m_tempRanking.end(), [&](pair<CGameObject*, TrackLocator> p)->bool {
		return p.first == pObj;
		});

	if (it == m_tempRanking.end())
		return TP;

	if (bDodge)
		m_pTGraph->EvaluatePoseWithDodge(it->second, lookAhead, TP, m_hazardRecords);
	else
		m_pTGraph->EvaluatePose(it->second, lookAhead, TP);

	return TP;
}

void CTrackMgr::Update_RankingUI()
{
	CRankMgr::GetInstance()->UpdateRank(m_tempRanking);
	//cout << "==========================\n";
	//for (auto& p : m_vecPlayer) {
	//	cout << "Lap : " << p.second.iLap << " s : " << p.second.s << "\n";
	//}
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