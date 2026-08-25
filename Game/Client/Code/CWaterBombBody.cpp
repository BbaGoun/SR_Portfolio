#include "pch.h"
#include "CWaterBombBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CWaterBombTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

CWaterBombBody::CWaterBombBody(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CWaterBombBody::~CWaterBombBody()
{
}

HRESULT CWaterBombBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0.f,0.f,0.f });
	// m_pTransformCom->Set_Pos({ -50.f,0.f,150.f });
	m_pTransformCom->Set_Scale({ 1.5f, 3.5f, 0.7f });

	m_fTimer = 0.f;

	Engine::CComponent* pComponent = nullptr;

	// 임시 반구
	pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

//void CWaterBombBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)	// 순간이동 코드
//{	
//	//CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");
//
//	//_vec3 vScale, vCartLook, vCartPos;
//	//vScale = m_pTransformCom->Get_Scale();
//	//pCartBody->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);
//	//pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
//
//	//vCartPos += vCartLook * 500.f;
//
//	//m_pTransformCom->Set_Pos(vCartPos);
//
//	_vec3 vScale = m_pTransformCom->Get_Scale();
//
//	m_fTimer += fFixedDeltaTime;
//
//	if (m_fTimer > 1.75f)
//	{
//		if (vScale.x < 16.f && vScale.y < 16.f && vScale.z < 16.f)
//		{
//			vScale.x += 40.f * fFixedDeltaTime;
//			vScale.y += 40.f * fFixedDeltaTime;
//			vScale.z += 40.f * fFixedDeltaTime;
//		}
//	}
//
//	if (m_fTimer > 3.5f)
//	{
//		m_pLayer->Delete_GameObject(this);
//	}
//
//	m_pTransformCom->Set_Scale(vScale);
//}

void CWaterBombBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	//CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");

	//_vec3 vScale, vCartLook, vCartPos;
	//vScale = m_pTransformCom->Get_Scale();
	//pCartBody->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);
	//pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);

	//vCartPos += vCartLook * 500.f;

	//m_pTransformCom->Set_Pos(vCartPos);

	_vec3 vScale = m_pTransformCom->Get_Scale();

	m_fTimer += fFixedDeltaTime;	// Create에서 생성했으면 여기 필요없음

	if (m_fTimer > 1.6f)
	{
		if (vScale.x < 16.f && vScale.y < 16.f && vScale.z < 16.f)
		{
			vScale.x += 50.f * fFixedDeltaTime;
			vScale.y += 50.f * fFixedDeltaTime;
			vScale.z += 50.f * fFixedDeltaTime;
		}
	}
	
	if (m_fTimer > 3.5f)
	{
		m_pLayer->Delete_GameObject(this);
	}

	m_pTransformCom->Set_Scale(vScale);
}

_int CWaterBombBody::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	//if (m_fTimer > 1.75f)
	//{
	//	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA
	//}
	//CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CWaterBombBody::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWaterBombBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CWaterBombBody* CWaterBombBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBombBody* pMissile = new CWaterBombBody(pGraphicDev);

	if (FAILED(pMissile->Ready_GameObject()))
	{
		Safe_Release(pMissile);
		MSG_BOX("pMissile Create Failed");
		return nullptr;
	}

	return pMissile;
}

void CWaterBombBody::Free()
{
	CGameObject::Free();
}
