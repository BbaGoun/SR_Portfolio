#include "pch.h"
#include "CItemicon.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
CItemicon::CItemicon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CItemicon::CItemicon(const CItemicon& rhs)
	:CGameObject(rhs)
{
}

CItemicon::~CItemicon()
{
}

HRESULT CItemicon::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ -540, 300, 10 });
	m_pTransformCom->Set_Scale({ 100.f,60.f,0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ItemiconTexture"));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	

	return S_OK;


}

void CItemicon::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CItemicon::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return CGameObject::Update_GameObject(fDeltaTime);


}

void CItemicon::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CItemicon::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pVIBufferCom->Render_Buffer();


}

CItemicon* CItemicon::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CItemicon* pObj = new CItemicon(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Itemicon Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CItemicon::Free()
{
}