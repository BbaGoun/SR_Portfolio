#include "pch.h"
#include "CCodyBody.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"
#include "CRenderer.h"

CCodyBody::CCodyBody(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CCodyBody::CCodyBody(const CGameObject& rhs) : CGameObject(rhs)
{
}

CCodyBody::~CCodyBody()
{
}

HRESULT CCodyBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 0.05f, 0.05f, 0.05f });

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CCody*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Cody"));
	pComponent->Set_Owner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

_int CCodyBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCodyBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CCodyBody::Render_GameObject()
{
	_matrix* matWorld;

	matWorld = m_pTransformCom->Get_World();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	m_pBufferCom->Render_Buffer();
}

CCodyBody* CCodyBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCodyBody* pObj = new CCodyBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCodyBody Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CCodyBody::Free()
{
	CGameObject::Free();
}
