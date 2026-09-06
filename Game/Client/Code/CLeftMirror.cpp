#include "pch.h"
#include "CLeftMirror.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"


CLeftMirror::CLeftMirror(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CLeftMirror::CLeftMirror(const CLeftMirror& rhs)
	:CGameObject(rhs)
{
}

CLeftMirror::~CLeftMirror()
{
}

HRESULT CLeftMirror::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pTransformCom->Set_Pos({ 500,0,1 });
	m_pTransformCom->Set_Scale({ 250,300,1 });


	//m_pTransformCom->Set_Pos({ 0,0,1 });
	//m_pTransformCom->Set_Scale({ WINCX,WINCY,1 });


	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	return S_OK;
}

void CLeftMirror::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

}

_int CLeftMirror::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHAUI, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CLeftMirror::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CLeftMirror::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	if (CRenderer::GetInstance()->Find_RenderTarget(L"LeftMirror"))
		m_pGraphicDev->SetTexture(0, CRenderer::GetInstance()->Find_RenderTarget(L"LeftMirror")->pRTTexture);
	m_pVIBufferCom->Render_Buffer();
}

CLeftMirror* CLeftMirror::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLeftMirror* pObj = new CLeftMirror(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CLeftMirror Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CLeftMirror::Free()
{
	CGameObject::Free();
}