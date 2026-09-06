#include "pch.h"
#include "CPlayerArm.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CCameraMgr.h"
#include "CPlayTimeMgr.h"
CPlayerArm::CPlayerArm(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPlayerArm::CPlayerArm(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CPlayerArm::~CPlayerArm()
{
}

HRESULT CPlayerArm::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	//Engine::CComponent* pComponent = nullptr;

	m_bBoost = false;
	m_eCartDirType = DIR_FORWARD;

	//m_vRotation.y = -90.f;
	return S_OK;
}

void CPlayerArm::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q,
		D3DXToRadian(m_vRotation.y),
		D3DXToRadian(m_vRotation.x),
		D3DXToRadian(m_vRotation.z));
	m_pTransformCom->Set_Quaternion(&q);
}

_int CPlayerArm::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_LeftMirrorRenderGroup(RENDER_NONALPHA, this);
	if (CPlayTimeMgr::GetInstance()->GetPlayTimer() > CPlayTimeMgr::GetInstance()->GetPlayTimer() + 10.f)
	{
		m_bKeyInput = false;
		m_vRotation.x = 0.f;
		m_vRotation.z = 0.f;
		m_vRotation.y = 0.f;
		return 0;
	}

	if (m_eCartDirType == DIR_FORWARD)
	{
		KeyInput(fDeltaTime);
	}
	else
	{
		// 후진(팔 원위치)
		m_vRotation.x = 0.f;
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CPlayerArm::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CPlayerArm::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

}
void CPlayerArm::KeyInput(const _float& fDeltaTime)
{
	if (m_bKeyInput == false)
		return;
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
	{
		// 고개 왼쪽
		if (m_vRotation.x > -20.f)
			m_vRotation.x -= 50 * fDeltaTime;
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
	{
		// 고개 오른쪽
		if (m_vRotation.x < 20.f)
			m_vRotation.x += 50 * fDeltaTime;
	}
	else
	{
		// 고개 원위치
		if (m_vRotation.x < -1.f)
			m_vRotation.x += 50 * fDeltaTime;
		else if (m_vRotation.x > 1.f)
			m_vRotation.x -= 50 * fDeltaTime;
		else
			m_vRotation.x = 0.f;
	}
}

CPlayerArm* CPlayerArm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerArm* pObj = new CPlayerArm(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CPlayerArm Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CPlayerArm::Free()
{
	CGameObject::Free();
}
