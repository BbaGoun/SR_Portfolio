#include "pch.h"
#include "CLand.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"

CLand::CLand(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CLand::CLand(const CGameObject& rhs) : CGameObject(rhs)
{
}

CLand::~CLand()
{
}

HRESULT CLand::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CTerrain*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Terrain"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_TerrainTexture"));
	pComponent->Set_Owner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

_int CLand::Update_GameObject(const _float& fTimeDelta)
{
	return CGameObject::Update_GameObject(fTimeDelta);
}

void CLand::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CLand::Render_GameObject()
{
	D3DXMATRIX* matWorld;

	matWorld = m_pTransformCom->Get_World();

	m_pTextureCom->Set_Texture(0);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	m_pBufferCom->Render_Buffer();
}

CLand* CLand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLand* pObj = new CLand(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Land Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CLand::Free()
{
	CGameObject::Free();
}
