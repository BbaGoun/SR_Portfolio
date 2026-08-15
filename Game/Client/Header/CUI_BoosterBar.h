#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_BoosterBar :
	public CGameObject
{
private:
	explicit CUI_BoosterBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_BoosterBar(const CGameObject& rhs);
	~CUI_BoosterBar() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex*			m_pBufferCom;
	CTexture*		m_pTextureCom;

	float			m_fCurGage;
	float			m_fGainGage;
	float			m_fSizeX;

public:
	static CUI_BoosterBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


