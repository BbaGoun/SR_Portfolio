#include "pch.h"
#include "CCart.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CCart::CCart(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_bDrift(false)
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
	
	m_bDrift = false;
	m_fLookForceAngle = 0.f;

	m_fBoostTurnAngle	= 0.5f;
	m_fNormalTurnAngle	= 0.8f;
	m_fDriftTurnAngle	= 1.5f;

	m_bBoost = false;

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(false);
	m_pColliderCom->Set_Extents({ 2.5f,1.5f,5.f });
	m_pColliderCom->SetColliderType(CUBE_COLLIDER);

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });


	//pComponent = m_pSphereColliderCom = dynamic_cast<CSphere_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SphereCollider"));
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//pComponent->Set_Owner(this);
	//
	//m_pSphereColliderCom->SetCenter({ 0.f,-0.5f,3.f });
	//m_pSphereColliderCom->SetRadius(6.f);
	//m_pSphereColliderCom->SetColliderType(SPHERE_COLLIDER);
	//
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_SphereCollider", pComponent });

	return S_OK;
}

void CCart::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, 0.f, m_vRotation.z);
	m_pTransformCom->Set_Quaternion(&q);

	m_pTransformCom->Move_Pos(&m_vForce, m_fSpeed, fFixedDeltaTime);
	
	m_vForce *= 0.98;
	if (D3DXVec3Length(&m_vForce) < 1.f)
		m_vForce *= 0;

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pColliderCom->Set_Offset(vLook*3);
}

_int CCart::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	KeyInput(fDeltaTime);
	UpdateBoost();
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCart::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
	UpdateDrift();
}

void CCart::Render_GameObject()
{
#ifdef _DEBUG
	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));
#endif
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
		if (m_bDrift == false)
			m_vForce += vLook;
		else
			m_vForce += vLook * 0.8f;
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_DOWN))
	{
		m_bBoost = false;
		m_fSpeed = 1.f;
		if (m_bDrift == false)
			m_vForce -= vLook;
		else
			m_vForce -= vLook * 0.8f;
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LCONTROL))
	{
		m_bBoost = true;
		m_fBoostCal = 1.05f;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
	{
		m_bDrift = true;
	}


	float fForceLength = D3DXVec3Length(&m_vForce);
	if (fForceLength < 1.0f)
		return;
	if (D3DXVec3Dot(&m_vForce, &vLook) > 0)	// m_vForce와 vLook의 내적값으로 전진후진 판단
	{
		if (m_bDrift == true)
		{
			_vec3 vTempForce, vCross;
			vTempForce = m_vForce;

			vLook.y = 0;
			vTempForce.y = 0;

			D3DXVec3Cross(&vCross, &vTempForce, &vLook);

			float fTurnAngle = min(m_fDriftTurnAngle, fForceLength * 0.04);
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
			{
				if (vCross.y < 0)
				{
					m_vRotation.z += fDeltaTime * 0.5f;
					m_vRotation.y += D3DXToRadian(-fTurnAngle);
				}
				else
				{
					m_vRotation.z += fDeltaTime * 0.5f;
					if (m_vRotation.z > 0)
						m_vRotation.z = 0;
					m_vRotation.y += D3DXToRadian(-fTurnAngle * 0.5);
				}
			}
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
			{
				if (vCross.y > 0)
				{
					m_vRotation.z += -fDeltaTime * 0.5f;
					m_vRotation.y += D3DXToRadian(fTurnAngle);
				}
				else
				{
					m_vRotation.z += -fDeltaTime * 0.5f;
					if (m_vRotation.z < 0)
						m_vRotation.z = 0;

					m_vRotation.y += D3DXToRadian(fTurnAngle * 0.5);
				}
			}
		}
		else if (m_bBoost == true)
		{
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
				m_vRotation.y += D3DXToRadian(-m_fBoostTurnAngle);
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
				m_vRotation.y += D3DXToRadian(m_fBoostTurnAngle);
		}
		else
		{
			float fTurnAngle = min(m_fNormalTurnAngle, fForceLength * 0.013);
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
			{
				m_vRotation.y += D3DXToRadian(-fTurnAngle);
				_matrix matRot;
				D3DXMatrixRotationY(&matRot, D3DXToRadian(-fTurnAngle));
				D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
			}
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
			{
				m_vRotation.y += D3DXToRadian(fTurnAngle);
				_matrix matRot;
				D3DXMatrixRotationY(&matRot, D3DXToRadian(fTurnAngle));
				D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
			}
		}
	}
	else
	{
		if (m_bDrift == true)
		{
			_vec3 vTempForce, vCross;
			vTempForce = m_vForce;

			vLook.y = 0;
			vTempForce.y = 0;

			D3DXVec3Cross(&vCross, &vTempForce, &vLook);

			float fTurnAngle = min(m_fDriftTurnAngle, fForceLength * 0.04);
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
			{
				if (vCross.y < 0)
				{
					m_vRotation.y += D3DXToRadian(fTurnAngle);
				}
				else
				{
					m_vRotation.y += D3DXToRadian(fTurnAngle * 0.5);
				}
			}
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
			{
				if (vCross.y > 0)
				{
					m_vRotation.y += D3DXToRadian(-fTurnAngle);
				}
				else
				{
					m_vRotation.y += D3DXToRadian(-fTurnAngle * 0.5);
				}
			}
		}
		else if (m_bBoost == true)
		{
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
				m_vRotation.y += D3DXToRadian(m_fBoostTurnAngle);
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
				m_vRotation.y += D3DXToRadian(-m_fBoostTurnAngle);
		}
		else
		{
			float fTurnAngle = min(m_fNormalTurnAngle, fForceLength * 0.013);
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
			{
				m_vRotation.y += D3DXToRadian(fTurnAngle);
				_matrix matRot;
				D3DXMatrixRotationY(&matRot, D3DXToRadian(fTurnAngle));
				D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
			}
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
			{
				m_vRotation.y += D3DXToRadian(-fTurnAngle);
				_matrix matRot;
				D3DXMatrixRotationY(&matRot, D3DXToRadian(-fTurnAngle));
				D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
			}
		}
	}
}

void CCart::UpdateDrift()
{
	if (m_bDrift == true)
	{
		_vec3 vLook, vTempForce;
		m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
		vTempForce = m_vForce;

		vLook.y = 0;
		vTempForce.y = 0;

		D3DXVec3Normalize(&vLook, &vLook);
		D3DXVec3Normalize(&vTempForce, &vTempForce);

		_vec3 vCross;
		D3DXVec3Cross(&vCross, &vTempForce, &vLook);

		m_fLookForceAngle = acosf(D3DXVec3Dot(&vLook, &vTempForce));

		m_vRotation.z *= 0.98;
		m_vRotation.z = clampT(float(m_vRotation.z), -0.1f, 0.1f);

		if (m_fLookForceAngle < 0.3f)
		{
			m_vRotation.z = 0;
			m_bDrift = false;
		}
	}
}

void CCart::UpdateBoost()
{
	if (m_bBoost == false)
		return;
	//if (m_fSpeed == 1)
	//{
	//	//_vec3 vLook, vTempForce;
	//	//m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	//	//vTempForce = m_vForce;
	//	//
	//	//vLook.y = 0;
	//	//vTempForce.y = 0;
	//	//
	//	//D3DXVec3Normalize(&vLook, &vLook);
	//	//D3DXVec3Normalize(&vTempForce, &vTempForce);
	//	//
	//	//float fAngle;
	//	//fAngle = D3DXVec3Dot(&vTempForce, &vLook);
	//	//
	//	//_matrix matRot;
	//	//D3DXMatrixRotationY(&matRot, D3DXToRadian(fAngle));
	//	//D3DXVec3TransformNormal(&m_vForce, &m_vForce, &matRot);
	//
	//	m_fSpeed += 2; 
	//
	//	cout << "Shoort Boost" << endl;
	//}

	if (m_fSpeed > 3)
		m_fBoostCal = 0.995;
	m_fSpeed *= m_fBoostCal;
	if (m_fSpeed < 1)
	{
		m_bBoost = false;
		m_fSpeed = 1;
	}
}


void CCart::Free()
{
	CGameObject::Free();
}
