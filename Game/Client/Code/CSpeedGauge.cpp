#include "pch.h"
#include "CSpeedGauge.h"

#include "CGameObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
CSpeedGauge::CSpeedGauge(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSpeedGauge::CSpeedGauge(const CSpeedGauge& rhs)
	:CGameObject(rhs)
{
}

CSpeedGauge::~CSpeedGauge()
{
}

HRESULT CSpeedGauge::Ready_GameObject()
{
	CGameObject::Ready_GameObject();
	m_pTransformCom->Set_Pos({ 0, -200, 10 });
	m_pTransformCom->Set_Scale({ 170.f,170.f,0.f });

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pVIBufferCom = dynamic_cast<CRcTex*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_RcTex"));
	if (pComponent == nullptr)
		return E_FAIL;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(CProtoMgr::GetInstance()->Get_CloneComponent(L"Proto_SpeedGaugeTexture"));
	if (pComponent == nullptr)
		return E_FAIL;


	
}

void CSpeedGauge::FixedUpdate_GameObject(const _float& fFixedDeltaTime)
{
}

_int CSpeedGauge::Update_GameObject(const _float& fDeltaTime)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);
	return CGameObject::Update_GameObject(fDeltaTime);
}

void CSpeedGauge::LateUpdate_GameObject(const _float& fDeltaTime)
{
	CGameObject::LateUpdate_GameObject(fDeltaTime);
}

void CSpeedGauge::Render_GameObject()
{
}
