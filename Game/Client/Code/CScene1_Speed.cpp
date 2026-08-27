#include "pch.h"
#include "CScene1_Speed.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CScene1_Speed::CScene1_Speed(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CScene1_Speed::CScene1_Speed(const CScene1_Speed& rhs) : CGameObject(rhs)
{
}

CScene1_Speed::~CScene1_Speed()
{
}

HRESULT CScene1_Speed::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_vPos = { -150.f, 0.f, 10.f };
	m_vScale = { 78.f, 88.f, 0.f };

	m_pTransformCom->Set_Pos(m_vPos);
	m_pTransformCom->Set_Scale(m_vScale);

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;
	

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Scene1_Speed"));
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;


	return S_OK;


}

void CScene1_Speed::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

	if (CheckCollisionUI(g_hWnd, m_vPos, m_vScale))
	{
		m_fFrame = 1;

	}

	else
	{
		m_fFrame = 0;
	}

}


_int CScene1_Speed::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CScene1_Speed::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CScene1_Speed::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(m_fFrame);
	m_pVIBufferCom->Render_Buffer();
}




CScene1_Speed* CScene1_Speed::Create(LPDIRECT3DDEVICE9 pGraphicDev)

{
	CScene1_Speed* pObj = new CScene1_Speed(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CScene1_Speed Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CScene1_Speed::Free()
{
	CGameObject::Free();
}
