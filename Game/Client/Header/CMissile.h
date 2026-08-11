#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CMissileTex;
}

class CMissile : public CGameObject
{
private:
	explicit CMissile(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMissile() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

private:
	Engine::CMissileTex*	m_pBufferCom;
			_float			m_fSpeed;
public:
	static CMissile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
