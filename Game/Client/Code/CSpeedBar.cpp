#include "pch.h"
#include "CSpeedBar.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CSpeedBar::CSpeedBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSpeedBar::CSpeedBar(const CSpeedBar& rhs)
	:CGameObject(rhs)
{
}

CSpeedBar::~CSpeedBar()
{
}

HRESULT CSpeedBar::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	
	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_pTransformCom->Set_Pos({ 0, -200, 10 });
	m_pTransformCom->Set_Scale({ 170.f,170.f,0.f });
	m_pTransformCom->Set_Owner(this);

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SpeedBarTexture"));
	if (nullptr == pComponent)
		return E_FAIL;

	return S_OK;


}

void CSpeedBar::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CSpeedBar::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CSpeedBar::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CSpeedBar::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pVIBufferCom->Render_Buffer();
}

CSpeedBar* CSpeedBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CSpeedBar* pObj = new CSpeedBar(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CSpeedBar Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CSpeedBar::Free()
{
}
