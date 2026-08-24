#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}

class CUI_Menu :
	public CGameObject
{
private:
	explicit CUI_Menu(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Menu(const CUI_Menu& rhs);
	virtual ~CUI_Menu();

private:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:
	CRcTex* m_pVIBufferCom;
	CTexture* m_pTextureCom;



public:
	static CUI_Menu* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


