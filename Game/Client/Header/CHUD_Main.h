#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CHUD_Main :
	public CGameObject
{
private:
	explicit CHUD_Main(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHUD_Main(const CGameObject& rhs);
	~CHUD_Main() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

public:
	static CHUD_Main* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


