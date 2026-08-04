#pragma once
#include "CScene.h"

class CScene_Test :
    public CScene
{
private:
    explicit CScene_Test(LPDIRECT3DDEVICE9 pGraphicDev);
    ~CScene_Test() override;

public:
	virtual			HRESULT		Ready_Scene() override;
	virtual			_int		Update_Scene(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_Scene(const _float& fDeltaTime) override;
	virtual			void		Render_Scene() override;

public:
    static CScene_Test* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT	Ready_Prototype();
	HRESULT Ready_GameLogic_Layer();
	HRESULT Ready_Environment_Layer();

protected:
	virtual void			Free() override;

};

