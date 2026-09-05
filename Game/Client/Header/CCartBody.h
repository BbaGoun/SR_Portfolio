#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
namespace Engine
{
	class CCube_Collider;
	class CSphere_Collider;
	class CArrowCol;
}
class CCartBody : public CGameObject
{
private:
	explicit CCartBody(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCartBody(const CGameObject& rhs);
	virtual ~CCartBody() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		PostReady_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

	
	virtual			void		CollisionEnter(CCollider* pOtherCollider) override;
	virtual			void		CollisionExit(CCollider* pOtherCollider) {};
	virtual			void		CollisionStay(CCollider* pOtherCollider) {};

	virtual			void		TriggerEnter(CCollider* pOtherCollider) override;
	virtual			void		TriggerExit(CCollider* pOtherCollider) {};
	virtual			void		TriggerStay(CCollider* pOtherCollider) {};

	void			BananaSpin(const _float& fDeltaTime);
	void			ThunderSpin(const _float& fDeltaTime);
	void			ThunderTimerUpdate(const _float& fDeltaTime);

	void			SetThunderSpinState(bool bThunderSpinState) { m_bThunderSpinState = bThunderSpinState; }
	bool			GetThunderSpinState()						{ return m_bThunderSpinState; }

	void			SetThunderTimer(bool fThunderTimer) { m_fThunderTimer = fThunderTimer; }
	float			GetThunderTimer() { return m_fThunderTimer; }

	void			SetThunderTimerOnOff(bool bThunderTimerOnOff) { 
		m_bThunderTimerOnOff = bThunderTimerOnOff; 
		m_bThunderSpinState = bThunderTimerOnOff;
	}
	float			GetThunderTimerOnOff() { return m_bThunderTimerOnOff; }

	bool			GetShieldHit() { return m_bShieldHit; }
	void			SetShieldHit(bool bShieldState) { m_bShieldHit = bShieldState; }

	bool			GetShieldActive() { return m_bShieldActive; }
	void			SetShieldActive(bool bActiveState) { m_bShieldActive = bActiveState; }

// Missile
	void			UpdateMissileHit(const _float& fDeltaTime);
	void			SetMissileHit(bool bHit) { m_bMissileHit = bHit; }

public:
	static CCartBody* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CCube_Collider* m_pColliderCom;

	bool			m_bBananaSpinState;
	bool			m_bThunderSpinState;
	float			m_fSpinSpeed;

	float			m_fScale;
	float			m_fThunderTimer;
	float			m_bThunderTimerOnOff;
	_vec3			m_vColliderSize;


	_float			m_fShieldTimer;
	_bool			m_bShieldHit;
	_bool			m_bShieldTimer;
	_bool			m_bShieldActive;

	bool			m_bMissileHit = false;
	float			m_fMissileTimer = 0.f;

protected:
	virtual		void		Free() override;
};

