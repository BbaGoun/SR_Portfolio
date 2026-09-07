#pragma once
#include "CScene.h"
#include "CLoadingThread.h"
#include "CUI_NexonLogo.h"

class CNexonVideo: public CScene
{
private:
	explicit CNexonVideo(LPDIRECT3DDEVICE9 pGraphicDev);
	~CNexonVideo() override;

public:
	virtual			HRESULT		Ready_Scene();
	virtual			_int		Update_Scene(const _float& fDeltaTime);
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime);
	virtual			void		Render_Scene();



private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_UI_Layer();


private:
	HRESULT		Ready_Prototype();

	

private:
	CLoadingThread* m_pLoadingThread;

public:
	static CNexonVideo* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	CGameObject* m_pNexonLogo;

private:
	virtual void Free();
};

