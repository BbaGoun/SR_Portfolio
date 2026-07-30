#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CTransform :
    public CComponent
{
protected:
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();

public:
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component(const _float& fTimeDelta) override;

protected:
	HRESULT	Ready_Transform();

public:
	void		Move_Pos(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
	{
		m_vInfo[INFO_POS] += *pDir * fSpeed * fTimeDelta;
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
		m_quaternion = *pQuater;
		Set_Dirty();
	}
	void		Rotate(QUATERNION eType, _float fAngle)
	{
		if (eType == QUATER_PITCH)
			fAngle = clampT(fAngle, -89.f, 89.f);

		D3DXQUATERNION qDelta;
		_vec3 vAxis;
		D3DXVec3Normalize(&vAxis, &m_vInfo[eType]);
		D3DXQuaternionRotationAxis(&qDelta, &vAxis, D3DXToRadian(fAngle));

		m_quaternion *= qDelta;
		D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
		Set_Dirty();
	}
	void		Multiple_Quaternion(D3DXQUATERNION* pQuater) {
		m_quaternion *= *pQuater;
		D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);
		Set_Dirty();
	}

	_matrix* Get_World();
	void	 Get_Info(INFO eType, _vec3* pInfo)
	{
		_matrix* pMatWorld = Get_World();
		_vec3 vUnit;
		memcpy(vUnit, &pMatWorld->m[eType][0], sizeof(_vec3));
		if (eType != INFO_POS)
			D3DXVec3Normalize(&vUnit, &vUnit);
		*pInfo = vUnit;
	}
	void Set_Dirty();
	
public:
	void FollowObj(_vec3* _pPos, _float _fSpeed, _float _fTimeDelta);
	_matrix* GetFollowRotation(_vec3* _pFollowDir, _matrix* _pRot);

private:

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;

private:
	_vec3			m_vInfo[INFO_END];

	D3DXQUATERNION	m_quaternion;
	_vec3			m_vScale;

	D3DXMATRIX		m_matWorld;
	_bool			m_bDirty;

protected:
	virtual void		Free() override;
};

END