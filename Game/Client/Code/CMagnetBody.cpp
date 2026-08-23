#include "pch.h"
#include "CMagnetBody.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CMagnetTex.h"
#include "CRenderer.h"
#include "CManagement.h"
#include <CCartBody.h>

CMagnetBody::CMagnetBody(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CMagnetBody::~CMagnetBody()
{
}

HRESULT CMagnetBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 8.f, 8.f, 7.f });

	m_fTimer = 0;

	pComponent = m_pBufferCom = dynamic_cast<CMagnetTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MagnetTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 1.f, 1.f, 1.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

_int CMagnetBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	CCartBody* pCartBody = dynamic_cast<CCartBody*>(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody"));
	CGameObject* pTargetPos = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget");

	_vec3 vPos, vCartPos, vLook, vDir, vTargetPos, vCartLook, vCartUp;
	
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);


	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	pCartBody->Get_Transform()->Get_Info(INFO_UP, &vCartUp);
	pCartBody->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);

	m_pTransformCom->Set_Pos(vCartPos + vCartLook * 3.f + vCartUp * 5);
	
	pTargetPos->Get_Transform()->Get_Info(INFO_POS, &vTargetPos);

	vDir = vTargetPos - vCartPos;

	if (D3DXVec3Length(&vDir) <= 0.001f)
		return 0;

	D3DXVec3Normalize(&vDir, &vDir);

	//_vec3 vCross;
	//D3DXVec3Cross(&vCross, &vLook, &vDir);

	//if (D3DXVec3Length(&vCross) <= 0.001f)
	//	return 0;

	//D3DXVec3Normalize(&vCross, &vCross);

	//
	//_float fDot = D3DXVec3Dot(&vLook, &vDir);

	//if (fDot > 1.f)
	//	fDot = 1.f;

	//else if (fDot < -1.f)
	//	fDot = -1.f;

	//_float fAngle = acosf(fDot);

	//_quaternion qRot;
	//D3DXQuaternionRotationAxis(&qRot, &vCross, fAngle);

	//m_pTransformCom->Multiple_Quaternion(&qRot);
////////////////////////////////////////////////////////////////////// 잘돼면 트랜스폼에 함수생성해서 불러오는 식
	//_vec3 vFixUp, vFixRight, vFixLook;
	//vFixUp = { 0.f, 1.f, 0.f };			// 기준값

	//D3DXVec3Normalize(&vFixLook, &vDir);

	//D3DXVec3Cross(&vFixRight, &vFixUp, &vFixLook);
	//D3DXVec3Cross(&vFixUp, &vFixLook, &vFixRight);

	//D3DXVec3Normalize(&vFixUp, &vFixUp);
	//D3DXVec3Normalize(&vFixRight, &vFixRight);

	//_matrix matFixRot;
	//D3DXMatrixIdentity(&matFixRot);

	//matFixRot._11 = vFixRight.x;
	//matFixRot._12 = vFixRight.y;
	//matFixRot._13 = vFixRight.z;

	//matFixRot._21 = vFixUp.x;
	//matFixRot._22 = vFixUp.y;
	//matFixRot._23 = vFixUp.z;

	//matFixRot._31 = vFixLook.x;
	//matFixRot._32 = vFixLook.y;
	//matFixRot._33 = vFixLook.z;

	//_quaternion qFixRot;
	//D3DXQuaternionRotationMatrix(&qFixRot, &matFixRot);

	_quaternion qRot;
	
	m_pTransformCom->GetFollowQuaternion(&vDir, &qRot);
	m_pTransformCom->Set_Quaternion(&qRot);

	m_fTimer += fDeltaTime;

	if (m_fTimer > 3.5f)
	{
		m_pLayer->Delete_GameObject(this);
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMagnetBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMagnetBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	// m_pColliderCom->Render_Component(D3DXCOLOR({ 1,0,0,1 }));
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMagnetBody::CollisionEnter(CCollider* pOtherCollider)
{
}

void CMagnetBody::TriggerEnter(CCollider* pOtherCollider)
{
}

CMagnetBody* CMagnetBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnetBody* pObj = new CMagnetBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMagnetBody Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CMagnetBody::Free()
{
	CGameObject::Free();
}
