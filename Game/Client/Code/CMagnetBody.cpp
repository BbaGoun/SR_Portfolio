#include "pch.h"
#include "CMagnetBody.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CMagnetTex.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"
#include <CCartBot.h>

CMagnetBody::CMagnetBody(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CMagnetBody::~CMagnetBody()
{
}

HRESULT CMagnetBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	CComponent* pComponent = nullptr;

	// 나중에 매시는 에디터로부터 생성

	m_pTransformCom->Set_Scale({ 10, 10, 10 });

	m_pBufferCom = static_cast<CMagnetTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MagnetTex"));
	m_pBufferCom->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", m_pBufferCom });

	m_fTimer = 0;

	return S_OK;
}

void CMagnetBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime) 
{
	CGameObject::FixedUpdate_GameObject(fFixedDeltaTime);
}

_int CMagnetBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMagnetBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMagnetBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//m_pTextureCom->Set_Texture(0);

	//Get_Component<CVIBuffer>()->Render_Buffer();
	m_pBufferCom->Render_Buffer();
	// m_pColliderCom->Render_Component(D3DXCOLOR({ 1,0,0,1 }));
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMagnetBody::CollisionEnter(CCollider* pOtherCollider)
{
}

void CMagnetBody::TriggerEnter(CCollider* pOtherCollider)
{
}

CMagnetBody* CMagnetBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMagnetBody* pObj = new CMagnetBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMagnetBody Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CMagnetBody::Free()
{
	CGameObject::Free();
}
