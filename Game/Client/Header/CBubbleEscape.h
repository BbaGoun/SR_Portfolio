#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CBubbleEscape :
	public CGameObject
{
private:
	explicit CBubbleEscape(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBubbleEscape(const CGameObject& rhs);
	~CBubbleEscape() override;

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
	_float	m_fFrame;

public:
	static CBubbleEscape* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


