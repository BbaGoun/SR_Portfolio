#pragma once
#include "CScene.h"

class CRacingScene :
    public CScene
{
private:
	explicit CRacingScene(LPDIRECT3DDEVICE9 pGraphicDev);
	~CRacingScene() override;

public:
	virtual			HRESULT		Ready_Scene() override;
	virtual			HRESULT		PostReady_Scene() override;
	virtual			void		FixedUpdate_Scene(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_Scene(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime) override;
	virtual			void		Render_Scene() override;

	virtual			void		OnLostDevice() override;
	virtual			void		OnResetDevice() override;

private:
	HRESULT	LoadSceneFromFile();

	HRESULT Ready_TrackMgr();
	HRESULT	Ready_RenderTarget();
	HRESULT Ready_GameLogic_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Collision_Matrix();

public:
	static CRacingScene* Create(LPDIRECT3DDEVICE9 pGraphicDev, MAP_ID eID);

private:
	MAP_ID m_eMapId = MAP_END;

protected:
	virtual void			Free() override;
};

