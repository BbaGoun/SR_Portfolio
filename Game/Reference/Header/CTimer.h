#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimer :  public CBase
{
private:
	explicit CTimer();
	virtual ~CTimer();

public:
	float GetUnscaledTime() { return m_fUnscaledTime; }
	float GetScaledTime() { return m_fScaledTime; }
	float GetLastFixedTime() { return m_fLastFixedTime; }
	float GetMaxDeltaTime() { return m_fMaxDeltaTime; }
	float GetUnscaledDeltaTime() { return m_fUnscaledDeltaTime; }
	float GetDeltaTime() { return m_fDeltaTime; }
	float GetFixedDeltaTime() { return m_fFixedDeltaTime; }
	float GetTimeScale() { return m_fTimeScale; }

	void SetUnscaledTime(float _UnscaledDeltaTime) { m_fUnscaledTime = _UnscaledDeltaTime; }
	void PlusUnscaledTime() { m_fUnscaledTime += m_fUnscaledDeltaTime; }

	void SetScaledTime(float _ScaledDeltaTime) { m_fScaledTime = _ScaledDeltaTime; }
	void PlusScaledTime() { m_fScaledTime += m_fDeltaTime; }

	void SetLastFixedTime(float _Time) { m_fLastFixedTime = _Time; }
	void PlusLastFixedTime() { m_fLastFixedTime += m_fFixedDeltaTime; }

	void SetMaxDeltaTime(float _MaxDeltaTime) { m_fMaxDeltaTime = _MaxDeltaTime; }
	void SetUnscaledDeltaTime(float _RealDeltaTime) { m_fUnscaledDeltaTime = _RealDeltaTime; }
	void SetDeltaTime(float _RealDeltaTime) { m_fDeltaTime = min(_RealDeltaTime * m_fTimeScale, m_fMaxDeltaTime); }
	void SetFixedDeltaTime(float _FixedDeltaTime) { m_fFixedDeltaTime = _FixedDeltaTime; }
	void SetTimeScale(float _TimeScale) { m_fTimeScale = max(0.f, _TimeScale); }

	int GetFixedStep(float* pFixed_DeltaTime);

public:
	HRESULT			Ready_Timer();
	void			Update_Timer();

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;

	LARGE_INTEGER		m_CpuTick;

	float m_fUnscaledTime;
	float m_fScaledTime;
	float m_fLastFixedTime;
	float m_fMaxDeltaTime;
	float m_fUnscaledDeltaTime;
	float m_fDeltaTime;
	float m_fFixedDeltaTime;
	float m_fTimeScale;

public:
	static CTimer* Create();
private:
	virtual void	Free();
};

END
