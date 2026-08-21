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

	_vec3 vPos, vCartPos, vLook, vDir, vTargetPos;
	
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);


	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	m_pTransformCom->Set_Pos({ vCartPos.x, vCartPos.y + 5.f, vCartPos.z + 2.f });	//m_pTransformCom->Set_Pos(vCartPos);
	pTargetPos->Get_Transform()->Get_Info(INFO_POS, &vTargetPos);

	vDir = vTargetPos - vCartPos;

	if (D3DXVec3Length(&vDir) <= 0.001f)
		return 0;

	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vCross;
	D3DXVec3Cross(&vCross, &vLook, &vDir);

	if (D3DXVec3Length(&vCross) <= 0.001f)
		return 0;

	D3DXVec3Normalize(&vCross, &vCross);

	
	_float fDot = D3DXVec3Dot(&vLook, &vDir);

	if (fDot > 1.f)
		fDot = 1.f;

	else if (fDot < -1.f)
		fDot = -1.f;

	_float fAngle = acosf(fDot);

	_quaternion qRot;
	D3DXQuaternionRotationAxis(&qRot, &vCross, fAngle);

	m_pTransformCom->Multiple_Quaternion(&qRot);

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
