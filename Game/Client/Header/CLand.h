#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CTerrain.h"
#include "CTexture.h"

class CLand :
	public CGameObject
{
private:
	explicit CLand(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLand(const CGameObject& rhs);
	~CLand() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta) override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CTransform* m_pTransformCom;
	CTerrain* m_pBufferCom;
	CTexture* m_pTextureCom;

public:
	static CLand* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};
