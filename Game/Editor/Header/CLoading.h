#pragma once
#include "CScene.h"
#include "CLoadingThread.h"

class CLoading :  public CScene
{
private:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	~CLoading() override;

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fDeltaTime);
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime);
	virtual			void		Render_Scene();

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag)	{ return S_OK; }
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag)			{ return S_OK; }

private:
	HRESULT		Ready_Prototype();

private:
	CLoadingThread*	m_pLoadingThread;

public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();
};

