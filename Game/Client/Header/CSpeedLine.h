#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CSpeedLine :
	public CGameObject
{
private:
	explicit CSpeedLine(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSpeedLine(const CGameObject& rhs);
	~CSpeedLine() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void		SetCart(CGameObject* pCart) { m_pCart = pCart; }

private:
	CRcTex*				m_pBufferCom;
	CTexture*			m_pTextureCom;

	float				m_fFrame;
	bool				m_bShow;
	CGameObject*		m_pCart;

public:
	static CSpeedLine* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


