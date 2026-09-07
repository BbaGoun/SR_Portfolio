#include "pch.h"
#include "CDriftTrail.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCalculator.h"
#include "CWheel.h"

CDriftTrail::CDriftTrail(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CDriftTrail::CDriftTrail(const CGameObject& rhs) : CGameObject(rhs)
{
}

CDriftTrail::~CDriftTrail()
{
}

HRESULT CDriftTrail::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_DriftSpark"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	pComponent = m_pMeshRibbonCom = static_cast<CMeshRibbon*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MeshRibbon"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	m_pMeshRibbonCom->Set_IsQuadCopy(true);

	return S_OK;
}

_int CDriftTrail::Update_GameObject(const _float& fDeltaTime)
{
	m_fRemoveTimer += fDeltaTime;
	m_fWidthTimer += fDeltaTime;
	if (m_fRemoveTimer > 0.1f || m_pMeshRibbonCom->Get_ControlPoints().size() > 10) {
		m_fRemoveTimer = 0.f;
		if(m_pMeshRibbonCom->Get_ControlPoints().size() > 1)
			m_pMeshRibbonCom->Delete_Tail();
		if (m_pMeshRibbonCom->Get_ControlPoints().size() <= 1) {
			CManagement::GetInstance()->Delete_GameObject(L"GameLogic", this);
			if (m_pWheel)
				m_pWheel->ForgetDriftTrail(this);
			return 0;
		}
	}
	if (m_fWidthTimer > m_fNextWidthTimer) {
		m_fWidthTimer = 0.f;
		m_fNextWidthTimer = 0.05f + 0.15f * CCalculator::RandInt() / 99.f;
		m_pMeshRibbonCom->Update_Wave();
	}

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_TRAIL, this);
	
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CDriftTrail::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CDriftTrail::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pMeshRibbonCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDriftTrail::Compute_ViewZ()
{
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	auto& points = m_pMeshRibbonCom->Get_ControlPoints();
	
	m_fViewZ = FLT_MAX;

	for (auto& p : points) {
		_vec3 viewPos = p.position;
		D3DXVec3TransformCoord(&viewPos, &viewPos, &matView);
		if (viewPos.z > 0.f && m_fViewZ > viewPos.z)
			m_fViewZ = viewPos.z;
	}
}

void CDriftTrail::Append_Point(_vec3 vPos)
{
	ControlPoint cp = {};
	// 90 고정이 아니라 지형에 맞춰서
	cp.bank = 90.f;
	cp.position = vPos;
	cp.width = 2;
	int a;
	m_pMeshRibbonCom->Append_Point(cp);
}

CDriftTrail* CDriftTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, CWheel* pWheel)
{
	CDriftTrail* pObj = new CDriftTrail(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CDriftTrail Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	pObj->m_pWheel = pWheel;
	pObj->Append_Point(vPos);

	return pObj;
}

void CDriftTrail::OnLostDevice()
{
	m_pMeshRibbonCom->OnLostDevice();
}

void CDriftTrail::OnResetDevice()
{
	m_pMeshRibbonCom->OnResetDevice();
}

void CDriftTrail::Free()
{
	CGameObject::Free();
}
