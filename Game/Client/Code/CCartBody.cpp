#include "pch.h"
#include "CCartBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CLand3.h"
#include "CManagement.h"
#include "CCollisionMgr.h"
CCartBody::CCartBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCartBody::CCartBody(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCartBody::~CCartBody()
{
}

HRESULT CCartBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0,2,3 });
	m_bBananaSpinState = false;


	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(false);
	m_pColliderCom->Set_Extents({ 2.5f,1.5f,5.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });
	return S_OK;
}

void CCartBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	BananaSpin(fFixedDeltaTime); 
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(m_vRotation.y), D3DXToRadian(m_vRotation.x), D3DXToRadian(m_vRotation.z));
	m_pTransformCom->Set_Quaternion(&q);
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
}

_int CCartBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCartBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);	
}

void CCartBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));
}

void CCartBody::CollisionEnter(CCollider* pOtherCollider)
{
	const _tchar* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcsncmp(wOtherTag, L"Obj_CollisionBox", 16) == 0)
	{
		CCollisionMgr::GetInstance()->PysicalCubevsCube(
			static_cast<CCube_Collider*>(pOtherCollider), m_pColliderCom);
		CCart* pCart = dynamic_cast<CCart*>(m_pParent);
		pCart->SetGainGage(0.f);	// m_fGainGage = 0.f;
		pCart->SetDrift(false);		// m_bDrift = false;
	}
}

void CCartBody::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();
	CCart* pCart = dynamic_cast<CCart*>(m_pParent);
	if (wcsncmp(wOtherTag, L"Rainbow_Cloud", 13) == 0)
	{
		if (pCart->GetRainbowUI() == false)
			pCart->SetRainbowUI(true);
	}

	if (wcsncmp(wOtherTag, L"Obj_Banana", 10) == 0)
	{
		if (pCart->GetBanana() == false)
		{
			pCart->SetBanana(true);
			pCart->SetBoost(false);
		}
	}
}
void CCartBody::BananaSpin(const _float& fDeltaTime)
{
	CCart* pCart = dynamic_cast<CCart*>(m_pParent);
	bool bCartBananaSpin = pCart->GetBanana();
	if (bCartBananaSpin == false)
		return;
	if (m_bBananaSpinState == false && bCartBananaSpin == true)
	{
		_vec3 vCartForce = pCart->Get_Force();
		m_fSpinSpeed = pCart->Get_Speed() * D3DXVec3Length(&vCartForce);
		m_fSpinSpeed = m_fSpinSpeed / 50 + 2;
		if (m_fSpinSpeed <= 1)m_fSpinSpeed++;
	}
	m_fSpinSpeed *= 0.98;
	m_bBananaSpinState = true;
	m_vRotation.y += 300* m_fSpinSpeed * fDeltaTime;
	if (m_vRotation.y > 1080 * m_fSpinSpeed)
	{
		m_vRotation.y = 0;
		pCart->SetBanana(false);
		m_bBananaSpinState = false;
	}
}

CCartBody* CCartBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartBody* pObj = new CCartBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCartBody Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCartBody::Free()
{
	CGameObject::Free();
}
