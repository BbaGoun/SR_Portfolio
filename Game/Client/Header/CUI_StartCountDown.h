#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_StartCountDown :
	public CGameObject
{
private:
	explicit CUI_StartCountDown(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_StartCountDown(const CGameObject& rhs);
	~CUI_StartCountDown() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void		UpdateFrame();
	void		SetFrame(int iFrame);

private:
	CRcTex*		m_pBufferCom;
	CTexture*	m_pTextureCom;

	
	float		m_fFrame;
	bool		m_bShow;
	float		m_fScale;
	float		m_fTimer;
	float		m_fTimerFlag;

public:
	static CUI_StartCountDown* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


