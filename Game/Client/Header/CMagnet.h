#pragma once

#include "CGameObject.h"

namespace Engine
{
	class CMagnetTex;
}

class CMagnet : public CGameObject
{
private:
	explicit CMagnet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMagnet() override;

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
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

	_float			m_fTimer;
	CGameObject*	m_pOwner;

public:
	static CMagnet* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOnwer);

protected:
	virtual void	Free() override;

};
