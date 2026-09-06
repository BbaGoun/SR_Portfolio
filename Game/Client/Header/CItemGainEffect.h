#pragma once
#include "CGameObject.h"
#include "CItemGainParticle.h"
#include "CTexture.h"

class CItemGainEffect : public CGameObject
{
private:
	explicit CItemGainEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItemGainEffect(const CGameObject& rhs);
	~CItemGainEffect() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		ResetParticle() { m_pParticleCom->ResetAll(); }
	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();

private:
	CTexture* m_pTextureCom;
	CItemGainParticle* m_pParticleCom;


public:
	static CItemGainEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

