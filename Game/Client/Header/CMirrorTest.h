#pragma once
#include "CScene.h"

class CMirrorTest :
	public CScene
{
private:
	explicit CMirrorTest(LPDIRECT3DDEVICE9 pGraphicDev);
	~CMirrorTest() override;

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

	HRESULT Ready_GameLogic_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Collision_Matrix();

public:
	static CMirrorTest* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void			Free() override;
};

