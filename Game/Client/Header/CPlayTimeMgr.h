#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "Engine_Parsing.h"

class CPlayTimeMgr :
    public CBase
{
    DECLARE_SINGLETON(CPlayTimeMgr)

private:
    explicit	CPlayTimeMgr();
    ~CPlayTimeMgr() override;

public:
    void            UpdateCPlayTimeMgr(const _float fDeltaTime);
    void            StartCountDown(const _float fDeltaTime);
    void            EndCoundDown(const _float fDeltaTime);
    
    float           GetPlayTimer()      { return m_fPlayTimer; }
    float           GetStartTimer()     { return m_fStartTimer; }
    bool            GetPlaying()        { return m_bPlaying; }

    void            SetRaceStart()      { m_bStart = true; }
    void            SetRaceEnd()        { m_bStart = false; }
    
private:

    float           m_fStartTimer;
    float			m_fPlayTimer;
    float			m_fTimerFlag;
    bool			m_bPlaying;
    bool            m_bStart;

private:
    // CBase을(를) 통해 상속됨
    void Free() override;
};

