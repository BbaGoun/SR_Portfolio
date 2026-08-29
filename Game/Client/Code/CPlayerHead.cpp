#include "pch.h"
#include "CPlayerHead.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CCameraMgr.h"
CPlayerHead::CPlayerHead(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPlayerHead::CPlayerHead(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CPlayerHead::~CPlayerHead()
{
}

HRESULT CPlayerHead::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	Engine::CComponent* pComponent = nullptr;

	//pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//pComponent->Set_Owner(this);
	//m_mapComponent.insert({ L"Com_Buffer", pComponent });
	//
	//pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_BoosterJet"));
	//if (nullptr == pComponent)
	//	return E_FAIL;
	//
	//m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_bBoost		= false;
	m_eCartDirType	= DIR_FORWARD;

	//m_vRotation.y = -90.f;
	return S_OK;
}

void CPlayerHead::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, 
										D3DXToRadian(m_vRotation.y),
										D3DXToRadian(m_vRotation.x), 
										D3DXToRadian(m_vRotation.z));
	m_pTransformCom->Set_Quaternion(&q);
}

_int CPlayerHead::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	if (m_bBoost == true)
	{
		m_vRotation.x = 0.f;
		// 고개 뒤로 x축 회전 -> z축 회전
		if (m_vRotation.z > -30.f)
			m_vRotation.z -= 50 *fDeltaTime;
		else
			m_vRotation.z = -30.f;

	}
	else
	{
		// 고개 원위치
		if (m_vRotation.z < 0.f)
			m_vRotation.z += 50 * fDeltaTime;
		else
			m_vRotation.z = 0.f;
		KeyInput(fDeltaTime);
	}

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CPlayerHead::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CPlayerHead::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

}
void CPlayerHead::KeyInput(const _float& fDeltaTime)
{
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LEFT))
	{
		// 고개 왼쪽
		if (m_vRotation.x > -20.f)
			m_vRotation.x -= 50 * fDeltaTime;
		// 위치 이동
	}
	else if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_RIGHT))
	{
		// 고개 오른쪽
		if (m_vRotation.x < 20.f)
			m_vRotation.x += 50 * fDeltaTime;

		// 위치 이동
	}
	else
	{
		// 고개 원위치
		if (m_vRotation.x < 0)
			m_vRotation.x += 50 * fDeltaTime;
		else if (m_vRotation.x > 0)
			m_vRotation.x -= 50 * fDeltaTime;
		else
			m_vRotation.x = 0.f;

	}
}

CPlayerHead* CPlayerHead::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerHead* pObj = new CPlayerHead(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CPlayerHead Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CPlayerHead::Free()
{
	CGameObject::Free();
}
