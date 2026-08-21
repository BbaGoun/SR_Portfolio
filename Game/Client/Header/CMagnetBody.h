#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CMagnetTex;
}

class CMagnetBody : public CGameObject
{
private:
	explicit CMagnetBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMagnetBody() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

private:
	Engine::CMagnetTex* m_pBufferCom;
	_float			m_fSpeed;
	_float			m_fAngle;

public:
	static CMagnetBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
