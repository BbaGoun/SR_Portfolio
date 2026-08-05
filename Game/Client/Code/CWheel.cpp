#include "pch.h"
#include "CWheel.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
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
		m_pTransformCom->Set_Pos({-2.5f ,-1, 3 });
		break;
	case Engine::WHEEL_FR:
		m_pTransformCom->Set_Pos({ 2.5f,-1, 3 });
		break;
	case Engine::WHEEL_BL:
		m_pTransformCom->Set_Pos({ -2.5f,-1,-3 });
		break;
	case Engine::WHEEL_BR:
		m_pTransformCom->Set_Pos({ 2.5f ,-1,-3 });
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
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CWheel::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CWheel::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	KeyInput();
	return _int();
}

void CWheel::LateUpdate_GameObject(const _float& fDeltaTime)
{
}

void CWheel::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

void CWheel::KeyInput()
{
	if (m_eWheelType >= WHEEL_BL)
		return;
	D3DXQUATERNION q;
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_UP))
	{
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(0.f), 0.f, 0.f);//yaw,pitch,roll
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_DOWN))
	{
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(0.f), 0.f, 0.f);//yaw,pitch,roll
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
	{
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(-45.f), 0.f, 0.f);//yaw,pitch,roll
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
	{
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(45.f), 0.f, 0.f);//yaw,pitch,roll
	}
	else
	{
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(0.f), 0.f, 0.f);//yaw,pitch,roll
	}
	m_pTransformCom->Set_Quaternion(&q);
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
