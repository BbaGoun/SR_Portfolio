#include "pch.h"
#include "CLand3.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CCameraMgr.h"
#include "CRenderer.h"

CLand3::CLand3(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CLand3::CLand3(const CGameObject& rhs) : CGameObject(rhs)
{
}

CLand3::~CLand3()
{
}

HRESULT CLand3::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Scale({ 1, 1, 2 });

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = static_cast<CTerrain3*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Terrain3"));
	pComponent->Set_Owner(this);

	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

_int CLand3::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CLand3::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CLand3::Render_GameObject()
{
	D3DXMATRIX* matWorld;

	matWorld = m_pTransformCom->Get_World();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetTexture(0, nullptr);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

bool CLand3::CheckInTerrain(_vec3 vPos)
{
	int iLenX = m_pBufferCom->GetVTXCNTX() - 1;
	int iLenZ = m_pBufferCom->GetVTXCNTZ() - 1;

	_vec3 vLandStartPos,vLandScale;
	m_pTransformCom->Get_Info(INFO_POS, &vLandStartPos);
	vLandScale = m_pTransformCom->Get_Scale();

	if (vPos.x < vLandStartPos.x || vPos.x > vLandStartPos.x + iLenX * vLandScale.x)
		return false;
	if (vPos.z < vLandStartPos.z || vPos.z > vLandStartPos.z + iLenZ * vLandScale.z)
		return false;
	return true;
}

CLand3* CLand3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLand3* pObj = new CLand3(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("Land Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CLand3::Free()
{
	CGameObject::Free();
}
