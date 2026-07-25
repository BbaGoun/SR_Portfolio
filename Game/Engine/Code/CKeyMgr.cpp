#include "Engine_Define.h"
#include "CKeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
	ZeroMemory(m_bKeyNextState, sizeof(m_bKeyNextState));
}

CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::KeyPressing(int iKey)
{
	if (m_bGetAsyncKey[iKey])
		return true;
	return false;
}

bool CKeyMgr::KeyDown(int iKey)
{
	if (!m_bKeyState[iKey] && m_bGetAsyncKey[iKey]) {
		//m_bKeyState[iKey] = true;
		m_bKeyNextState[iKey] = true;
		return true;
	}
	return false;
}

bool CKeyMgr::KeyUp(int iKey)
{
	if (m_bKeyState[iKey] && !m_bGetAsyncKey[iKey]) {
		//m_bKeyState[iKey] = false;
		m_bKeyNextState[iKey] = false;
		return true;
	}
	return false;
}

void CKeyMgr::Key_Update()
{
	for (int iKey = 0; iKey < VK_MAX; ++iKey) {
		m_bGetAsyncKey[iKey] = GetAsyncKeyState(iKey) & 0x8000;
	}
}

void CKeyMgr::Key_LateUpdate()
{
	for (int iKey = 0; iKey < VK_MAX; ++iKey) {
		if (m_bKeyState[iKey] != m_bKeyNextState[iKey])
			m_bKeyState[iKey] = m_bKeyNextState[iKey];

		if (!m_bKeyState[iKey] && m_bGetAsyncKey[iKey]) {
			m_bKeyState[iKey] = !m_bKeyState[iKey];
		}

		if (m_bKeyState[iKey] && !m_bGetAsyncKey[iKey]) {
			m_bKeyState[iKey] = !m_bKeyState[iKey];
		}
	}
}

void CKeyMgr::Free()
{
}
