#include "pch.h"
#include "CItemBox.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "COuterBox.h"
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

	m_fTimer	= 0.f;
	m_bShow		= true;
	m_fSpeed	= 5.f;
	m_vForce	= { 0, 1, 0 };

	Set_CollisionLayer(CL_ITEM_BOX);

	return S_OK;
}

void CItemBox::PostReady_GameObject()
{
	m_pBufferCom = Get_Component<COuterBox>();
	m_pTextureCom = Get_Component<CTexture>();
	m_pColliderCom; Get_Component<CSphere_Collider>();

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vOrigin = vPos;
}

void CItemBox::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	float deltaY = vPos.y - m_vOrigin.y;

	if (deltaY > 2.f)
		m_vForce.y = -0.5f;
	else if (deltaY < -2.f)
		m_vForce.y = 0.5f;

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
	CRenderer::GetInstance()->Add_LeftMirrorRenderGroup(RENDER_NONALPHA, this);

	if (m_bShow == false)
		m_fTimer += fDeltaTime;
	if (m_fTimer > 5.f)
	{
		m_fTimer = 0.f;
		SetShow(true);
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
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();
		//m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));
	}
}

void CItemBox::TriggerEnter(CCollider* pOtherCollider)
{
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
