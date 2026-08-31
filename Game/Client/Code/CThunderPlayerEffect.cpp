#include "pch.h"
#include "CThunderPlayerEffect.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCameraMgr.h"

CThunderPlayerEffect::CThunderPlayerEffect(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CThunderPlayerEffect::CThunderPlayerEffect(const CGameObject& rhs) : CGameObject(rhs)
{
}

CThunderPlayerEffect::~CThunderPlayerEffect()
{
}

HRESULT CThunderPlayerEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;
	//m_pTransformCom->Set_Scale({ 25,20,1 });
	pComponent = m_pBufferCom = static_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_ThunderPlayerEffectTexture"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_fTimer	= 0;
	m_fFrame	= 0;
	m_fSize		= 15;

	return S_OK;
}

_int CThunderPlayerEffect::Update_GameObject(const _float& fDeltaTime)
{
	CGameObject* pCartBody = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_CartBody");
	_vec3 vCartBodyPos, vDir;
	pCartBody->Get_Transform()->Get_Info(INFO_POS, &vCartBodyPos);

	m_pTransformCom->Set_Pos({ vCartBodyPos.x,vCartBodyPos.y,vCartBodyPos.z});

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	if (m_fSize < 25)
		m_fSize += 5 * fDeltaTime;
	m_pTransformCom->Set_Scale({ m_fSize * 1.2f,m_fSize,1 });

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CThunderPlayerEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CThunderPlayerEffect::Render_GameObject()
{
	_matrix	matWorld, matView;
	matView = CCameraMgr::GetInstance()->GetCameraInfo().matView;
	m_pTransformCom->Set_Billboard(&matView);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(m_fFrame);
	m_pBufferCom->Render_Buffer();
}

CThunderPlayerEffect* CThunderPlayerEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThunderPlayerEffect* pObj = new CThunderPlayerEffect(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CThunderPlayerEffect Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}

	return pObj;
}

void CThunderPlayerEffect::Free()
{
	CGameObject::Free();
}
