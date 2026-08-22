#include "pch.h"
#include "CSkidMark.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCameraMgr.h"

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
	m_pTransformCom->Set_Scale({ 1,1,1 });

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, 0, D3DXToRadian(90), 0);
	m_pTransformCom->Set_Quaternion(&q);

	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SkidMark"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fTimer = 0;
	m_fFrame = 0;

	return S_OK;
}

_int CSkidMark::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	m_fTimer += fDeltaTime;
	if (m_fTimer > 15.f)
		m_pLayer->Delete_GameObject(this);

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
	m_pBufferCom->Render_Buffer();
}

CSkidMark* CSkidMark::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkidMark* pObj = new CSkidMark(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CSkidMark Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CSkidMark::Free()
{
	CGameObject::Free();
}
