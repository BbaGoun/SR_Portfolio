#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CScene3_CharBG :
	public CGameObject
{
private:
	explicit CScene3_CharBG(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID);
	explicit CScene3_CharBG(const CScene3_CharBG& rhs);
	virtual ~CScene3_CharBG();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void			SetSelected(bool bSelected);//{ bSelected = m_bSelected; };
	bool	    	GetSelected() { return m_bSelected; }
	void			SetMouseHover(bool bMouseHover) { m_bMouseHover = bMouseHover; }

private:
	CRcTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;

	float			m_fFrame;
	bool			m_bSelected;
	bool			m_bMouseHover;
	CHAR_TYPE		m_eSlotNum;

public:
	static CScene3_CharBG* Create(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_TYPE eID);


protected:
	virtual		void		Free() override;

};