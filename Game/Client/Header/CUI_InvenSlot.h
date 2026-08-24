#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
}
class CUI_InvenSlot :
	public CGameObject
{
private:
	explicit CUI_InvenSlot(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID);
	explicit CUI_InvenSlot(const CUI_InvenSlot& rhs);
	virtual ~CUI_InvenSlot();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void SetItem(CGameObject* pItem) { m_pItem = pItem; }
	void SetBG(CGameObject* pBG) { m_pBG = pBG; }

private:
	Engine::CRcTex* m_pVIBufferCom;

public:
	static CUI_InvenSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID);

	_vec3			m_vPos;
	_vec3			m_vScale;

	CGameObject*	m_pItem;
	CGameObject*	m_pBG;
	INEN_SLOT_NUM	m_eSlotNum;

protected:
	virtual		void		Free() override;

};