#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_Shield2 :
	public CGameObject
{
private:
	explicit CUI_Shield2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Shield2(const CGameObject& rhs);
	~CUI_Shield2() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	bool	m_bCurState;
	float	m_fTimer;

public:
	static CUI_Shield2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};