#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CHUD_Gage :
	public CGameObject
{
private:
	explicit CHUD_Gage(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHUD_Gage(const CGameObject& rhs);
	~CHUD_Gage() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float m_fFrame;

public:
	static CHUD_Gage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


