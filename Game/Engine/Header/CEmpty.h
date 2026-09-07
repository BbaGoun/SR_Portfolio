#pragma once
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CEmpty :
    public CGameObject
{
protected:
	explicit CEmpty(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEmpty(const CEmpty& rhs);
	virtual ~CEmpty() override;

public:
	virtual		HRESULT		Ready_GameObject() override;
	virtual		int			Update_GameObject(const _float& fDeltaTime) override;
	virtual		void		Render_GameObject() override;

	static CEmpty* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

END