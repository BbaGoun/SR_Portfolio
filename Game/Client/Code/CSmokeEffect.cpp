#include "pch.h"
#include "CSmokeEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CSmokeEffect::CSmokeEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSmokeEffect::CSmokeEffect(const CGameObject& rhs)
	:CGameObject(rhs)
{
}

CSmokeEffect::~CSmokeEffect()
{
}

HRESULT CSmokeEffect::Ready_GameObject()
{
	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Smoke"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_pSmoke = CSmoke::Create(m_pGraphicDev);
	m_pSmoke->Ready_PSystem();

	return S_OK;
}

_int CSmokeEffect::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PARTICLE, this);
	CGameObject* pCart = CManagement::GetInstance()->Find_GameObjectByTag(L"GameLogic", L"Obj_Cart");
	
	_vec3 vPos,vLook;
	pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);
	pCart->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
	D3DXVec3Normalize(&vLook, &vLook);
	//vPos += _vec3({ 0, 1, 0 }) * -0.3;// +vLook * -6.f;
	cout << vPos.x << "\t" << vPos.y << "\t" << vPos.z << endl;
	vLook *= -1;
	m_pSmoke->SetOrigin(vPos);
	m_pSmoke->SetBackDir(vLook);

	m_pSmoke->Update_PSystme(fDeltaTime);

	return CGameObject::Update_GameObject(fDeltaTime);
}

void CSmokeEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CSmokeEffect::Render_GameObject()
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pTextureCom->Set_Texture(0);
	m_pSmoke->Render_Particle();
}

CSmokeEffect* CSmokeEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSmokeEffect* pObj = new CSmokeEffect(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CSmokeEffect Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}
void CSmokeEffect::Free()
{
	CGameObject::Free(); 
	Safe_Release(m_pSmoke);
}
