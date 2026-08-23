#include "pch.h"
#include "CMinimapCart.h"
#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CMinimapCart::CMinimapCart(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMinimapCart::CMinimapCart(const CMinimapCart& rhs)
	:CGameObject(rhs)
{
}

CMinimapCart::~CMinimapCart()
{
}

HRESULT CMinimapCart::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_pTransformCom->Set_Scale({ 5,1,5 });

	Engine::CComponent* pComponent = nullptr;
	pComponent = m_pVIBufferCom = dynamic_cast<CArrowTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ArrowCol"));
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	if (pComponent == nullptr)
		return E_FAIL;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MinimapCart"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });
	return S_OK;
}

void CMinimapCart::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CMinimapCart::Update_GameObject(const _float& fDeltaTime)
{
	//CRenderer::GetInstance()->Add_RenderGroup(RENDER_TARGET, this);
	CRenderer::GetInstance()->Add_RenderTargetGroup(L"Minimap", this);

	CGameObject* pCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");
	_vec3 vPos;
	pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);
	m_vRotation = pCart->Get_Rotation();
	D3DXQUATERNION q=pCart->Get_Transform()->Get_WorldQuaternion();
	m_pTransformCom->Set_Quaternion(&q);
	m_pTransformCom->Set_Pos(vPos);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMinimapCart::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMinimapCart::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetTexture(0, nullptr);
	m_pVIBufferCom->Render_Buffer();
}

CMinimapCart* CMinimapCart::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMinimapCart* pObj = new CMinimapCart(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMinimapCart Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CMinimapCart::Free()
{
	CGameObject::Free();
}