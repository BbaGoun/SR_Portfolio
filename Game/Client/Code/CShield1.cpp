#include "pch.h"
#include "CShield1.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"
#include "SoundMgr.h"
#include "CMissileTarget.h"

CShield1::CShield1(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CShield1::CShield1(const CGameObject& rhs) : CGameObject(rhs)
{
}

CShield1::~CShield1()
{
}

HRESULT CShield1::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer = 0;
	m_bShow = false;

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Pos({ 0.f,0.f,0.f });
	m_pTransformCom->Set_Scale({ 12.f, 12.0f, 12.0f });

	pComponent = m_pBufferCom = dynamic_cast<CHalfSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_HalfSphere"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ShieldEffect"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CShield1::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == true)
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
		m_fTimer += fDeltaTime;
		if (m_fTimer >= 2.f)
		{
			m_fTimer = 0;
			m_bShow = false;
		}
		m_fFrame += 30 * fDeltaTime;
		if (m_fFrame > 6)
		{
			m_fFrame = 0;
		}

		return CGameObject::Update_GameObject(fDeltaTime);
	}
}

void CShield1::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CShield1::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetTexture(0, nullptr);
}

CShield1* CShield1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShield1* pObj = new CShield1(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CShield1 Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CShield1::Free()
{
	CGameObject::Free();
}
