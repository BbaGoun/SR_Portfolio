#include "CTimer.h"

CTimer::CTimer() : m_fUnscaledTime(0.f), m_fScaledTime(0.f)
, m_fLastFixedTime(0.f), m_fMaxDeltaTime(0.333f)
, m_fUnscaledDeltaTime(0.f), m_fDeltaTime(0.f)
, m_fFixedDeltaTime(0.02f), m_fTimeScale(1.f)
{
	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

CTimer::~CTimer()
{
}

int CTimer::GetFixedStep(float* pFixed_DeltaTime)
{
	float fFixedAccumulator = GetScaledTime() - GetLastFixedTime();
	float fixedDt = GetFixedDeltaTime();
	int fixedSteps = 0;
	while (fFixedAccumulator >= fixedDt) {
		fFixedAccumulator -= fixedDt;
		++fixedSteps;
		PlusLastFixedTime();
	}

	*pFixed_DeltaTime = fixedDt;

	return fixedSteps;
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);			// 1077
	QueryPerformanceCounter(&m_LastTime);			// 1085
	QueryPerformanceCounter(&m_FixTime);			// 1090

	QueryPerformanceFrequency(&m_CpuTick);		// cpu tick 값을 얻어오는 함수

	return S_OK;
}

void CTimer::Update_Timer()
{
	QueryPerformanceCounter(&m_FrameTime);			// 1500

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	float elapsed = (m_FrameTime.QuadPart - m_LastTime.QuadPart) / (_float)m_CpuTick.QuadPart;

	SetUnscaledDeltaTime(elapsed);
	SetDeltaTime(elapsed);

	PlusUnscaledTime();
	PlusScaledTime();

	m_LastTime = m_FrameTime;
}

CTimer* CTimer::Create()
{
	CTimer* pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
	{
		Engine::Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTimer::Free()
{
}

