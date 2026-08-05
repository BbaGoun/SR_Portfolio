#include "pch.h"
#include "CCart.h"

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
	return E_NOTIMPL;
}

void CCart::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
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
		MSG_BOX("Obj_Test Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CCart::Free()
{
	CGameObject::Free();
}
