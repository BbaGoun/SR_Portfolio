#include "pch.h"
#include "CMissileTarget.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCollisionMgr.h"
#include "CManagement.h"
#include "CCube_Collider.h"	

CMissileTarget::CMissileTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMissileTarget::~CMissileTarget()
{
}

HRESULT CMissileTarget::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fSpeed = 0.f;
	m_fMaxSpeed = 3.f;
	m_pTransformCom->Set_Pos({ 0,0,100.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetColliderType(CUBE_COLLIDER);
	m_pColliderCom->Set_Extents({ 1.f, 1.f, 1.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CMissileTarget::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	_vec3 vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vPos.y += m_vForce.y * fFixedDeltaTime;

	m_pTransformCom->Set_Pos(vPos);
}

_int CMissileTarget::Update_GameObject(const _float& fDeltaTime)
{

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMissileTarget::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMissileTarget::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

CMissileTarget* CMissileTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissileTarget* pObj = new CMissileTarget(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMissileTarget_FL Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CMissileTarget::Free()
{
	CGameObject::Free();
}
