#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_UnderBar :
	public CGameObject
{
private:
	explicit CUI_UnderBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_UnderBar(const CGameObject& rhs);
	~CUI_UnderBar() override;

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
	static CUI_UnderBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


