#include "pch.h"
#include "CBox.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCollisionMgr.h"
#include "CManagement.h"

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBox::CBox(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CBox::~CBox()
{
}

HRESULT CBox::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	//pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//pComponent->Set_Owner(this);
	//
	//m_pColliderCom->SetCenter({ 0,0,100.f });
	//m_pColliderCom->SetSize({ 2.5f,1.f,5.f });
	//m_pColliderCom->SetColliderType(CUBE_COLLIDER);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CBox::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CBox::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CBox::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

CBox* CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBox* pObj = new CBox(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBox_FL Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CBox::Free()
{
	CGameObject::Free();
}
