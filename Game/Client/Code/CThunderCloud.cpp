#include "pch.h"
#include "CThunderCloud.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCameraMgr.h"
#include "CCart.h"
#include "CThunder.h"
#include "CThunderPlayerEffect.h"
#include "CThunderFloorEffect.h"
#include "CCartBody.h"
#include "SoundMgr.h"
#include <CShield1.h>
#include <CShield2.h>
#include <CCartBot.h>

CThunderCloud::CThunderCloud(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget) 
	: CGameObject(pGraphicDev)
	,m_pTarget(pTarget)
{
}

CThunderCloud::CThunderCloud(const CGameObject& rhs) : CGameObject(rhs)
{
}

CThunderCloud::~CThunderCloud()
{
}

HRESULT CThunderCloud::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Scale({ 10,10,1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ThunderCloudTexture"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fThunderDelayTimer	= 0;
	m_fFrame				= 0;
	m_bClosed				= false;
	m_bCreateThunder		= false;

	m_pThunder				= nullptr;
	m_pThunderPlayerEffect	= nullptr;
	m_pThunderFloorEffect		= nullptr;

	return S_OK;
}

_int CThunderCloud::Update_GameObject(const _float& fDeltaTime)
{
	CCartBody* pCartBody = nullptr;
	CCart* pCart = dynamic_cast<CCart*>(m_pTarget);
	CCartBot* pCartBot = dynamic_cast<CCartBot*>(m_pTarget);

	for (auto& pChild : m_pTarget->Get_Children())
	{
		if (dynamic_cast<CCartBody*>(pChild))
		{
			pCartBody = dynamic_cast<CCartBody*>(pChild);
		}
	}
	
	_vec3 vCartPos, vPos, vDir;
	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	m_pTransformCom->Get_Info(INFO_POS,&vPos);
	m_pTransformCom->Set_Pos({ vCartPos.x,vCartPos.y + 10,vCartPos.z });

	m_fThunderDelayTimer += fDeltaTime;
	
	if (m_bCreateThunder == false && m_fThunderDelayTimer > 1.5f)
	{
		//if (pCartBody->GetThunderTimerOnOff() == false)
		//{
		//	pCartBody->SetThunderTimerOnOff(true);
		//	m_fFrame = 1;
		//}

		CreateThunder();

		if (pCart && pCart->GetShield1())
		{
			static_cast<CShield1*>(pCart->GetShield1())->SetShow(false);
			static_cast<CShield2*>(pCart->GetShield2())->SetShow(true);

		}
		//else if (pCartBot && pCartBot->GetShield1())
		//{
		//	static_cast<CShield1*>(pCartBot->GetShield1())->SetShow(false);
		//	static_cast<CShield2*>(pCartBot->GetShield2())->SetShow(true);
		//}
		//if (pCartBody->GetShieldActive())
		//	pCartBody->SetShieldHit(true);

		else 
		{
			if (pCartBody->GetThunderTimerOnOff() == false)
			{
				pCartBody->SetThunderTimerOnOff(true);
				m_fFrame = 1;
			}

			CreateThunderPlayerEffect();
			CreateThunderFloorEffect();
		}
		// CreateThunderPlayerEffect();
		// CreateThunderFloorEffect();
	}
	else if(m_bCreateThunder == true)
	{
		if (m_fFrame < 1)
		{
			m_fFrame += 5 * fDeltaTime;
		}
		else if (m_fFrame < 2)
		{
			m_fFrame += 5 * fDeltaTime;
		}
		else
		{
			m_fFrame = 0;
		}
		if (pCartBody->GetThunderSpinState() == false)
		{
			m_pLayer->Delete_GameObject(this);
			m_pLayer->Delete_GameObject(m_pThunder);
			m_pLayer->Delete_GameObject(m_pThunderFloorEffect);
			m_pLayer->Delete_GameObject(m_pThunderPlayerEffect);
		}
	}
	

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);


	
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CThunderCloud::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CThunderCloud::Render_GameObject()
{
	_matrix	matWorld, matView;
	matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
	m_pTransformCom->Set_Billboard(&matView);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

CThunderCloud* CThunderCloud::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pTarget)
{
	CThunderCloud* pObj = new CThunderCloud(pGraphicDev,pTarget);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CThunderCloud Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CThunderCloud::CreateThunder()
{
	SoundMgr::GetInstance().PlaySound(L"Effect/Item_thunderbolt/Thunder.ogg", SOUND_THUNDER, 0.4f);
	m_bCreateThunder = true;
	m_pThunder = CThunder::Create(m_pGraphicDev);

	if (nullptr == m_pThunder)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_Thunder", m_pThunder)))
		return;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y -= 5.f;
	m_pThunder->Get_Transform()->Set_Pos(vPos);
	Set_ChildTuneDefault(m_pThunder);
	m_pThunder->SetLayer(m_pLayer);
}

void CThunderCloud::CreateThunderPlayerEffect()
{
	SoundMgr::GetInstance().PlaySound(L"Effect/Item_thunderbolt/ThunderPlayer.ogg", SOUND_THUNDERPLAYER, 0.4f);
	m_pThunderPlayerEffect = CThunderPlayerEffect::Create(m_pGraphicDev, m_pTarget);

	if (nullptr == m_pThunderPlayerEffect)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_ThunderPlayerEffect", m_pThunderPlayerEffect)))
		return;

	m_pThunderPlayerEffect->SetLayer(m_pLayer);
}

void CThunderCloud::CreateThunderFloorEffect()
{
	m_pThunderFloorEffect = CThunderFloorEffect::Create(m_pGraphicDev, m_pTarget);

	if (nullptr == m_pThunderFloorEffect)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_Thunder", m_pThunderFloorEffect)))
		return;

	m_pThunderFloorEffect->SetLayer(m_pLayer);
}

void CThunderCloud::Free()
{
	CGameObject::Free();
}
