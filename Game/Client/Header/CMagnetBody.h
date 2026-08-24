#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CMagnetTex;
	class CCube_Collider;
}

class CMagnetBody : public CGameObject
{
private:
	explicit CMagnetBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMagnetBody() override;

public:
	virtual			HRESULT		Ready_GameObject()									  override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)			  override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)		  override;
	virtual			void		Render_GameObject()									  override;

	virtual			void		CollisionEnter(CCollider* pOtherCollider) override;
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void		TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

private:
	Engine::CMagnetTex* m_pBufferCom;
	CCube_Collider* m_pColliderCom;

	_float			m_fTimer;

public:
	static CMagnetBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void	Free() override;

};
