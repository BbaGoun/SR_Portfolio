#include "pch.h"
#include "CMagnetBody.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CMagnetTex.h"
#include "CRenderer.h"
#include "CManagement.h"

CMagnetBody::CMagnetBody(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CMagnetBody::~CMagnetBody()
{
}

HRESULT CMagnetBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 3.5f, 3.5f, 3.5f });
	m_pTransformCom->Set_Pos({ 0.f, 5.f, 20.f });

	pComponent = m_pBufferCom = dynamic_cast<CMagnetTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MagnetTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 1.f, 1.f, 1.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

_int CMagnetBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMagnetBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMagnetBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	// m_pColliderCom->Render_Component(D3DXCOLOR({ 1,0,0,1 }));
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMagnetBody::CollisionEnter(CCollider* pOtherCollider)
{
}

void CMagnetBody::TriggerEnter(CCollider* pOtherCollider)
{
}

CMagnetBody* CMagnetBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnetBody* pObj = new CMagnetBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMagnetBody Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CMagnetBody::Free()
{
	CGameObject::Free();
}
