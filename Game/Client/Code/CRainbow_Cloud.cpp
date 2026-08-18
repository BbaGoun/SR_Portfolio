#include "pch.h"
#include "CRainbow_Cloud.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"

CRainbow_Cloud::CRainbow_Cloud(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CRainbow_Cloud::CRainbow_Cloud(const CGameObject& rhs) : CGameObject(rhs)
{
}

CRainbow_Cloud::~CRainbow_Cloud()
{
}

HRESULT CRainbow_Cloud::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Scale({ 60,30,1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RainBow_Alpha"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 30,15,1 });
	m_mapComponent.insert({ L"Com_Collider", pComponent });

	m_fTimer = 0;

	return S_OK;
}

_int CRainbow_Cloud::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	m_fTimer += fDeltaTime;

	if (m_fTimer > 4)
	{
		m_pLayer->Delete_GameObject(this);
	}
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CRainbow_Cloud::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CRainbow_Cloud::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRainbow_Cloud::CollisionEnter(CCollider* pOtherCollider)
{
}

void CRainbow_Cloud::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcscmp(wOtherTag, L"Obj_Cart") == 0)
	{

	}
}

CRainbow_Cloud* CRainbow_Cloud::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRainbow_Cloud* pObj = new CRainbow_Cloud(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CRainbow_Cloud Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CRainbow_Cloud::Free()
{
	CGameObject::Free();
}
