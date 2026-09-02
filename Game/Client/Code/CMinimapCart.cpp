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

	m_pTransformCom->Set_Scale({ 3,1,3 });

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
	CRenderer::GetInstance()->Add_RenderTargetGroup(L"Minimap", this);

	CGameObject* pCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");
	_vec3 vPos;
	pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);
	m_vRotation = pCart->Get_Rotation();
	D3DXQUATERNION q=pCart->Get_Transform()->Get_WorldQuaternion();
	m_pTransformCom->Set_Quaternion(&q);

	vPos.y = 0.5f;
	cout << "MinmapCartY: " << vPos.y << endl;
	
	m_pTransformCom->Set_Pos(vPos);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMinimapCart::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMinimapCart::Render_GameObject()
{
	_matrix matOldView, matMinimapView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);

	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, nullptr, &matOldView);

	_vec3 vEye = { matCamWorld._41, matCamWorld._42, matCamWorld._43 };
	_vec3 vLook = { matCamWorld._31, matCamWorld._32, matCamWorld._33 };

	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vNewEye = { vEye.x, 8.5f, vEye.z };
	_vec3 vAt = vNewEye + vLook * 15;

	_matrix matView;
	D3DXMatrixLookAtLH(&matMinimapView, &vNewEye, &vAt, &vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matMinimapView);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetTexture(0, nullptr);
	m_pVIBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
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