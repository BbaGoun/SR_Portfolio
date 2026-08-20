#include "CSphere_Collider.h"
#include "CTransform.h"
#include "CGameObject.h"

CSphere_Collider::CSphere_Collider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCollider(pGraphicDev)
{
	m_eColliderType = SPHERE_COLLIDER;

}

CSphere_Collider::CSphere_Collider(const CCollider& rhs)
	:CCollider(rhs)
{
	m_eColliderType = SPHERE_COLLIDER;
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
	vOwnerPos += m_vOffset;

	m_tBoundingSphere.Center = ToXMFLOAT3(vOwnerPos);
}

void CSphere_Collider::Render_Component(D3DXCOLOR color)
{
	LPD3DXMESH pSphere;
	D3DXCreateSphere(m_pGraphicDev, m_tBoundingSphere.Radius, 16, 16, &pSphere, NULL);
	// 오염을 방지하여 복사
	_matrix matWorld = *m_pOwner->Get_Transform()->Get_World();
	matWorld.m[3][0] += m_vOffset.x;
	matWorld.m[3][1] += m_vOffset.y;
	matWorld.m[3][2] += m_vOffset.z;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	// 1. 와이어 모드 켜기
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// 2. 텍스처 stage 0 설정: 모든 입력 대신 고정된 상수 색상(TFACTOR)을 사용하도록 설정
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

	// 3. 원하는 색상 지정
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, DWORD(color));

	// 그리기
	pSphere->DrawSubset(0);

	// 4. 다음 그리기를 위해 상태 원상복구
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); // 기본값 복구
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); // 기본값 복구

	pSphere->Release();
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