#include "CCube_Collider.h"
#include "CTransform.h"
#include "CGameObject.h"

CCube_Collider::CCube_Collider()
{
}

CCube_Collider::CCube_Collider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCollider(pGraphicDev)
{
}

CCube_Collider::CCube_Collider(const CCollider& rhs)
	:CCollider(rhs)
{
}

CCube_Collider::~CCube_Collider()
{
}

HRESULT CCube_Collider::Ready_CCube_Collider()
{
	return S_OK;
}

_int CCube_Collider::Update_Component(const _float& fTimeDelta)
{
	return 0;
}
void CCube_Collider::LateUpdate_Component(const _float& fTimeDelta)
{
	CTransform* pOwnerTransfrom = (m_pOwner->Get_Transform());
	_vec3 vOwnerPos;
	pOwnerTransfrom->Get_Info(INFO_POS, &vOwnerPos);
	m_vCenter = vOwnerPos;
}
CCube_Collider* CCube_Collider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCube_Collider* pCube_Collider = new CCube_Collider(pGraphicDev);

	if (FAILED(pCube_Collider->Ready_CColliderCom()))
	{
		Safe_Release(pCube_Collider);
		MSG_BOX("pCube_Collider Create Failed");
		return nullptr;
	}

	return pCube_Collider;

}
CComponent* CCube_Collider::Clone()
{
	CCube_Collider* pCube_Collider = new CCube_Collider(*this);

	if (FAILED(pCube_Collider->Ready_CColliderCom()))
	{
		Safe_Release(pCube_Collider);
		MSG_BOX("pCube_Collider Create Failed");
		return nullptr;
	}
	return pCube_Collider;
}

void CCube_Collider::Free()
{
	CCollider::Free();
}