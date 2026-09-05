#include "pch.h"
#include "CBoostAura.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CCameraMgr.h"
CBoostAura::CBoostAura(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoostAura::CBoostAura(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CBoostAura::~CBoostAura()
{
}

HRESULT CBoostAura::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	D3DXQUATERNION q;
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterJetEffect"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CBoostAura::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CBoostAura::Update_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent->Get_Parent())->GetBoost())
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

		m_fFrame += 4 * fDeltaTime;
		m_pTransformCom->Set_Scale(_vec3({ 0.6,0.6,0.6 }) * m_fFrame / 2);
		if (m_fFrame > 4)
			m_fFrame = 0;

		return CGameObject::Update_GameObject(fDeltaTime);
	}
	else
		m_fFrame = 0;
}

void CBoostAura::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (dynamic_cast<CCart*>(m_pParent->Get_Parent())->GetBoost())
		CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBoostAura::Render_GameObject()
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

CBoostAura* CBoostAura::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoostAura* pObj = new CBoostAura(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBoostAura Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CBoostAura::Free()
{
	CGameObject::Free();
}
