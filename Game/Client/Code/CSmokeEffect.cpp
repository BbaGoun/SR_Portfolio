#include "pch.h"
#include "CSmokeEffect.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCart.h"

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
	m_pCart = nullptr;

	CGameObject::Ready_GameObject();

	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_Smoke"));
	pComponent->Set_Owner(this);
	m_mapComponent.insert({ L"Com_Texture", pComponent });

	m_pSmoke = CSmoke::Create(m_pGraphicDev, m_fSize, m_iParticleCnt, m_fMinAge, m_fMaxAge);

	return S_OK;
}

_int CSmokeEffect::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_PARTICLE, this);
	
	_vec3 vPos,vLook,vRight;
	m_pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);
	m_pCart->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
	m_pCart->Get_Transform()->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	if (m_bTwin) {
		if (rand() % 2 == 0)
			vPos += _vec3({ 0,1,0 }) * 0.5f + vLook * -2.5f + vRight * -1.1f;
		else
			vPos += _vec3({ 0,1,0 }) * 0.5f + vLook * -2.5f + vRight * 1.1f;
	}
	else
		vPos += -3 * vLook;

	vLook *= -1;
	m_pSmoke->SetOrigin(vPos);
	m_pSmoke->SetBackDir(vLook);

	if(!m_bTwin)
		m_pSmoke->Update_PSystme(fDeltaTime);
	else if (static_cast<CCart*>(m_pCart)->GetBoost() == false)
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

CSmokeEffect* CSmokeEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, float fSize, int iParticleCnt, float fMinAge, float fMaxAge)
{
	CSmokeEffect* pObj = new CSmokeEffect(pGraphicDev);
	pObj->m_fSize = fSize;
	pObj->m_iParticleCnt = iParticleCnt;
	pObj->m_fMinAge = fMinAge;
	pObj->m_fMaxAge = fMaxAge;

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CSmokeEffect Create Failed");
		Safe_Release(pObj);
		return nullptr;
	}
	return pObj;
}

void CSmokeEffect::OnLostDevice() 
{
	m_pSmoke->OnLostDevice();
}
void CSmokeEffect::OnResetDevice() 
{
	m_pSmoke->OnResetDevice();
}

void CSmokeEffect::SetCart(CGameObject* pObj)
{
	m_pCart = pObj;

	if (m_pCart == nullptr)
		return; 
	
	_vec3 vPos, vLook, vRight;
	m_pCart->Get_Transform()->Get_Info(INFO_POS, &vPos);
	m_pCart->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
	m_pCart->Get_Transform()->Get_Info(INFO_RIGHT, &vRight);
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	vPos += _vec3({ 0, 1, 0 }) * 1.f + vLook * -1.5f + vRight * -1.5f;
	vLook *= -1;
	m_pSmoke->SetOrigin(vPos);
	m_pSmoke->SetBackDir(vLook);
	m_pSmoke->ResetAll();
}


void CSmokeEffect::Free()
{
	Safe_Release(m_pSmoke);
	CGameObject::Free(); 
}
