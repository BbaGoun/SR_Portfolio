#include "pch.h"
#include "CPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CCart.h"
#include "CCameraMgr.h"
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	//Engine::CComponent* pComponent = nullptr;

	m_bBoost = false;

	//m_vRotation.y = 90.f;
	return S_OK;
}

void CPlayer::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q,
		D3DXToRadian(m_vRotation.y),
		D3DXToRadian(m_vRotation.x),
		D3DXToRadian(m_vRotation.z));
	m_pTransformCom->Set_Quaternion(&q);
}

_int CPlayer::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
	CRenderer::GetInstance()->Add_LeftMirrorRenderGroup(RENDER_NONALPHA, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CPlayer::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pObj = new CPlayer(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CPlayer Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CPlayer::Free()
{
	CGameObject::Free();
}
