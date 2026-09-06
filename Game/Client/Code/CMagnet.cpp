#include "pch.h"
#include "CMagnet.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CMagnetTex.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"
#include <CCartBot.h>
#include <CMagnetBody.h>

CMagnet::CMagnet(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CMagnet::~CMagnet()
{
}

HRESULT CMagnet::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	// y축 180도 돌리기
	m_fTimer = 0;

	return S_OK;
}

void CMagnet::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CGameObject* pTarget = nullptr;

	if (CCart* pCart = dynamic_cast<CCart*>(m_pOwner)) {
		pTarget = pCart->GetMagnetTarget();
		if (!pTarget) {
			m_pLayer->Delete_GameObject(this);
			return;
		}
	}
	else if (CCartBot* pCartBot = dynamic_cast<CCartBot*>(m_pOwner))
	{
		//pTarget = pCartBot->GetMagnetTarget();
		//if (!pTarget) {
		//	m_pLayer->Delete_GameObject(this);
		//	return 0;
		//}
	}

	_vec3 vCartPos, vLook, vTargetPos, vDir;
	_vec3 pos;
	m_pTransformCom->Get_Info(INFO_POS, &pos);

	m_pOwner->Get_Transform()->Get_Info(INFO_POS, &vCartPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

	pTarget->Get_Transform()->Get_Info(INFO_POS, &vTargetPos);

	vDir = vTargetPos - vCartPos;
	vLook.y = 0;
	vDir.y = 0;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Normalize(&vLook, &vLook);
	float dot = D3DXVec3Dot(&vLook, &vDir);

	_vec3 cross;
	D3DXVec3Cross(&cross, &vLook, &vDir);

	float yawError = atan2f(cross.y, dot);

	float maxYawSpeed = D3DXToRadian(180.f); // 초당 90도
	float maxYawStep = maxYawSpeed * fFixedDeltaTime;

	float yawStep =
		clampT(yawError, -maxYawStep, maxYawStep);

	m_pTransformCom->Rotate(QUATER_YAW, D3DXToDegree(yawStep));
	vCartPos.y += 5;
	m_pTransformCom->Set_Pos(vCartPos);

	m_fTimer += fFixedDeltaTime;

	if (m_fTimer > 3.5f)
	{
		m_pLayer->Delete_GameObject(this);
	}
}

_int CMagnet::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMagnet::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMagnet::Render_GameObject()
{

}

void CMagnet::CollisionEnter(CCollider* pOtherCollider)
{
}

void CMagnet::TriggerEnter(CCollider* pOtherCollider)
{
}

CMagnet* CMagnet::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* pOwner)
{
	CMagnet* pObj = new CMagnet(pGraphicDev);
	pObj->m_pOwner = pOwner;

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMagnet Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CMagnet::Free()
{
	CGameObject::Free();
}
