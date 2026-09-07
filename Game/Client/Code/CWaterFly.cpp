#include "pch.h"
#include "CWaterFly.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
//#include "CWaterFlyTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"
#include "CCameraMgr.h"
#include "CCart.h"
#include "CCartBot.h"
#include <SoundMgr.h>

CWaterFly::CWaterFly(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget)
	: CGameObject(pGraphicDev)
	,m_pTarget(pTarget)
{
}

CWaterFly::~CWaterFly()
{
}

HRESULT CWaterFly::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer		= 0.f;
	m_fSpeed		= 400.f;
	m_fFrame = 0.f;
	//m_fAngle		= 0.f;
	

	m_pTransformCom->Set_Scale({ 2.5f, 2.5f, 0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_WaterFlyEffect"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CWaterFly::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	_vec3 vTargetPos, vMyPos;
	m_pTarget->Get_Transform()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vMyPos);
	
	vTargetPos.y += 1.f;

	_vec3 vDir = vTargetPos - vMyPos;

	if (D3DXVec3Length(&vDir) < 1.f)
	{
		if (CCart* pCart = dynamic_cast<CCart*>(m_pTarget))
		{
			SoundMgr::GetInstance().PlaySound(L"Effect/Item_waterbombFly/fired.ogg", SOUND_WATERFLY, 0.4f, true);

			pCart->SetBubble(true);
		}
		else if (CCartBot* pCartBot = dynamic_cast<CCartBot*>(m_pTarget))
		{
			SoundMgr::GetInstance().PlaySound(L"Effect/Item_waterbombFly/fired.ogg", SOUND_WATERFLY, 0.4f, true);

			pCartBot->SetBubble(true);
		}
		m_pLayer->Delete_GameObject(this);
		return;
	}
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->Move_Pos(&vDir, m_fSpeed, fFixedDeltaTime);
}

_int CWaterFly::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	return iExit;
}

void CWaterFly::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWaterFly::Render_GameObject()
{
	_matrix	matWorld, matView;
	matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
	m_pTransformCom->Set_Billboard(&matView);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

CWaterFly* CWaterFly::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget)
{
	CWaterFly* pWaterFly = new CWaterFly(pGraphicDev, pTarget);

	if (FAILED(pWaterFly->Ready_GameObject()))
	{
		Safe_Release(pWaterFly);
		MSG_BOX("pWaterFly Create Failed");
		return nullptr;
	}
	return pWaterFly;
}

void CWaterFly::Free()
{
	CGameObject::Free();
}
