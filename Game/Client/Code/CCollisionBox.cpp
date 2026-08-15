#include "pch.h"
#include "CCollisionBox.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CInnerBox.h"
CCollisionBox::CCollisionBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCollisionBox::CCollisionBox(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCollisionBox::~CCollisionBox()
{
}

HRESULT CCollisionBox::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 10,10,10 });
	
	pComponent = m_pBufferCom = dynamic_cast<CInnerBox*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_InnerBox"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	
	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(false);
	m_pColliderCom->Set_Extents({ 10,10,10 });
	m_pColliderCom->SetColliderType(CUBE_COLLIDER);
	
	m_mapComponent.insert({ L"Com_Collider", pComponent });


	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_V_ItemBoxTexture"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });


	return S_OK;
}

void CCollisionBox::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CCollisionBox::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCollisionBox::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CCollisionBox::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

CCollisionBox* CCollisionBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollisionBox* pObj = new CCollisionBox(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCollisionBox Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCollisionBox::Free()
{
	CGameObject::Free();
}
