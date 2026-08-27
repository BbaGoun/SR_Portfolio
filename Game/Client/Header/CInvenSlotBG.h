#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CInvenSlotBG :
	public CGameObject
{
private:
	explicit CInvenSlotBG(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID);
	explicit CInvenSlotBG(const CInvenSlotBG& rhs);
	virtual ~CInvenSlotBG();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	//void			SetSelected();
	void			SetSelected(bool bSelected);//{ bSelected = m_bSelected; };
	bool	    	GetSelected() { return m_bSelected; }
	void			SetMouseHover(bool bMouseHover) { m_bMouseHover = bMouseHover; }

private:
	CRcTex*			m_pVIBufferCom;
	CTexture*		m_pTextureCom;

	float			m_fFrame;
	bool			m_bSelected;
	bool			m_bMouseHover;
	INEN_SLOT_NUM	m_eSlotNum;
	
public:
	static CInvenSlotBG* Create(LPDIRECT3DDEVICE9 pGraphicDev, INEN_SLOT_NUM eID);


protected:
	virtual		void		Free() override;

};