#include "pch.h"
#include "CScene3_ColorSetBG.h"

#include "CScene.h"
#include "CCollisionTest.h"

#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"

#include "CScene3_ColorBtn.h"

CScene3_ColorSetBG::CScene3_ColorSetBG(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene3_ColorSetBG::CScene3_ColorSetBG(const CGameObject& rhs) : CGameObject(rhs)
{
}

CScene3_ColorSetBG::~CScene3_ColorSetBG()
{
}

HRESULT CScene3_ColorSetBG::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ -300, -50, 5 });
	m_pTransformCom->Set_Scale({ 156,108,1 });
	CComponent* pComponent = nullptr;


	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene3_ColorSetBG"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	Set_Show(false);

	return S_OK;
}


void CScene3_ColorSetBG::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

}

_int CScene3_ColorSetBG::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return 0;
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	


	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene3_ColorSetBG::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return ;

	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene3_ColorSetBG::Render_GameObject()
{
	if (m_bShow == false)
		return ;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

}

CScene3_ColorSetBG* CScene3_ColorSetBG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene3_ColorSetBG* pObj = new CScene3_ColorSetBG(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene3_ColorSetBG Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CScene3_ColorSetBG::Free()
{
	CGameObject::Free();
}
