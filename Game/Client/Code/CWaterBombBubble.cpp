#include "pch.h"
#include "CWaterBombBubble.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"

CWaterBombBubble::CWaterBombBubble(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CWaterBombBubble::CWaterBombBubble(const CGameObject& rhs) : CGameObject(rhs)
{
}

CWaterBombBubble::~CWaterBombBubble()
{
}

HRESULT CWaterBombBubble::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer		= 0.f;

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 12.f, 12.f, 12.f });
	pComponent = m_pBufferCom = dynamic_cast<CSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Sphere"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_WaterBombBubble"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CWaterBombBubble::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{	

}

_int CWaterBombBubble::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return 0;
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);


	return CGameObject::Update_GameObject(fDeltaTime);
}

void CWaterBombBubble::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bShow == false)
		return;
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CWaterBombBubble::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

}

CWaterBombBubble* CWaterBombBubble::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterBombBubble* pObj = new CWaterBombBubble(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CWaterBombBubble Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CWaterBombBubble::Free()
{
	CGameObject::Free();
}
