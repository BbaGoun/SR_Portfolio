#pragma once

#include "CBase.h"
#include "CGraphicDev.h"
#include "CCube.h"


class CMainEditor : public CBase
{
private:		// 생성자, 소멸자
	explicit CMainEditor();
	virtual ~CMainEditor();

public:
	HRESULT		Ready_MainEditor();
	void		FixedUpdate_MainEditor();
	int			Update_MainEditor();
	void		LateUpdate_MainEditor();
	void		Render_MainEditor();

private:
	HRESULT		Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT		Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CGraphicDev*	m_pDeviceClass;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Engine::CCube* m_pCubeBuffer = nullptr;
	LPDIRECT3DTEXTURE9 m_pSceneTex = nullptr;
	LPDIRECT3DSURFACE9 m_pSceneDepth = nullptr;
	UINT m_rtW = 0, m_rtH = 0;


public:		// 생성 함수
	static CMainEditor* Create();

private:	// 소멸 함수
	virtual void	Free();
};

