#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CRcTex.h"
#include "CTexture.h"

class CMirror :
	public CGameObject
{
private:
	explicit CMirror(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMirror(const CGameObject& rhs);
	~CMirror() override;

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
	static CMirror* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};
