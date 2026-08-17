#include "pch.h"
#include "CCartBody.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CLand3.h"
#include "CManagement.h"
CCartBody::CCartBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCartBody::CCartBody(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCartBody::~CCartBody()
{
}

HRESULT CCartBody::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0,0,3 });
	m_bBananaSpinState = false;

	return S_OK;
}

void CCartBody::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	BananaSpin(fFixedDeltaTime); 
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(m_vRotation.y), D3DXToRadian(m_vRotation.x), D3DXToRadian(m_vRotation.z));
	m_pTransformCom->Set_Quaternion(&q);
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
}

_int CCartBody::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCartBody::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);	
}

void CCartBody::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
}

void CCartBody::BananaSpin(const _float& fDeltaTime)
{
	CCart* pCart = dynamic_cast<CCart*>(m_pParent);
	bool bCartBananaSpin = pCart->GetBanana();
	if (bCartBananaSpin == false)
		return;
	if (m_bBananaSpinState == false && bCartBananaSpin == true)
	{
		_vec3 vCartForce = pCart->Get_Force();
		m_fSpinSpeed = pCart->Get_Speed() * D3DXVec3Length(&vCartForce);
		m_fSpinSpeed = m_fSpinSpeed / 50 + 2;
		if (m_fSpinSpeed <= 1)m_fSpinSpeed++;
	}
	m_fSpinSpeed *= 0.98;
	m_bBananaSpinState = true;
	m_vRotation.y += 300* m_fSpinSpeed * fDeltaTime;
	if (m_vRotation.y > 1080 * m_fSpinSpeed)
	{
		m_vRotation.y = 0;
		pCart->SetBanana(false);
		m_bBananaSpinState = false;
	}
}

CCartBody* CCartBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCartBody* pObj = new CCartBody(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCartBody Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCartBody::Free()
{
	CGameObject::Free();
}
