#include "pch.h"
#include "CCartBody1.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
CCartBody1::CCartBody1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCartBody1::CCartBody1(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCartBody1::~CCartBody1()
{
}

HRESULT CCartBody1::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CQuadrangularPrism*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_QuadrangularPrism"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CCartBody1::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CCartBody1::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCartBody1::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CCartBody1::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

CCartBody1* CCartBody1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartBody1* pObj = new CCartBody1(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCartBody1 Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCartBody1::Free()
{
	CGameObject::Free();
}
