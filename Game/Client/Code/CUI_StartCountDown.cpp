#include "pch.h"
#include "CUI_StartCountDown.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CPlayTimeMgr.h"

CUI_StartCountDown::CUI_StartCountDown(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_StartCountDown::CUI_StartCountDown(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_StartCountDown::~CUI_StartCountDown()
{
}

HRESULT CUI_StartCountDown::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_StartCountDown"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);
	m_pTransformCom->Set_Pos({ 0.f, vp.Height*0.1f, 1.f });

	m_fFrame		= 0.f;
	m_fTimer		= 0.f;
	m_bShow			= false;
	m_fTimerFlag	= 0.f;

	return S_OK;
}

_int CUI_StartCountDown::Update_GameObject(const _float& fDeltaTime)
{
	UpdateFrame();

	if (m_bShow)
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);


		if (m_fFrame <= 2.f)
		{
			if (m_fScale < 100.f)
				m_fScale += 100 * fDeltaTime;
			else
				m_fScale = 100.f;
			m_pTransformCom->Set_Scale({ m_fScale,m_fScale,1 });
		}
		else
		{
			m_fTimer += fDeltaTime;
			if (m_fTimer >= 0.5f)
				m_bShow = false;
		}

		return CGameObject::Update_GameObject(fDeltaTime);
	}
}

void CUI_StartCountDown::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow)
		CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_StartCountDown::Render_GameObject()
{
	if (m_bShow)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_fFrame);

		m_pBufferCom->Render_Buffer();
	}
}

void CUI_StartCountDown::UpdateFrame()
{
	float fStartTimer = CPlayTimeMgr::GetInstance()->GetStartTimer();
	
	if (fStartTimer > 4.f)
		return;

	if (fStartTimer > 3.f)
	{
		SetFrame(4);
	}
	else if (m_fTimerFlag < 2.f && fStartTimer > 2.f)
	{
		m_fTimerFlag = fStartTimer;
		SetFrame(1);
	}
	else if (m_fTimerFlag < 1.f && fStartTimer > 1.f)
	{
		m_fTimerFlag = fStartTimer;
		SetFrame(2);
	}
	else if (m_fTimerFlag == 0.f && fStartTimer > 0.f)
	{
		m_fTimerFlag = fStartTimer;
		SetFrame(3);
	}
}

void CUI_StartCountDown::SetFrame(int iFrame)
{
	_D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	switch (iFrame)
	{
	case 1:
		m_bShow = true; 
		m_fScale = 0.f;
		m_fFrame = 0;
		break;
	case 2:
		m_bShow = true;
		m_fScale = 0.f;
		m_fFrame = 1;
		break;
	case 3:
		m_bShow = true;
		m_fScale = 0.f;
		m_fFrame = 2;
		break;
	case 4:
		m_bShow = true;
		m_pTransformCom->Set_Scale({ 400,80,0 });
		m_fFrame = 3;
		break;
	default:
		break;
	}
}

CUI_StartCountDown* CUI_StartCountDown::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_StartCountDown* pObj = new CUI_StartCountDown(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_StartCountDown Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_StartCountDown::Free()
{
	CGameObject::Free();
}
