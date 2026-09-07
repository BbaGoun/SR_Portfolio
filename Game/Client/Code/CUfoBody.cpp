#include "pch.h"
#include "CUfoBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CWaterBombTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

CUfoBody::CUfoBody(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CUfoBody::~CUfoBody()
{
}

HRESULT CUfoBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	 m_pTransformCom->Set_Pos({ 0.f,-1000.f,0.f });
	 m_pTransformCom->Set_Scale({ 8.5f, 8.2f, 0.35f });
	// m_pTransformCom->Set_Scale({ 1.5f, 1.2f, 1.35f });

	m_fTimer = 0.f;
	m_fFrame = 0.f;

	Engine::CComponent* pComponent = nullptr;

	// 임시 반구
	//pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	//if (nullptr == pComponent)
	//	return E_FAIL;

	//pComponent->Set_Owner(this);

	//m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_UfoEffect"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CUfoBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	//m_pTransformCom->Set_Pos({ 0.f, 0.f, 0.f });

	//_vec3 vScale = m_pTransformCom->Get_Scale();

	//m_pTransformCom->Set_Scale(vScale);

	_vec3 vScale = m_pTransformCom->Get_Scale();

	if (m_pParent != nullptr)
	{
		m_pTransformCom->Set_Pos({ 0.f, 5.f, 0.f });
	}

	m_fTimer += fFixedDeltaTime;

	m_pTransformCom->Set_Scale(vScale);
}

_int CUfoBody::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	// CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	m_fFrame += 15.f * fTimeDelta;

	if (m_fFrame >= 7.f)
		m_fFrame = 0.f;

	if (m_fTimer <= 1.32f)
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA
	}

	if (m_fTimer >= 1.85f)
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA
	}

	return iExit;
}

void CUfoBody::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CUfoBody::Render_GameObject()
{
	//_matrix	matWorld, matView;
	//matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
	//m_pTransformCom->Set_Billboard(&matView);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	// m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CUfoBody* CUfoBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUfoBody* pUfoBody = new CUfoBody(pGraphicDev);

	if (FAILED(pUfoBody->Ready_GameObject()))
	{
		Safe_Release(pUfoBody);
		MSG_BOX("pUfo Create Failed");
		return nullptr;
	}

	return pUfoBody;
}

void CUfoBody::Free()
{
	CGameObject::Free();
}
