#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CSpeedGauge :
	public CGameObject
{
private:
	explicit CSpeedGauge(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSpeedGauge(const CSpeedGauge& rhs);
	virtual ~CSpeedGauge();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	Engine::CRcTex* m_pVIBufferCom;
	Engine::CTexture* m_pTextureCom;


	_float	m_fFrame;

public:
	static CSpeedGauge* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};

