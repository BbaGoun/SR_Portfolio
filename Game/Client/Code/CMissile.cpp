#include "pch.h"
#include "CMissile.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMissileTex.h"
#include "CCollisionMgr.h"
#include "CCube_Collider.h"
#include <CCartBody.h>
#include <CCart.h>
#include "CShield1.h"
#include "CShield2.h"
#include "CCartBot.h"
#include "SoundMgr.h"
#include "CMissileEffect.h"

CMissile::CMissile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMissile::~CMissile()
{
}

HRESULT CMissile::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fSpeed = 180.f;
	m_fAngle = 0.f;

	Set_CollisionLayer(CL_ITEM);

	return S_OK;
}

void CMissile::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CTransform* pTargetTransform = m_pTarget->Get_Transform();
	// 여기선 ID_DYNAMIC이 아닌 ID_STATIC 로 사용
	if (nullptr == pTargetTransform)
		return;

	_vec3 vTargetPos;
	pTargetTransform->Get_Info(INFO_POS, &vTargetPos);

	 _vec3 vMissilePos;
	m_pTransformCom->Get_Info(INFO_POS, &vMissilePos);
	//cout << vMissilePos.x << " | " << vMissilePos.y << " | " << vMissilePos.z << " | " << "\n";

	_vec3 vDir = vTargetPos - vMissilePos;
	_float fDistance = D3DXVec3Length(&vDir);

	_float radius = clampT(fDistance, 0.f, 10.f);

	m_fAngle += D3DXToRadian(720.f) * fFixedDeltaTime;
	m_fAngle = fmodf(m_fAngle, D3DX_PI * 2.f);

	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransformCom->Move_Pos(&vDir, m_fSpeed, fFixedDeltaTime);

	_vec3 vWorldLook = { 0, 0, 1 };

	float dot = D3DXVec3Dot(&vDir, &vWorldLook);
	_vec3 cross;
	D3DXVec3Cross(&cross, &vDir, &vWorldLook);
	float fRadian = atan2f(cross.y, dot);

	_quaternion q;
	D3DXQuaternionRotationYawPitchRoll(&q, fRadian, 0, 0);
	m_pTransformCom->Set_Quaternion(&q);
	
	CGameObject* pBody = m_vecChildren.front();
	_vec3 offset{
		cosf(m_fAngle) * radius,
		sinf(m_fAngle) * radius,
		0.f
	};
	pBody->Get_Transform()->Set_Pos(offset);

	if (fDistance < 3.f) {
		CCartBody* pCartBody = nullptr;
		for (auto& pChild : m_pTarget->Get_Children())
		{
			if (pCartBody = dynamic_cast<CCartBody*>(pChild))
			{
				break;
			}
		}
		if (CCart* pCart = dynamic_cast<CCart*>(m_pTarget))
		{
			CShield1* pShield = static_cast<CShield1*>(pCart->GetShield1());
			CShield2* pShield2 = static_cast<CShield2*>(pCart->GetShield2());
			if (pShield->GetShow() || pShield2->GetShow())
			{
				pShield->SetShow(false);
				pShield2->SetShow(true);
			}
			else
			{
				SoundMgr::GetInstance().PlaySound(L"Effect/Item_rocket/exploding.ogg", SOUND_MISSILE, 0.4f);
				static_cast<CCartBody*>(pCartBody)->SetMissileHit(true);
				pCartBody->Set_Force({ 0,20,0 });
				pCart->Set_Force({ 0,0,0 });
				pCart->SetMissileHit(true);

				CGameObject* pMissileEffect = CMissileEffect::Create(m_pGraphicDev);

				if (pMissileEffect == nullptr)
					return;
				if (FAILED(m_pLayer->Add_GameObject(L"Obj_MissileEffect", pMissileEffect)))
					return;

				_vec3 vCartPos;
				pCart->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
				pMissileEffect->Get_Transform()->Set_Pos(vCartPos);
			}
		}
		else if (CCartBot* pCartBot = dynamic_cast<CCartBot*>(m_pTarget))
		{
			CShield1* pShield = static_cast<CShield1*>(pCartBot->GetShield1());
			CShield2* pShield2 = static_cast<CShield2*>(pCartBot->GetShield2());
			if (pShield->GetShow() || pShield2->GetShow())
			{
				pShield->SetShow(false);
				pShield2->SetShow(true);
			}
			else
			{
				SoundMgr::GetInstance().PlaySound(L"Effect/Item_rocket/exploding.ogg", SOUND_MISSILE, 0.4f);
				static_cast<CCartBody*>(pCartBody)->SetMissileHit(true);
				pCartBody->Set_Force({ 0,20,0 });
				pCartBot->Set_Force({ 0,0,0 });
				pCartBot->SetMissileHit(true);

				CGameObject* pMissileEffect = CMissileEffect::Create(m_pGraphicDev);

				if (pMissileEffect == nullptr)
					return;
				if (FAILED(m_pLayer->Add_GameObject(L"Obj_MissileEffect", pMissileEffect)))
					return;

				_vec3 vCartBotPos;
				pCartBot->Get_Transform()->Get_Info(INFO_POS, &vCartBotPos);
				pMissileEffect->Get_Transform()->Set_Pos(vCartBotPos);
			}
		}
		m_pLayer->Delete_GameObject(this);
		m_pLayer->Delete_GameObject(m_pSmoke);
	}
}

_int CMissile::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CMissile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CMissile::Render_GameObject()
{
}

void CMissile::CollisionEnter(CCollider* pOtherCollider)
{

}

void CMissile::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();
}

CMissile* CMissile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissile* pMissile = new CMissile(pGraphicDev);

	if (FAILED(pMissile->Ready_GameObject()))
	{
		Safe_Release(pMissile);
		MSG_BOX("pMissile Create Failed");
		return nullptr;
	}

	return pMissile;
}

void CMissile::Free()
{

	CGameObject::Free();
}
