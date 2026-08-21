#pragma once
#include "CGameObject.h"


namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CItemicon :
	public CGameObject
{
private:
	explicit CItemicon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CItemicon(const CItemicon& rhs);
	virtual ~CItemicon();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	Engine::CRcTex* m_pVIBufferCom;
	Engine::CTexture* m_pTextureCom;



public:
	static CItemicon* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};