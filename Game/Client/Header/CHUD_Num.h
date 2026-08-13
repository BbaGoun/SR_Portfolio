#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CHUD_Num :
	public CGameObject
{
private:
	explicit CHUD_Num(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CHUD_Num(const CGameObject& rhs);
	~CHUD_Num() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CRcTex* m_pBufferCom;
	CTexture* m_pTextureCom;

	float m_fCartSpeed;
	float m_fFirstFrame;
	float m_fSecondFrame;
	float m_fThirdFrame;
	float m_fNum;

public:
	static CHUD_Num* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


