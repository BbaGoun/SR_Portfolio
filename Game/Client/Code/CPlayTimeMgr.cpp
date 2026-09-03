#include "pch.h"
#include "Engine_Define.h"
#include "CPlayTimeMgr.h"
#include "CCart.h"
#include "CCartBody.h"
#include "CWheel.h"
#include "CPlayerHead.h"
#include "CPlayer.h"
#include "SoundMgr.h"
#include "CUI_StartCountDown.h"
#include "CUI_EndCountDown.h"
#include "CRenderer.h"
#include "CCameraMgr.h"
IMPLEMENT_SINGLETON(CPlayTimeMgr)

CPlayTimeMgr::CPlayTimeMgr()
{
	int a;
	m_fStartTimer	= 0.f;
	m_fPlayTimer	= 0.f;
	m_fTimerFlag	= 0.f;
	m_bStart		= false;
	m_bPlaying		= false;
	m_fPlayEndTime	= 300000.f;
}

CPlayTimeMgr::~CPlayTimeMgr()
{
	Free();
}

void CPlayTimeMgr::UpdateCPlayTimeMgr(const _float& fDeltaTime)
{
	if (m_bStart)
	{
		StartCountDown(fDeltaTime);
		UpdatePlayTime(fDeltaTime);
		EndCoundDown(fDeltaTime);
	}
}


void CPlayTimeMgr::UpdatePlayTime(const _float& fDeltaTime)
{
	if (m_bPlaying)
	{
		CRenderer::GetInstance()->SetBlur(true);
		m_fPlayTimer += fDeltaTime;
	}
}

void CPlayTimeMgr::StartCountDown(const _float& fDeltaTime)
{
	m_fStartTimer += fDeltaTime;

	if (m_fStartTimer > 4.f)
		return;

	if (m_fStartTimer > 3.f)
	{
		m_bPlaying = true;
		SoundMgr::GetInstance().PlaySound(L"Effect/lab/count_go.flac", SOUND_EFFECT1, 0.4f);
	}
	else if (m_fTimerFlag < 2.f && m_fStartTimer > 2.f)
	{
		m_fTimerFlag = m_fStartTimer;
		SoundMgr::GetInstance().PlaySound(L"Effect/lab/count_n.flac", SOUND_STARTCOUNT3, 0.4f);
	}
	else if (m_fTimerFlag < 1.f && m_fStartTimer > 1.f)
	{
		m_fTimerFlag = m_fStartTimer;
		SoundMgr::GetInstance().PlaySound(L"Effect/lab/count_n.flac", SOUND_STARTCOUNT2, 0.4f);
	}
	else if (m_fTimerFlag == 0.f && m_fStartTimer > 0.f)
	{
		m_fTimerFlag = m_fStartTimer;
		SoundMgr::GetInstance().PlaySound(L"Effect/lab/count_n.flac", SOUND_STARTCOUNT1, 0.4f);
	}
}

void CPlayTimeMgr::EndCoundDown(const _float& fDeltaTime)
{
	if (m_fPlayTimer < m_fPlayEndTime || m_bPlaying == false)
		return;

	//if (m_fPlayTimer > m_fPlayEndTime + 10.f && m_bPlaying == true)
	//{
	//	m_bPlaying = false;
	//	SoundMgr::GetInstance().StopSound(SOUND_BOOST);
	//	SoundMgr::GetInstance().StopSound(SOUND_DRIFT);
	//	SoundMgr::GetInstance().PlaySound(L"Effect/lab/race_over.flac", SOUND_ENDCOUND, 0.4f);
	//}
	//else
	//{
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		if (m_fTimerFlag < m_fPlayEndTime + float(i) && m_fPlayTimer > m_fPlayEndTime + float(i))
	//		{
	//			m_fTimerFlag = m_fPlayTimer;
	//			SoundMgr::GetInstance().PlaySound(L"Effect/lab/ro_count.flac", SOUND_ENDCOUND, 0.4f);
	//		}
	//	}
	//}
}

void CPlayTimeMgr::Free()
{
}
