#include "pch.h"
#include "CHUD_Num.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"

CHUD_Num::CHUD_Num(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CHUD_Num::CHUD_Num(const CGameObject& rhs) : CGameObject(rhs)
{
}

CHUD_Num::~CHUD_Num()
{
}

HRESULT CHUD_Num::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 34, 45, 1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_HUDNum"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fThirdFrame	= 0;
	m_fSecondFrame	= 0;
	m_fFirstFrame	= 0;

	return S_OK;
}

_int CHUD_Num::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"GameLogic", L"Obj_Cart", L"Com_Transform");
	CGameObject* pCart = pCom->Get_Owner();

	_vec3 vCartForce = pCart->Get_Force();
	m_fCartSpeed = pCart->Get_Speed() * D3DXVec3Length(&vCartForce);

	m_fThirdFrame	= int(m_fCartSpeed) % 10;
	m_fCartSpeed /= 10;
	m_fSecondFrame	= int(m_fCartSpeed) % 10;
	m_fCartSpeed /= 10;
	m_fFirstFrame	= int(m_fCartSpeed) % 10;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CHUD_Num::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CHUD_Num::Render_GameObject()
{
	m_pTransformCom->Set_Pos({ -35,-328,1 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFirstFrame);

	m_pBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ 0,-328,1 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fSecondFrame);

	m_pBufferCom->Render_Buffer();

	m_pTransformCom->Set_Pos({ 35,-328,1 });
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fThirdFrame);

	m_pBufferCom->Render_Buffer();
}

CHUD_Num* CHUD_Num::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHUD_Num* pObj = new CHUD_Num(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CHUD_Num Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CHUD_Num::Free()
{
	CGameObject::Free();
}
