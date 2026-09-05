#include "pch.h"
#include "CWaterFlyBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CWaterBombTex.h"
#include "CCollisionMgr.h"
#include "CRcTex.h"
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
	m_pTransformCom->Set_Pos({ 0.f,-1000.f,0.f });
	m_pTransformCom->Set_Scale({ 5.4f, 5.9f, 0.f });

	m_fTimer = 0.f;
	m_fFrame = 0.f;

	m_pTextureCom = nullptr;

	Engine::CComponent* pComponent = nullptr;


	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_WaterFlyEffect"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

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

	m_fFrame += 105.f * fTimeDelta;

	if (m_fFrame >= 3.f)
		m_fFrame = 0.f;


	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CWaterFlyBody::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWaterFlyBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	// m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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
