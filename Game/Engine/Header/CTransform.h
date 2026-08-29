#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTransform :
    public CComponent
{
protected:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	virtual _int Update_Component(const _float& fDeltaTime) override;
	virtual void LateUpdate_Component(const _float& fDeltaTime) override;

protected:
	HRESULT	Ready_Transform();

public:
	void		Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fDeltaTime)
	{
		m_vInfo[INFO_POS] += *pDir * fSpeed * fDeltaTime;
		Set_Dirty();
	}

	void		Set_Pos(_vec3 _newPos) {
		m_vInfo[INFO_POS] = _newPos;
		Set_Dirty();
	}
	void		Set_Scale(_vec3 _newScale) {
		m_vScale = _newScale;
		Set_Dirty();
	}

	void		Set_Quaternion(D3DXQUATERNION* pQuater) {
		m_localQuaternion = m_defaultQuaternion * (*pQuater);
		D3DXQuaternionNormalize(&m_localQuaternion, &m_localQuaternion);
		Set_Dirty();
	}
	void		Set_DefaultQuaternion(D3DXQUATERNION* pQuater) {
		m_defaultQuaternion = *pQuater;
		m_localQuaternion = *pQuater;
		Set_Dirty();
	}

	void		Set_LocalWorld(_matrix* _MatLocal);

	_quaternion Get_Quaternion() {
		return m_localQuaternion;
	}
	_quaternion Get_WorldQuaternion() {
		Get_World();
		return m_worldQuaternion;
	}

	void		Rotate(QUATERNION eType, _float fAngle)
	{
		if (eType == QUATER_PITCH)
			fAngle = clampT(fAngle, -89.f, 89.f);

		D3DXQUATERNION qDelta;
		_vec3 vAxis;
		D3DXVec3Normalize(&vAxis, &m_vInfo[eType]);
		D3DXQuaternionRotationAxis(&qDelta, &vAxis, D3DXToRadian(fAngle));

		m_localQuaternion *= qDelta;
		D3DXQuaternionNormalize(&m_localQuaternion, &m_localQuaternion);
		Set_Dirty();
	}
	void		Multiple_Quaternion(D3DXQUATERNION* pQuater) {
		m_localQuaternion *= *pQuater;
		D3DXQuaternionNormalize(&m_localQuaternion, &m_localQuaternion);
		Set_Dirty();
	}

	_matrix* Get_World();
	_matrix* Get_LocalWorld() {
		Get_World();
		return &m_matLocalWorld;
	}
	void	 Get_Info(INFO eType, _vec3* pInfo)
	{
		_matrix* pMatWorld = Get_World();
		_vec3 vUnit;
		memcpy(vUnit, &pMatWorld->m[eType][0], sizeof(_vec3));
		if (eType != INFO_POS)
			D3DXVec3Normalize(&vUnit, &vUnit);
		*pInfo = vUnit;
	}
	void	 Get_LocalInfo(INFO eType, _vec3* pInfo)
	{
		Get_World();
		_vec3 vUnit;
		memcpy(vUnit, &m_matLocalWorld.m[eType][0], sizeof(_vec3));
		if (eType != INFO_POS)
			D3DXVec3Normalize(&vUnit, &vUnit);
		*pInfo = vUnit;
	}
	void Set_Dirty();

	_vec3		Get_Scale() { return m_vScale; }
	void		Set_Billboard(_matrix* matView);
	
public:
	void FollowObj(_vec3* _pPos, _float _fSpeed, _float _fDeltaTime);
	_matrix* GetFollowRotation(_vec3* _pFollowDir, _matrix* _pRot);
	_quaternion* GetFollowQuaternion(_vec3* _pFollowDir, _quaternion* _pQuater);

	void		Chase_Target(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta);
	_matrix*	Compute_Lookattarget(const _vec3* pPos);

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;

private:
	_vec3			m_vInfo[INFO_END];

	D3DXQUATERNION	m_worldQuaternion;
	D3DXQUATERNION	m_localQuaternion;
	_quaternion		m_defaultQuaternion;
	_vec3			m_vScale;

	_matrix			m_matBillboard;
	_matrix			m_matWorld;
	_matrix			m_matLocalWorld;
	_bool			m_bDirty;

protected:
	virtual void		Free() override;
};

END