#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :
	public CComponent
{
protected:
	explicit CCamera();
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component(const _float& fTimeDelta) override;
	void SetCamera_BeforeRender();

public:
	void Rotate(QUATERNION eType, const _float& fAngle)
	{
		*(((float*)&m_vAngle) + eType) += fAngle;

		m_vAngle.x = clampT<float>(m_vAngle.x, -30.f, 30.f);
	}

protected:
	HRESULT	Ready_Camera();

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;

public:
	_vec3			m_vAngle;

	_float			m_fFov;
	_float			m_fAspect;
	_float			m_fNear;
	_float			m_fFar;

protected:
	virtual void		Free() override;
};

END