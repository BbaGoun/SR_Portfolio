#include "pch.h"
#include "CTargetAim.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CTargetAim::CTargetAim(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pColliderCom(nullptr)
{
}

CTargetAim::CTargetAim(const CGameObject& rhs)
	: CGameObject(rhs)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pColliderCom(nullptr)
{
}

CTargetAim::~CTargetAim()
{
}

HRESULT CTargetAim::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 15.f, 15.f, 0.f });

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));

	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_TargetAimTexture"));
	 
	 	if (nullptr == pComponent)
		    return E_FAIL;

	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 5.f, 5.f, 0.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

_int CTargetAim::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CTargetAim::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CTargetAim::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CTargetAim::CollisionEnter(CCollider* pOtherCollider)
{
}

void CTargetAim::TriggerEnter(CCollider* pOtherCollider)
{
}

CTargetAim* CTargetAim::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTargetAim* pTargetAim = new CTargetAim(pGraphicDev);

	if (FAILED(pTargetAim->Ready_GameObject()))
	{
		MSG_BOX("CTargetAim Create Failed");
		Safe_Release(pTargetAim);
		return nullptr;
	}

	return pTargetAim;
}

void CTargetAim::Free()
{
	CGameObject::Free();
}