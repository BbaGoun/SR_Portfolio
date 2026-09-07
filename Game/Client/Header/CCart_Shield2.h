#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CSphere;
}


class CCart_Shield2 :
	public CGameObject
{
private:
	explicit CCart_Shield2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCart_Shield2(const CGameObject& rhs);
	~CCart_Shield2() override;

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
	static CCart_Shield2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};
