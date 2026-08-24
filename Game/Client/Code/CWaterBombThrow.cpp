#include "pch.h"
#include "CWaterBombThrow.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"

CWaterBombThrow::CWaterBombThrow(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CWaterBombThrow::CWaterBombThrow(const CGameObject& rhs) : CGameObject(rhs)
{
}

CWaterBombThrow::~CWaterBombThrow()
{
}

HRESULT CWaterBombThrow::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer = 0.f;

	m_pTransformCom->Set_Pos({ 0.f,5.f,0.f });

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 1.7f , 1.7f, 0.f });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	//pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RainBow_Alpha"));
	//pComponent->Set_Owner(this);
	//m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CWaterBombThrow::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");

	_vec3 vUp, vPos, vCartPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);


	vPos += vUp * 0.6f;	

	m_pTransformCom->Set_Pos(vPos);

	m_fTimer += fFixedDeltaTime;

	if (m_fTimer > 3.5f)
	{
		m_pLayer->Delete_GameObject(this);
	}

}

_int CWaterBombThrow::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CWaterBombThrow::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CWaterBombThrow::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	// m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	//m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWaterBombThrow::CollisionEnter(CCollider* pOtherCollider)
{
}

void CWaterBombThrow::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcscmp(wOtherTag, L"Obj_Cart") == 0)
	{

	}
}

CWaterBombThrow* CWaterBombThrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBombThrow* pObj = new CWaterBombThrow(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CWaterBombThrow Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CWaterBombThrow::Free()
{
	CGameObject::Free();
}
