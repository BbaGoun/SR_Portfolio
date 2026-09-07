#include "pch.h"
#include "CTimer.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
CTimer::CTimer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CTimer::CTimer(const CTimer& rhs)
	:CGameObject(rhs)
{
}

CTimer::~CTimer()
{
}

HRESULT CTimer::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 500.f, 200.f, 10.f });
	m_pTransformCom->Set_Scale({ 15.f,15.f,0.f });
	

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_TimerTexture"));
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	

	m_fFrame_1 = 0;
	m_fFrame_10 = 0;
	m_fFrame_100 = 0;

	return S_OK;


}

void CTimer::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CTimer::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	m_fFrame_1 += fDeltaTime;


	if (m_fFrame_1 >= 10)
	{
		m_fFrame_1 = 0;
		m_fFrame_10++;
	}

	if (m_fFrame_10 >= 10)
	{
		m_fFrame_10 = 0;
		m_fFrame_100++;
	}


	return CGameObject::Update_GameObject(fDeltaTime);


}

void CTimer::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CTimer::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ 550.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_1);
	m_pVIBufferCom->Render_Buffer();
	
	m_pTransformCom->Set_Pos({ 520.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_10);
	m_pVIBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ 490.f, 200.f, 10.f });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame_100);
	m_pVIBufferCom->Render_Buffer();

}

CTimer* CTimer::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CTimer* pObj = new CTimer(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CTimer Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CTimer::Free()
{
}