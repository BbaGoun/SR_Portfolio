#include "pch.h"
#include "CUfo.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CUfoTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

CUfo::CUfo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUfo::~CUfo()
{
}

HRESULT CUfo::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer	= 0.f;
	m_fSpeed	= 400.f;
	//m_fAngle		= 0.f;
	m_fUfoBack  = 0.f;
	m_fUfoFront = 0.f;
	m_fUfoRight = 0.f;
	m_vSavePos  = { 0.f, 0.f, 0.f };

	m_bSavePos  = false;

	m_pTransformCom->Set_Pos({ 0.f, 0.f, 0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 1.0f, 1.0f, 1.0f });
	//m_pColliderCom->Set_Offset({ 0.f, 4.f, 0.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CUfo::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");

	CGameObject* pTarget1 = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget");
	//CGameObject* pTarget2 = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget2");
	//CGameObject* pTarget3 = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget3");
	//CGameObject* pTarget4 = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget4");

	_vec3 vFlyPos, vCartPos, vCartUp, vCartLook, vCartRight,
		vTargetPos1, vTargetPos2, vTargetPos3, vTargetPos4,
		vDirTarget1, vDirTarget2, vDirTarget3, vDirTarget4,
		vDirTarget, vTargetUp; // vMovePos;

	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	pCartBody->Get_Transform()->Get_Info(INFO_UP, &vCartUp);
	pCartBody->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);
	pCartBody->Get_Transform()->Get_Info(INFO_RIGHT, &vCartRight);


	pTarget1->Get_Transform()->Get_Info(INFO_UP, &vTargetUp);
	pTarget1->Get_Transform()->Get_Info(INFO_POS, &vTargetPos1);
	//pTarget2->Get_Transform()->Get_Info(INFO_POS, &vTargetPos2);
	//pTarget3->Get_Transform()->Get_Info(INFO_POS, &vTargetPos3);
	//pTarget4->Get_Transform()->Get_Info(INFO_POS, &vTargetPos4);

	vCartPos += vCartUp * 0.f;

	vFlyPos = vCartPos;

	// vFlyPos.x += m_vForce.x;
	vFlyPos.y += m_vForce.y;
	//vFlyPos.z += m_vForce.z;

	vFlyPos -= vCartLook * m_fUfoBack;
	vFlyPos += vCartLook * m_fUfoFront;
	vFlyPos += vCartRight * m_fUfoRight;

	//m_pTransformCom->Set_Pos(vFlyPos);
////////////////////////////////////////////////////////////////////////
// 구현 후 하드코딩 수정
	m_fTimer += fFixedDeltaTime;

	if (m_fTimer < 1.85f)
	{
		m_pTransformCom->Set_Pos(vFlyPos);
	}

	if (m_fTimer > 0.f && m_fTimer < 0.06f)
	{
		m_vForce.y += 2.0f;
		m_fUfoRight += 0.31f;
	}

	if (m_fTimer >= 0.06f && m_fTimer < 0.11f)
	{
		m_vForce.y += 1.38f;
		m_fUfoRight += 0.25f;
	}

	if (m_fTimer >= 0.11f && m_fTimer < 0.16f)
	{
		m_vForce.y += 0.63f;
		m_fUfoRight += 0.19f;
	}

	if (m_fTimer >= 0.16f && m_fTimer < 0.21f)
	{
		m_vForce.y += 0.06f;
		m_fUfoRight += 0.13f;
	}

	if (m_fTimer >= 0.21f && m_fTimer < 0.27f)
	{
		m_vForce.y -= 0.06f;
		m_fUfoRight += 0.31f;
	}

	if (m_fTimer >= 0.27f && m_fTimer < 0.35f)
	{
		m_vForce.y -= 0.31f;
		m_fUfoRight += 0.31f;
	}

	if (m_fTimer >= 0.35f && m_fTimer < 0.42f)
	{
		m_vForce.y += 0.19f;
		m_fUfoRight -= 0.19f;
	}

	if (m_fTimer >= 0.42f && m_fTimer < 0.48f)
	{
		m_vForce.y += 0.31f;
		m_fUfoRight -= 0.38f;
	}

	if (m_fTimer >= 0.48f && m_fTimer < 0.54f)
	{
		m_vForce.y += 0.08f;
		m_fUfoRight -= 0.56f;
	}

	if (m_fTimer >= 0.54f && m_fTimer < 0.64f)
	{
		m_vForce.y -= 0.31f;
		m_fUfoRight -= 0.69f;
	}

	if (m_fTimer >= 0.64f && m_fTimer < 0.68f)
	{
		m_fUfoBack += 0.10f;
		m_vForce.y -= 0.05f;
		m_fUfoRight += 0.05f;
	}

	if (m_fTimer >= 0.68f && m_fTimer < 0.72f)
	{
		m_fUfoBack += 0.15f;
		m_vForce.y -= 0.05f;
		m_fUfoRight += 0.25f;
	}

	if (m_fTimer >= 0.72f && m_fTimer < 0.76f)
	{
		m_fUfoBack += 0.19f;
		m_vForce.y -= 0.10f;
		m_fUfoRight += 0.30f;
	}

	if (m_fTimer >= 0.76f && m_fTimer < 0.80f)
	{
		m_fUfoBack += 0.25f;
		m_vForce.y -= 0.15f;
		m_fUfoRight += 0.35f;
	}

	if (m_fTimer >= 0.80f && m_fTimer < 0.84f)
	{
		m_fUfoBack += 0.25f;
		m_vForce.y -= 0.15f;
		m_fUfoRight += 0.50f;
	}

	if (m_fTimer >= 0.84f && m_fTimer < 0.88f)
	{
		m_fUfoBack += 0.25f;
		m_vForce.y -= 0.15f;
		m_fUfoRight += 0.60f;
	}

	if (m_fTimer >= 0.88f && m_fTimer < 0.98f)
	{
		m_fUfoBack -= 0.15f;
		m_vForce.y += 0.02f;
		m_fUfoRight += 0.40f;
	}

	if (m_fTimer >= 0.98f && m_fTimer < 1.02f)
	{
		m_fUfoBack -= 0.18f;
		m_vForce.y += 0.15f;
		m_fUfoRight += 0.45f;
	}

	if (m_fTimer >= 1.02f && m_fTimer < 1.06f)
	{
		m_fUfoBack -= 0.35f;
		m_vForce.y += 0.35f;
		m_fUfoRight += 0.25f;
	}

	if (m_fTimer >= 1.06f && m_fTimer < 1.10f)
	{
		m_fUfoBack -= 0.55f;
		m_vForce.y += 0.55f;
		m_fUfoRight += 0.15f;
	}
	
	if (m_fTimer >= 1.10f && m_fTimer < 1.14f)
	{
		m_fUfoBack -= 1.25f;
		m_vForce.y += 0.55f;
		m_fUfoRight += 0.05f;
	}
	
	if (m_fTimer >= 1.14f && m_fTimer < 1.18f)
	{
		m_fUfoBack -= 3.25f;
		m_vForce.y += 0.55f;
		m_fUfoRight += 0.00f;
	}

	// 바디에서 렌더 끄기
	if (m_fTimer >= 1.18f && m_fTimer < 1.32f)
	{
		m_fUfoBack -= 5.25f;
		m_vForce.y += 0.55f;
		m_fUfoRight -= 0.05f;
	}

	if (m_fTimer >= 1.32f && m_bSavePos == false)
	{
		m_pTransformCom->Get_Info(INFO_POS, &m_vSavePos);
		m_bSavePos = true;
	}


	if (m_fTimer >= 1.32f && m_fTimer < 1.85f)	// 안보이는 상태에서 타겟 머리위로 빠르게 이동
	{
		vDirTarget1 = vTargetPos1 - m_vSavePos;
		//vDirTarget2 = vTargetPos2 - m_vSavePos;
		//vDirTarget3 = vTargetPos3 - m_vSavePos;
		//vDirTarget4 = vTargetPos4 - m_vSavePos;

		// 하드 코딩 수정
		//if (D3DXVec3Length(&vDirTarget1) < D3DXVec3Length(&vDirTarget2)
		//	&& D3DXVec3Length(&vDirTarget1) < D3DXVec3Length(&vDirTarget3)
		//	&& D3DXVec3Length(&vDirTarget1) < D3DXVec3Length(&vDirTarget4))
		//{
			vDirTarget = vDirTarget1;
		//}

		//else if (D3DXVec3Length(&vDirTarget2) < D3DXVec3Length(&vDirTarget1)
		//	&& D3DXVec3Length(&vDirTarget2) < D3DXVec3Length(&vDirTarget3)
		//	&& D3DXVec3Length(&vDirTarget2) < D3DXVec3Length(&vDirTarget4))
		//{
		//	vDirTarget = vDirTarget2;
		//}

		//else if (D3DXVec3Length(&vDirTarget3) < D3DXVec3Length(&vDirTarget1)
		//	&& D3DXVec3Length(&vDirTarget3) < D3DXVec3Length(&vDirTarget2)
		//	&& D3DXVec3Length(&vDirTarget3) < D3DXVec3Length(&vDirTarget4))
		//{
		//	vDirTarget = vDirTarget3;
		//}

		//else
		//{
		//	vDirTarget = vDirTarget4;
		//}

		if (D3DXVec3Length(&vDirTarget) <= 0.001f)
			return;

		D3DXVec3Normalize(&vDirTarget, &vDirTarget);

		// m_pTransformCom->Move_Pos(&vDirTarget, m_fSpeed, fFixedDeltaTime);
		m_pTransformCom->Set_Pos(vTargetPos1 += vTargetUp * 15.f);

		m_pTransformCom->Get_Info(INFO_POS, &m_vSavePos);
	}

	//if (m_fTimer >= 1.90f  && m_fTimer < 2.00f )		// 바디에서 랜더 켜기
	//{
	//	m_vSavePos.y -= 1.55f;
	//	m_pTransformCom->Set_Pos(m_vSavePos);
	//}

	_quaternion q;

	m_pTransformCom->GetFollowQuaternion(&vCartLook, &q);
	m_pTransformCom->Set_Quaternion(&q);
}

_int CUfo::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CUfo::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CUfo::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pTextureCom->Set_Texture(0);
	//m_pBufferCom->Render_Buffer();

	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CUfo::CollisionEnter(CCollider* pOtherCollider)
{
}

void CUfo::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();
	if (wcsncmp(wOtherTag, L"Obj_MissileTarget", 17) == 0)
	{
		// m_pLayer->Delete_GameObject(this);
	}
}

CUfo* CUfo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUfo* pUfo = new CUfo(pGraphicDev);

	if (FAILED(pUfo->Ready_GameObject()))
	{
		Safe_Release(pUfo);
		MSG_BOX("pUfo Create Failed");
		return nullptr;
	}

	return pUfo;
}

void CUfo::Free()
{

	CGameObject::Free();
}
