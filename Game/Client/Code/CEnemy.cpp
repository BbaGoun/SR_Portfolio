#include "pch.h"
#include "CEnemy.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CManagement.h"

CEnemy::CEnemy(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CEnemy::CEnemy(const CGameObject& rhs) : CGameObject(rhs)
{
}

CEnemy::~CEnemy()
{
}

HRESULT CEnemy::Ready_GameObject()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CPyramid*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Pyramid"));
	pComponent->SetOwner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_DogTexture"));
	pComponent->SetOwner(this);

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Transform");
	pComponent->SetOwner(this);
	m_pTransformComp = static_cast<CTransform*>(pComponent);
	m_pTransformComp->m_qRotation = { sinf(D3DXToRadian(90) / (float)2), 0, 0, cosf(D3DXToRadian(90) / (float)2) };

	m_mapComponent[ID_STATIC].insert({ L"Com_Transform", pComponent });

	m_pTransformComp->m_vScale = { 1, 1, 1 };

	return S_OK;
}

_int CEnemy::Update_GameObject(const _float& fTimeDelta)
{
	return CGameObject::Update_GameObject(fTimeDelta);
}

void CEnemy::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Follow(fTimeDelta);
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEnemy::Render_GameObject()
{
	D3DXMATRIX *matWorld;

	matWorld = m_pTransformComp->Get_World();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEnemy::Follow(const _float& fTimeDelta)
{
	_vec3 playerPos;
	static_cast<CTransform*>(
		CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameObject", L"Obj_Player", L"Com_Transform")
	)->Get_Info(INFO_POS, &playerPos);

	m_pTransformComp->FollowObj(&playerPos, 7.5, fTimeDelta);
}

CEnemy* CEnemy::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEnemy* pObj = new CEnemy(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CEnemy Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CEnemy::Free()
{
	CGameObject::Free();
}
