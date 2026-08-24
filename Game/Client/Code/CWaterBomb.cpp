#include "pch.h"
#include "CWaterBomb.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CWaterBombTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

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
	m_fSpeed	= 0.f;
	m_fAngle	= 0.f;

	// m_pTransformCom->Set_Pos({ -50.f,0.f,150.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 40.f, 12.f, 40.f });
	m_pColliderCom->Set_Offset({ 0.f, 4.f, 0.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CWaterBomb::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	m_fTimer += fFixedDeltaTime;

	if (m_fTimer > 3.f)
	{
		m_pLayer->Delete_GameObject(this);
	}
}

_int CWaterBomb::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	//m_pLayer->Delete_GameObject(this);

	return iExit;
}

void CWaterBomb::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CWaterBomb::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pTextureCom->Set_Texture(0);
	//m_pBufferCom->Render_Buffer();

	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWaterBomb::CollisionEnter(CCollider* pOtherCollider)
{
}

void CWaterBomb::TriggerEnter(CCollider* pOtherCollider)
{
	// 컨셉에 맞게 따로 삭제
	//const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	//if (wcscmp(wOtherTag, L"Obj_MissileTarget") == 0)
	//{
	//	vector<CGameObject*> vecChildren = Get_Children();

	//	for (auto& pChild : vecChildren)
	//	{
	//		pChild->To_Root();
	//		m_pLayer->Delete_GameObject(pChild);
	//	}
	//	m_pLayer->Delete_GameObject(this);
	//}
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
