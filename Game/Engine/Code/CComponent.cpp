#include "CComponent.h"
#include "CGameObject.h"

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bClone(false), m_pOwner(nullptr)
{
	m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_bClone(true), m_pOwner(nullptr)
{
	wcsncpy_s(m_wProtoTag, rhs.m_wProtoTag, 256);
	m_pGraphicDev->AddRef();
}

CComponent::~CComponent()
{
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDev);
}
