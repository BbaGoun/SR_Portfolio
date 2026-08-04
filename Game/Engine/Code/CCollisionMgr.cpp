#include "CCollisionMgr.h"
#include "CManagement.h"

#include <io.h>
#include <iostream>
#include "CCube_Collider.h"
#include "CSphere_Collider.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::FixedUpdate_Component()
{
}

_int CCollisionMgr::Update_Component(const _float& fTimeDelta)
{
	return 0;
}
void CCollisionMgr::LateUpdate_Component()
{
}

void CCollisionMgr::Collision(CCollider* pDstCollider, CCollider* pSrcCollider)
{
	COLLIDER_TYPE pDstType = pDstCollider->GetColliderType();
	COLLIDER_TYPE pSrcType = pSrcCollider->GetColliderType();

	if (pDstType == CUBE_COLLIDER && pSrcType == CUBE_COLLIDER)
	{
		if (CubeVsCube(pDstCollider, pSrcCollider))
		{
			MSG_BOX("Collision!");
			if (pDstCollider->GetIsTrigger() || pSrcCollider->GetIsTrigger())
			{
				//pDstCollider->OnTrigger 호출
				//pSrcCollider->OnTrigger 호출
				return;
			}
			else
			{
				//pDstCollider->OnCollision 호출
				//pSrcCollider->OnCollision 호출
			}
		}
	}
	else if (pDstType == SPHERE_COLLIDER && pSrcType == SPHERE_COLLIDER)
	{
		if (SphereVsSphere(pDstCollider, pSrcCollider))
		{
			MSG_BOX("Collision!");
			if (pDstCollider->GetIsTrigger() || pSrcCollider->GetIsTrigger())
			{
				//pDstCollider->OnTrigger 호출
				//pSrcCollider->OnTrigger 호출
				return;
			}
			else
			{
				//pDstCollider->OnCollision 호출
				//pSrcCollider->OnCollision 호출
			}
		}

	}
	else if (pDstType == CAPSULE_COLLIDER && pSrcType == CAPSULE_COLLIDER)
	{

	}
	else if (pDstType == CUBE_COLLIDER && pSrcType == SPHERE_COLLIDER)
	{

	}
	else if (pDstType == SPHERE_COLLIDER && pSrcType == CAPSULE_COLLIDER)
	{

	}
	else if (pDstType == CAPSULE_COLLIDER && pSrcType == CUBE_COLLIDER)
	{

	}

	return;
}

bool CCollisionMgr::CubeVsCube(CCollider* pDstCollider, CCollider* pSrcCollider)
{
	vector<_vec3> vAxis;

	CTransform* pDstTransform = dynamic_cast<CTransform*>(pDstCollider->Get_Owner()->Get_Component(ID_DYNAMIC, L"Com_Transform"));
	CTransform* pSrcTransform = dynamic_cast<CTransform*>(pSrcCollider->Get_Owner()->Get_Component(ID_DYNAMIC, L"Com_Transform"));

	_vec3 vDstAxis,vSrcAxis;
	for (int i = 0; i < 3; ++i)
	{
		memcpy(&vDstAxis, &(pDstTransform->Get_World()->m[i][0]), sizeof(_vec3));
		vAxis.push_back(*D3DXVec3Normalize(&vDstAxis,&vDstAxis));

		memcpy(&vSrcAxis, &(pSrcTransform->Get_World()->m[i][0]), sizeof(_vec3));
		vAxis.push_back(*D3DXVec3Normalize(&vSrcAxis, &vSrcAxis));
		for (int j = 0; j < 3; j++)
		{
			memcpy(&vSrcAxis, &(pSrcTransform->Get_World()->m[j][0]), sizeof(_vec3));
			_vec3 vCross;
			D3DXVec3Cross(&vCross, &vDstAxis, &vSrcAxis);
			if (D3DXVec3Length(&vCross) > 0.f)
			{
				vAxis.push_back(*D3DXVec3Normalize(&vCross, &vCross));
			}
		}
	}

	_vec3 vDstPos = dynamic_cast<CCube_Collider*>(pDstCollider)->GetCenter();
	_vec3 vSrcPos = dynamic_cast<CCube_Collider*>(pSrcCollider)->GetCenter();

	_matrix	matDstWorld, matSrcWorld;
	_vec3 vRight, vUp, vLook, vDstScale, vSrcScale;

	vDstScale = dynamic_cast<CCube_Collider*>(pDstCollider)->GetSize();
	vSrcScale = dynamic_cast<CCube_Collider*>(pSrcCollider)->GetSize();
	for (auto Axis : vAxis)
	{
		// 중심좌표 투영
		float fDstCenter = D3DXVec3Dot(&vDstPos, &Axis);
		float fSrcCenter = D3DXVec3Dot(&vSrcPos, &Axis);
		float fDistance = fabsf(fDstCenter - fSrcCenter);

		// 반지름 투영
		matDstWorld = *(pDstTransform->Get_World());
		memcpy(&vRight, &matDstWorld.m[0][0], sizeof(_vec3));
		memcpy(&vUp,	&matDstWorld.m[1][0], sizeof(_vec3));
		memcpy(&vLook,	&matDstWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);
		D3DXVec3Normalize(&vLook, &vLook);

		vRight	*= vDstScale.x;
		vUp		*= vDstScale.y;
		vLook	*= vDstScale.z;
		_vec3 vSum = vDstPos + vRight + vUp + vLook;
		float fDstRadius = fabsf(D3DXVec3Dot(&vSum, &Axis) - fDstCenter);

		matSrcWorld = *(pSrcTransform->Get_World());
		memcpy(&vRight, &matSrcWorld.m[0][0], sizeof(_vec3));
		memcpy(&vUp,	&matSrcWorld.m[1][0], sizeof(_vec3));
		memcpy(&vLook,	&matSrcWorld.m[2][0], sizeof(_vec3));

		D3DXVec3Normalize(&vRight,	&vRight);
		D3DXVec3Normalize(&vUp,		&vUp);
		D3DXVec3Normalize(&vLook,	&vLook);

		vRight	*= vSrcScale.x;
		vUp		*= vSrcScale.y;
		vLook	*= vSrcScale.z;
		vSum = vSrcPos + vRight + vUp + vLook;

		float fSrcRadius = fabsf(D3DXVec3Dot(&vSum, &Axis) - fSrcCenter);
		if (fDistance > fDstRadius + fSrcRadius)
			return false;
	}
	return true;
}

bool CCollisionMgr::SphereVsSphere(CCollider* pDst, CCollider* pSrc)
{
	_vec3 vDstPos = dynamic_cast<CSphere_Collider*>(pDst)->GetCenter();
	_vec3 vSrcPos = dynamic_cast<CSphere_Collider*>(pSrc)->GetCenter();
	
	float vDstRadius = dynamic_cast<CSphere_Collider*>(pDst)->GetRadius();
	float vSrcRadius = dynamic_cast<CSphere_Collider*>(pSrc)->GetRadius();

	_vec3 vDeltaPos = vDstPos - vSrcPos;
	
	if (D3DXVec3Length(&vDeltaPos) > vDstRadius + vSrcRadius)
		return false;
	return true;
}

bool CCollisionMgr::CubeVsSphere(CCollider* pDst, CCollider* pSrc)
{

	return false;
}

bool CCollisionMgr::SphereVsCube(CCollider* pDst, CCollider* pSrc)
{

	return false;
}

//void CCollisionMgr::AddCollider(OBJID eID, CCollider* pCollider)
//{
//	if (eID > OBJ_END || pCollider == nullptr)
//		return;
//	m_ColliderList[eID].push_back(pCollider);
//}


