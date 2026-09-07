#include "pch.h"
#include "CBanana.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CRcTex.h"

CBanana::CBanana(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CBanana::CBanana(const CGameObject& rhs) : CGameObject(rhs)
{
}

CBanana::~CBanana()
{
}

HRESULT CBanana::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 3,3,3 });

	CComponent* pComponent = nullptr;
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BananaTexture"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CSphere_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SphereCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Radius(1.5f);
	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

_int CBanana::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CBanana::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBanana::Render_GameObject()
{
	_matrix	matWorld, matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pTransformCom->Set_Billboard(&matView);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBanana::CollisionEnter(CCollider* pOtherCollider)
{
}

void CBanana::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcscmp(wOtherTag, L"Obj_CartBody") == 0)
	{
		m_pLayer->Delete_GameObject(this);
	}
}

CBanana* CBanana::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBanana* pObj = new CBanana(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBanana Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CBanana::Free()
{
	CGameObject::Free();
}
