#pragma once
#include "CGameObject.h"
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
	static CCart* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void	KeyInput(const _float& fDeltaTime);
private:
	_float	m_fSpeed;
	_float	m_fAngle;

protected:
	virtual		void		Free() override;
};

