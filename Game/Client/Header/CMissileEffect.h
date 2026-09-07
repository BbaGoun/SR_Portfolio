#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"
#include "CCube_Collider.h"

namespace Engine
{
	class CSphere;
	class CRcTex;
}

class CMissileEffect : public CGameObject
{
private:
	explicit CMissileEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMissileEffect(const CGameObject& rhs);
	~CMissileEffect() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CSphere*	m_pBufferCom;
	CTexture*	m_pTextureCom;

	_float		m_fTimer;
	_float		m_fFrame = 0.f;

public:
	static CMissileEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};