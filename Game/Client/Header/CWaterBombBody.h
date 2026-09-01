#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CSphere;		
}

class CWaterBombBody : public CGameObject
{
private:
	explicit CWaterBombBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWaterBombBody() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

private:
	Engine::CSphere* m_pBufferCom;
	_float	m_fTimer;

public:
	static CWaterBombBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
