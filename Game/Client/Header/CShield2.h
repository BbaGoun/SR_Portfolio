#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CSphere;
}


class CShield2 :
	public CGameObject
{
private:
	explicit CShield2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShield2(const CGameObject& rhs);
	~CShield2() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void			SetShow(bool bShow) { m_bShow = bShow; }
	bool			GetShow()			{ return m_bShow; }

private:
	CSphere* m_pBufferCom;
	CTexture* m_pTextureCom;

	bool	m_bShow = false;
	float	m_fTimer;
	float	m_fScale = 1.f;
	float	m_fAlpha = 10.f;
public:
	static CShield2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};
