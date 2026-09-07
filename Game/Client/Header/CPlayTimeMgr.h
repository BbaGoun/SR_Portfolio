#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "Engine_Parsing.h"
#include "CRankMgr.h"
#include "CTrackMgr.h"
class CPlayTimeMgr :
    public CBase
{
    DECLARE_SINGLETON(CPlayTimeMgr)

private:
    explicit	CPlayTimeMgr();
    ~CPlayTimeMgr() override;

public:
    void            UpdateCPlayTimeMgr(const _float& fDeltaTime);

    void            UpdatePlayTime(const _float& fDeltaTime);
    void            StartCountDown(const _float& fDeltaTime);
    void            EndCoundDown(const _float& fDeltaTime);
    
    float           GetPlayEndTime()    { return m_fPlayEndTime; }
    float           GetPlayTimer()      { return m_fPlayTimer; }
    float           GetTimerFlag()      { return m_fTimerFlag; }
    float           GetStartTimer()     { return m_fStartTimer; }
    bool            GetPlaying()        { return m_bPlaying; }

    void            SetRaceStart()      { m_bStart = true; }

    void            SetRaceEnd(){ 
        m_fStartTimer = 0.f;
        m_fPlayTimer = 0.f;
        m_fTimerFlag = 0.f;
        m_bStart = false;
        m_bPlaying = false;
    }
    void            SetCountDownStart() { m_fPlayEndTime = m_fPlayTimer; }

private:
    void            UpdateTimerUI();

private:
    float           m_fPlayEndTime;
    float           m_fStartTimer;
    float			m_fPlayTimer;
    float			m_fTimerFlag;
    bool			m_bPlaying;
    bool            m_bStart;

private:
    void Free() override;
};

