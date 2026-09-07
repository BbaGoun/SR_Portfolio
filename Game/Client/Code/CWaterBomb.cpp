#include "pch.h"
#include "CWaterBomb.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"
#include "CSphere_Collider.h"

CWaterBomb::CWaterBomb(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CWaterBomb::~CWaterBomb()
{
}

HRESULT CWaterBomb::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer	= 0.f;
	//m_fSpeed	= 0.f;
	//m_fAngle	= 0.f;

	Engine::CComponent* pComponent = nullptr;

	// 콜라이더
	pComponent = m_pColliderCom = dynamic_cast<CSphere_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SphereCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Radius(0.01f);
	m_pColliderCom->Set_Offset({ 0.f, 0.f, 0.f });
	m_mapComponent.insert({ L"Com_Collider", pComponent });


	return S_OK;
}

void CWaterBomb::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	float fScale = clampT(m_fTimer * 50.f, 0.f, 50.f);

	m_pColliderCom->Set_Radius(fScale * 0.45f);

	CGameObject::FixedUpdate_GameObject(fFixedDeltaTime);
}

_int CWaterBomb::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_fTimer += fTimeDelta;

	float fScale = clampT(m_fTimer * 50.f, 0.f, 50.f);
	m_vRotation.y += fTimeDelta;
	m_vecChildren[0]->Get_Transform()->Set_Scale({ fScale, fScale*2.f, fScale*2.f });

	if (m_fTimer > 3.f)
	{
		m_pLayer->Delete_GameObject(this);
	}

	return iExit;
}

void CWaterBomb::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWaterBomb::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pTextureCom->Set_Texture(0);
	// m_pBufferCom->Render_Buffer();

	//m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWaterBomb::CollisionEnter(CCollider* pOtherCollider)
{
}

void CWaterBomb::TriggerEnter(CCollider* pOtherCollider)
{
}

CWaterBomb* CWaterBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBomb* pWaterBomb = new CWaterBomb(pGraphicDev);

	if (FAILED(pWaterBomb->Ready_GameObject()))
	{
		Safe_Release(pWaterBomb);
		MSG_BOX("pWaterBomb Create Failed");
		return nullptr;
	}

	return pWaterBomb;
}

void CWaterBomb::Free()
{
	CGameObject::Free();
}
