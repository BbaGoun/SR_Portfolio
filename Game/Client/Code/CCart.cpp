#include "pch.h"
#include "CCart.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

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
	return S_OK;
}

void CCart::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
	_vec3 pos;
	m_pTransformCom->Get_Info(INFO_POS, &pos);
	pos += m_vForce * fFixedDeltaTime;
}

_int CCart::Update_GameObject(const _float& fDeltaTime)
{
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

}

void CCart::Free()
{
	CGameObject::Free();
}
