#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "Engine_Parsing.h"
#include "CCart.h"
//#include "CCartBot.h"
#include "CTrackGraph.h"

class CTrackMgr :
    public CBase
{
    DECLARE_SINGLETON(CTrackMgr)

private:
    explicit	CTrackMgr();
    ~CTrackMgr() override;

public:
    void Register_Track(CGameObject* pGraphObj);
    void Register_Player(CCart* pPlayer);
    //void Register_Bot(CCartBot* pBot);   

    void Update_Locator();
private:
    void Update_RankingUI();

private:
    CTrackGraph* m_pTGraph;
    vector<pair<CCart*, TrackLocator>> m_vecPlayer;
    //vector<pair<CCartBot*, TrackLocator>> m_vecBot;
    vector<pair<CGameObject*, TrackLocator>> m_tempRanking;

private:
    // CBase을(를) 통해 상속됨
    void Free() override;
};

