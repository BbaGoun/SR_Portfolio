#include "pch.h"
#include "CThunder.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCameraMgr.h"
#include <CThunderCloud.h>

CThunder::CThunder(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CThunder::CThunder(const CGameObject& rhs) : CGameObject(rhs)
{
}

CThunder::~CThunder()
{
}

HRESULT CThunder::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	m_pTransformCom->Set_Scale({ 10,10,1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ThunderTexture"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fTimer = 0;
	m_fFrame = 0;

	return S_OK;
}

_int CThunder::Update_GameObject(const _float& fDeltaTime)
{
	CGameObject* pThunderCloud = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_ThunderCloud");
	_vec3 vThunderCloudPos, vPos, vDir;
	pThunderCloud->Get_Transform()->Get_Info(INFO_POS, &vThunderCloudPos);

	m_pTransformCom->Set_Pos({ vThunderCloudPos.x,vThunderCloudPos.y - 5.f,vThunderCloudPos.z });

	//m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CGameObject::Compute_ViewZ(&vPos);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	m_fFrame += 10 * fDeltaTime;

	if (m_fFrame > 5)
	{
		m_fFrame = 0;
	}
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CThunder::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CThunder::Render_GameObject()
{
	_matrix	matWorld, matView;
	matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
	m_pTransformCom->Set_Billboard(&matView);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();

}

CThunder* CThunder::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThunder* pObj = new CThunder(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CThunder Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CThunder::Free()
{
	CGameObject::Free();
}
