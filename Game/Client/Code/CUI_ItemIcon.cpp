#include "pch.h"
#include "CUI_ItemIcon.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "CDInputMgr.h"


CUI_ItemIcon::CUI_ItemIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_ItemIcon::CUI_ItemIcon(const CUI_ItemIcon& rhs)
	:CGameObject(rhs)
{
}

CUI_ItemIcon::~CUI_ItemIcon()
{
}

HRESULT CUI_ItemIcon::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ItemIconTexture"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	
	m_fFirstSlot = ITEM_END;
	m_fSecondSlot = ITEM_END;
	

	return S_OK;


}

void CUI_ItemIcon::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_ItemIcon::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_I))
	{
		if (m_fFirstSlot == ITEM_END)
		{
			m_fFirstSlot = rand() % ITEM_END;
		}

		else if (m_fFirstSlot != ITEM_END)
		{
			m_fSecondSlot = rand() % ITEM_END;
		}
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_Z))
	{
		m_fFirstSlot = m_fSecondSlot;

		m_fSecondSlot = ITEM_END;
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_ItemIcon::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_ItemIcon::Render_GameObject()
{
	if (m_fFirstSlot < ITEM_END)
	{
		m_pTransformCom->Set_Pos({ -505.f, 300.f, 1.f });
		m_pTransformCom->Set_Scale({ 80.f,80.f,0.f });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_fFirstSlot);
		m_pVIBufferCom->Render_Buffer();
	}
	if (m_fSecondSlot < ITEM_END)
	{
		m_pTransformCom->Set_Pos({ -585.f, 310.f, 1.f });
		m_pTransformCom->Set_Scale({ 60.f,60.f,0.f });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_fSecondSlot);
		m_pVIBufferCom->Render_Buffer();
	}

}

CUI_ItemIcon* CUI_ItemIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CUI_ItemIcon* pObj = new CUI_ItemIcon(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Itemicon Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_ItemIcon::Free()
{
	CGameObject::Free();
}