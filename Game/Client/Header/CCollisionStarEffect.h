#pragma once
#include "CGameObject.h"
#include "CStarParticle.h"
#include "CTexture.h"

class CCollisionStarEffect : public CGameObject
{
private:
	explicit CCollisionStarEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollisionStarEffect(const CGameObject& rhs);
	~CCollisionStarEffect() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		ResetParticle() { m_pParticleCom->ResetAll(); }
	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();

private:
	CTexture*		m_pTextureCom;
	CStarParticle*	m_pParticleCom;


public:
	static CCollisionStarEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

