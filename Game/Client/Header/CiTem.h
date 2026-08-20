#pragma once
#include "CScene.h"

class CItem :
	public CScene
{
private:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev);
	~CItem() override;

public:
	virtual			HRESULT		Ready_Scene() override;
	virtual			void		FixedUpdate_Scene(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_Scene(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime) override;
	virtual			void		Render_Scene() override;

public:
	static CItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT	Ready_Prototype() { return S_OK; }
	HRESULT Ready_GameLogic_Layer();
	HRESULT Ready_Environment_Layer() { return S_OK; }

protected:
	virtual void			Free() override;

};

