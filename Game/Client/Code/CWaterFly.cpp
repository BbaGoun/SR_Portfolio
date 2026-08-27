#include "pch.h"
#include "CWaterFly.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CWaterFlyTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

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
	//m_fSpeed		= 0.f;
	//m_fAngle		= 0.f;
	m_fFlyBack		= 0.f;
	m_fFlyFront		= 0.f; 
	m_vSavePos		= { 0.f, 0.f, 0.f };

	m_bSavePos		= false;

	m_pTransformCom->Set_Pos({ 0.f, 0.f, 0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 1.f, 0.9f, 0.f });
	//m_pColliderCom->Set_Offset({ 0.f, 4.f, 0.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CWaterFly::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");
	CGameObject* pTarget = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget");

	_vec3 vFlyPos, vCartPos, vCartUp, vCartLook, vTargetPos, vDir, vMovePos;

	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	pCartBody->Get_Transform()->Get_Info(INFO_UP, &vCartUp);
	pCartBody->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);
	pTarget->Get_Transform()->Get_Info(INFO_POS, &vTargetPos);

	vCartPos += vCartUp * 0.f;

	vFlyPos = vCartPos;

	vFlyPos.x += m_vForce.x;
	vFlyPos.y += m_vForce.y;
	vFlyPos.z += m_vForce.z;

	vFlyPos -= vCartLook * m_fFlyBack;
	vFlyPos += vCartLook * m_fFlyFront;

	// m_pTransformCom->Set_Pos(vCartPos + vCartUp * m_vForce.y);
	// m_pTransformCom->Set_Pos(vCartPos);
	m_pTransformCom->Set_Pos(vFlyPos);	

	m_fTimer += fFixedDeltaTime;
	////////////////////////////////////////////////////////////////////////
	// 구현 후 하드코딩 수정
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

	vDir = vTargetPos - m_vSavePos;

	_float fDistance = D3DXVec3Length(&vDir);

	if (D3DXVec3Length(&vDir) <= 0.001f)
		return;

	D3DXVec3Normalize(&vDir, &vDir);

	vMovePos = m_vSavePos + vDir * fDistance;
	m_pTransformCom->Set_Pos(vMovePos);

	if (m_fTimer > 5.f)
	{
		// m_bSavePos = false;
		m_pLayer->Delete_GameObject(this);
	}
	////////////////////////////////////////////////////////////////////////
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

	if (wcscmp(wOtherTag, L"Obj_MissileTarget") == 0)
	{
		vector<CGameObject*> vecChildren = Get_Children();

		for (auto& pChild : vecChildren)
		{
			pChild->To_Root();
			m_pLayer->Delete_GameObject(pChild);
		}
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
