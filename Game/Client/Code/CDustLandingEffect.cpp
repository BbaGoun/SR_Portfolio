#include "pch.h"
#include "CDustLandingEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CDustLandingEffect::CDustLandingEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CDustLandingEffect::CDustLandingEffect(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CDustLandingEffect::~CDustLandingEffect()
{
}

HRESULT CDustLandingEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Smoke"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_pParticleCom = CDustLandingParticle::Create(m_pGraphicDev);

	return S_OK;
}

_int CDustLandingEffect::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PARTICLE, this);
	CGameObject* pCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart");

	_vec3 vPos, vLook;
	pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	//vPos += _vec3({ 0, -1, 0 }) * 0.8f;
	m_pParticleCom->SetOrigin(vPos);

	m_pParticleCom->Update_PSystme(fDeltaTime);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CDustLandingEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CDustLandingEffect::Render_GameObject()
{
	if (m_pParticleCom->IsDead())
		return;
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pTextureCom->Set_Texture(0);
	m_pParticleCom->Render_Particle();
}

CDustLandingEffect* CDustLandingEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDustLandingEffect* pObj = new CDustLandingEffect(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CDustLandingEffect Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CDustLandingEffect::OnLostDevice()
{
	m_pParticleCom->OnLostDevice();
}
void CDustLandingEffect::OnResetDevice()
{
	m_pParticleCom->OnResetDevice();
}



void CDustLandingEffect::Free()
{
	CGameObject::Free();
	Safe_Release(m_pParticleCom);
}
