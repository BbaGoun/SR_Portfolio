#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CQuadrangularPrism;
	class CCube_Collider;
}
class CCollisionBox : public CGameObject
{
private:
	explicit CCollisionBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollisionBox(const CGameObject& rhs);
	virtual ~CCollisionBox() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	static CCollisionBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CQuadrangularPrism* m_pBufferCom;
	Engine::CCube_Collider*		m_pColliderCom;

protected:
	virtual		void		Free() override;
};

