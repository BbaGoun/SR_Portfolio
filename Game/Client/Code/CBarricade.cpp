#include "pch.h"
#include "CBarricade.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CTrackMgr.h"
#include <SoundMgr.h>

CBarricade::CBarricade(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CBarricade::CBarricade(const CGameObject& rhs) : CGameObject(rhs)
{
}

CBarricade::~CBarricade()
{
}

HRESULT CBarricade::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Scale({ 10.f,5.f,2.f });
	pComponent = m_pBufferCom = static_cast<CCube*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Cube"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Barricade"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(false);
	m_pColliderCom->Set_Extents({ 5.f,2.5f,1.f });
	m_mapComponent.insert({ L"Com_Collider", pComponent });

	m_fTimer = 0;

	Set_CollisionLayer(CL_ITEM);

	return S_OK;
}

void CBarricade::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	float fHeight = clampT(m_fTimer * 6.f, 0.f, 3.f);
	_vec3 newPos = m_vOriginPos + _vec3{0, fHeight, 0};

	m_pTransformCom->Set_Pos(newPos);
}

_int CBarricade::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	m_fTimer += fDeltaTime;

	if (m_bFirst && m_fTimer > 0.5f) {
		m_bFirst = false;
		SoundMgr::GetInstance().PlaySound(L"Effect/Item_Barricade/장애물 등장.ogg", SOUND_BARRICATE, 0.4f);
	}

	if (m_fTimer > 5)
	{
		m_pLayer->Delete_GameObject(this);
	}
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CBarricade::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBarricade::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
}

void CBarricade::CollisionEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcscmp(wOtherTag, L"Obj_CartBody") == 0 || wcscmp(wOtherTag, L"Obj_CartBotBody") == 0)
	{
		CTrackMgr::GetInstance()->Delete_Hazard(this);
		m_pLayer->Delete_GameObject(this);
	}
}

void CBarricade::TriggerEnter(CCollider* pOtherCollider)
{
	
}

CBarricade* CBarricade::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBarricade* pObj = new CBarricade(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBarricade Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CBarricade::Free()
{
	CGameObject::Free();
}
