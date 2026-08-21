#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CSkidMark : public CGameObject
{
private:
	explicit CSkidMark(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkidMark(const CGameObject& rhs);
	~CSkidMark() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex*		m_pBufferCom;
	CTexture*	m_pTextureCom;

	float		m_fTimer;
	float		m_fFrame;

public:
	static CSkidMark* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


