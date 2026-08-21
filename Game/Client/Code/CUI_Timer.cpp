#include "pch.h"
#include "CUI_Timer.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
CUI_Timer::CUI_Timer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CUI_Timer::CUI_Timer(const CUI_Timer& rhs)
	:CGameObject(rhs)
{
}

CUI_Timer::~CUI_Timer()
{
}

HRESULT CUI_Timer::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 500.f, 200.f, 10.f });
	m_pTransformCom->Set_Scale({ 24.f,33.f,0.f });
	

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_TimerTexture"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	

	m_fFrame_1 = 0;
	m_fFrame_10 = 0;
	m_fFrame_60 = 0;
	m_fFrame_600 = 0;

	return S_OK;


}

void CUI_Timer::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_Timer::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	m_fFrame_1 += fDeltaTime;


	if (m_fFrame_1 >= 10)
	{
		m_fFrame_1 = 0;
		m_fFrame_10++;
	}

	if (m_fFrame_10 >= 6)
	{
		m_fFrame_10 = 0;
		m_fFrame_60++;
	}

	if (m_fFrame_60 >= 10)
	{
		m_fFrame_60 = 0;
		m_fFrame_600++;
	}


	return CGameObject::Update_GameObject(fDeltaTime);


}

void CUI_Timer::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Timer::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ 550.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_1);
	m_pVIBufferCom->Render_Buffer();
	
	m_pTransformCom->Set_Pos({ 520.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_10);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ 460.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_60);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ 430.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_600);
	m_pVIBufferCom->Render_Buffer();


}

CUI_Timer* CUI_Timer::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CUI_Timer* pObj = new CUI_Timer(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_Timer Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CUI_Timer::Free()
{
	CGameObject::Free();
}