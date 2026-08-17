#include "pch.h"
#include "CCart.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CRainBow_Cloud.h"
#include "CManagement.h"
#include "CBanana.h"
#include "CCollisionMgr.h"
#include "CLand3.h"

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
	m_vForce			= { 0,0,0 };

	m_fSpeed			= 1.f;
	m_fMaxSpeed			= 3.f;
	
	m_bDrift			= false;
	m_fLookForceAngle	= 0.f;

	m_fBoostTurnAngle	= 0.5f;
	m_fNormalTurnAngle	= 0.8f;
	m_fDriftTurnAngle	= 1.5f;

	m_bBoost			= false;
	m_bRainbowUI		= false;
	m_bBanana			= false;

	m_fBananaTimer		= 0.f;

	m_fCurGage			= 0.f;
	m_fGainGage			= 0.f;

	m_fBoostItemCnt		= 0.f;

	
	m_vBananaSpinStartLook = { 0,0,0 };

	m_eCartState = CART_STATE_GROUND;
	m_vTerrainNormal = { 0,1,0 };
	
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_pColliderCom->Set_Owner(this);
	m_pColliderCom->SetIsTrigger(false);
	m_pColliderCom->Set_Extents({ 2.5f,1.5f,5.f });
	m_pColliderCom->SetColliderType(CUBE_COLLIDER);

	m_mapComponent.insert({L"Com_Collider", pComponent});


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
	UpdateGravity();

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, m_vRotation.z);
	m_pTransformCom->Set_Quaternion(&q);

	m_pTransformCom->Move_Pos(&m_vForce, m_fSpeed, fFixedDeltaTime);
	
	m_vForce *= 0.98;
	if (D3DXVec3Length(&m_vForce) < 1.f)
		m_vForce *= 0;

	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pColliderCom->Set_Offset(vLook*3);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	AdjustPosY_Slope(vPos);

}

_int CCart::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	KeyInput(fDeltaTime);
	UpdateBoost();
	UpdateDrift();
	OutputCarState();
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCart::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CCart::Render_GameObject()
{
#ifdef _DEBUG
	m_pColliderCom->Render_Component(D3DXCOLOR({ 0,1,0,1 }));
#endif
}

void CCart::CollisionEnter(CCollider* pOtherCollider)
{
	const _tchar* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcsncmp(wOtherTag, L"Obj_CollisionBox", 16) == 0)
	{
		CCollisionMgr::GetInstance()->PysicalCubevsCube(
			static_cast<CCube_Collider*>(pOtherCollider), m_pColliderCom);
		m_fGainGage = 0.f;
		m_bDrift = false;
  }
}

void CCart::TriggerEnter(CCollider* pOtherCollider)
{
	const WCHAR* wOtherTag = pOtherCollider->Get_Owner()->GetTag();

	if (wcsncmp(wOtherTag, L"Rainbow_Cloud", 13) == 0)
	{
		if (m_bRainbowUI == false)
			m_bRainbowUI = true;
	}

	if (wcsncmp(wOtherTag, L"Obj_Banana", 10) == 0)
	{
		if (m_bBanana == false)
		{
			m_bBanana = true;
			m_bBoost = false;
			m_vBananaSpinStartLook = m_vForce;
			D3DXVec3Normalize(&m_vBananaSpinStartLook, &m_vBananaSpinStartLook);
			
		}
	}
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
	if (m_bBanana == true)
		return;
	_vec3 vLook;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);

	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_Q))
	{
		CreateRainbowObject();
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyDown(DIKEYBOARD_W))
	{
		CreateBananaObject();
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LCONTROL))
	{
		if (m_fBoostItemCnt > 0)
		{
			--m_fBoostItemCnt;
			m_bBoost = true;
			m_fBoostCal = 1.05f;
		}
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_UP))
	{
		if (m_bDrift == false)
			m_vForce += vLook;
		else
			m_vForce += vLook * 0.8f;
	}
	else
	{
		m_fSpeed = 1.f;
		m_bBoost = false;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_DOWN))
	{
		m_fSpeed = 1.f;
		if (m_bDrift == false)
			m_vForce -= vLook;
		else
			m_vForce -= vLook * 0.8f;
	}

	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT) && m_eCartState == CART_STATE_GROUND)
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

		if (m_fLookForceAngle < 0.3f || m_eCartState != CART_STATE_GROUND)
		{
			m_fCurGage += m_fGainGage;
			if (m_fCurGage >= 100.f)
			{
				m_fCurGage = 0;
				++m_fBoostItemCnt;
			}
			m_fGainGage = 0;
			m_vRotation.z = 0;
			m_bDrift = false;
		}
		else
		{
			m_fGainGage += m_fLookForceAngle * 0.5f;
			m_fGainGage += D3DXVec3Length(&m_vForce) * m_fSpeed * 0.005f;
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

void CCart::CreateRainbowObject()
{
	CGameObject* pGameObject = CRainbow_Cloud::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return ;

	if (FAILED(m_pLayer->Add_GameObject(L"Rainbow_Cloud", pGameObject)))
		return ;

	_vec3 vPos,vLook;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	vPos += vLook * 100;
	pGameObject->Get_Transform()->Set_Pos(vPos);

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, 0.f, 0.f);
	pGameObject->Get_Transform()->Set_Quaternion(&q);
	pGameObject->SetLayer(m_pLayer);
}

void CCart::CreateBananaObject()
{
	CGameObject* pGameObject = CBanana::Create(m_pGraphicDev);

	if (nullptr == pGameObject)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Obj_Banana", pGameObject)))
		return;

	_vec3 vPos, vLook;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	vPos -= vLook * 10;
	pGameObject->Get_Transform()->Set_Pos(vPos);

	pGameObject->SetLayer(m_pLayer);
}

void CCart::AdjustPosY_Slope(_vec3 pos)
{
	CLand3* pLand3 = dynamic_cast<CLand3*>(CManagement::GetInstance()->Find_GameObjectByTag(L"Environment", L"Env_Land3"));
	CTerrain3* pTerrain3 = pLand3->Get_Component<CTerrain3>();

	_vec3 originPos = pos;
	if (pLand3->CheckInTerrain(pos))
	{
		// Land3의 로컬로 내림
		_matrix* pMatWorld = pLand3->Get_Component<CTransform>()->Get_World();
		_matrix matInvWorld;
		D3DXMatrixInverse(&matInvWorld, 0, pMatWorld);
		D3DXVec3TransformCoord(&pos, &pos, &matInvWorld);

		// 평면 구하기
		D3DXPLANE plane = pTerrain3->GetPlane(pos);
		float fLocalPlaneY = -(plane.a * pos.x + plane.c * pos.z + plane.d) / plane.b;

		// 법선 구하기
		m_vTerrainNormal = { plane.a ,plane.b ,plane.c };
		_matrix matNormal;
		D3DXMatrixTranspose(&matNormal, &matInvWorld);
		D3DXVec3TransformNormal(&m_vTerrainNormal, &m_vTerrainNormal, &matNormal);
		D3DXVec3Normalize(&m_vTerrainNormal, &m_vTerrainNormal);



		// Local에서의 CartPosition
		_vec3 vLocalPos = { pos.x,fLocalPlaneY,pos.z };

		// World에서의 CartPosition
		_vec3 vWorldPos;
		D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, pMatWorld);

		float fDeltaY = originPos.y - vWorldPos.y;
		// m_eCart_State 업데이트
		if (m_eCartState == CART_STATE_GROUND)
		{
			if (fDeltaY <= 0.1f)
			{
				m_eCartState = CART_STATE_GROUND;
				m_pTransformCom->Set_Pos({ originPos.x,vWorldPos.y,originPos.z });

				// 경사면에 맞게 카트 몸체 회전
				_vec3 vCartUp;
				m_pTransformCom->Get_Info(INFO_UP, &vCartUp);
				float fRadian = acosf(D3DXVec3Dot(&vCartUp, &m_vTerrainNormal));

				_vec3 vAxis;
				D3DXVec3Cross(&vAxis, &vCartUp, &m_vTerrainNormal);

				D3DXQUATERNION q;
				D3DXQuaternionRotationAxis(&q, &vAxis, (fRadian));
				m_pTransformCom->Multiple_Quaternion(&q);
			}
			else
			{
				m_eCartState = CART_STATE_AIR;
			}
		}
		else if(m_eCartState == CART_STATE_AIR)
		{
			if (fDeltaY <= 0.1f)
			{
				m_eCartState = CART_STATE_GROUND;
				m_pTransformCom->Set_Pos({ originPos.x,vWorldPos.y,originPos.z });

				// 경사면에 맞게 카트 몸체 회전
				_vec3 vCartUp;
				m_pTransformCom->Get_Info(INFO_UP, &vCartUp);
				float fRadian = acosf(D3DXVec3Dot(&vCartUp, &m_vTerrainNormal));

				_vec3 vAxis;
				D3DXVec3Cross(&vAxis, &vCartUp, &m_vTerrainNormal);

				D3DXQUATERNION q;
				D3DXQuaternionRotationAxis(&q, &vAxis, (fRadian));
				m_pTransformCom->Multiple_Quaternion(&q);
			}
			else
			{
				m_eCartState = CART_STATE_AIR;
			}
		}
	}
	else //맵 전체를 지형으로 덮으면 이 부분은 필요 없을듯?
	{
		if (originPos.y  <= 0.f)
		{
			m_eCartState = CART_STATE_GROUND;
			m_pTransformCom->Set_Pos({ originPos.x,0,originPos.z });
			m_vTerrainNormal = { 0,1,0 };
		}
		else
		{
			m_eCartState = CART_STATE_AIR;
		}
	}
}

void CCart::UpdateGravity()
{
	/*
	중력 -> 지면의 -Look , -Up 성분으로 분해(투영으로 분해)
              지면의  -Look = (지면 법선 x 지면의 Right)
              지면의 - Up    = -Normal
	*/

	_vec3 vGravity = { 0,-0.98f,0 };
	_vec3 vCartUp, vPlaneRight, vPlaneLook;
	float fSize;
	
	switch (m_eCartState)
	{
	case Engine::CART_STATE_GROUND:
		if (m_vTerrainNormal != _vec3({ 0,1,0 }))
		{
			// 평면의 Right벡터
			m_pTransformCom->Get_Info(INFO_UP, &vCartUp);
			D3DXVec3Cross(&vPlaneRight, &m_vTerrainNormal, &vCartUp);

			// 평면의 Look
			D3DXVec3Cross(&vPlaneLook, &vPlaneRight, &m_vTerrainNormal);

			// 중력의 성분 중에 -Look 방향의 성분만 받기
			// -Look벡터에 Gravity 투영해서 -Look 방향의 크기 구하기
			D3DXVec3Normalize(&vPlaneLook, &vPlaneLook);
			vPlaneLook *= -1;
			fSize = D3DXVec3Dot(&vPlaneLook, &vGravity);

			// 구한 크기에 -Look 방향벡터 곱해서 vForce에 적용
			m_vForce += fSize * vPlaneLook;
		}

		break;
	case Engine::CART_STATE_AIR:
		// 중력 전부 다 받기
		m_vForce += vGravity;
		break;
	case Engine::CART_STATE_LANDING:
		break;
	case Engine::CART_STATE_END:
		break;
	default:
		break;
	}
}

void CCart::OutputCarState()
{
	switch (m_eCartState)
	{
	case Engine::CART_STATE_GROUND:
		cout << "CART_STATE_GROUND" << endl;
		break;
	case Engine::CART_STATE_AIR:
		cout << "CART_STATE_AIR" << endl;
		break;
	case Engine::CART_STATE_LANDING:
		cout << "CART_STATE_LANDING" << endl;
		break;
	case Engine::CART_STATE_END:
		cout << "CART_STATE_GROUND" << endl;
		break;
	default:
		break;
	}
}


void CCart::Free()
{
	CGameObject::Free();
}
