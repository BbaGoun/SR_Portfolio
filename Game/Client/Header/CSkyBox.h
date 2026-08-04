#pragma once
#include "CGameObject.h"
#include "CTransform.h"
#include "CInnerBox.h"
#include "CTexture.h"

class CSkyBox :
	public CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyBox(const CGameObject& rhs);
	~CSkyBox() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

private:

private:
	CInnerBox* m_pBufferCom;
	CTexture* m_pTextureCom;

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};
