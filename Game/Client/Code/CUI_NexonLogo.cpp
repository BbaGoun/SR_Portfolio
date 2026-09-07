#include "pch.h"
#include "CUI_NexonLogo.h"

#include "CScene.h"
#include "CCollisionTest.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include <SoundMgr.h>


CUI_NexonLogo::CUI_NexonLogo(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_NexonLogo::CUI_NexonLogo(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_NexonLogo::~CUI_NexonLogo()
{
}

HRESULT CUI_NexonLogo::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);
	m_pTransformCom->Set_Pos({ 0, 20, 10 });
	m_pTransformCom->Set_Scale({ float(vp.Width), float(vp.Height), 1 });

	CComponent* pComponent = nullptr;


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_UI_NexonLogo"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fFrame = 0;

	SoundMgr::GetInstance().PlaySound(L"Effect/Nexon Logo Animation_[cut_5sec].mp3", SOUND_EFFECT1, 0.4f);
	return S_OK;
}


void CUI_NexonLogo::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

}

_int CUI_NexonLogo::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHAUI, this);

	m_fFrame += 15.f * fDeltaTime;


	if (m_fFrame >= m_fMaxFrame)
	{
		m_fFrame = m_fMaxFrame - 1.f;
		m_bVideoFinished = true;
	}
	

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_NexonLogo::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_NexonLogo::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

CUI_NexonLogo* CUI_NexonLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_NexonLogo* pObj = new CUI_NexonLogo(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_NexonLogo Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_NexonLogo::Free()
{
	CGameObject::Free();
	
}
