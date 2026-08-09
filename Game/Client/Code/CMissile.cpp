#include "pch.h"
#include "CMissile.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMissileTex.h"

CMissile::CMissile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMissile::~CMissile()
{
}

HRESULT CMissile::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	// m_pTransformCom->m_vScale = { 0.2f, 0.2f, 0.3f };
	// m_pTransformCom->m_vInfo[INFO_POS] = { 3.f, 1.3f, 0.f };

	m_pTransformCom->Set_Pos({ 3.f, 1.3f, 0.f });
	Engine::CComponent* pComponent = nullptr;

	// 미사일
	pComponent = m_pBufferCom = dynamic_cast<CMissileTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MissileTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CMissile::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{



}

_int CMissile::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);	// 그래서 일반 도형은 RENDER_NONALPHA

	return iExit;
}

void CMissile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CMissile::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CMissile* CMissile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissile* pMissile = new CMissile(pGraphicDev);

	if (FAILED(pMissile->Ready_GameObject()))
	{
		Safe_Release(pMissile);
		MSG_BOX("pMissile Create Failed");
		return nullptr;
	}

	return pMissile;
}

void CMissile::Free()
{

	CGameObject::Free();
}
