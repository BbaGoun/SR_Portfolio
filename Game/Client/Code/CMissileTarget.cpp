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
	//m_pTransformCom->Set_Pos({ 0.f,0.f, 170.f });
	m_pTransformCom->Set_Pos({ 0.f,0.f, 60.f });
	m_pTransformCom->Set_Scale({ 1.5f, 1.5f, 1.f });
	m_fTimer			= 0.f;

	m_bMissileHit		= false;
	m_bWaterBombHit		= false;
	m_bWaterFlyHit		= false;
	m_bWaterBubble		= false;
	m_bWaterFly			= false;

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
	m_pColliderCom->SetIsTrigger(true);
	m_pColliderCom->Set_Extents({ 2.5f, 1.5f, 3.f });

	m_mapComponent.insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CMissileTarget::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	if (m_bMissileHit == false && m_bWaterBombHit == false && m_bWaterFlyHit == false)
		return;

	if (m_bMissileHit)
	{
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

	if (m_bWaterBombHit)
	{
		m_fTimer += fFixedDeltaTime;
		CGameObject* pBubble = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_WaterBombBubble");

		_vec3 vPos, vBubblePos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		if (m_fTimer < 0.08f)
		{
			vPos.y += m_vForce.y * fFixedDeltaTime;
		}	

		//else if (m_fTimer < 0.08f)
		//{
		//	m_vForce.y = 0;
		//}

		else if (m_fTimer > 0.15f && m_fTimer < 0.40f)
		{
			m_vRotation.x -= D3DXToRadian(100.f) * fFixedDeltaTime;
			m_vRotation.y += D3DXToRadian(100.f) * fFixedDeltaTime;

			if (m_vRotation.x <= D3DXToRadian(-25.f))
			{
				m_vRotation.x = D3DXToRadian(-25.f);
			}

			if (m_vRotation.y >= D3DXToRadian(25.f))
			{
				m_vRotation.y = D3DXToRadian(25.f);
			}
		}
			
		else if (m_fTimer > 1.0f && m_fTimer < 2.f)
		{
			m_vRotation.x += D3DXToRadian(5.f) * fFixedDeltaTime;
			m_vRotation.y -= D3DXToRadian(5.f) * fFixedDeltaTime;

			if (m_vRotation.x >= D3DXToRadian(-20.f))
			{
				m_vRotation.x = D3DXToRadian(-20.f);
			}

			if (m_vRotation.y <= D3DXToRadian(20.f))
			{
				m_vRotation.y = D3DXToRadian(20.f);
			}

		}

		else if (m_fTimer >= 2.f)
		{
			vPos.y -= m_vForce.y * 0.45f * fFixedDeltaTime;
			m_vForce.y -= 5.f * fFixedDeltaTime;
		}

		_quaternion q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, 0.f);
		m_pTransformCom->Set_Quaternion(&q);

		if (vPos.y <= 0.f)
		{
			vPos.y = 0.f;
			m_vForce.y = 0.f;
			m_vRotation.x = 0.f;
			m_vRotation.y = 0.f;
			m_bWaterBombHit = false;
			m_bWaterBubble	= false;
			pBubble->GetLayer()->Delete_GameObject(pBubble);
			m_fTimer = 0.f;

			_quaternion qReset;
			D3DXQuaternionRotationYawPitchRoll(&qReset, 0.f, 0.f, 0.f);
			m_pTransformCom->Set_Quaternion(&qReset);
		}

		m_pTransformCom->Set_Pos(vPos);	
		pBubble->Get_Transform()->Set_Pos(vPos);
		//pBubble->GetLayer()->Delete_GameObject(pBubble);
	}

	if (m_bWaterFlyHit)
	{
		m_fTimer += fFixedDeltaTime;
		CGameObject* pBubble = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_WaterBombBubble");

		_vec3 vPos, vFlyPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		if (m_fTimer < 0.08f)
		{
			vPos.y += m_vForce.y * fFixedDeltaTime;
		}

		//else if (m_fTimer < 0.08f)
		//{
		//	m_vForce.y = 0;
		//}

		else if (m_fTimer > 0.15f && m_fTimer < 0.40f)
		{
			m_vRotation.x -= D3DXToRadian(100.f) * fFixedDeltaTime;
			m_vRotation.y += D3DXToRadian(100.f) * fFixedDeltaTime;

			if (m_vRotation.x <= D3DXToRadian(-25.f))
			{
				m_vRotation.x = D3DXToRadian(-25.f);
			}

			if (m_vRotation.y >= D3DXToRadian(25.f))
			{
				m_vRotation.y = D3DXToRadian(25.f);
			}
		}

		else if (m_fTimer > 1.0f && m_fTimer < 2.f)
		{
			m_vRotation.x += D3DXToRadian(5.f) * fFixedDeltaTime;
			m_vRotation.y -= D3DXToRadian(5.f) * fFixedDeltaTime;

			if (m_vRotation.x >= D3DXToRadian(-20.f))
			{
				m_vRotation.x = D3DXToRadian(-20.f);
			}

			if (m_vRotation.y <= D3DXToRadian(20.f))
			{
				m_vRotation.y = D3DXToRadian(20.f);
			}

		}

		else if (m_fTimer >= 2.f)
		{
			vPos.y -= m_vForce.y * 0.45f * fFixedDeltaTime;
			m_vForce.y -= 5.f * fFixedDeltaTime;
		}

		_quaternion q;
		D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, 0.f);
		m_pTransformCom->Set_Quaternion(&q);

		if (vPos.y <= 0.f)
		{
			vPos.y = 0.f;
			m_vForce.y = 0.f;
			m_vRotation.x = 0.f;
			m_vRotation.y = 0.f;
			m_bWaterFlyHit = false;
			m_bWaterBubble = false;
			pBubble->GetLayer()->Delete_GameObject(pBubble);
			m_fTimer = 0.f;

			_quaternion qReset;
			D3DXQuaternionRotationYawPitchRoll(&qReset, 0.f, 0.f, 0.f);
			m_pTransformCom->Set_Quaternion(&qReset);
		}

		m_pTransformCom->Set_Pos(vPos);
		pBubble->Get_Transform()->Set_Pos(vPos);
	}
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

	//if (wcsncmp(wOtherTag, L"Obj_WaterBomb", 13) == 0)
	if (wcscmp(wOtherTag, L"Obj_WaterBomb") == 0)
	{
		if (m_bWaterBombHit == false && m_bWaterBubble == false)
		{
			m_bWaterBombHit = true;
			m_bWaterBubble	= true;
			m_vForce.y = 120.f;
			m_vRotation.x += D3DXToRadian(0.f);
			m_vRotation.y += D3DXToRadian(0.f);
		}
	}

	if (wcscmp(wOtherTag, L"Obj_WaterFly") == 0)
	{
		if (m_bWaterFlyHit == false && m_bWaterBubble == false)
		{
			m_bWaterFlyHit = true;
			m_bWaterBubble = true;
			m_vForce.y = 120.f;
			m_vRotation.x += D3DXToRadian(0.f);
			m_vRotation.y += D3DXToRadian(0.f);
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
