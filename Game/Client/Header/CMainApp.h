#pragma once

#include "CBase.h"
#include "CGraphicDev.h"
#include "CTimerMgr.h"
#include "CFrameMgr.h"
#include "CManagement.h"

class CMainApp : public CBase
{
private:		// 생성자, 소멸자
	explicit CMainApp();
	virtual ~CMainApp();

public:
	HRESULT		Ready_MainApp();
	void		FixedUpdate_MainApp(const float& fFixedDeltaTime);
	int			Update_MainApp(const float& fDeltaTime);
	void		LateUpdate_MainApp(const float& fDeltaTime);
	void		Render_MainApp();

	bool		ResetTest();
	void		Reset_MainApp();
	bool		IsLost_Device() { return m_bDeviceLost; }
	void		Resize_MainApp(UINT uWidth, UINT uHeight);

private:
	HRESULT		Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

	void		OnResetDevice();

private:
	Engine::CGraphicDev*	m_pDeviceClass;
	Engine::CManagement*	m_pManagementClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	bool					m_bDeviceLost;

public:		// 생성 함수
	static CMainApp* Create();

private:	// 소멸 함수
	virtual void	Free();
};

