#include "pch.h"
#include "CBoostJet.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CCameraMgr.h"
CBoostJet::CBoostJet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoostJet::CBoostJet(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CBoostJet::~CBoostJet()
{
}

HRESULT CBoostJet::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 0.5,0.5,0.5 });
	m_pTransformCom->Set_Pos({ 0.3f,0.12f,-1.1f });
	D3DXQUATERNION q;
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterJet"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CBoostJet::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CBoostJet::Update_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent->Get_Parent())->GetBoost())
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

		m_fFrame += 20 * fDeltaTime;
		if (m_fFrame > 2)
			m_fFrame = 0;

		return CGameObject::Update_GameObject(fDeltaTime);
	}
}

void CBoostJet::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent->Get_Parent())->GetBoost())
		CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBoostJet::Render_GameObject()
{
	if (dynamic_cast<CCart*>(m_pParent->Get_Parent())->GetBoost())
	{
		m_pTransformCom->Set_Pos({ 0.3f,0.12f,-1.1f });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_fFrame);
		m_pBufferCom->Render_Buffer();

		m_pTransformCom->Set_Pos({ -0.3f,0.12f,-1.1f });
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_fFrame);
		m_pBufferCom->Render_Buffer();
	}
}

CBoostJet* CBoostJet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoostJet* pObj = new CBoostJet(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBoostJet Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CBoostJet::Free()
{
	CGameObject::Free();
}
