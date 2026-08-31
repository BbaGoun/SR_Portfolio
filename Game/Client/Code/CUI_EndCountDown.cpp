#include "pch.h"
#include "CUI_EndCountDown.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CPlayTimeMgr.h"

CUI_EndCountDown::CUI_EndCountDown(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_EndCountDown::CUI_EndCountDown(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_EndCountDown::~CUI_EndCountDown()
{
}

HRESULT CUI_EndCountDown::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_EndCountDown"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);
	m_pTransformCom->Set_Pos({ 0.f, vp.Height * 0.1f, 1.f });

	m_fFrame		= 0.f;
	m_fTimer		= 0.f;
	m_fTimerFlag	= 0.f;
	m_bShow			= false;
	m_iShakeCnt		= 0;
	m_vForce		= { 1,0,0 };


	return S_OK;
}

_int CUI_EndCountDown::Update_GameObject(const _float& fDeltaTime)
{
	UdateFrame();
	if (m_bShow)
	{
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);


		if (m_fFrame > 0.f)
		{
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);

			if (vPos.x < 0)
			{
				_D3DVIEWPORT9 vp;
				m_pGraphicDev->GetViewport(&vp);
				m_fSpeed = vp.Width * 3;
				m_pTransformCom->Move_Pos(&m_vForce, m_fSpeed, fDeltaTime);
			}
			else if (vPos.x >= 0 && m_iShakeCnt < 10)
			{
				m_pTransformCom->Set_Pos({ 0,vPos.y,1 });
				Shake(fDeltaTime);
			}
			else
			{
				m_pTransformCom->Move_Pos(&m_vForce, m_fSpeed, fDeltaTime);
			}
		}
		return CGameObject::Update_GameObject(fDeltaTime);
	}
}

void CUI_EndCountDown::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow)
		CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_EndCountDown::Render_GameObject()
{
	if (m_bShow)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(m_fFrame);

		m_pBufferCom->Render_Buffer();
	}
}

void CUI_EndCountDown::SetFrame(int iFrame)
{
	_D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	m_bShow = true;
	m_fFrame = iFrame;

	m_iShakeCnt = 0;

	if (iFrame > 0)
	{
		m_pTransformCom->Set_Scale(_vec3({ 136.f,100.f,0 }) * 0.8f);
		m_pTransformCom->Set_Pos({ -(vp.Width * 0.5f),vp.Height * 0.1f,1.f });
	}
	else
	{
		m_pTransformCom->Set_Scale(_vec3({ 339.f,100.f,0 }) * 0.8f);
		m_pTransformCom->Set_Pos({ 0,vp.Height * 0.1f,1.f });
	}
}

void CUI_EndCountDown::Shake(const _float fDeltaTime)
{
	++m_iShakeCnt;
	if (m_iShakeCnt % 3 != 0)
		return;
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	_D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	if (vPos.y > vp.Height * 0.1f)
		m_pTransformCom->Set_Pos({ 0,vPos.y - 40.f,1 });
	
	else if (vPos.y < vp.Height * 0.1f)
		m_pTransformCom->Set_Pos({ 0,vPos.y + 20.f,1 });
	
	else
		m_pTransformCom->Set_Pos({ 0,vPos.y + 20.f,1 });

}

void CUI_EndCountDown::UdateFrame()
{
	float fEndTime		= CPlayTimeMgr::GetInstance()->GetPlayEndTime();
	float fPlayTimer	= CPlayTimeMgr::GetInstance()->GetPlayTimer();
	bool  bPlaying		= CPlayTimeMgr::GetInstance()->GetPlaying();

	//if (fPlayTimer < fEndTime || bPlaying == false)
	//	return;

	if (fPlayTimer > fEndTime + 10.f && bPlaying == false)
	{
		SetFrame(0);
	}
	else
	{
		for (int i = 0; i < 10; ++i)
		{
			if (m_fTimerFlag < fEndTime + float(i) && fPlayTimer > fEndTime + float(i))
			{
				SetFrame(10 - i);
				m_fTimerFlag = fPlayTimer;
			}
		}
	}
}

CUI_EndCountDown* CUI_EndCountDown::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_EndCountDown* pObj = new CUI_EndCountDown(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_EndCountDown Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_EndCountDown::Free()
{
	CGameObject::Free();
}
