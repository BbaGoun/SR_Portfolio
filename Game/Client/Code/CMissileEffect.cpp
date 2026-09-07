#include "pch.h"
#include "CMissileEffect.h"
#include "CGraphicDev.h"
#include "CProtoMgr.h"
#include "CTexture.h"
#include "CRenderer.h"
#include "CManagement.h"

CMissileEffect::CMissileEffect(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CMissileEffect::CMissileEffect(const CGameObject& rhs) : CGameObject(rhs)
{
}

CMissileEffect::~CMissileEffect()
{
}

HRESULT CMissileEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	m_fTimer = 0.f;

	CComponent* pComponent = nullptr;

	m_pTransformCom->Set_Scale({ 12.f, 12.f, 12.f });
	pComponent = m_pBufferCom = dynamic_cast<CSphere*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Sphere"));
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_MissileEffectToon"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	return S_OK;
}

void CMissileEffect::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{

}

_int CMissileEffect::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	m_fTimer += fDeltaTime;

	float fScale = clampT(m_fTimer * 50.f, 0.f, 20.f);
	//m_vRotation.y += fDeltaTime;
	m_pTransformCom->Set_Scale({ fScale, fScale, fScale });

	if (m_fTimer > 3.f)
	{
		m_pLayer->Delete_GameObject(this);
	}

	m_fFrame += 90 / 3.f * fDeltaTime;
	if (m_fFrame > 90)
		m_fFrame = 0;

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CMissileEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CMissileEffect::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(m_fFrame);

	m_pBufferCom->Render_Buffer();
}

CMissileEffect* CMissileEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMissileEffect* pObj = new CMissileEffect(pGraphicDev);
	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CMissileEffect Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CMissileEffect::Free()
{
	CGameObject::Free();
}
