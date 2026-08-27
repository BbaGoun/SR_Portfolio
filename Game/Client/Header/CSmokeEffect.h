#pragma once
#include "CGameObject.h"
#include "CSmoke.h"
#include "CTexture.h"

class CSmokeEffect :  public CGameObject
{
private:
	explicit CSmokeEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSmokeEffect(const CGameObject& rhs);
	~CSmokeEffect() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;


	virtual			void		OnLostDevice();
	virtual			void		OnResetDevice();
	virtual			void		SetCart(CGameObject* pObj);

private:
	CTexture*		m_pTextureCom;
	CSmoke*			m_pSmoke;

	CGameObject*	m_pCart;

public:
	static CSmokeEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual		void		Free() override;
};

