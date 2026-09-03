#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CHalfSphere.h"
#include "CTexture.h"

class CSkyDome :
	public CGameObject
{
private:
	explicit CSkyDome(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyDome(const CGameObject& rhs);
	~CSkyDome() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CHalfSphere* m_pBufferCom;
	CTexture* m_pTextureCom;

public:
	static CSkyDome* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};
