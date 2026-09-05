#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CHalfSphere;
}

class CCart_Shield1 :
	public CGameObject
{
private:
	explicit CCart_Shield1(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCart_Shield1(const CGameObject& rhs);
	~CCart_Shield1() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CHalfSphere* m_pBufferCom;
	CTexture* m_pTextureCom;

	bool	m_bCurState;
	float	m_fTimer;

public:
	static CCart_Shield1* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};
