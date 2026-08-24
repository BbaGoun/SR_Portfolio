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

	m_pTransformCom->Set_Pos({ -50.f,0.f,150.f });
	m_pTransformCom->Set_Scale({ 1.5f, 3.5f, 0.7f });

	Engine::CComponent* pComponent = nullptr;

	// 임시 반구
	pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CWaterBombBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	_vec3 vScale;
	vScale = m_pTransformCom->Get_Scale();

	if (vScale.x < 18.f && vScale.y < 18.f && vScale.z < 18.f)
	{
		vScale.x += 50.f * fFixedDeltaTime;
		vScale.y += 50.f * fFixedDeltaTime;
		vScale.z += 50.f * fFixedDeltaTime;
	}

	// 디테일 나중에 반구 만들고
	//if (vScale.x < 12.f && vScale.z < 12.f)
	//{
	//	vScale.x += 30.f * fFixedDeltaTime;
	//	vScale.z += 30.f * fFixedDeltaTime;
	//}

	//if (vScale.y < 50.f)
	//{
	//	vScale.y += 60.f * fFixedDeltaTime;
	//}

	m_pTransformCom->Set_Scale(vScale);
}

_int CWaterBombBody::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

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
