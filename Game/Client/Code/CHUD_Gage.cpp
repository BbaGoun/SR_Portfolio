#include "pch.h"
#include "CHUD_Gage.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"

CHUD_Gage::CHUD_Gage(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CHUD_Gage::CHUD_Gage(const CGameObject& rhs) : CGameObject(rhs)
{
}

CHUD_Gage::~CHUD_Gage()
{
}

HRESULT CHUD_Gage::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Pos({ 0,-307.5,1 });
	m_pTransformCom->Set_Scale({ 282, 105, 1 });


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_HUDGage"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	return S_OK;
}

_int CHUD_Gage::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	m_fFrame += 10 * fDeltaTime;
	if (m_fFrame > 5)
		m_fFrame = 0;
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CHUD_Gage::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CHUD_Gage::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CHUD_Gage* CHUD_Gage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHUD_Gage* pObj = new CHUD_Gage(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CHUD_Gage Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CHUD_Gage::Free()
{
	CGameObject::Free();
}
