#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "Engine_Parsing.h"
#include "CCart.h"
#include "CCartBot.h"
#include "CTrackGraph.h"

class CTrackMgr :
    public CBase
{
    DECLARE_SINGLETON(CTrackMgr)

private:
    explicit	CTrackMgr();
    ~CTrackMgr() override;

public:
    void    Register_Track(CGameObject* pGraphObj);
    void    Register_Player(CCart* pPlayer);
    void    Register_Bot(CCartBot* pBot);   
    void    Register_Hazard(CGameObject* pObj, ITEM_TYPE eID);
    void    Delete_Hazard(CGameObject* pObj);

    void    Update_Locator();
    TrackPose Compute_TargetPose(CGameObject* pObj, float lookAhead, bool bDodge);
    void    Set_MaxLap(int _iMaxLap) { m_iMaxLap = _iMaxLap; }

private:
    void Update_RankingUI();
    void Update_LapUI();

private:
    CTrackGraph* m_pTGraph;
    vector<pair<CCart*, TrackLocator>> m_vecPlayer;
    vector<pair<CCartBot*, TrackLocator>> m_vecBot;
    vector<pair<CGameObject*, TrackLocator>> m_tempRanking;
    list<HazardRecord> m_hazardRecords;

    int m_iMaxLap;
    bool m_bAlreadyGoal = false;

private:
    // CBase을(를) 통해 상속됨
    void Free() override;
};

