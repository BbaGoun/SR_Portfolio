#include "pch.h"
#include "CUI_ItemSlot.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CCart.h"
#include "CManagement.h"

CUI_ItemSlot::CUI_ItemSlot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_ItemSlot::CUI_ItemSlot(const CUI_ItemSlot& rhs)
	:CGameObject(rhs)
{
}

CUI_ItemSlot::~CUI_ItemSlot()
{
}

HRESULT CUI_ItemSlot::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ItemSlotTexture"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom2 = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ItemSlotChangeTexture"));
	m_mapComponent.insert({ L"Com_Texture2", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	
	return S_OK;
}

void CUI_ItemSlot::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_ItemSlot::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);
	CCart* pCart = static_cast<CCart*>(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart"));

	m_bSlotChange = pCart->GetSlotChange();
	if (m_bSlotChange) {
		pCart->SetSlotChange(false);
		m_iSlotChangeAnim = 0;
	}

	if (m_bSlotChange || m_iSlotChangeAnim != 0) {
		m_iSlotChangeAnim += 1;
		if (m_iSlotChangeAnim % 20 == 0) {
			m_bSlotChange = false;
			m_iSlotChangeAnim = 0;
		}
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_ItemSlot::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_ItemSlot::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ -540.f, 300.f, 5.f });
	m_pTransformCom->Set_Scale({ 164.f,92.f,0.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ -425.f, 310.f, 5.f });
	m_pTransformCom->Set_Scale({ 60.f,60.f,0.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom2->Set_Texture(m_iSlotChangeAnim / 5);
	m_pVIBufferCom->Render_Buffer();
}

CUI_ItemSlot* CUI_ItemSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CUI_ItemSlot* pObj = new CUI_ItemSlot(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Itemicon Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_ItemSlot::Free()
{
	CGameObject::Free();
}