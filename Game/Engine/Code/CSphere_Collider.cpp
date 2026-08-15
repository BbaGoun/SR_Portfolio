#include "CSphere_Collider.h"
#include "CTransform.h"
#include "CGameObject.h"

CSphere_Collider::CSphere_Collider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCollider(pGraphicDev)
{
}

CSphere_Collider::CSphere_Collider(const CCollider& rhs)
	:CCollider(rhs)
{
}

CSphere_Collider::~CSphere_Collider()
{
}

HRESULT CSphere_Collider::Ready_CSphere_Collider()
{
	return S_OK;
}

_int CSphere_Collider::Update_Component(const _float& fTimeDelta)
{
	return 0;
}
void CSphere_Collider::LateUpdate_Component(const _float& fTimeDelta)
{
	CTransform* pOwnerTransfrom = (m_pOwner->Get_Transform());

	_vec3 vOwnerPos;
	pOwnerTransfrom->Get_Info(INFO_POS, &vOwnerPos);
	m_vCenter = vOwnerPos;
}

CSphere_Collider* CSphere_Collider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphere_Collider* pCube_Collider = new CSphere_Collider(pGraphicDev);

	if (FAILED(pCube_Collider->Ready_CColliderCom()))
	{
		Safe_Release(pCube_Collider);
		MSG_BOX("pCube_Collider Create Failed");
		return nullptr;
	}

	return pCube_Collider;

}
CComponent* CSphere_Collider::Clone()
{
	CSphere_Collider* pSphere_Collider = new CSphere_Collider(*this);

	if (FAILED(pSphere_Collider->Ready_CColliderCom()))
	{
		Safe_Release(pSphere_Collider);
		MSG_BOX("pSphere_Collider Create Failed");
		return nullptr;
	}
	return pSphere_Collider;
}

void CSphere_Collider::Free()
{
	CCollider::Free();
}