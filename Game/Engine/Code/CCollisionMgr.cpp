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
			}
			else
			{
				//pDstCollider->OnCollision 호출
				//pSrcCollider->OnCollision 호출
			}
		}
	}
	else if (pDstType == CUBE_COLLIDER && pSrcType == SPHERE_COLLIDER)
	{
		if (CubeVsSphere(pDstCollider, pSrcCollider))
		{
			MSG_BOX("Collision!");
			if (pDstCollider->GetIsTrigger() || pSrcCollider->GetIsTrigger())
			{
				//pDstCollider->OnTrigger 호출
				//pSrcCollider->OnTrigger 호출
			}
			else
			{
				//pDstCollider->OnCollision 호출
				//pSrcCollider->OnCollision 호출
			}
		}
	}
	else if (pDstType == SPHERE_COLLIDER && pSrcType == CUBE_COLLIDER)
	{
		if (SphereVsCube(pDstCollider, pSrcCollider))
		{
			MSG_BOX("Collision!");
			if (pDstCollider->GetIsTrigger() || pSrcCollider->GetIsTrigger())
			{
				//pDstCollider->OnTrigger 호출
				//pSrcCollider->OnTrigger 호출
			}
			else
			{
				//pDstCollider->OnCollision 호출
				//pSrcCollider->OnCollision 호출
			}
		}
	}
	//else if (pDstType == CAPSULE_COLLIDER && pSrcType == CAPSULE_COLLIDER)
	//{
	//
	//}
	//else if (pDstType == SPHERE_COLLIDER && pSrcType == CAPSULE_COLLIDER)
	//{
	//
	//}
	//else if (pDstType == CAPSULE_COLLIDER && pSrcType == CUBE_COLLIDER)
	//{
	//
	//}

	return;
}

bool CCollisionMgr::CubeVsCube(CCollider* pDstCollider, CCollider* pSrcCollider)
{
	vector<_vec3> vAxis;

	CTransform* pDstTransform = dynamic_cast<CTransform*>(pDstCollider->Get_Owner()->Get_Transform());
	CTransform* pSrcTransform = dynamic_cast<CTransform*>(pSrcCollider->Get_Owner()->Get_Transform());

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

		float fDstRadius = fabs(D3DXVec3Dot(&vRight, &Axis)) 
						 + fabs(D3DXVec3Dot(&vUp,	 &Axis)) 
						 + fabs(D3DXVec3Dot(&vLook,  &Axis));

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

		float fSrcRadius = fabs(D3DXVec3Dot(&vRight, &Axis)) 
						 + fabs(D3DXVec3Dot(&vUp,	 &Axis)) 
						 + fabs(D3DXVec3Dot(&vLook,	 &Axis));
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

bool CCollisionMgr::CubeVsSphere(CCollider* pCube, CCollider* pSphere)
{
	CCube_Collider* pCubeCollider = dynamic_cast<CCube_Collider*>(pCube);
	CSphere_Collider* pSphereCollider = dynamic_cast<CSphere_Collider*>(pSphere);

	_vec3 vCubePos = pCubeCollider->GetCenter();
	_vec3 vSpherePos = pSphereCollider->GetCenter();

	_vec3 vDistance = vSpherePos - vCubePos;

	CTransform* pCubeTransform = pCube->Get_Owner()->Get_Transform();

	_matrix matCubeWorld;
	_vec3 vRight, vUp, vLook;
	matCubeWorld = *(pCubeTransform->Get_World());

	memcpy(&vRight, &matCubeWorld.m[0][0], sizeof(_vec3));
	memcpy(&vUp,	&matCubeWorld.m[1][0], sizeof(_vec3));
	memcpy(&vLook,	&matCubeWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	float fX = D3DXVec3Dot(&vDistance, &vRight);
	float fY = D3DXVec3Dot(&vDistance, &vUp);
	float fZ = D3DXVec3Dot(&vDistance, &vLook);

	fX = clampT(fX, -pCubeCollider->GetSize().x, pCubeCollider->GetSize().x);
	fY = clampT(fY, -pCubeCollider->GetSize().y, pCubeCollider->GetSize().y);
	fZ = clampT(fZ, -pCubeCollider->GetSize().z, pCubeCollider->GetSize().z);

	_vec3 vCloseSet = vCubePos + vRight * fX + vUp * fY + vLook * fZ;

	vDistance = (vCloseSet - vSpherePos);

	float fDistance = D3DXVec3Length(&vDistance);

	return fDistance <= pSphereCollider->GetRadius();
}

bool CCollisionMgr::SphereVsCube(CCollider* pSphere, CCollider* pCube)
{
	CCube_Collider* pCubeCollider = dynamic_cast<CCube_Collider*>(pCube);
	CSphere_Collider* pSphereCollider = dynamic_cast<CSphere_Collider*>(pSphere);

	_vec3 vCubePos = pCubeCollider->GetCenter();
	_vec3 vSpherePos = pSphereCollider->GetCenter();

	_vec3 vDistance = vSpherePos - vCubePos;

	CTransform* pCubeTransform = pCube->Get_Owner()->Get_Transform();

	_matrix matCubeWorld;
	_vec3 vRight, vUp, vLook;
	matCubeWorld = *(pCubeTransform->Get_World());

	memcpy(&vRight, &matCubeWorld.m[0][0], sizeof(_vec3));
	memcpy(&vUp, &matCubeWorld.m[1][0], sizeof(_vec3));
	memcpy(&vLook, &matCubeWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);
	D3DXVec3Normalize(&vLook, &vLook);

	float fX = D3DXVec3Dot(&vDistance, &vRight);
	float fY = D3DXVec3Dot(&vDistance, &vUp);
	float fZ = D3DXVec3Dot(&vDistance, &vLook);

	fX = clampT(fX, -pCubeCollider->GetSize().x, pCubeCollider->GetSize().x);
	fY = clampT(fY, -pCubeCollider->GetSize().y, pCubeCollider->GetSize().y);
	fZ = clampT(fZ, -pCubeCollider->GetSize().z, pCubeCollider->GetSize().z);

	_vec3 vCloseSet = vCubePos + vRight * fX + vUp * fY + vLook * fZ;

	vDistance = (vCloseSet - vSpherePos);

	float fDistance = D3DXVec3Length(&vDistance);

	return fDistance <= pSphereCollider->GetRadius();
}

//void CCollisionMgr::AddCollider(OBJID eID, CCollider* pCollider)
//{
//	if (eID > OBJ_END || pCollider == nullptr)
//		return;
//	m_ColliderList[eID].push_back(pCollider);
//}


