#pragma once
#include "CGameObject.h"
#include "Engine_Enum.h"
#include "CDriftTrail.h"
#include "CSkidMark.h"

namespace Engine
{
	class CCartWheelCol;
	class CCube_Collider;
}
class CWheel : public CGameObject
{
private:
	explicit CWheel(LPDIRECT3DDEVICE9 pGraphicDev, WHEEL_TYPE eType);
	explicit CWheel(const CGameObject& rhs);
	virtual ~CWheel() override;

public:
	virtual			HRESULT		Ready_GameObject() override;
	virtual			void		FixedUpdate_GameObject(const _float& fFixedDeltaTime) override;
	virtual			_int		Update_GameObject(const _float& fDeltaTime) override;
	virtual			void		LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual			void		Render_GameObject() override;

public:
	void			SetWheelType(WHEEL_TYPE eType) { m_eWheelType = eType; }
	WHEEL_TYPE		GetWheelType() { return m_eWheelType; }
	void			KeyInput(const _float& fDeltaTime);

	void			ResetPrePos();
	void			CreateSkidMark();
	void			CreateDriftTrail();
	bool			CheckInTerrain();
	void			ForgetDriftTrail(CDriftTrail* pDriftTrail);
	void			ForgetSkidMark(CSkidMark* pSkidMark);
public:
	static CWheel* Create(LPDIRECT3DDEVICE9 pGraphicDev, WHEEL_TYPE eType);

private:
	Engine::CCartWheelCol*	m_pBufferCom;
	Engine::CCube_Collider* m_pColliderCom;
	WHEEL_TYPE				m_eWheelType;

	float					m_fDistSum;
	_vec3					m_vPrePos;

	float					m_fScale;
	_vec3					m_vColliderSize;

	float					m_fRayMinDist;

	CDriftTrail*			m_pDriftTrail = nullptr;
	CSkidMark*				m_pSkidMark = nullptr;

protected:
	virtual		void		Free() override;
};

