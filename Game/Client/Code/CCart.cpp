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
	m_fSpeed = 2.f;
	return S_OK;
}

void CCart::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	m_pTransformCom->Move_Pos(&m_vForce, m_fSpeed, fFixedDeltaTime);
	m_vForce *= 0.98;
	_float fForceLength = D3DXVec3Length(&m_vForce);
	if (fForceLength < 0.1f) m_vForce *= 0;

	//cout << m_vForce.z << endl;
}

_int CCart::Update_GameObject(const _float& fDeltaTime)
{
	KeyInput(fDeltaTime);
	return _int();
}

void CCart::LateUpdate_GameObject(const _float& fDeltaTime)
{
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
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_UP))
	{
		if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
		{
			if (m_fAngle > 0)
				m_fAngle = 0;
			if (m_fAngle > -45)
				m_fAngle -= 1;
			_matrix matRot;
			D3DXMatrixRotationY(&matRot, D3DXToRadian(m_fAngle));
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			cout << vLook.x << "\t" << vLook.y << "\t" << vLook.z << endl;

		}																		   
		else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))	   
		{																		   
			//D3DXQUATERNION q;													   
			//D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(45.f), 0.f, 0.f);  //yaw,pitch,roll
			//m_pTransformCom->Set_Quaternion(&q);
		}

		m_vForce += vLook;
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_DOWN))
	{
		m_vForce -= vLook;
	}
	//else
	//{
	//	m_vForce.z = 0;
	//}

}

void CCart::Free()
{
	CGameObject::Free();
}
