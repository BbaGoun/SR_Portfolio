#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CBackGround :
	public CGameObject
{
private:
	explicit CBackGround(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBackGround(const CGameObject& rhs);
	~CBackGround() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void			Change_BackgroundTexture(BACKGROUND);
private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	BACKGROUND		m_eCurrentBackground = BACKGROUND_LOADING;

public:
	static CBackGround* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


