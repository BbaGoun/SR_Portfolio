#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CSphere;
}

class CShield1 :
	public CGameObject
{
private:
	explicit CShield1(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShield1(const CGameObject& rhs);
	~CShield1() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CSphere* m_pBufferCom;
	CTexture* m_pTextureCom;

	bool	m_bCurState;
	float	m_fTimer;

public:
	static CShield1* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};