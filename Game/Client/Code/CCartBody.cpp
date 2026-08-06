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

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CCartBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CCartBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	KeyInput(fDeltaTime);
	return _int();
}

void CCartBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
}

void CCartBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CCartBody::KeyInput(const _float& fDeltaTime)
{
	D3DXQUATERNION q;
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_UP))
	{

	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_DOWN))
	{

	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
	{
		
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
	{
		
	}
	else
	{
		
	}
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
