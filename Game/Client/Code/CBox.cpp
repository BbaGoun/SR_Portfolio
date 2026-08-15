#include "pch.h"
#include "CBox.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"

CBox::CBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBox::CBox(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CBox::~CBox()
{
}

HRESULT CBox::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fSpeed = 0.f;
	m_fMaxSpeed = 3.f;
	m_pTransformCom->Set_Pos({ 0,0,100.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCartBodyCol*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CartBodyCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCube_Collider*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CubeCollider"));
	if (nullptr == pComponent)
		return E_FAIL;
	pComponent->Set_Owner(this);

	m_pColliderCom->SetCenter({ 0,0,100.f });
	m_pColliderCom->SetSize({ 2.5f,1.f,5.f });
	m_pColliderCom->SetColliderType(CUBE_COLLIDER);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CBox::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
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
	if (m_fSpeed > 0.1f)
		m_pTransformCom->Move_Pos(&vLook, m_fSpeed, fFixedDeltaTime);
	else
		m_pTransformCom->Move_Pos(&vLook, m_fSpeed, fFixedDeltaTime);
}

_int CBox::Update_GameObject(const _float& fDeltaTime)
{
	KeyInput(fDeltaTime);

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CBox::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CBox::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pBufferCom->Render_Buffer();
}

void CBox::KeyInput(const _float& fDeltaTime)
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
			{
				if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
					m_vRotation.y += D3DXToRadian(-1.5);
				else
					m_vRotation.y += D3DXToRadian(-0.5);

			}
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
			{
				if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
					m_vRotation.y += D3DXToRadian(1.5);
				else
					m_vRotation.y += D3DXToRadian(0.5);
			}
		}
		else
		{
			if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
			{
				if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
					m_vRotation.y += D3DXToRadian(1.5);
				else
					m_vRotation.y += D3DXToRadian(0.5);
			}
			else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
			{
				if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
					m_vRotation.y += D3DXToRadian(-1.5);
				else
					m_vRotation.y += D3DXToRadian(-0.5);
			}
		}
	}
}

CBox* CBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBox* pObj = new CBox(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CBox_FL Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CBox::Free()
{
	CGameObject::Free();
}
