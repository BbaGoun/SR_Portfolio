#include "CCube_Collider.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCalculator.h"

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
	vOwnerPos += m_vOffset;

	D3DXQUATERNION q = pOwnerTransfrom->Get_Quaternion();
	m_tBoundingBox.Orientation = ToXMFLOAT4(q);
	m_tBoundingBox.Center = ToXMFLOAT3(vOwnerPos);
}

void CCube_Collider::Render_Component(D3DCOLOR color)
{
	DirectX::XMFLOAT3 vCorners[8];

	m_tBoundingBox.GetCorners(vCorners);

	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[0]), ToVec3(vCorners[1]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[1]), ToVec3(vCorners[2]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[2]), ToVec3(vCorners[3]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[3]), ToVec3(vCorners[0]), color);

	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[4]), ToVec3(vCorners[5]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[5]), ToVec3(vCorners[6]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[6]), ToVec3(vCorners[7]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[7]), ToVec3(vCorners[4]), color);
	
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[0]), ToVec3(vCorners[4]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[1]), ToVec3(vCorners[5]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[2]), ToVec3(vCorners[6]), color);
	CCalculator::DrawRayLine(m_pGraphicDev, ToVec3(vCorners[3]), ToVec3(vCorners[7]), color);
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