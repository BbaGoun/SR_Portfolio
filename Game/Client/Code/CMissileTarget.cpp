#include "pch.h"
#include "CMissileTarget.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCollisionMgr.h"
#include "CManagement.h"
#include "CCube_Collider.h"	

CMissileTarget::CMissileTarget(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMissileTarget::~CMissileTarget()
{
}

HRESULT CMissileTarget::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fSpeed = 0.f;
	m_fMaxSpeed = 3.f;
	m_pTransformCom->Set_Pos({ 0,0,100.f });

	m_bMissileHit = false;

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(false);
	m_pColliderCom->Set_Extents({ 2.5f, 1.5f, 5.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CMissileTarget::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (m_bMissileHit == false)
		return;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vPos.y += m_vForce.y * fFixedDeltaTime;
	m_vForce.y -= 30.f * fFixedDeltaTime;

	m_vRotation.x += D3DXToRadian(350.f) * fFixedDeltaTime;

	if (m_vRotation.x >= D3DXToRadian(1080.f))
	{
		m_vRotation.x = D3DXToRadian(1080.f);
	}

	_quaternion q;
	D3DXQuaternionRotationYawPitchRoll(&q, 0.f, m_vRotation.x, 0.f);
	m_pTransformCom->Set_Quaternion(&q);

	if (vPos.y <= 0.f)
	{
		vPos.y = 0.f;
		m_vForce.y = 0.f;
		m_vRotation.x = 0.f;
		m_bMissileHit = false;

		_quaternion qReset;
		D3DXQuaternionRotationYawPitchRoll(&qReset, 0.f, 0.f, 0.f);
		m_pTransformCom->Set_Quaternion(&qReset);
	}

	m_pTransformCom->Set_Pos(vPos);
}

_int CMissileTarget::Update_GameObject(const _float& fDeltaTime)
{

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMissileTarget::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMissileTarget::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

void CMissileTarget::CollisionEnter(CCollider* pOtherCollider)
{
}

void CMissileTarget::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcsncmp(wOtherTag, L"Obj_Missile", 11) == 0)
	{
		if (m_bMissileHit == false)
		{
			m_bMissileHit = true;
			m_vForce.y = 30.f;
			m_vRotation.x += D3DXToRadian(0.f);
		}
	}
}

CMissileTarget* CMissileTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissileTarget* pObj = new CMissileTarget(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMissileTarget_FL Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CMissileTarget::Free()
{
	CGameObject::Free();
}
