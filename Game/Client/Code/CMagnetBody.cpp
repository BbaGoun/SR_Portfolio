#include "pch.h"
#include "CMagnetBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMagnetTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"

CMagnetBody::CMagnetBody(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMagnetBody::~CMagnetBody()
{
}

HRESULT CMagnetBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pTransformCom->Set_Scale({ 1.5f, 1.5f, 1.5f });

	Engine::CComponent* pComponent = nullptr;

	// 자석
	pComponent = m_pBufferCom = dynamic_cast<CMagnetTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MagnetTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CMagnetBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

}

_int CMagnetBody::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CMagnetBody::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMagnetBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CMagnetBody* CMagnetBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnetBody* pMagnet = new CMagnetBody(pGraphicDev);

	if (FAILED(pMagnet->Ready_GameObject()))
	{
		Safe_Release(pMagnet);
		MSG_BOX("pMagnet Create Failed");
		return nullptr;
	}

	return pMagnet;
}

void CMagnetBody::Free()
{
	CGameObject::Free();
}
