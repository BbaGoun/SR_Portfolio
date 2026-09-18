#include "pch.h"
#include "CMirror.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"
#include "CRenderer.h"

CMirror::CMirror(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CMirror::CMirror(const CGameObject& rhs) : CGameObject(rhs)
{
}

CMirror::~CMirror()
{
}

HRESULT CMirror::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SkyDome"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CMirror::Update_GameObject(const _float& fDeltaTime)
{
	//CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_MIRROR, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMirror::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMirror::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CMirror* CMirror::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMirror* pObj = new CMirror(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CMirror::Free()
{
	CGameObject::Free();
}
