#include "pch.h"
#include "CWaterFly.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CWaterFlyTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"
#include "CCameraMgr.h"

CWaterFly::CWaterFly(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CWaterFly::~CWaterFly()
{
}

HRESULT CWaterFly::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer		= 0.f;
	m_fSpeed		= 400.f;
	//m_fAngle		= 0.f;
	m_fFlyBack		= 0.f;
	m_fFlyFront		= 0.f; 

	m_vSavePos		= { 0.f, 0.f, 0.f };

	m_bSavePos		= false;

	m_pTransformCom->Set_Pos({ 0.f, -1000.f, 0.f });

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

void CWaterFly::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CGameObject* pCartBody =CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");

	CGameObject* pTarget1 = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget");
	CGameObject* pTarget2 = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget2");
	CGameObject* pTarget3 = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget3");
	CGameObject* pTarget4 = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget4");

	_vec3 vFlyPos, vCartPos, vCartUp, vCartLook,
		vTargetPos1, vTargetPos2, vTargetPos3, vTargetPos4,
		vDirTarget1, vDirTarget2, vDirTarget3, vDirTarget4,
		vDirTarget; // vMovePos;

	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	pCartBody->Get_Transform()->Get_Info(INFO_UP, &vCartUp);
	pCartBody->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);

	pTarget1->Get_Transform()->Get_Info(INFO_POS, &vTargetPos1);
	pTarget2->Get_Transform()->Get_Info(INFO_POS, &vTargetPos2);
	pTarget3->Get_Transform()->Get_Info(INFO_POS, &vTargetPos3);
	pTarget4->Get_Transform()->Get_Info(INFO_POS, &vTargetPos4);

	vCartPos += vCartUp * 0.f;

	vFlyPos = vCartPos;

	vFlyPos.x += m_vForce.x;
	vFlyPos.y += m_vForce.y;
	//vFlyPos.z += m_vForce.z;

	vFlyPos -= vCartLook * m_fFlyBack;
	vFlyPos += vCartLook * m_fFlyFront;


	//m_pTransformCom->Set_Pos(vFlyPos);
////////////////////////////////////////////////////////////////////////
// 구현 후 하드코딩 수정
	m_fTimer += fFixedDeltaTime;
	if (m_fTimer < 1.20f)
	{
		m_pTransformCom->Set_Pos(vFlyPos);
	}

	if (m_fTimer > 0.f && m_fTimer < 0.06f)
	{
		m_vForce.y += 1.5f;

	}

	if (m_fTimer > 0.60f && m_fTimer < 0.65f)
	{
		m_vForce.y += 0.5f;
		m_fFlyBack += 2.0f;
	}

	if (m_fTimer > 0.65f && m_fTimer < 0.66f)
	{
		m_vForce.y += 1.3f;
		m_fFlyBack += 2.0f;
	}

	if (m_fTimer > 0.66f && m_fTimer < 0.67f)
	{
		m_vForce.y += 1.3f;
		m_fFlyBack += 3.0f;
	}


	if (m_fTimer > 0.67f && m_fTimer < 0.68)
	{
		m_vForce.x += 0.6f;
		m_vForce.y += 1.2f;
		m_fFlyBack += 3.0f;
	}

	if (m_fTimer > 0.68f && m_fTimer < 0.69)
	{
		m_vForce.x += 0.6f;
		m_vForce.y += 1.2f;
		m_fFlyBack += 3.0f;
	}

	if (m_fTimer > 0.69f && m_fTimer < 0.70)
	{
			m_vForce.x += 0.6f;
			m_vForce.y += 1.2f;
			m_fFlyBack += 3.0f;
	}

	if (m_fTimer > 0.70f && m_fTimer < 0.71)
	{
			m_vForce.y += 1.2f;
			m_fFlyBack += 3.0f;
	}

	if (m_fTimer > 0.71f && m_fTimer < 0.75)
	{
			m_vForce.y += 1.2f;
			m_fFlyBack += 3.0f;
	}

	if (m_fTimer > 0.75f && m_fTimer < 0.80)
	{
			m_fFlyFront += 1.0f;
			m_vForce.y += 1.2f;
	}

	if (m_fTimer > 0.80f && m_fTimer < 0.85)
	{
			m_fFlyFront += 7.0f;
			m_vForce.y += 1.2f;
	}

	if (m_fTimer > 0.95f && m_fTimer < 1.20f)
	{
			m_vForce.x -= 0.8f;
			m_fFlyFront += 1.3f;
			m_vForce.y -= 0.8f;
	}

	if (m_fTimer > 1.20f && m_bSavePos == false)
	{
		m_vSavePos = vFlyPos;
		m_bSavePos = true;
	}

	if (m_fTimer > 1.20f)
	{
		vDirTarget1 = vTargetPos1 - m_vSavePos;
		vDirTarget2 = vTargetPos2 - m_vSavePos;
		vDirTarget3 = vTargetPos3 - m_vSavePos;
		vDirTarget4 = vTargetPos4 - m_vSavePos;

		// 하드 코딩 수정
		if (D3DXVec3Length(&vDirTarget1) < D3DXVec3Length(&vDirTarget2) 
			&& D3DXVec3Length(&vDirTarget1) <  D3DXVec3Length(&vDirTarget3)
			&& D3DXVec3Length(&vDirTarget1) < D3DXVec3Length(&vDirTarget4))
		{
			vDirTarget = vDirTarget1;
		}

		else if (D3DXVec3Length(&vDirTarget2) < D3DXVec3Length(&vDirTarget1) 
			&& D3DXVec3Length(&vDirTarget2) < D3DXVec3Length(&vDirTarget3)
			&& D3DXVec3Length(&vDirTarget2) < D3DXVec3Length(&vDirTarget4))
		{
			vDirTarget = vDirTarget2;
		}

		else if (D3DXVec3Length(&vDirTarget3) < D3DXVec3Length(&vDirTarget1)
			&& D3DXVec3Length(&vDirTarget3) < D3DXVec3Length(&vDirTarget2)
			&& D3DXVec3Length(&vDirTarget3) < D3DXVec3Length(&vDirTarget4))
		{
			vDirTarget = vDirTarget3;
		}

		else
		{
			vDirTarget = vDirTarget4;
		}

		if (D3DXVec3Length(&vDirTarget) <= 0.001f)
			return;

		D3DXVec3Normalize(&vDirTarget, &vDirTarget);

		m_pTransformCom->Move_Pos(&vDirTarget, m_fSpeed, fFixedDeltaTime);
	}

	_quaternion q;

	m_pTransformCom->GetFollowQuaternion(&vCartLook, &q);
	m_pTransformCom->Set_Quaternion(&q);
}

_int CWaterFly::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CWaterFly::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CWaterFly::Render_GameObject()
{
	_matrix	matWorld, matView;
	matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
	m_pTransformCom->Set_Billboard(&matView);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pTextureCom->Set_Texture(0);
	//m_pBufferCom->Render_Buffer();

	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWaterFly::CollisionEnter(CCollider* pOtherCollider)
{
}

void CWaterFly::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();
	// if (wcsncmp(wOtherTag, L"Obj_MissileTarget", 17) == 0)
	// if (wcscmp(wOtherTag, L"Obj_MissileTarget") == 0)
	if (wcsncmp(wOtherTag, L"Obj_MissileTarget", 17) == 0)
	{
		//vector<CGameObject*> vecChildren = Get_Children();

		//for (auto& pChild : vecChildren)
		//{
		//	pChild->To_Root();
		//	m_pLayer->Delete_GameObject(pChild);
		//}
	 m_pLayer->Delete_GameObject(this);
	}
}

CWaterFly* CWaterFly::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterFly* pWaterFly = new CWaterFly(pGraphicDev);

	if (FAILED(pWaterFly->Ready_GameObject()))
	{
		Safe_Release(pWaterFly);
		MSG_BOX("pWaterFly Create Failed");
		return nullptr;
	}

	return pWaterFly;
}

void CWaterFly::Free()
{

	CGameObject::Free();
}
