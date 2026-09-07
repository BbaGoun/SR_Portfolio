#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CCube_Collider;
	class CRcTex;
	class CTexture;
}

class CWaterFly : public CGameObject
{
private:
	explicit CWaterFly(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget);
	virtual ~CWaterFly() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
	_float				m_fTimer;
	_float				m_fFrame;

	CGameObject*		m_pTarget;
	
public:
	static CWaterFly* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget);

protected:
	virtual void	Free() override;

};
