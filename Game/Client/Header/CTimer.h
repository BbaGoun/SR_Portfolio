#pragma once
#include "CGameObject.h"


namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CTimer :
	public CGameObject
{
private:
	explicit CTimer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTimer(const CTimer& rhs);
	virtual ~CTimer();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	Engine::CRcTex* m_pVIBufferCom;
	Engine::CTexture* m_pTextureCom;

	_float		m_fFrame_1;
	_float		m_fFrame_10;
	_float		m_fFrame_100;


public:
	static CTimer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	

protected:
	virtual		void		Free() override;

};