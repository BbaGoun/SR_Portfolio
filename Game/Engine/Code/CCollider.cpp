#include "CCollider.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_bIsTrigger(false), m_eColliderType(CT_END)
{
	m_eID = ID_DYNAMIC;
}

CCollider::CCollider(const CCollider& rhs)
	:CComponent(rhs),m_bIsTrigger(rhs.m_bIsTrigger), m_eColliderType(rhs.m_eColliderType)
{
	m_eID = ID_DYNAMIC;
}

CCollider::~CCollider()
{
}


HRESULT CCollider::Ready_CColliderCom()
{
	return S_OK;
}


_int CCollider::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void CCollider::LateUpdate_Component()
{

}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollider* pCollider = new CCollider(pGraphicDev);

	if (FAILED(pCollider->Ready_CColliderCom()))
	{
		Safe_Release(pCollider);
		MSG_BOX("pCollider Create Failed");
		return nullptr;
	}

	return pCollider;
}


void CCollider::Free()
{
	CComponent::Free();
}

CComponent* CCollider::Clone()
{
	CCollider* pCollider = new CCollider(*this);
	if (FAILED(pCollider->Ready_CColliderCom()))
	{
		Safe_Release(pCollider);
		MSG_BOX("pCollider Create Failed");
		return nullptr;
	}

	return pCollider;
}
