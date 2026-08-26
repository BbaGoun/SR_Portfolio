#pragma once
#include "CGameObject.h"
#include "CDustLandingParticle.h"
#include "CTexture.h"

class CDustLandingEffect : public CGameObject
{
private:
	explicit CDustLandingEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDustLandingEffect(const CGameObject& rhs);
	~CDustLandingEffect() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		ResetParticle() { m_pParticleCom->ResetAll(); }
	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();


private:
	CTexture*				m_pTextureCom;
	CDustLandingParticle*	m_pParticleCom;


public:
	static CDustLandingEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

