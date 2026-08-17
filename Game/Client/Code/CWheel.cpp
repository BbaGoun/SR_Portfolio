#include "pch.h"
#include "CWheel.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CCart.h"
#include "CLand3.h"

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
		m_pTransformCom->Set_Pos({-2.5f ,-1, 6 });
		break;
	case Engine::WHEEL_FR:
		m_pTransformCom->Set_Pos({ 2.5f,-1, 6 });
		break;
	case Engine::WHEEL_BL:
		m_pTransformCom->Set_Pos({ -2.5f,-1,0 });
		break;								
	case Engine::WHEEL_BR:					
		m_pTransformCom->Set_Pos({ 2.5f ,-1,0 });
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

	CComponent* pCom = CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment", L"Env_Land3", L"Com_Buffer");
	CTerrain3* pTerrain3 = dynamic_cast<CTerrain3*>(pCom);


	if (pCart->GetDrift())
	{
		CLand3* pLand3 = dynamic_cast<CLand3*>(pCom->Get_Owner());
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		if (pLand3->CheckInTerrain(vPos))
		{
			// Land3의 로컬로 내림
			_matrix* pMatWorld = pLand3->Get_Transform()->Get_World();
			_matrix matInvWorld;
			D3DXMatrixInverse(&matInvWorld, 0, pMatWorld);
			D3DXVec3TransformCoord(&vPos, &vPos, &matInvWorld);

			pTerrain3->Set_SkidMark(vPos);
		}
	}
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
