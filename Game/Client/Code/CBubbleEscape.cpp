#include "pch.h"
#include "CBubbleEscape.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"
#include "SoundMgr.h"
#include "CMissileTarget.h"

CBubbleEscape::CBubbleEscape(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CBubbleEscape::CBubbleEscape(const CGameObject& rhs) : CGameObject(rhs)
{
}

CBubbleEscape::~CBubbleEscape()
{
}

HRESULT CBubbleEscape::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Pos({ 0,15,1 });
	m_pTransformCom->Set_Scale({ 200, 160, 1 });


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RainBow_NonAlpha"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fTimer = 0;
	m_bCurState = false;

	return S_OK;
}

_int CBubbleEscape::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CMissileTarget* pTarget4 = dynamic_cast<CMissileTarget*>(CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_MissileTarget4"));


	if (pTarget4 != nullptr && pTarget4->GetBubbleUI())
		m_bCurState = true;

	else
		m_bCurState = false;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CBubbleEscape::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBubbleEscape::Render_GameObject()
{
	if (m_bCurState == true)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();
	}
}

CBubbleEscape* CBubbleEscape::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBubbleEscape* pObj = new CBubbleEscape(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBubbleEscape Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CBubbleEscape::Free()
{
	CGameObject::Free();
}
