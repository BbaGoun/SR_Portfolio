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
	}

	void		Rotate(QUATERNION eType, const _float& fAngle)
	{
		D3DXQUATERNION qDelta;
		D3DXQuaternionRotationAxis(&qDelta, &m_vInfo[eType], D3DXToRadian(fAngle));
		m_qRotation *= qDelta;
		D3DXQuaternionNormalize(&m_qRotation, &m_qRotation);
	}

	_matrix* Get_World() { return &m_matWorld; }
	void	 Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));
	}
	
public:
	void FollowObj(_vec3* _pPos, _float _fSpeed, _float _fTimeDelta);
	_matrix* GetFollowRotation(_vec3* _pFollowDir, _matrix* _pRot);

public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone() override;

public:
	_vec3			m_vInfo[INFO_END];

	D3DXQUATERNION	m_qRotation;
	_vec3			m_vScale;

	D3DXMATRIX		m_matWorld;

protected:
	virtual void		Free() override;
};

END