#include "pch.h"
#include "CUI_SideMirrorFrame.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"

CUI_SideMirrorFrame::CUI_SideMirrorFrame(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CUI_SideMirrorFrame::CUI_SideMirrorFrame(const CGameObject& rhs) : CGameObject(rhs)
{
}

CUI_SideMirrorFrame::~CUI_SideMirrorFrame()
{
}

HRESULT CUI_SideMirrorFrame::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Pos({ 0,0,0 });

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SideMirrorFrame"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CUI_SideMirrorFrame::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_LeftMirrorRenderGroup(RENDER_NONALPHAUI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CUI_SideMirrorFrame::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CUI_SideMirrorFrame::Render_GameObject()
{
	m_pTransformCom->Set_Scale({ 256, 384, 1 }); // 뷰포트 크기로 변경
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

CUI_SideMirrorFrame* CUI_SideMirrorFrame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_SideMirrorFrame* pObj = new CUI_SideMirrorFrame(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CUI_SideMirrorFrame Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CUI_SideMirrorFrame::Free()
{
	CGameObject::Free();
}
