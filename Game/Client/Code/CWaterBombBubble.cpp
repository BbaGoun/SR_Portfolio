#include "pch.h"
#include "CWaterBombBubble.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"

CWaterBombBubble::CWaterBombBubble(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CWaterBombBubble::CWaterBombBubble(const CGameObject& rhs) : CGameObject(rhs)
{
}

CWaterBombBubble::~CWaterBombBubble()
{
}

HRESULT CWaterBombBubble::Ready_GameObject()
{
	//CGameObject::Ready_GameObject();

	//m_fTimer		= 0.f;

	//m_bBubbleAppear	= false;

	//m_pTransformCom->Set_Pos({ -10.f,0.f,0.f });

	//CComponent* pComponent = nullptr;

	//// 임시 물방울
	//m_pTransformCom->Set_Scale({ 1.5f, 3.5f, 0.7f });
	//pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	//if (nullptr == pComponent)
	//	return E_FAIL;

	//pComponent->Set_Owner(this);
	//m_mapComponent.insert({ L"Com_Buffer", pComponent });

	//pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	//pComponent->Set_Owner(this);
	//m_mapComponent.insert({ L"Com_Buffer", pComponent });

	// 임시 물방울 충돌처리
	//pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	//if (nullptr == pComponent)
	//	return E_FAIL;

	//m_pColliderCom->Set_Owner(this);
	//m_pColliderCom->SetIsTrigger(true);
	//m_pColliderCom->Set_Extents({ 1.7f, 1.7f, 1.7f });
	//m_pColliderCom->Set_Offset({ 0.f, 0.f, 0.f });

	//m_mapComponent.insert({ L"Com_Collider", pComponent });

	//pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RainBow_Alpha"));
	//pComponent->Set_Owner(this);
	//m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CWaterBombBubble::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	//CGameObject* pTarget = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget");

	//_vec3 vScale, vTargetPos;

	//pTarget->Get_Transform()->Get_Info(INFO_POS, &vTargetPos);
	//
	//vScale = m_pTransformCom->Get_Scale();

	//m_fTimer += fFixedDeltaTime;	// Create에서 생성했으면 여기 필요없음

	//if (m_fTimer > 1.6f)
	//{
	//	if (vScale.x < 4.f && vScale.y < 4.f && vScale.z < 4.f)
	//	{
	//		vScale.x += 4.2f * fFixedDeltaTime;
	//		vScale.y += 4.2f * fFixedDeltaTime;
	//		vScale.z += 4.2f * fFixedDeltaTime;
	//	}
	//}

	//m_pTransformCom->Set_Scale(vScale);
	////m_pTransformCom->Set_Pos(vTargetPos);
}

_int CWaterBombBubble::Update_GameObject(const _float& fDeltaTime)
{
	//if (m_fTimer > 1.6f)
	//{
	//CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA
	//}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CWaterBombBubble::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CWaterBombBubble::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	// m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	//m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWaterBombBubble::CollisionEnter(CCollider* pOtherCollider)
{
}

void CWaterBombBubble::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcscmp(wOtherTag, L"Obj_Cart") == 0)
	{

	}
}

CWaterBombBubble* CWaterBombBubble::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBombBubble* pObj = new CWaterBombBubble(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CWaterBombBubble Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CWaterBombBubble::Free()
{
	CGameObject::Free();
}
