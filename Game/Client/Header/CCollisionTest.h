#pragma once
#include "CScene.h"

class CCollisionTest : public CScene
{
private:
	explicit CCollisionTest(LPDIRECT3DDEVICE9 pGraphicDev);
	~CCollisionTest() override;

public:
	virtual			HRESULT		Ready_Scene() override;
	virtual			void		FixedUpdate_Scene(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_Scene(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime) override;
	virtual			void		Render_Scene() override;

public:
	static CCollisionTest* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT	Ready_Prototype();
	HRESULT Ready_GameLogic_Layer();
	HRESULT Ready_Environment_Layer();
	HRESULT Ready_UI_Layer();
	HRESULT Ready_Collision_Matrix();


protected:
	virtual void			Free() override;

};

