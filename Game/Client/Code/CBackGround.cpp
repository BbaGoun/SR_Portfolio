#include "pch.h"
#include "CBackGround.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"
#include "CRenderer.h"


CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CBackGround::CBackGround(const CGameObject& rhs) : CGameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0.f, 0.f, 100.f });
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_LogoTexture"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CBackGround::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CGameObject::Compute_ViewZ(&vPos);
	return CGameObject::Update_GameObject(fDeltaTime);

}

void CBackGround::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBackGround::Render_GameObject()
{
	m_pTextureCom->Set_Texture(m_eCurrentBackground);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	
	m_pBufferCom->Render_Buffer();
}

void CBackGround::Change_BackgroundTexture(BACKGROUND eID)
{
	m_eCurrentBackground = eID;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround* pObj = new CBackGround(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CBackGround::Free()
{
	CGameObject::Free();
}
