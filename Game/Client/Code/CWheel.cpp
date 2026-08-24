#include "pch.h"
#include "CWheel.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CCart.h"
#include "CLand3.h"
#include "CSkidMark.h"

CWheel::CWheel(LPDIRECT3DDEVICE9 pGraphicDev, WHEEL_TYPE eType)
	:CGameObject(pGraphicDev),m_eWheelType(eType)
{
}

CWheel::CWheel(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CWheel::~CWheel()
{
}

HRESULT CWheel::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	switch (m_eWheelType)
	{
	case Engine::WHEEL_FL:
		m_pTransformCom->Set_Pos({-2.5f ,1, 6 });
		break;
	case Engine::WHEEL_FR:
		m_pTransformCom->Set_Pos({ 2.5f,1, 6 });
		break;
	case Engine::WHEEL_BL:
		m_pTransformCom->Set_Pos({ -2.5f,1,0 });
		break;								
	case Engine::WHEEL_BR:					
		m_pTransformCom->Set_Pos({ 2.5f ,1,0 });
		break;
	case Engine::WHEEL_END:
		break;
	default:
		break;
	}

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCartWheelCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartWheelCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CWheel::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	CCart* pCart = dynamic_cast<CCart*>(m_pParent->Get_Parent());
	_vec3 vParentForce = pCart->Get_Force();
	float fParentForceLen = D3DXVec3Length(&vParentForce);

	_vec3 vPlayerLook;
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vPlayerLook);

	if (D3DXVec3Dot(&vPlayerLook, &vParentForce) >= 0)
		m_vRotation.x += fParentForceLen * fFixedDeltaTime;
	else
		m_vRotation.x -= fParentForceLen * fFixedDeltaTime;

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, m_vRotation.y, m_vRotation.x, 0.f);
	m_pTransformCom->Set_Quaternion(&q);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	_vec3 originPos = vPos;
	if (pCart->GetDrift())
	{
		_vec3 vDeltaPos;
		vDeltaPos = vPos - m_vPrePos;
		m_fDistSum += D3DXVec3Length(&vDeltaPos);
		if (m_fDistSum >= 0.01f)
		{
			CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land3", L"Com_Buffer");
			CTerrain3* pTerrain3 = dynamic_cast<CTerrain3*>(pCom);
			CLand3* pLand3 = dynamic_cast<CLand3*>(pCom->Get_Owner());
			if (pLand3->CheckInTerrain(vPos))
			{
				// Land3의 로컬로 내림
				_matrix* pMatWorld = pLand3->Get_Component<CTransform>()->Get_World();
				_matrix matInvWorld;
				D3DXMatrixInverse(&matInvWorld, 0, pMatWorld);
				D3DXVec3TransformCoord(&vPos, &vPos, &matInvWorld);

				// 평면 구하기
				D3DXPLANE plane = pTerrain3->GetPlane(vPos);
				float fLocalPlaneY = -(plane.a * vPos.x + plane.c * vPos.z + plane.d) / plane.b;

				// Local에서의 CartWheelPosition
				_vec3 vLocalPos = { vPos.x,fLocalPlaneY,vPos.z };

				// World에서의 CartWheelPosition
				_vec3 vWorldPos;
				D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, pMatWorld);

				float fDeltaY = originPos.y - vWorldPos.y;
				if (m_eWheelType == WHEEL_FR)
					cout << fDeltaY << endl;
				if (fDeltaY <= 1.2f)
				{
					CreateSkidMark();
					m_fDistSum = 0;
				}
			}
			else
			{
				CreateSkidMark();
				m_fDistSum = 0;
			}
		}
	}
	else
	{
		m_fDistSum = 0;
	}
	m_vPrePos = vPos;
}

_int CWheel::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	KeyInput(fDeltaTime);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CWheel::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CWheel::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

void CWheel::KeyInput(const _float& fDeltaTime)
{
	_vec3 vParentForce = m_pParent->Get_Parent()->Get_Force();
	float fParentForceLen = D3DXVec3Length(&vParentForce);
	if (fParentForceLen > 5.0f)
	{
		m_vRotation.y = 0;
		return;
	}
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT)&& m_eWheelType < WHEEL_BL)
	{
		m_vRotation.y = -45;
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT) && m_eWheelType < WHEEL_BL)
	{
		m_vRotation.y = 45;
	}
	else
	{
		m_vRotation.y = 0;
	}
}

void CWheel::ResetPrePos()
{
	m_pTransformCom->Get_Info(INFO_POS, &m_vPrePos);
}

void CWheel::CreateSkidMark()
{
	CGameObject* pGameObject = CSkidMark::Create(m_pGraphicDev);
	
	if (nullptr == pGameObject)
		return;

	if (FAILED(m_pLayer->Add_GameObject(L"Proto_SkidMark", pGameObject)))
		return;
	pGameObject->SetLayer(m_pLayer);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y -= 1;
	pGameObject->Get_Transform()->Set_Pos(vPos);

	//CCartBody의 WorldQuaternion을 가져옴
	D3DXQUATERNION q = m_pParent->Get_Parent()->Get_Transform()->Get_WorldQuaternion();
	pGameObject->Get_Transform()->Multiple_Quaternion(&q);
}

CWheel* CWheel::Create(LPDIRECT3DDEVICE9 pGraphicDev,WHEEL_TYPE eType)
{
	CWheel* pObj = new CWheel(pGraphicDev, eType);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CWheel_FL Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CWheel::Free()
{
	CGameObject::Free();
}
