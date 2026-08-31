#include "pch.h"
#include "CSkidMark.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCalculator.h"
#include "CWheel.h"

CSkidMark::CSkidMark(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CSkidMark::CSkidMark(const CGameObject& rhs) : CGameObject(rhs)
{
}

CSkidMark::~CSkidMark()
{
}

HRESULT CSkidMark::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SkidMark"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	pComponent = m_pMeshRibbonCom = static_cast<CMeshRibbon*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MeshRibbon"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });
	m_pMeshRibbonCom->Set_IsQuadCopy(false);
	m_pMeshRibbonCom->Set_RibbonType(RIBBON_FLAT);

	return S_OK;
}

_int CSkidMark::Update_GameObject(const _float& fDeltaTime)
{
	m_fRemoveTimer += fDeltaTime;
	m_fWidthTimer += fDeltaTime;
	if (m_fRemoveTimer > 10.f) {
		m_fRemoveTimer = 0.f;
		if (m_pMeshRibbonCom->Get_ControlPoints().size() > 1)
			m_pMeshRibbonCom->Delete_Tail();
		if (m_pMeshRibbonCom->Get_ControlPoints().size() <= 1) {
			CManagement::GetInstance()->Delete_GameObject(L"GameLogic", this);
			if (m_pWheel)
				m_pWheel->ForgetSkidMark(this);
			return 0;
		}
	}
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_SKID, this);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CSkidMark::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CSkidMark::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);
	m_pMeshRibbonCom->Render_Buffer();
}

void CSkidMark::Compute_ViewZ()
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

void CSkidMark::Append_Point(_vec3 vPos)
{
	ControlPoint cp = {};

	cp.bank = 0;
	cp.position = vPos;
	cp.width = 1;

	m_pMeshRibbonCom->Append_Point(cp);
}

CSkidMark* CSkidMark::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, CWheel* pWheel)
{
	CSkidMark* pObj = new CSkidMark(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CSkidMark Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	pObj->m_pWheel = pWheel;
	pObj->Append_Point(vPos);

	return pObj;
}

void CSkidMark::OnLostDevice()
{
	m_pMeshRibbonCom->OnLostDevice();
}

void CSkidMark::OnResetDevice()
{
	m_pMeshRibbonCom->OnResetDevice();
}

void CSkidMark::Free()
{
	CGameObject::Free();
}
