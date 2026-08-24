#include "pch.h"
#include "CCollisionStarEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CCollisionStarEffect::CCollisionStarEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCollisionStarEffect::CCollisionStarEffect(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CCollisionStarEffect::~CCollisionStarEffect()
{
}

HRESULT CCollisionStarEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_CollisionStarEffect"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_pParticleCom = CStarParticle::Create(m_pGraphicDev);

	return S_OK;
}

_int CCollisionStarEffect::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PARTICLE, this);
	CGameObject* pCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart");// 추후 플레이어로 변경

	_vec3 vPos, vLook;
	pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	vPos += _vec3({ 0, 1, 0 }) * 1.5f;
	m_pParticleCom->SetOrigin({0,0,0});

	m_pParticleCom->Update_PSystme(fDeltaTime);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CCollisionStarEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CCollisionStarEffect::Render_GameObject()
{
	if (m_pParticleCom->IsDead())
		return;
	//_matrix matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pTextureCom->Set_Texture(0);
	m_pParticleCom->Render_Particle();
}

CCollisionStarEffect* CCollisionStarEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollisionStarEffect* pObj = new CCollisionStarEffect(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CCollisionStarEffect Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CCollisionStarEffect::OnLostDevice()
{
	m_pParticleCom->OnLostDevice();
}
void CCollisionStarEffect::OnResetDevice()
{
	m_pParticleCom->OnResetDevice();
}


void CCollisionStarEffect::Free()
{
	CGameObject::Free();
	Safe_Release(m_pParticleCom);
}
