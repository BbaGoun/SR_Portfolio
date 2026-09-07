#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CCartBodyCol;		
}

class CUfoBody : public CGameObject
{
private:
	explicit CUfoBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUfoBody() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

private:
	Engine::CCartBodyCol* m_pBufferCom;
	_float	m_fTimer;

public:
	static CUfoBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
