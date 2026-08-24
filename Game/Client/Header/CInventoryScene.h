#pragma once
#include "CScene.h"

class CInventoryScene : public CScene
{
private:
	explicit CInventoryScene(LPDIRECT3DDEVICE9 pGraphicDev);
	~CInventoryScene() override;

public:
	virtual			HRESULT		Ready_Scene() override;
	virtual			void		FixedUpdate_Scene(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_Scene(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime) override;
	virtual			void		Render_Scene() override;

	virtual			void		OnLostDevice() override;
	virtual			void		OnResetDevice() override;


public:
	static CInventoryScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT	Ready_Prototype();
	HRESULT	Ready_RenderTarget();
	HRESULT Ready_GameLogic_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Collision_Matrix();


protected:
	virtual void			Free() override;

};

