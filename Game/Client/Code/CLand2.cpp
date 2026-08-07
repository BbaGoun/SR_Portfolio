#include "pch.h"
#include "CLand2.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"
#include "CRenderer.h"

CLand2::CLand2(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CLand2::CLand2(const CGameObject& rhs) : CGameObject(rhs)
{
}

CLand2::~CLand2()
{
}

HRESULT CLand2::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 2, 2, 2 });

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CTerrain2*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Terrain2"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

_int CLand2::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CLand2::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CLand2::Render_GameObject()
{
	D3DXMATRIX* matWorld;

	matWorld = m_pTransformCom->Get_World();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetTexture(0, nullptr);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CLand2* CLand2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLand2* pObj = new CLand2(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Land Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CLand2::Free()
{
	CGameObject::Free();
}
