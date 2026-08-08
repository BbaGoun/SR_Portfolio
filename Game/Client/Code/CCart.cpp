#include "pch.h"
#include "CCart.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CCart::CCart(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_eCurCartState(CART_STOP)
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
	m_vForce = { 0,0,0 };
	m_fSpeed = 1.f;
	m_fMaxSpeed = 3.f;
	m_eCurCartState = CART_STOP;
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
	//m_fSpeed *= 0.98;
	//if (fabsf(m_fSpeed) < 0.1f)
	//	m_fSpeed = 0;
	//_vec3 vLook;
	//m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	//m_pTransformCom->Move_Pos(&vLook, m_fSpeed, fFixedDeltaTime);

	m_pTransformCom->Move_Pos(&m_vForce, 1, fFixedDeltaTime);
	
	m_vForce *= 0.98;
	if (D3DXVec3Length(&m_vForce) < 1.f)
		m_vForce *= 0;
}

_int CCart::Update_GameObject(const _float& fDeltaTime)
{
	KeyInput(fDeltaTime);
	UpdateState();
	//COutCartState();
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
	{
		if (m_eCurCartState != CART_DRIFT)
			m_vForce += vLook;
		else
			m_vForce += vLook * 0.8f;
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_DOWN))
	{
		if (m_eCurCartState != CART_DRIFT)
			m_vForce -= vLook;
		else
			m_vForce -= vLook * 0.8f;
	}

	float fForceLength = D3DXVec3Length(&m_vForce);

	if (fForceLength > 0.1f)
	{
		if (D3DXVec3Dot(&m_vForce,&vLook) > 0)	// fForcceLength와 Look의 내적값으로 전진후진 판단
		{
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
			{
				if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
				{
					m_eCurCartState = CART_DRIFT;
					m_vRotation.y += D3DXToRadian(-1.5);
				}
				else
				{
					m_vRotation.y += D3DXToRadian(-0.5);
					_matrix matRot;
					D3DXMatrixRotationY(&matRot, D3DXToRadian(-0.5));
					D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
				}
			}
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
			{
				if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
				{
					m_eCurCartState = CART_DRIFT;
					m_vRotation.y += D3DXToRadian(1.5);
				}
				else
				{
					m_vRotation.y += D3DXToRadian(0.5);
					_matrix matRot;
					D3DXMatrixRotationY(&matRot, D3DXToRadian(0.5));
					D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
				}
			}
		}
		else
		{
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
			{
				if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
					m_vRotation.y += D3DXToRadian(1.5);
				else
				{
					m_vRotation.y += D3DXToRadian(0.5);
					_matrix matRot;
					D3DXMatrixRotationY(&matRot, D3DXToRadian(0.5));
					D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
				}
			}
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
			{
				if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
					m_vRotation.y += D3DXToRadian(-1.5);
				else
				{
					m_vRotation.y += D3DXToRadian(-0.5);
					_matrix matRot;
					D3DXMatrixRotationY(&matRot, D3DXToRadian(-0.5));
					D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
				}
			}
		}
	}
}

void CCart::UpdateState()
{
	if (m_eCurCartState == CART_DRIFT)
	{
		_vec3 vLook, vTempForce;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		vTempForce = m_vForce;

		vLook.y = 0;
		vTempForce.y = 0;

		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&vTempForce, &vTempForce);

		cout << acosf(D3DXVec3Dot(&vLook, &vTempForce)) << endl;
		if (acosf(D3DXVec3Dot(&vLook, &vTempForce)) < 0.3f)
		{
			m_eCurCartState = CART_DRIVE;
			cout << "EndDrift" << endl;
		}
	}
}

void CCart::COutCartState()
{
	switch (m_eCurCartState)
	{
	case CART_STOP:
		cout << "CART_STOP" << endl;
		break;
	case CART_DRIVE:
		cout << "CART_DRIVE" << endl;
		break;
	case CART_DRIFT:
		cout << "CART_DRIFT" << endl;
		break;
	case CART_BOOST:
		cout << "CART_BOOST" << endl;
		break;
	case CART_END:
		break;
	default:
		break;
	}
}

void CCart::Free()
{
	CGameObject::Free();
}
