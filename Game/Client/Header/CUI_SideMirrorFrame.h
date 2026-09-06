#pragma once
#include "CGameObject.h"
#include "CRcTex.h"
#include "CTexture.h"

class CUI_SideMirrorFrame :
	public CGameObject
{
private:
	explicit CUI_SideMirrorFrame(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_SideMirrorFrame(const CGameObject& rhs);
	~CUI_SideMirrorFrame() override;

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
	static CUI_SideMirrorFrame* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};


