#include "pch.h"
#include "CWaterFlyBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CWaterBombTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

CWaterFlyBody::CWaterFlyBody(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CWaterFlyBody::~CWaterFlyBody()
{
}

HRESULT CWaterFlyBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	// m_pTransformCom->Set_Pos({ 0.f,0.f,0.f });
	m_pTransformCom->Set_Scale({ 0.4f, 0.9f, 0.f });

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

void CWaterFlyBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	m_pTransformCom->Set_Pos({ 0.f, 0.f, 0.f });

	_vec3 vScale = m_pTransformCom->Get_Scale();

	m_pTransformCom->Set_Scale(vScale);
}

_int CWaterFlyBody::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CWaterFlyBody::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWaterFlyBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CWaterFlyBody* CWaterFlyBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterFlyBody* pWaterFlyBody = new CWaterFlyBody(pGraphicDev);

	if (FAILED(pWaterFlyBody->Ready_GameObject()))
	{
		Safe_Release(pWaterFlyBody);
		MSG_BOX("pWaterFlyBody Create Failed");
		return nullptr;
	}

	return pWaterFlyBody;
}

void CWaterFlyBody::Free()
{
	CGameObject::Free();
}
