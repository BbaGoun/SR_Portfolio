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

	m_bCreate	= false;

	m_vThrowLook = { 0.f, 0.f, 0.f };

	m_pTransformCom->Set_Pos({ 0.f, -1000.f, 0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CSphere_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SphereCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Radius(60.f);
	m_pColliderCom->Set_Offset({ 0.f, 0.f, 0.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CWaterBomb::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");

	_vec3 vCartPos; // , vCartLook;

	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	//pCartBody->Get_Transform()->Get_Info(INFO_LOOK, &vCartLook);

	//vCartPos += vCartLook * 200.f;

	m_fTimer += fFixedDeltaTime;

	if (m_fTimer > 1.6f && m_bCreate == false)
	{
		vCartPos += m_vThrowLook * 200.f;
		m_pTransformCom->Set_Pos(vCartPos);
		m_bCreate = true;
	}

	if (m_fTimer > 3.f)
	{
		m_bCreate = false;
		m_pLayer->Delete_GameObject(this);
	}
}

_int CWaterBomb::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (m_fTimer > 1.75f)
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA
	}

	return iExit;
}

void CWaterBomb::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CWaterBomb::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pTextureCom->Set_Texture(0);
	// m_pBufferCom->Render_Buffer();

	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWaterBomb::CollisionEnter(CCollider* pOtherCollider)
{
}

void CWaterBomb::TriggerEnter(CCollider* pOtherCollider)
{
	// 컨셉에 맞게 따로 삭제
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	//if (wcscmp(wOtherTag, L"Obj_MissileTarget") == 0)
	if (wcsncmp(wOtherTag, L"Obj_MissileTarget", 17) == 0)
	{
		 vector<CGameObject*> vecChildren = Get_Children();

		for (auto& pChild : vecChildren)
		{

			pChild->To_Root();
			// m_pLayer->Delete_GameObject(pChild);
		}
		m_pLayer->Delete_GameObject(this);
	}
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
