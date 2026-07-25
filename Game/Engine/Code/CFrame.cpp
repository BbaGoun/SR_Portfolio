#include "CFrame.h"

CFrame::CFrame() : m_fAccTimeDelta(0.f), m_fCallLimit(0.f)
{
}

CFrame::~CFrame()
{
}

_bool CFrame::IsPermit_Call(const _float& fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta;

	if (m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fAccTimeDelta = 0.f;

		return true;
	}

	return false;
}

HRESULT CFrame::Ready_Frame(const _float& fCallLimit)
{
	m_fCallLimit = 1.f / fCallLimit;

	return S_OK;
}

CFrame* CFrame::Create(const _float& fCallLimit)
{
	CFrame* pFrame = new CFrame;

	if (FAILED(pFrame->Ready_Frame(fCallLimit)))
	{
		Safe_Release(pFrame);
		MSG_BOX("CFrame Create Failed");
		return nullptr;
	}

	return pFrame;
}

void CFrame::Free()
{
}
