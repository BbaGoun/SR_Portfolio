#pragma once
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CAlphaEmpty :
	public CGameObject
{
protected:
	explicit CAlphaEmpty(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAlphaEmpty(const CAlphaEmpty& rhs);
	virtual ~CAlphaEmpty() override;

public:
	virtual		HRESULT		Ready_GameObject() override;
	virtual		int			Update_GameObject(const _float& fDeltaTime) override;
	virtual		void		Render_GameObject() override;

	static CAlphaEmpty* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

END