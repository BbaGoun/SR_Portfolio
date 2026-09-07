#pragma once
#include "CGameObject.h"
#include "CCartBot.h"
#include "CCart.h"


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

public:
	void			Set_Player(CCart* pCart) { m_pCart = pCart; m_pCartBot = nullptr; }
	void			Set_Bot(CCartBot* pCartBot) { m_pCart = nullptr; m_pCartBot = pCartBot; }

	void			Set_PosFirst(_vec3 vPos) { m_vPosFirst = vPos; }
	void			Set_PosSecond(_vec3 vPos) { m_vPosSecond = vPos; }
	void			Set_ScaleFirst(_vec3 vScale) { m_vScaleFirst = vScale; }
	void			Set_ScaleSecond(_vec3 vScale) { m_vScaleSecond = vScale; }

private:
	Engine::CRcTex*		m_pVIBufferCom;
	Engine::CTexture*	m_pTextureCom;

	ITEM_TYPE			m_eFirstSlot = ITEM_END;
	ITEM_TYPE			m_eSecondSlot = ITEM_END;

	_vec3				m_vPosFirst;
	_vec3				m_vPosSecond;
	_vec3				m_vScaleFirst;
	_vec3				m_vScaleSecond;

	CCart*				m_pCart = nullptr;
	CCartBot*			m_pCartBot = nullptr;

public:
	static CUI_ItemIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual		void		Free() override;

};