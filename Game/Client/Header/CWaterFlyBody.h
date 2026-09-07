#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CTexture;
	class CRcTex;
}

class CWaterFlyBody : public CGameObject
{
private:
	explicit CWaterFlyBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWaterFlyBody() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
			_float	m_fTimer;
			_float	m_fFrame;

public:
	static CWaterFlyBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
