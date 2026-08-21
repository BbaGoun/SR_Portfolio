#pragma once
#include "CGameObject.h"


namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CUI_ItemIcon :
	public CGameObject
{
private:
	explicit CUI_ItemIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_ItemIcon(const CUI_ItemIcon& rhs);
	virtual ~CUI_ItemIcon();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


private:
	Engine::CRcTex*		m_pVIBufferCom;
	Engine::CTexture*	m_pTextureCom;

	ITEM_TYPE			m_eFirstSlot;
	ITEM_TYPE			m_eSecondSlot;

public:
	static CUI_ItemIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};