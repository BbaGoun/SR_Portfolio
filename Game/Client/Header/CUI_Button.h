#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_Button :
	public CGameObject
{
private:
	explicit CUI_Button(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Button(const CGameObject& rhs);
	~CUI_Button() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float	m_fFrame;
	_vec3	m_vPos;
	_vec3	m_vScale;
public:
	static CUI_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


