#include "pch.h"
#include "CCartBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
CCartBody::CCartBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCartBody::CCartBody(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCartBody::~CCartBody()
{
}

HRESULT CCartBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0,0,3 });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);

	m_pColliderCom->SetCenter({ 0.f,-0.5f,3.f });
	m_pColliderCom->SetSize({ 2.5f,1.5f,5.f });
	m_pColliderCom->SetColliderType(CUBE_COLLIDER);

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


	pComponent = m_pSphereColliderCom = dynamic_cast<CSphere_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SphereCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);

	m_pSphereColliderCom->SetCenter({ 0.f,-0.5f,3.f });
	m_pSphereColliderCom->SetRadius(6.f);
	m_pSphereColliderCom->SetColliderType(SPHERE_COLLIDER);

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_SphereCollider", pComponent });

	

	return S_OK;
}

void CCartBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CCartBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCartBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
	
}

void CCartBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
}

CCartBody* CCartBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartBody* pObj = new CCartBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCartBody_FL Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCartBody::Free()
{
	CGameObject::Free();
}
