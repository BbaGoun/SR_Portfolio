#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CScene3_ColorSetBG :
	public CGameObject
{
private:
	explicit CScene3_ColorSetBG(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CScene3_ColorSetBG(const CGameObject& rhs);
	~CScene3_ColorSetBG() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;
	void						Set_Show(bool bShow) { m_bShow = bShow; };
	bool						Get_Show() { return m_bShow; }
	

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	bool	m_bShow;

	
public:
	static CScene3_ColorSetBG* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	

protected:
	virtual		void		Free() override;
};


