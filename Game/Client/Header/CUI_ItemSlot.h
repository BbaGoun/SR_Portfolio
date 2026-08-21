#pragma once
#include "CGameObject.h"


namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CUI_ItemSlot :
	public CGameObject
{
private:
	explicit CUI_ItemSlot(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_ItemSlot(const CUI_ItemSlot& rhs);
	virtual ~CUI_ItemSlot();

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
	static CUI_ItemSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};