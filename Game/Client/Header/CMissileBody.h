#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CMissileTex;
}

class CMissileBody : public CGameObject
{
private:
	explicit CMissileBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMissileBody() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

private:
	Engine::CMissileTex* m_pBufferCom;

public:
	static CMissileBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
