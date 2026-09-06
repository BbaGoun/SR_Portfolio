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

	// m_pTransformCom->Set_Scale({ 0.f, 3.5f, 0.f });
	m_pTransformCom->Set_Scale({ 0.f, 0.f, 0.f });

	m_pTextureCom = nullptr;

	Engine::CComponent* pComponent = nullptr;


	// 버퍼
	pComponent = m_pBufferCom = dynamic_cast<CHalfSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_HalfSphere"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	// 텍스처
	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_WaterBombBody"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CWaterBombBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CGameObject::FixedUpdate_GameObject(fFixedDeltaTime);
}

_int CWaterBombBody::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CWaterBombBody::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWaterBombBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	// m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	// m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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
