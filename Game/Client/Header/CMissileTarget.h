#pragma once
#include "CGameObject.h"
namespace Engine
{
	class CCartBodyCol;
	class CCube_Collider;
}
class CMissileTarget : public CGameObject
{
private:
	explicit CMissileTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMissileTarget() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	virtual			void		CollisionEnter(CCollider* pOtherCollider) override;
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void	    TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

	bool		GetMissileHit() { return m_bMissileHit; }
	void		SetMissileHit(bool bMissileHit) { m_bMissileHit = bMissileHit; }
public:
	static CMissileTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CCartBodyCol*		m_pBufferCom;
	Engine::CCube_Collider*		m_pColliderCom;
			_float				m_fMaxSpeed;
			_bool				m_bMissileHit;
			_bool				m_bWaterBombHit;

protected:
	virtual		void		Free() override;
};

