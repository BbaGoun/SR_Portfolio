#include "pch.h"
#include "CItemBox.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CInnerBox.h"
#include "CCart.h"
#include "CManagement.h"

CItemBox::CItemBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CItemBox::CItemBox(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CItemBox::~CItemBox()
{
}

HRESULT CItemBox::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	Engine::CComponent* pComponent = nullptr;

	//m_pTransformCom->Set_Pos({-200,0,-200});
	m_pTransformCom->Set_Scale({ 5,5,5 });

	pComponent = m_pBufferCom = dynamic_cast<CInnerBox*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_InnerBox"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });


	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 5,5,5 });

	m_mapComponent.insert({ L"Com_Collider", pComponent });


	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_V_ItemBoxTexture"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fTimer	= 0.f;
	m_bShow		= true;
	m_fSpeed	= 5.f;

	return S_OK;
}

void CItemBox::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (vPos.y < 5)
		m_vForce.y = 1;
	else if (vPos.y > 10)
		m_vForce.y = -1;

	m_vRotation.y += 0.5f * fFixedDeltaTime;
	m_vRotation.x += 0.5f * fFixedDeltaTime; 
	m_vRotation.z += 0.5f * fFixedDeltaTime;

	m_pTransformCom->Move_Pos(&m_vForce, m_fSpeed, fFixedDeltaTime);
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
	m_pTransformCom->Set_Quaternion(&q);
}

_int CItemBox::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

	if (m_bShow == false)
		m_fTimer += fDeltaTime;
	if (m_fTimer > 5.f)
	{
		m_fTimer = 0.f;
		m_bShow = true;
	}
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CItemBox::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CItemBox::Render_GameObject()
{
	if (m_bShow)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();
		m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

void CItemBox::TriggerEnter(CCollider* pOtherCollider)
{
	//const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();
	//if (wcsncmp(wOtherTag, L"Obj_CartBody", 12) == 0)
	//{
	//	m_bShow = false;
	//}
}
CItemBox* CItemBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItemBox* pObj = new CItemBox(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CItemBox Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CItemBox::Free()
{
	CGameObject::Free();
}
