#pragma once

#include "Engine_Define.h"
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CDInputMgr : public CBase
{
	DECLARE_SINGLETON(CDInputMgr)

private:
	explicit CDInputMgr(void);
	virtual ~CDInputMgr(void);

public:
	_byte	Get_DIKeyState(_ubyte byKeyID)
	{
		return m_byKeyState[byKeyID];
	}

	_bool	Get_DIKeyDown(_ubyte byKeyID)
	{
		return !(m_byBeforeKeyState[byKeyID] & 0x80)
			&& (m_byKeyState[byKeyID] & 0x80);
	}

	_bool	Get_DIKeyUp(_ubyte byKeyID)
	{
		return (m_byBeforeKeyState[byKeyID] & 0x80)
			&& !(m_byKeyState[byKeyID] & 0x80);
	}

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{
		return m_tMouseState.rgbButtons[eMouse];
	}

	_bool	Get_DIMouseKeyDown(MOUSEKEYSTATE eMouse) {
		return !(
			(m_tBeforeMouseState.rgbButtons[eMouse] & 0x80) ||
			(m_tBeforeMouseState.rgbButtons[eMouse] & -0x80)
			)
			&&
			(
			(m_tMouseState.rgbButtons[eMouse] & 0x80) ||
			(m_tMouseState.rgbButtons[eMouse] & -0x80)
			);
	}

	_bool	Get_DIMouseKeyUp(MOUSEKEYSTATE eMouse) {
		return (
			(m_tBeforeMouseState.rgbButtons[eMouse] & 0x80) ||
			(m_tBeforeMouseState.rgbButtons[eMouse] & -0x80)
			)
			&&
			!(
			(m_tMouseState.rgbButtons[eMouse] & 0x80) ||
			(m_tMouseState.rgbButtons[eMouse] & -0x80)
			);
	}

	// 현재 마우스의 특정 축 좌표를 반환
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);
	void	LateUpdate_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_byBeforeKeyState[256];
	_byte					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	// IX, IY, IZ는 모니터에서 이전 위치에 대한 각 축에서 상대적 이동 값
	DIMOUSESTATE			m_tBeforeMouseState;
	DIMOUSESTATE			m_tMouseState;

public:
	virtual void	Free(void);

};
END


