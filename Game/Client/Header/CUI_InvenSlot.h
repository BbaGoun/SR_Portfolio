#pragma once
#include "CGameObject.h"
#include "CInvenSlotBG.h"

namespace Engine
{
	class CRcTex;
}
class CUI_InvenSlot :
	public CGameObject
{
private:
	explicit CUI_InvenSlot(LPDIRECT3DDEVICE9 pGraphicDev, INVEN_SLOT_NUM eID);
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

	void SetBGSelected(bool bSelected) { dynamic_cast<CInvenSlotBG*>(m_pBG)->SetSelected(bSelected); }
	bool GetBGSelected() { return dynamic_cast<CInvenSlotBG*>(m_pBG)->GetSelected(); }

	void Set_Slot(INVEN_SLOT_NUM eSlot) { m_eSelectSlot = eSlot; };


	void	Set_Show(bool bShow) { m_bShow = bShow; };
	bool	Get_Show() { return m_bShow; }

private:
	Engine::CRcTex* m_pVIBufferCom;

	

public:
	static CUI_InvenSlot* Create(LPDIRECT3DDEVICE9 pGraphicDev, INVEN_SLOT_NUM eID);

	_vec3			m_vPos;
	_vec3			m_vScale;

	bool			m_bShow;

	CGameObject*	m_pItem;
	CGameObject*	m_pBG;

	INVEN_SLOT_NUM	m_eSlotNum;
	INVEN_SLOT_NUM m_eSelectSlot;

protected:
	virtual		void		Free() override;

};