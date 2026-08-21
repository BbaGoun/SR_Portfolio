#include "pch.h"
#include "CMissileBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMissileTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

CMissileBody::CMissileBody(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMissileBody::~CMissileBody()
{
}

HRESULT CMissileBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pTransformCom->Set_Scale({ 4.5f, 4.5f, 3.5f });

	Engine::CComponent* pComponent = nullptr;

	// 미사일
	pComponent = m_pBufferCom = dynamic_cast<CMissileTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MissileTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CMissileBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	
}

_int CMissileBody::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CMissileBody::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMissileBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CMissileBody* CMissileBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissileBody* pMissile = new CMissileBody(pGraphicDev);

	if (FAILED(pMissile->Ready_GameObject()))
	{
		Safe_Release(pMissile);
		MSG_BOX("pMissile Create Failed");
		return nullptr;
	}

	return pMissile;
}

void CMissileBody::Free()
{
	CGameObject::Free();
}
