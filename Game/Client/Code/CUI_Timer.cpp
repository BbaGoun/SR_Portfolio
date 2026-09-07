#include "pch.h"
#include "CUI_Timer.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

#include "CPlayTimeMgr.h"
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

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_NumberTexture"));
	if (pComponent == nullptr)
		return E_FAIL;
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	

	m_iMilli_1 = 0;
	m_iMilli_10 = 0;
	m_iSecond_1 = 0;
	m_iSecond_10 = 0;
	m_iMin_1 = 0;
	m_iMin_10 = 0;

	
	return S_OK;
}

void CUI_Timer::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CUI_Timer::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_Timer::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_Timer::Render_GameObject()
{
	m_pTransformCom->Set_Scale({ 100.f,48.f, 1.f });
	m_pTransformCom->Set_Pos({ 400.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(13);
	m_pVIBufferCom->Render_Buffer();

	// m_iMin_10;
	m_pTransformCom->Set_Scale({ 20.f,28.f,1.f });
	m_pTransformCom->Set_Pos({ 470.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iMin_10);
	m_pVIBufferCom->Render_Buffer();

	// m_iMin_1;
	m_pTransformCom->Set_Pos({ 490.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iMin_1);
	m_pVIBufferCom->Render_Buffer();

	// 콜론
	m_pTransformCom->Set_Pos({ 510.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(11);
	m_pVIBufferCom->Render_Buffer();

	// m_iSecond_10;
	m_pTransformCom->Set_Pos({ 530.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iSecond_10);
	m_pVIBufferCom->Render_Buffer();

	// m_iSecond_1;
	m_pTransformCom->Set_Pos({ 550.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iSecond_1);
	m_pVIBufferCom->Render_Buffer();

	// 콜론
	m_pTransformCom->Set_Pos({ 570.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(11);
	m_pVIBufferCom->Render_Buffer();

	// m_iMilli_10;
	m_pTransformCom->Set_Pos({ 590.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iMilli_10);
	m_pVIBufferCom->Render_Buffer();

	// m_iMilli_1;
	m_pTransformCom->Set_Pos({ 610.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_iMilli_1);
	m_pVIBufferCom->Render_Buffer();
}

void CUI_Timer::Set_Timer(float _fPlayTime)
{
	m_iMin_10 = int(floor(_fPlayTime / 600.f));
	m_iMin_1 = int(floor(_fPlayTime / 60.f)) - 10 * m_iMin_10;
	m_iSecond_10 = int(floor(_fPlayTime / 10.f)) - 6 * m_iMin_1 - 60 * m_iMin_10;
	m_iSecond_1 = int(floor(_fPlayTime / 1.f)) - 10 * m_iSecond_10 + 60 * m_iMin_1 - 600 * m_iMin_10;
	m_iMilli_10 = int(floor(fmodf(_fPlayTime, 1.f) / 0.1f));
	m_iMilli_1 = int(floor(fmodf(_fPlayTime, 1.f) / 0.01f)) - 10 * m_iMilli_10;
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