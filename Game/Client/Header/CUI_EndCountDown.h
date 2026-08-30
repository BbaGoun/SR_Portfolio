#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_EndCountDown :
	public CGameObject
{
private:
	explicit CUI_EndCountDown(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_EndCountDown(const CGameObject& rhs);
	~CUI_EndCountDown() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	void		SetFrame(int iFrame);
	void		Shake(const _float fDeltaTime);
	void		UdateFrame();

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;


	float		m_fFrame;
	bool		m_bShow;
	float		m_fScale;
	float		m_fTimer;
	int			m_iShakeCnt;
	float		m_fTimerFlag;

public:
	static CUI_EndCountDown* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


