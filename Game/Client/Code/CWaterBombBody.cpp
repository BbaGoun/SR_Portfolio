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
	// m_pTransformCom->Set_Scale({ 0.f, 3.5f, 0.f });
	m_pTransformCom->Set_Scale({ 20.f, 20.f, 20.f });
	m_fTimer = 0.f;

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Sphere"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CWaterBombBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	// CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");

	// _vec3 vScale, vCartLook, vCartPos;
	// vScale = m_pTransformCom->Get_Scale();
	//pCartBody->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);
	// pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);

	// m_pTransformCom->Set_Pos(vCartPos);

	_vec3 vScale = m_pTransformCom->Get_Scale();

	if (m_pParent != nullptr)
	{
		m_pTransformCom->Set_Pos({ 0.f, 0.f, 0.f });
	}

	m_fTimer += fFixedDeltaTime;	

	if (m_fTimer > 1.6f)
	{
		if (vScale.x < 120.f && vScale.y < 120.f && vScale.z < 120.f)
		{
			vScale.x += 380.f * fFixedDeltaTime;
			vScale.y += 380.f * fFixedDeltaTime;
			vScale.z += 380.f * fFixedDeltaTime;
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

	if (m_fTimer > 1.75f)
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA
	}
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

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CWaterBombBody* CWaterBombBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBombBody* pWaterBombBody = new CWaterBombBody(pGraphicDev);

	if (FAILED(pWaterBombBody->Ready_GameObject()))
	{
		Safe_Release(pWaterBombBody);
		MSG_BOX("pWaterBombBody Create Failed");
		return nullptr;
	}

	return pWaterBombBody;
}

void CWaterBombBody::Free()
{
	CGameObject::Free();
}
