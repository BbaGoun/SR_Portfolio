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

void CCollisionMgr::Collision(CCollider* pDstCollider, CCollider* pSrcCollider)
{
	COLLIDER_TYPE pDstType = pDstCollider->GetColliderType();
	COLLIDER_TYPE pSrcType = pSrcCollider->GetColliderType();

	if (pDstType == CUBE_COLLIDER && pSrcType == CUBE_COLLIDER)
	{
		CCube_Collider* pDstCubeCollider = dynamic_cast<CCube_Collider*>(pDstCollider);
		CCube_Collider* pSrcCubeCollider = dynamic_cast<CCube_Collider*>(pSrcCollider);
		if (pDstCubeCollider->Get_Info().Intersects(pSrcCubeCollider->Get_Info()))
		{
			//MSG_BOX("Collision!");
			if (pDstCollider->GetIsTrigger() || pSrcCollider->GetIsTrigger())
			{
				pDstCubeCollider->Get_Owner()->TriggerEnter();
				pSrcCubeCollider->Get_Owner()->TriggerEnter();
			}
			else
			{
				PysicalCubevsCube(pDstCubeCollider, pSrcCubeCollider);
				pDstCubeCollider->Get_Owner()->TriggerEnter();
				pSrcCubeCollider->Get_Owner()->TriggerEnter();
			}
		}
	}
	//else if (pDstType == SPHERE_COLLIDER && pSrcType == SPHERE_COLLIDER)
	//{
	//	if (SphereVsSphere(pDstCollider, pSrcCollider))
	//	{
	//		MSG_BOX("Collision!");
	//		if (pDstCollider->GetIsTrigger() || pSrcCollider->GetIsTrigger())
	//		{
	//			//pDstCollider->OnTrigger 호출
	//			//pSrcCollider->OnTrigger 호출
	//		}
	//		else
	//		{
	//			//pDstCollider->OnCollision 호출
	//			//pSrcCollider->OnCollision 호출
	//		}
	//	}
	//}
	//else if (pDstType == CUBE_COLLIDER && pSrcType == SPHERE_COLLIDER)
	//{
	//	if (CubeVsSphere(pDstCollider, pSrcCollider))
	//	{
	//		MSG_BOX("Collision!");
	//		if (pDstCollider->GetIsTrigger() || pSrcCollider->GetIsTrigger())
	//		{
	//			//pDstCollider->OnTrigger 호출
	//			//pSrcCollider->OnTrigger 호출
	//		}
	//		else
	//		{
	//			//pDstCollider->OnCollision 호출
	//			//pSrcCollider->OnCollision 호출
	//		}
	//	}
	//}
	//else if (pDstType == SPHERE_COLLIDER && pSrcType == CUBE_COLLIDER)
	//{
	//	if (SphereVsCube(pDstCollider, pSrcCollider))
	//	{
	//		MSG_BOX("Collision!");
	//		if (pDstCollider->GetIsTrigger() || pSrcCollider->GetIsTrigger())
	//		{
	//			//pDstCollider->OnTrigger 호출
	//			//pSrcCollider->OnTrigger 호출
	//		}
	//		else
	//		{
	//			//pDstCollider->OnCollision 호출
	//			//pSrcCollider->OnCollision 호출
	//		}
	//	}
	//}
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

void CCollisionMgr::PysicalCubevsCube(CCube_Collider* pDst, CCube_Collider* pSrc)
{
	_vec3 DstCenter = ToVec3(pDst->Get_Info().Center);
	_vec3 SrcCenter = ToVec3(pSrc->Get_Info().Center);
	_vec3 vDS = SrcCenter - DstCenter;
	D3DXVec3Normalize(&vDS,&vDS);
	

	_vec3 DstHalfSize = ToVec3(pDst->Get_Info().Extents);
	_vec3 SrcHalfSize = ToVec3(pSrc->Get_Info().Extents);

	float fX = (DstHalfSize.x + SrcHalfSize.x) - fabsf(DstCenter.x - SrcCenter.x);
	float fY = (DstHalfSize.y + SrcHalfSize.y) - fabsf(DstCenter.y - SrcCenter.y);
	float fZ = (DstHalfSize.z + SrcHalfSize.z) - fabsf(DstCenter.z - SrcCenter.z);

	CGameObject* pDstObj = pDst->Get_Owner();
	CTransform* pDstTransform = pDstObj->Get_Transform();

	CGameObject* pSrcObj = pSrc->Get_Owner();
	CTransform* pSrcTransform = pSrcObj->Get_Transform();


	_vec3 vPos;
	_vec3 MTV = { 0,0,0 };
	pSrcTransform->Get_Info(INFO_POS, &vPos);
	if (fY <= fX && fY <= fZ)
	{
		vPos.y += vDS.y * fY;
	}
	else
	{
		// 1. 축 정리
		vector<_vec3> vAxis;
		_vec3 vDstAxis, vSrcAxis;

		memcpy(&vDstAxis, &(pDstTransform->Get_World()->m[0][0]), sizeof(_vec3));
		vAxis.push_back(*D3DXVec3Normalize(&vDstAxis, &vDstAxis));

		memcpy(&vSrcAxis, &(pSrcTransform->Get_World()->m[2][0]), sizeof(_vec3));
		vAxis.push_back(*D3DXVec3Normalize(&vSrcAxis, &vSrcAxis));


		// 2. 투영
		_vec3 vDstPos = ToVec3(dynamic_cast<CCube_Collider*>(pDst)->Get_Info().Center);
		_vec3 vSrcPos = ToVec3(dynamic_cast<CCube_Collider*>(pSrc)->Get_Info().Center);

		_matrix	matDstWorld, matSrcWorld;
		_vec3 vRight, vUp, vLook, vDstScale, vSrcScale;

		vDstScale = ToVec3(dynamic_cast<CCube_Collider*>(pDst)->Get_Info().Extents);
		vSrcScale = ToVec3(dynamic_cast<CCube_Collider*>(pSrc)->Get_Info().Extents);
		
		float fMin = 123456789;
		for (auto Axis : vAxis)
		{
			// 중심좌표 투영
			float fDstCenter = D3DXVec3Dot(&vDstPos, &Axis);
			float fSrcCenter = D3DXVec3Dot(&vSrcPos, &Axis);
			float fDistance	 = fabsf(fDstCenter - fSrcCenter);

			// 반지름 투영
			matDstWorld = *(pDstTransform->Get_World());

			memcpy(&vRight, &matDstWorld.m[0][0], sizeof(_vec3));
			memcpy(&vUp,	&matDstWorld.m[1][0], sizeof(_vec3));
			memcpy(&vLook,	&matDstWorld.m[2][0], sizeof(_vec3));

			D3DXVec3Normalize(&vRight,	&vRight);
			D3DXVec3Normalize(&vUp,		&vUp);
			D3DXVec3Normalize(&vLook,	&vLook);

			vRight	*= vDstScale.x;
			vUp		*= vDstScale.y;
			vLook	*= vDstScale.z;

			float fDstRadius = fabs(D3DXVec3Dot(&vRight, &Axis))
							 + fabs(D3DXVec3Dot(&vUp, &Axis))
							 + fabs(D3DXVec3Dot(&vLook, &Axis));

			matSrcWorld = *(pSrcTransform->Get_World());
			memcpy(&vRight, &matSrcWorld.m[0][0], sizeof(_vec3));
			memcpy(&vUp, &matSrcWorld.m[1][0], sizeof(_vec3));
			memcpy(&vLook, &matSrcWorld.m[2][0], sizeof(_vec3));

			D3DXVec3Normalize(&vRight, &vRight);
			D3DXVec3Normalize(&vUp, &vUp);
			D3DXVec3Normalize(&vLook, &vLook);

			vRight *= vSrcScale.x;
			vUp *= vSrcScale.y;
			vLook *= vSrcScale.z;

			float fSrcRadius = fabs(D3DXVec3Dot(&vRight, &Axis))
							 + fabs(D3DXVec3Dot(&vUp, &Axis))
							 + fabs(D3DXVec3Dot(&vLook, &Axis));

			if (fMin > fDstRadius + fSrcRadius - fDistance)
			{
				fMin = fDstRadius + fSrcRadius - fDistance;
				_vec3 vDir = vSrcPos - vDstPos;

				if (D3DXVec3Dot(&vDir, &Axis) < 0.f)
					Axis *= -1.f;

				MTV = Axis * fMin;
			}
		}
		vPos += MTV;
	}
	pSrcTransform->Set_Pos(vPos);


	// 1. 속도 0으로 고정
	//pSrcObj->Set_Force(_vec3({ 0,0,0 }));

	// 2. 살짝 뒤로 튕기기(?)
	_vec3 vNewForce = pSrcObj->Get_Force();
	//D3DXVec3Normalize(&vNewForce, &vNewForce);
	pSrcObj->Add_Force(MTV * D3DXVec3Length(&vNewForce) * 1.5);
}

//bool CCollisionMgr::CubeVsCube(CCollider* pDstCollider, CCollider* pSrcCollider)
//{
//	vector<_vec3> vAxis;
//
//	CTransform* pDstTransform = dynamic_cast<CTransform*>(pDstCollider->Get_Owner()->Get_Transform());
//	CTransform* pSrcTransform = dynamic_cast<CTransform*>(pSrcCollider->Get_Owner()->Get_Transform());
//
//	_vec3 vDstAxis,vSrcAxis;
//	for (int i = 0; i < 3; ++i)
//	{
//		memcpy(&vDstAxis, &(pDstTransform->Get_World()->m[i][0]), sizeof(_vec3));
//		vAxis.push_back(*D3DXVec3Normalize(&vDstAxis,&vDstAxis));
//
//		memcpy(&vSrcAxis, &(pSrcTransform->Get_World()->m[i][0]), sizeof(_vec3));
//		vAxis.push_back(*D3DXVec3Normalize(&vSrcAxis, &vSrcAxis));
//		for (int j = 0; j < 3; j++)
//		{
//			memcpy(&vSrcAxis, &(pSrcTransform->Get_World()->m[j][0]), sizeof(_vec3));
//			_vec3 vCross;
//			D3DXVec3Cross(&vCross, &vDstAxis, &vSrcAxis);
//			if (D3DXVec3Length(&vCross) > 0.f)
//			{
//				vAxis.push_back(*D3DXVec3Normalize(&vCross, &vCross));
//			}
//		}
//	}
//
//	_vec3 vDstPos = dynamic_cast<CCube_Collider*>(pDstCollider)->GetCenter();
//	_vec3 vSrcPos = dynamic_cast<CCube_Collider*>(pSrcCollider)->GetCenter();
//
//	_matrix	matDstWorld, matSrcWorld;
//	_vec3 vRight, vUp, vLook, vDstScale, vSrcScale;
//
//	vDstScale = dynamic_cast<CCube_Collider*>(pDstCollider)->GetSize();
//	vSrcScale = dynamic_cast<CCube_Collider*>(pSrcCollider)->GetSize();
//	for (auto Axis : vAxis)
//	{
//		// 중심좌표 투영
//		float fDstCenter = D3DXVec3Dot(&vDstPos, &Axis);
//		float fSrcCenter = D3DXVec3Dot(&vSrcPos, &Axis);
//		float fDistance = fabsf(fDstCenter - fSrcCenter);
//
//		// 반지름 투영
//		matDstWorld = *(pDstTransform->Get_World());
//		memcpy(&vRight, &matDstWorld.m[0][0], sizeof(_vec3));
//		memcpy(&vUp,	&matDstWorld.m[1][0], sizeof(_vec3));
//		memcpy(&vLook,	&matDstWorld.m[2][0], sizeof(_vec3));
//		D3DXVec3Normalize(&vRight, &vRight);
//		D3DXVec3Normalize(&vUp, &vUp);
//		D3DXVec3Normalize(&vLook, &vLook);
//
//		vRight	*= vDstScale.x;
//		vUp		*= vDstScale.y;
//		vLook	*= vDstScale.z;
//
//		float fDstRadius = fabs(D3DXVec3Dot(&vRight, &Axis)) 
//						 + fabs(D3DXVec3Dot(&vUp,	 &Axis)) 
//						 + fabs(D3DXVec3Dot(&vLook,  &Axis));
//
//		matSrcWorld = *(pSrcTransform->Get_World());
//		memcpy(&vRight, &matSrcWorld.m[0][0], sizeof(_vec3));
//		memcpy(&vUp,	&matSrcWorld.m[1][0], sizeof(_vec3));
//		memcpy(&vLook,	&matSrcWorld.m[2][0], sizeof(_vec3));
//
//		D3DXVec3Normalize(&vRight,	&vRight);
//		D3DXVec3Normalize(&vUp,		&vUp);
//		D3DXVec3Normalize(&vLook,	&vLook);
//
//		vRight	*= vSrcScale.x;
//		vUp		*= vSrcScale.y;
//		vLook	*= vSrcScale.z;
//
//		float fSrcRadius = fabs(D3DXVec3Dot(&vRight, &Axis)) 
//						 + fabs(D3DXVec3Dot(&vUp,	 &Axis)) 
//						 + fabs(D3DXVec3Dot(&vLook,	 &Axis));
//		if (fDistance > fDstRadius + fSrcRadius)
//			return false;
//	}
//	return true;
//}

//bool CCollisionMgr::SphereVsSphere(CCollider* pDst, CCollider* pSrc)
//{
//	_vec3 vDstPos = dynamic_cast<CSphere_Collider*>(pDst)->GetCenter();
//	_vec3 vSrcPos = dynamic_cast<CSphere_Collider*>(pSrc)->GetCenter();
//	
//	float vDstRadius = dynamic_cast<CSphere_Collider*>(pDst)->GetRadius();
//	float vSrcRadius = dynamic_cast<CSphere_Collider*>(pSrc)->GetRadius();
//
//	_vec3 vDeltaPos = vDstPos - vSrcPos;
//	
//	if (D3DXVec3Length(&vDeltaPos) > vDstRadius + vSrcRadius)
//		return false;
//	return true;
//}

//bool CCollisionMgr::CubeVsSphere(CCollider* pCube, CCollider* pSphere)
//{
//	CCube_Collider* pCubeCollider = dynamic_cast<CCube_Collider*>(pCube);
//	CSphere_Collider* pSphereCollider = dynamic_cast<CSphere_Collider*>(pSphere);
//
//	_vec3 vCubePos = pCubeCollider->GetCenter();
//	_vec3 vSpherePos = pSphereCollider->GetCenter();
//
//	_vec3 vDistance = vSpherePos - vCubePos;
//
//	CTransform* pCubeTransform = pCube->Get_Owner()->Get_Transform();
//
//	_matrix matCubeWorld;
//	_vec3 vRight, vUp, vLook;
//	matCubeWorld = *(pCubeTransform->Get_World());
//
//	memcpy(&vRight, &matCubeWorld.m[0][0], sizeof(_vec3));
//	memcpy(&vUp,	&matCubeWorld.m[1][0], sizeof(_vec3));
//	memcpy(&vLook,	&matCubeWorld.m[2][0], sizeof(_vec3));
//	D3DXVec3Normalize(&vRight, &vRight);
//	D3DXVec3Normalize(&vUp, &vUp);
//	D3DXVec3Normalize(&vLook, &vLook);
//
//	float fX = D3DXVec3Dot(&vDistance, &vRight);
//	float fY = D3DXVec3Dot(&vDistance, &vUp);
//	float fZ = D3DXVec3Dot(&vDistance, &vLook);
//
//	fX = clampT(fX, -pCubeCollider->GetSize().x, pCubeCollider->GetSize().x);
//	fY = clampT(fY, -pCubeCollider->GetSize().y, pCubeCollider->GetSize().y);
//	fZ = clampT(fZ, -pCubeCollider->GetSize().z, pCubeCollider->GetSize().z);
//
//	_vec3 vCloseSet = vCubePos + vRight * fX + vUp * fY + vLook * fZ;
//
//	vDistance = (vCloseSet - vSpherePos);
//
//	float fDistance = D3DXVec3Length(&vDistance);
//
//	return fDistance <= pSphereCollider->GetRadius();
//}
//
//bool CCollisionMgr::SphereVsCube(CCollider* pSphere, CCollider* pCube)
//{
//	CCube_Collider* pCubeCollider = dynamic_cast<CCube_Collider*>(pCube);
//	CSphere_Collider* pSphereCollider = dynamic_cast<CSphere_Collider*>(pSphere);
//
//	_vec3 vCubePos = ToVec3(pCubeCollider->GetInfo().Center);
//	_vec3 vSpherePos = pSphereCollider->GetCenter();
//
//	_vec3 vDistance = vSpherePos - vCubePos;
//
//	CTransform* pCubeTransform = pCube->Get_Owner()->Get_Transform();
//
//	_matrix matCubeWorld;
//	_vec3 vRight, vUp, vLook;
//	matCubeWorld = *(pCubeTransform->Get_World());
//
//	memcpy(&vRight, &matCubeWorld.m[0][0], sizeof(_vec3));
//	memcpy(&vUp, &matCubeWorld.m[1][0], sizeof(_vec3));
//	memcpy(&vLook, &matCubeWorld.m[2][0], sizeof(_vec3));
//	D3DXVec3Normalize(&vRight, &vRight);
//	D3DXVec3Normalize(&vUp, &vUp);
//	D3DXVec3Normalize(&vLook, &vLook);
//
//	float fX = D3DXVec3Dot(&vDistance, &vRight);
//	float fY = D3DXVec3Dot(&vDistance, &vUp);
//	float fZ = D3DXVec3Dot(&vDistance, &vLook);
//
//	fX = clampT(fX, -pCubeCollider->GetSize().x, pCubeCollider->GetSize().x);
//	fY = clampT(fY, -pCubeCollider->GetSize().y, pCubeCollider->GetSize().y);
//	fZ = clampT(fZ, -pCubeCollider->GetSize().z, pCubeCollider->GetSize().z);
//
//	_vec3 vCloseSet = vCubePos + vRight * fX + vUp * fY + vLook * fZ;
//
//	vDistance = (vCloseSet - vSpherePos);
//
//	float fDistance = D3DXVec3Length(&vDistance);
//
//	return fDistance <= pSphereCollider->GetRadius();
//}

//void CCollisionMgr::AddCollider(OBJID eID, CCollider* pCollider)
//{
//	if (eID > OBJ_END || pCollider == nullptr)
//		return;
//	m_ColliderList[eID].push_back(pCollider);
//}


