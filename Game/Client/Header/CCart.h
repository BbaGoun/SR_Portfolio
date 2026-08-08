#pragma once
#include "CGameObject.h"

enum CART_STATE {
	CART_STOP,
	CART_DRIVE,
	CART_DRIFT,
	CART_BOOST,
	CART_END
};
class CCart :  public CGameObject
{
private:
	explicit CCart(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCart(const CGameObject& rhs);
	virtual ~CCart() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override {};

public:
	static CCart*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void			KeyInput(const _float& fDeltaTime);
	void			UpdateState();
	void			COutCartState();

private:
	_float		m_fMaxSpeed;
	CART_STATE	m_eCurCartState;


protected:
	virtual		void		Free() override;
};

