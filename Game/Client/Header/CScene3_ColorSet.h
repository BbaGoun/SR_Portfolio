#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}
class CScene3_ColorSet :
	public CGameObject
{
private:
	explicit CScene3_ColorSet(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_COLOR eID);
	explicit CScene3_ColorSet(const CScene3_ColorSet& rhs);
	virtual ~CScene3_ColorSet();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void			SetSelected(bool bSelected);//{ bSelected = m_bSelected; };
	bool	    	GetSelected() { return m_bSelected; }
	
	void						Set_Show1(bool bShow) { m_bShow = bShow; };
	bool						Get_Show1() { return m_bShow; }


private:
	CRcTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;

	_vec3	m_vPos;
	_vec3	m_vScale;


	float			m_fFrame;
	bool			m_bSelected;
	bool			m_bMouseHover;


public:
	static CScene3_ColorSet* Create(LPDIRECT3DDEVICE9 pGraphicDev, CHAR_COLOR eID);
	CHAR_COLOR	m_eColorNum;

	bool		m_bShow;

protected:
	virtual		void		Free() override;

};