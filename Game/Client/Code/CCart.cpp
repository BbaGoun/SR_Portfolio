#include "pch.h"
#include "CCart.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CCart::CCart(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCart::CCart(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCart::~CCart()
{
}

HRESULT CCart::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_fSpeed = 0.f;
	m_fMaxSpeed = 3.f;
	return S_OK;
}

void CCart::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

	//m_pTransformCom->Rotate(QUATER_YAW, m_vRotation.y);
	
	////////////////////방향////////////////////////
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, 0.f, 0.f);
	m_pTransformCom->Set_Quaternion(&q);

	//m_vRotation *= 0.98;
	//_float fRotationLength = D3DXVec3Length(&m_vRotation);
	//if (fRotationLength < 0.1f) m_vRotation *= 0;

	////////////////////이동////////////////////////
	m_fSpeed *= 0.98;
	if (fabsf(m_fSpeed) < 0.1f)
		m_fSpeed = 0;
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	if(m_fSpeed > 0.1f)
		m_pTransformCom->Move_Pos(&vLook, m_fSpeed, fFixedDeltaTime);
	else
		m_pTransformCom->Move_Pos(&vLook, m_fSpeed, fFixedDeltaTime);

}

_int CCart::Update_GameObject(const _float& fDeltaTime)
{
	KeyInput(fDeltaTime);
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCart::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}


CCart* CCart::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCart* pObj = new CCart(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Cart Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCart::KeyInput(const _float& fDeltaTime)
{
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_UP))
		m_fSpeed += 1;
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_DOWN))
		m_fSpeed -= 1;

	if (fabsf(m_fSpeed) > 0.1f)
	{
		if (m_fSpeed > 0)
		{
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
				m_vRotation.y += D3DXToRadian(-0.5);
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
				m_vRotation.y += D3DXToRadian(0.5);
		}
		else
		{
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
				m_vRotation.y += D3DXToRadian(0.5);
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
				m_vRotation.y += D3DXToRadian(-0.5);
		}
	}
}

void CCart::Free()
{
	CGameObject::Free();
}
