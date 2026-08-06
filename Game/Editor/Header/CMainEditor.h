#pragma once

#include "CBase.h"
#include "CGraphicDev.h"
#include "CTimerMgr.h"
#include "CFrameMgr.h"

#include "CManagement.h"

class CMainEditor : public CBase
{
private:		// 생성자, 소멸자
	explicit CMainEditor();
	virtual ~CMainEditor();

public:
	HRESULT		Ready_MainEditor();
	void		FixedUpdate_MainEditor(const float& fFixedDeltaTime);
	int			Update_MainEditor(const float& fDeltaTime);
	void		LateUpdate_MainEditor(const float& fDeltaTime);
	void		Render_MainEditor();

private:
	HRESULT		Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CGraphicDev* m_pDeviceClass;
	Engine::CManagement* m_pManagementClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;


public:		// 생성 함수
	static CMainEditor* Create();

private:	// 소멸 함수
	virtual void	Free();
};

